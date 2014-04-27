Connecting Internally w/o an Object Repository
-------------------------------------

### Creating a Network Reactor ###
// TODO: Describe the code snippets

```cpp
// Filename: main.cpp
#include "MyReactor.h"
using namespace astron;


int main(int argc, const char* argv[])
{
  MyReactor r;
  r.run("example.net", 1057);
}

```


```cpp
// Filename: MyReactor.h
#include <libastron-cxx/internal/InternalConnection.h>
using namespace astron;

class MyReactor : public InternalConnection
{
    public:
        void run(std::string astron_addr, channel_t channel);

    protected:
        void handle_datagram(Datagram &dg); // inherited from InternalConnection   
}
```
**Note:** You could use a Connection object instead of an InternalConnection here, but InternalConnection
provides some additional useful utility methods like *subscribe_channel()* and *set_conn_name()*.

```cpp
// Filename: MyReactor.cppMyReactor(
#include "MyReactor.h"

void MyReactor::run(std::string astron_addr, channel_t channel)
{
    r.connect(astron_addr);
    r.set_conn_name("MyReactor");
    r.subscribe_channel(channel);
    r.poll_forever();
}

void MyReactor::handle_datagram(Datagram &dg)
{
    uint8_t num_receivers = dg.read_uint8();
    channel_t* receivers = new channel_t[num_receivers];
    for(uint8_t i = 0; i < num_receivers; ++i)
    {
        receivers[i] = dgi.read_channel();
    }
    channel_t sender = dgi.read_channel();
    uint16_t message_type = dgi.read_uint16();

    switch(message_type) 
    {
        case MESSAGE_TYPE_THAT_I_WANT_TO_HANDLE:

            // do cool stuff

            break;

        default:

            // do anything I want to do on unexpected or ignored message type

            break;
    }

    delete [] receivers;
}
```

### Creating a custom Connection with an Event Loop ###

// TODO: Give examples.
