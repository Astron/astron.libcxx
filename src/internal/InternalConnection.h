#pragma once
#include "util/Connection.h"
#include "util/DatagramIterator.h"

namespace astron   // open namespace
{


class InternalConnection : public Connection
{
	public:
		InternalConnection();

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
        virtual void handle_datagram(const Datagram &dg, DatagramIterator &dgi);
    
    private:
        void handle_datagram(const Datagram &dg);
};


} // close namespace
