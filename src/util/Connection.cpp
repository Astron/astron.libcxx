#include "Connection.h"
#include <string>

namespace astron   // open namespace
{
using boost::asio::ip;
using boost::asio::ip::tcp;


Connection::Connection()
{
}

// connect(std::string) expects a domain-name or ip-address, with optional port,
// resolves that address, and opens a tcp connection to the astron cluster.
bool Connection::connect(std::string addr)
{
	std::string port;

	// Parse out port from address string
	unsigned int col_index = str.find_last_of(":");
	unsigned int sqr_index = str.find_last_of("]");
	if(col_index != std::string::npos && col_index > sqr_index)
	{
		addr = addr.substr(0, col_index);
		port = addr.substr(col_index + 1);
	}
	else
	{
		port = "7199";
	}

	// Resolve the address with the port as the designated service.
	boost::asio::io_service ios;
	tcp::resolver resolver(ios);
	tcp::resolver::query query(addr, port);

	// Perform synchronous resolution of the addresss. Note that, synchronous resolution is well
	// suited to being used in libastron, wheras asynchronous should be used in Astron proper.
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

	// Connect to the first endpoint
	tcp::socket s(ios);
	s.connect(addr_it);

	// TODO: Store socket somewhere?

	return true;
}


void Connection::send_datagram(const Datagram &dg)
{
}


// recv_datagram waits for the next datagram and stores it in dg.
void Connection::recv_datagram(Datagram &dg)
{
}

// poll_datagram receives a datagram if one is immediately available.
// Returns true if a datagram was received and false otherwise.
// When given a timeout, it will wait upto the specified time before returning.
bool Connection::poll_datagram(Datagram &dg)
{

	return false;
}
//bool poll_datagram(Datagram &dg, <timeout>);

// poll_forever will block forever and receive datagrams as they come in.
// When a datagram is received the (overridable) handle_datagram method is called.
bool Connection::poll_forever()
{

	return  false;

}


} // close namespace astron
