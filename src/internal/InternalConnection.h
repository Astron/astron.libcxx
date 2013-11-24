
#pragma once
#include "InternalMessages.h"
#include "util/Connection.h"
namespace astron   // open namespace
{


class InternalConnection : public Connection
{
	public:
		InternalConnection();

		// connect opens a tcp connection to the astron cluster
		void connect(std::string addr = "127.0.0.1", uint16_t port = 7199);

		// Control messages
		void subscribe_channel(channel_t channel);
		void unsubscribe_channel(channel_t channel);
		void subscribe_range(channel_t lo, channel_t hi);
		void unsubscribe_range(channel_t lo, channel_t hi);
		void add_post_remove(const Datagram &dg);
		void clear_post_removes();
		void set_conn_url(std::string uri);
		void set_conn_name(std::string name);

	protected:
		void handle_datagram(Datagram &dg)
		{
		}
};


} // close namespace
