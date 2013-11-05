Creating A Simple AI
-----------------------

### The Main routine ###
//TODO: Describe the code snippets

```cpp
#include <libastron-cxx/internal/InternalRepository.h>
using namespace astron;
```

#### Setting up the Shard ####
```cpp
int main(int argc, const char* argv[])
{
	InternalRepository* air = new InternalRepository();
	Shard shard* = new Shard(air, AI_CHANNEL, STATESERVER_CHANNEL, MIN_ID, MAX_ID);

	/** Setup initial Shard state **/
	/// Example setup code
	//shard.activate_object(WORLD_OBJECT_ID);
	//shard.activate_object(CITY_FOO_ID);
	//shard.activate_object(CITY_BAR_ID);
	//
	//DistributedNPCsAI fooNpcs = new DistributedNPCsAI();
	//fooNpcs.set_location(CITY_FOO_ID, 0);
	//shard.generate_object(fooNpcs);
	//
	//DistributedNPCsAI barNpcs = new DistributedNPCsAI();
	//barNpcs.set_location(CITY_BAR_ID, 0);
	//shard.generate_object(barNpcs);
```

#### Option A: Running a main event loop ####
```cpp

	while(true)
	{
		// Process all messages waiting on the channel
		air->poll_till_empty();

		// do game logic
	};

}
```

#### Option B: Object-only game behavior ####
```cpp

	air->poll_forever();

}
```

### Defining the AI-behavior/-view for DistributedClasses ###
//TODO: Document and provide examples for subclassing the DistributedObject class.
