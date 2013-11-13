#include "InternalConnection.h"

namespace astron {
    
    InternalConnection::InternalConnection(){
        
    }
    
    // connect opens a tcp connection to the astron cluster
    void InternalConnection::connect(std::string addr, uint16_t port)
    {
        
    }
    
    // Control messages
    void InternalConnection::subscribe_channel(channel_t channel){
        
    }
    
    
    void InternalConnection::unsubscribe_channel(channel_t channel){
        
    }
    
    void InternalConnection::subscribe_range(channel_t lo, channel_t hi){
        
    }
    
    void InternalConnection::unsubscribe_range(channel_t lo, channel_t hi){
        
    }
    
    void InternalConnection::add_post_remove(const Datagram &dg){
        
    }
    
    void InternalConnection::clear_post_removes()
    {
        
    }
    
    void InternalConnection::set_conn_url(std::string uri){
        
    }
    
    void InternalConnection::set_conn_name(std::string name){
        
    }
    
}//close namespace