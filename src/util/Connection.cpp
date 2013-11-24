#include "Connection.h"
#include <list>
#include <string>
#include <boost/bind.hpp>

namespace astron
{
// open namespace
using namespace boost::asio;
using namespace boost::system;
using boost::asio::ip::tcp;


// Creates an empty connection, ready for use
Connection::Connection() : m_async_buf(NULL), m_async_size(0), m_expecting_data(false),
	m_is_async(false), m_is_forever(false)
{
}

// Create a Connection from an already opened socket.
Connection::Connection(boost::asio::ip::tcp::socket *socket) : m_async_buf(NULL), m_async_size(0),
	m_expecting_data(false), m_is_async(false), m_is_forever(false)
{
	set_socket(socket);
}

// Destructor
Connection::~Connection()
{
	if(m_socket)
	{
		m_socket->close();
	}
	delete m_socket;
}


// connect open a tcp connection to an astron cluster.
// A domain-name or ip-address and port can be specified.
// Connects to "localhost:7199" by default.
// Returns false if connection failed.
bool Connection::connect(std::string host, uint16_t port)
{
	return connect(host + ":" + std::to_string((uint32_t)port));
}

// connect with a single string arguments expects a domain-name or ip-address
// with optional port, resolves the string to an address,
// and opens a tcp connection to an astron cluster.
// Returns false if connection failed.
bool Connection::connect(std::string host)
{
	std::string addr, port;
	if(m_socket)
	{
		throw std::logic_error("Trying to connect when already connected.");
	}

	// Parse out port from address string
	unsigned int col_index = host.find_last_of(":");
	unsigned int sqr_index = host.find_last_of("]");
	if(col_index != std::string::npos && col_index < sqr_index)
	{
		addr = host.substr(0, col_index);
		port = host.substr(col_index + 1);
	}
	else
	{
		addr = host;
		port = "57123";
	}

	// Resolve the address with the port as the designated service.
	boost::asio::io_service ios;
	tcp::resolver resolver(ios);
	tcp::resolver::query query(addr, port);

	// Perform synchronous resolution of the addresss.
	error_code err;
	tcp::resolver::iterator addr_it = resolver.resolve(query, err);
	if(err)
	{
		// TODO: Maybe a log message or return a more descriptive error?
		return false;
	}

	// If there is at least one endpoint
	if(addr_it == tcp::resolver::iterator())
	{
		// TODO: Maybe a log message or return a more descriptive error?
		return false;
	}

	// Setup socket
	m_socket = new tcp::socket(ios);


	// Connect to the first available endpoint
	m_socket->connect(*addr_it);
    
    boost::asio::socket_base::keep_alive keepalive(true);
	boost::asio::ip::tcp::no_delay nodelay(true);
	m_socket->set_option(keepalive);
	m_socket->set_option(nodelay);
    
	return true;
}

// send_datagram immediately sends a datagram.
void Connection::send_datagram(const Datagram &dg)
{
	dgsize_t len = dg.size();
	try
	{
		m_socket->non_blocking(true);
		m_socket->native_non_blocking(true);
		std::list<boost::asio::const_buffer> gather;
		gather.push_back(boost::asio::buffer((uint8_t*)&len, sizeof(dgsize_t)));
		gather.push_back(boost::asio::buffer(dg.get_data(), dg.size()));
		m_socket->send(gather);
	}
	catch(std::exception &e)
	{
		// We assume that the message just got dropped if the remote end died
		// before we could send it.
		disconnect();
	}
}

error_code Connection::_receive(Datagram &dg)
{
	if(m_is_async)
	{
		throw std::logic_error("Attempting synchronous receive during an asynchronous operation.");
	}

	// Get datagram size
	error_code err;
	read(*m_socket, boost::asio::buffer(m_size_buf, sizeof(dgsize_t)), err);
	if(err)
	{
		return err;
	}

	// Get datagram data
	dgsize_t data_size = *(dgsize_t*)m_size_buf;
	read(*m_socket, boost::asio::buffer(dg.add_buffer(data_size), data_size));
	return err;
}

void Connection::_async_receive()
{
	try
	{
		if(m_expecting_data) // Read data
		{
			async_read(*m_socket, boost::asio::buffer(m_async_buf, m_async_size),
			           boost::bind(&Connection::_handle_data, this,
			                       boost::asio::placeholders::error,
			                       boost::asio::placeholders::bytes_transferred));
		}
		else // Read length
		{
			async_read(*m_socket, boost::asio::buffer(m_size_buf, sizeof(dgsize_t)),
			           boost::bind(&Connection::_handle_size, this,
			                       boost::asio::placeholders::error,
			                       boost::asio::placeholders::bytes_transferred));
		}
	}
	catch(std::exception &e)
	{
		// An exception happening when trying to initiate a read is a clear
		// indicator that something happened to the connection. Therefore:
		disconnect();
	}
}

void Connection::_handle_size(const boost::system::error_code &ec, size_t bytes_transferred)
{
	if(ec.value() != 0)
	{
		handle_disconnect();
		return;
	}

	if(m_is_forever)
	{
		dgsize_t old_size = m_async_size;
		m_async_size = *(dgsize_t*)m_size_buf;
		if(m_async_size > old_size)
		{
			delete [] m_async_buf;
			m_async_buf = new uint8_t[m_async_size];
		}
	}
	else
	{
		m_async_size = *(dgsize_t*)m_size_buf;
		m_async_buf = new uint8_t[m_async_size];
	}

	m_expecting_data = true;
	_async_receive();
}

void Connection::_handle_data(const boost::system::error_code &ec, size_t bytes_transferred)
{
	if(ec.value() != 0)
	{
		handle_disconnect();
		return;
	}

	m_expecting_data = false;
	if(m_is_forever)
	{
		// Create a datagram from the received data (datagram copies the buffer, buffer is reused)
		Datagram dg(m_async_buf, m_async_size);

		// Handle the datagram
		handle_datagram(dg);

		// Wait for another datagram
		_async_receive();
	}
	else
	{
		// Create a datagram from the received data (no-copy, buffer is not reused)
		Datagram dg(m_async_buf, m_async_size, m_async_size);

		// We are no longer performing an asynchronous operation
		m_async_buf = NULL; // the buffer is deleted by the datagram, when it is done
		m_is_async = false;

		// Handle the datagram
		handle_datagram(dg);
	}
}

// recv_datagram waits for the next datagram and stores it in dg.
void Connection::recv_datagram(Datagram &dg)
{
	m_socket->non_blocking(false);
	m_socket->native_non_blocking(false);
	_receive(dg);
}

// recv_datagram called with no datagram, returns immediately and asynchronously
// calls the subclasses's handle_datagram method on the next received datagram.
void Connection::recv_datagram()
{
	m_is_async = true;
	_async_receive();
}


// poll_datagram receives a datagram if one is immediately available.
// Returns true if a datagram was received and false otherwise.
bool Connection::poll_datagram(Datagram &dg)
{
	m_socket->non_blocking(true);
	m_socket->native_non_blocking(true);
	return _receive(dg) != boost::asio::error::would_block;
}

// poll_forever will block forever and receive datagrams as they come in.
// When a datagram is received the (overridable) handle_datagram method is called.
void Connection::poll_forever()
{
	m_is_async = true;
	m_is_forever = true;
	_async_receive();
}

void Connection::set_socket(tcp::socket *socket)
{
	if(m_socket)
	{
		throw std::logic_error("Trying to set a socket of a Connection whose socket was already set.");
	}
	m_socket = socket;

	boost::asio::socket_base::keep_alive keepalive(true);
	m_socket->set_option(keepalive);

	boost::asio::ip::tcp::no_delay nodelay(true);
	m_socket->set_option(nodelay);
}

// handle_datagram is called whenever a packet is received after having called poll_forever.
// Can be overridden by subclasses to provide custom message behavior.
void Connection::handle_datagram(const Datagram &dg)
{
}

// disconnect closes the tcp socket.
// Can be overriden by subclasses to handle clean-up before disconnecting.
void Connection::disconnect()
{
	m_socket->close();
}

// handle_disconnect is called when the remote host closes the connection,
// or if the connection is otherwise lost; handle_disconnect is not called by disconnect();
void Connection::handle_disconnect()
{
}

} // close namespace astron
