#include "repository/ObjectRepository.h"
#include "connection/InternalConnection.h"

class InternalRepository : public ObjectRepository, InternalConnection
{
	public:
		// Manual constructor.
		InternalRepository();

		// Autorun constructor: Connects to the cluster and subscribes to the channel argument.
		InternalRepository(std::string addr, uint16_t port, channel_t channel);

		// attach_object adds a locally hosted object to the repository.
		//     Typically this is called when hosting an UberDOG.
		void attach_object(DistributedObject object);

		// attach_object_view subscribes to a remotely hosted object with view argument.
		//     Typically this is used to listen to an UberDOG.
		void attach_object_view(DistributedObject view);

		// create_distributed_view creates a stateserver object initialized from the view argument.
		void create_distributed_view(DistributedObject view);

	private:
		void handle_object_entry(...);
};
