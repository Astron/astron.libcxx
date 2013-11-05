#pragma once
namespace astron { // open namespace

class Shard
{
	public:
		Shard(InternalRepository* air, // the internal repository to attach object-views to
		      channel_t ai_channel, // the ai channel to receive object updates on
		      channel_t stateserver = INVALID_CHANNEL, // the stateserver to generate objects on
		      doid_t min_doid = INVALID_DO_ID, doid_t max_doid = INVALID_DO_ID); // doid gen range

		// generate_object creates a stateserver object initialized from the view argument.
		void generate_object(DistributedObject view);

		// activates an object onto a dbss
		void activate_object(doid_t object_id);
};


} // close namespace