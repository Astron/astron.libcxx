Creating A Simple Uberdog
---------------------------

### The Main routine ###
//TODO: Describe the code snippets

**With quick setup**
```cpp
#include <libastron-cxx/InternalRepository.h>
#include "MyUberdog.h"

using namespace astron;

#define UBERDOG_ID 10002

int main(int argc, const char* argv[])
{
	MyUberdog* ud = new MyUberdog(UBERDOG_ID);
	InternalRepository air = InternalRepository(ud);
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

	air.connect();
	air.subscribe_channel(UBERDOG_ID);
}
```

### Defining the Uberdog Behavior ###
//TODO: Document and provide examples for subclassing the DistributedObject class.
