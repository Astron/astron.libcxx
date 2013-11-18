#pragma once
#include "util/Datagram.h"
#include <boost/asio.hpp>
#include <chrono>

namespace astron
{
// open namespace


class Connection
{
	public:
		// Creates an empty connection, ready for use
		Connection();

		// Create a Connection from an already opened socket.
		Connection(boost::asio::ip::tcp::socket *socket);

		// Destructor
		~Connection();


		/* State operations */

		// connect open a tcp connection to an astron cluster.
		// A domain-name or ip-address and port can be specified.
		// Connects to "localhost:7199" by default.
		// Returns false if connection failed.
		bool connect(std::string host, uint16_t port);

		// connect with a single string arguments expects a domain-name or ip-address
		// with optional port formatted as "addr:port".
		// Returns false if connection failed.
		bool connect(std::string addr = "127.0.0.1:7199");

		// disconnect closes the tcp socket.
		// Can be overriden by subclasses to handle clean-up before disconnecting.
		virtual void disconnect();

		// is_connected returns true if the connection has an open socket, false otherwise.
		bool is_connected();

		// set_socket can be used to give the connection an already connected socket
		void set_socket(boost::asio::ip::tcp::socket *socket);


		/* I/O operations */

		// send_datagram immediately sends a datagram.
		void send_datagram(const Datagram &dg);

		// recv_datagram waits for the next datagram and stores it in dg.
		void recv_datagram(Datagram &dg);

		// poll_datagram(datagram) receives a datagram if one is immediately available.
		// Returns true if a datagram was received and false otherwise.
		// When given a timeout, it will wait upto the specified time before returning.
		bool poll_datagram(Datagram &dg);
		bool poll_datagram(Datagram &dg, std::chrono::nanoseconds timeout);

		// poll_forever will block forever and receive datagrams as they come in.
		// When a datagram is received the connection's handle_datagram method is called.
		void poll_forever();

	protected:
		// handle_datagram is called whenever a packet is received after having called poll_forever.
		// Can be overridden by subclasses to provide custom message behavior.
		virtual void handle_datagram(const Datagram &dg);

		boost::asio::ip::tcp::socket *m_socket;
};


} // close namespace
