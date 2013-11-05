#pragma once
#include "util/Datagram.h"

// Implementation Notes:
//     If necessary connection can subclass another class (maybe NetworkClient for example);
//     However, any inherited virtual functions must be implemented. Subclasses of connection
//     should only be required to implement handle_datagram.
class Connection
{
	public:
		Connection();

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
};
