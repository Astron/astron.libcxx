Creating A Simple Uberdog
---------------------------

### The Main routine ###
//TODO: Describe the code snippets

**With quick setup**
```cpp
#include <libastron-cxx/internal/InternalRepository.h>
#include "MyUberdog.h"

using namespace astron;

#define UBERDOG_ID 10002

int main(int argc, const char* argv[])
{
	MyUberdog* ud = new MyUberdog(UBERDOG_ID);
	InternalRepository air = InternalRepository(ud, "example.com");
	air.poll_forever();
}
```

**With manual setup**
```cpp
int main(int argc, const char* argv[])
{
	MyUberdog* ud = new MyUberdog(UBERDOG_ID);
	InternalRepository air = InternalRepository();
	air.attach_object(ud);

	// do some other fancy setup

	air.connect("example.com");
	air.subscribe_channel(UBERDOG_ID);
	air.poll_forever();
}
```

### Defining the Uberdog Behavior ###
//TODO: Document and provide examples for subclassing the DistributedObject class.
