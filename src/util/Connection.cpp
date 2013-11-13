#include "Connection.h"

using boost::asio::ip::tcp;

namespace astron {
    Connection::Connection():NetworkClient(){
        
        hasDatagram = false;
        
    }
    
    // connect opens a tcp connection to the astron cluster
    void Connection::connect(std::string addr, uint16_t port)
    {
        boost::asio::io_service io_service;
        tcp::socket s(io_service);
        tcp::endpoint endpoint(boost::asio::ip::address_v4::from_string(addr) , port);
        s.connect(endpoint);
        
        NetworkClient::set_socket(&s);
    }
    
    
    void Connection::send_datagram(const Datagram &dg)
    {
        network_send(dg);
    }
    
    
    // recv_datagram waits for the next datagram and stores it in dg.
    void Connection::recv_datagram(Datagram &dg)
    {
        //We are in polling mode
        isPolling = true;
        
        //TODO: This is a horable busy wait loop....
        //Make this better.
        while (!hasDatagram);
        if (currentDatagram) {
            dg = *currentDatagram;
        }
        
        //Clean-up
        hasDatagram = false;
        currentDatagram = NULL;
        isPolling = false;
        
        
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
    
    
    //Implementations of NetworkClient Virtual Functions
    void Connection::network_datagram(astron::Datagram &dg)
    {
        if (isPolling) {
            //we are in sync mode... set we have a datagram.
            //This is probably not thread safe atm...
            currentDatagram = &dg;
            hasDatagram = true;
        }else{
            //otherwise we are doing async networking
            handle_datagram(dg);
            
        }
        
        
        
    }
    
    
    void Connection::network_disconnect()
    {
        //OUR NETWORK DISCONNECTED!!! :(
        
    }

    
    
}