#include "InternalConnection.h"
#include "InternalMessages.h"
namespace astron    // open namespace
{


InternalConnection::InternalConnection(){
    
}

// Control messages
void InternalConnection::subscribe_channel(channel_t channel)
{
    Datagram dg(CONTROL_ADD_CHANNEL);
    dg.add_channel(channel);
    send_datagram(dg);
}


void InternalConnection::unsubscribe_channel(channel_t channel)
{
    Datagram dg(CONTROL_REMOVE_CHANNEL);
    dg.add_channel(channel);
    send_datagram(dg);
}

void InternalConnection::subscribe_range(channel_t lo, channel_t hi)
{
    Datagram dg(CONTROL_ADD_RANGE);
    dg.add_channel(lo);
    dg.add_channel(hi);
    send_datagram(dg);
}

void InternalConnection::unsubscribe_range(channel_t lo, channel_t hi)
{
    Datagram dg(CONTROL_REMOVE_RANGE);
    dg.add_channel(lo);
    dg.add_channel(hi);
    send_datagram(dg);
}

void InternalConnection::add_post_remove(const Datagram &post)
{
    Datagram dg(CONTROL_ADD_POST_REMOVE);
    dg.add_blob(post);
    send_datagram(dg);
}

void InternalConnection::clear_post_removes()
{
    send_datagram(Datagram(CONTROL_CLEAR_POST_REMOVES));
}

void InternalConnection::set_conn_url(std::string uri)
{
    Datagram dg(CONTROL_SET_CON_URL);
    dg.add_string(uri);
    send_datagram(dg);
}

void InternalConnection::set_conn_name(std::string name)
{
    Datagram dg(CONTROL_SET_CON_NAME);
    dg.add_string(name);
    send_datagram(dg);
}


} // close namespace astron
