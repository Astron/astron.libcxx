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
		// recv_datagram called with no datagram, returns immediately and asynchronously
		// calls the subclasses's handle_datagram method on the next received datagram.
		void recv_datagram(Datagram &dg);
		void recv_datagram();

		// poll_datagram(datagram) receives a datagram if one is immediately available.
		// Returns true if a datagram was received and false otherwise.
		bool poll_datagram(Datagram &dg);

		// poll_forever will block forever and receive datagrams as they come in.
		// When a datagram is received the connection's handle_datagram method is called.
		void poll_forever();

	protected:
		// handle_datagram is called whenever a packet is received after having called poll_forever.
		// Can be overridden by subclasses to provide custom message behavior.
		virtual void handle_datagram(const Datagram &dg);

		// handle_disconnect is called when the remote host closes the connection,
		// or if the connection is otherwise lost; handle_disconnect is not called by disconnect();
		virtual void handle_disconnect();

		boost::asio::ip::tcp::socket *m_socket;

	private:
        boost::asio::io_service ios;
		boost::system::error_code _receive(Datagram &dg);
		void _async_receive();
		void _handle_size(const boost::system::error_code &ec, size_t bytes_transferred);
		void _handle_data(const boost::system::error_code &ec, size_t bytes_transferred);

		uint8_t m_size_buf[sizeof(dgsize_t)]; // buffer to receive datagram sizes into
		uint8_t* m_async_buf; // pointer to buffer allocated for an asynchronous receive
		dgsize_t m_async_size; // size of the buffer allocated for an asynchronous receive
		bool m_expecting_data; // whether the next asynchronous read is data or a length tag.
		bool m_is_async; // whether the connection is currently performing an asynchronous receive.
		bool m_is_forever; // whether the connection is polling forever
};


} // close namespace
