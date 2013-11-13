#include "Connection.h"


namespace astron {
    Connection::Connection(){
        
    }
    
    void Connection::send_datagram(const Datagram &dg)
    {
        
        
    }
    
    // recv_datagram waits for the next datagram and stores it in dg.
    void Connection::recv_datagram(Datagram &dg)
    {
        
    }
    
    // poll_datagram receives a datagram if one is immediately available.
    // Returns true if a datagram was received and false otherwise.
    // When given a timeout, it will wait upto the specified time before returning.
    bool Connection::poll_datagram(Datagram &dg){
        
        return false;
    }
    //bool poll_datagram(Datagram &dg, <timeout>);
    
    // poll_forever will block forever and receive datagrams as they come in.
    // When a datagram is received the (overridable) handle_datagram method is called.
    bool Connection::poll_forever(){
        
        return  false;
        
    }

    
    
}