#pragma once
#include "InternalConnection.h"
#include "objects/ObjectRepository.h"
namespace astron { // open namespace


class InternalRepository : public ObjectRepository, public InternalConnection
{
	public:
		// Manual constructor.
		InternalRepository();

		// Autorun AI constructors: Connects to the cluster and subscribes to the channel argument.
		InternalRepository(channel_t channel);
		InternalRepository(channel_t channel, std::string addr, uint16_t port);

		// Autorun object constructor: Attaches the object argument, connects to the cluster,
		//                             and subscribes to the object's id.
		InternalRepository(DistributedObject* object);
		InternalRepository(DistributedObject* object, std::string addr, uint16_t port);

		// attach_object adds a locally hosted object to the repository.
		//     Typically this is called when hosting an UberDOG.
		void attach_object(DistributedObject* object);

		// attach_object_view subscribes to a remotely hosted object with view argument.
		//     Typically this is used to listen to an UberDOG.
		void attach_object_view(DistributedObject view);



		/** IN PROGRESS API **/
		// generate_object creates a stateserver object initialized from the view argument.
		void generate_object(DistributedObject view, channel_t stateserver);

		// activates an object onto a dbss
		void activate_object(doid_t object_id);

	private:
		void handle_object_entry(...);
};


} // close namespace
