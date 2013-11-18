#include "Connection.h"
#include <list>
#include <string>

namespace astron
{
// open namespace
using namespace boost::asio;
using boost::asio::ip::tcp;


// Creates an empty connection, ready for use
Connection::Connection()
{
}

// Create a Connection from an already opened socket.
Connection::Connection(boost::asio::ip::tcp::socket *socket)
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
	if(col_index != std::string::npos && col_index > sqr_index)
	{
		addr = host.substr(0, col_index);
		port = host.substr(col_index + 1);
	}
	else
	{
		addr = host;
		port = "7199";
	}

	// Resolve the address with the port as the designated service.
	boost::asio::io_service ios;
	tcp::resolver resolver(ios);
	tcp::resolver::query query(addr, port);

	// Perform synchronous resolution of the addresss.
	boost::system::error_code err;
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
	boost::asio::socket_base::keep_alive keepalive(true);
	boost::asio::ip::tcp::no_delay nodelay(true);
	m_socket->set_option(keepalive);
	m_socket->set_option(nodelay);

	// Connect to the first available endpoint
	m_socket->connect(*addr_it);
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
		gather.push_back(boost::asio::buffer((uint8_t*)&len, 2));
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

// recv_datagram waits for the next datagram and stores it in dg.
void Connection::recv_datagram(Datagram &dg)
{
	// TODO: Implement
}

// poll_datagram receives a datagram if one is immediately available.
// Returns true if a datagram was received and false otherwise.
// When given a timeout, it will wait upto the specified time before returning.
bool Connection::poll_datagram(Datagram &dg)
{
	// TODO: Implement
	return false;
}
//bool poll_datagram(Datagram &dg, <timeout>);

// poll_forever will block forever and receive datagrams as they come in.
// When a datagram is received the (overridable) handle_datagram method is called.
void Connection::poll_forever()
{
	// TODO: Implement
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


} // close namespace astron
