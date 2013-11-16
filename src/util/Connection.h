#pragma once
#include "util/Datagram.h"
namespace astron   // open namespace
{


class Connection
{
	public:
		Connection();

		// connect opens a tcp connection to the astron cluster
		void connect(std::string addr = "127.0.0.1", uint16_t port = 7199);

		/** I/O operations **/
		// send_datagram immediately sends the datagram.
		void send_datagram(const Datagram &dg);

		// recv_datagram waits for the next datagram and stores it in dg.
		void recv_datagram(Datagram &dg);

		// poll_datagram receives a datagram if one is immediately available.
		// Returns true if a datagram was received and false otherwise.
		// When given a timeout, it will wait upto the specified time before returning.
		bool poll_datagram(Datagram &dg);
		//bool poll_datagram(Datagram &dg, <timeout>);

		// poll_forever will block forever and receive datagrams as they come in.
		// When a datagram is received the (overridable) handle_datagram method is called.
		bool poll_forever();

	protected:
		virtual void handle_datagram(Datagram &dg);
};


} // close namespace
