#pragma once
#include "ClientMessages.h"
#include "util/Datagram.h"
namespace astron { // open namespace


// Implementation Notes:
//     If necessary connection can subclass another class (maybe NetworkClient for example);
//     However, any inherited virtual functions must be implemented. Subclasses of connection
//     should only be required to implement handle_datagram.
class ClientConnection
{
	public:
		ClientConnection(std::string uri);

		// connect starts a connection to the server, negotiates Hello and starts sending
		// heartbeats periodically. It returns after negotiation is complete.
		void connect(uint32_t dc_hash, std::string version);

		// disconnect cleanly closes the connection.
		// Internally: Sends a CLIENT_DISCONNECT, and waits for connection to close.
		void disconnect();

		// Outgoing datagram
		void send_datagram(const Datagram &dg);

	protected:
		virtual void handle_datagram(const Datagram &dg) = 0;
		virtual void handle_eject(std::string reason) = 0;
};


} // close namespace