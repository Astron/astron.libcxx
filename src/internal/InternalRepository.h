#pragma once
#include "InternalConnection.h"
#include "objects/ObjectRepository.h"
#include "objects/DistributedObject.h"
#include <chrono>
namespace astron   // open namespace
{


class InternalRepository : public ObjectRepository, protected InternalConnection
{
	public:
		// Manual constructor.
		InternalRepository();
		// Auto-connect constructors: Connects to the cluster with the given addr and port
		InternalRepository(std::string host, uint16_t port);
		InternalRepository(std::string addr);
		// Auto-connect as object constructors: Attaches the object argument, connects to
		//                                      the cluster, and subscribes to the object's id.
		InternalRepository(DistributedObject* object);
		InternalRepository(DistributedObject* object, std::string host, uint16_t port);
		InternalRepository(DistributedObject* object, std::string addr);

		/* Publicly inherit InternalConnection's 'connect' methods */
		using InternalConnection::connect;

		// poll_forever blocks forever and processes datagrams as they come in
		void poll_forever();

		// poll_till_empty receives datagrams from the cluster until the receive buffer is empty.
		void poll_till_empty();
		void poll_till_empty(std::chrono::nanoseconds timeout);

		// attach_object adds a locally hosted object to the repository.
		//     Typically this is called when hosting an UberDOG.
		void attach_object(DistributedObject* object);
		// attach_object_view subscribes to a remotely hosted object with view argument.
		//     Typically this is used to listen to an UberDOG.
		void attach_object_view(DistributedObject* view);

	private:
		void handle_object_entry(...);
};


} // close namespace
