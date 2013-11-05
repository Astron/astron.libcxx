#include "messages/internalMessages.h"

// Implementation Notes:
//     If necessary connection can subclass another class (maybe NetworkClient for example);
//     However, any inherited virtual functions must be implemented. Subclasses of connection
//     should only be required to implement handle_datagram.
class InternalConnection
{
	public:
		InternalConnection(std::string addr = "127.0.0.1", uint16_t port = 7199);

		// Control messages
		void subscribe_channel(channel_t channel);
		void unsubscribe_channel(channel_t channel);
		void subscribe_range(channel_t lo, channel_t hi);
		void unsubscribe_range(channel_t lo, channel_t hi);
		void add_post_remove(const Datagram &dg);
		void clear_post_removes();
		void set_conn_url(std::string uri);
		void set_conn_name(std::string name);

		// Outgoing datagram
		void send_datagram(const Datagram &dg);

	protected:
		virtual void handle_datagram(const Datagram &dg) = 0;
};
