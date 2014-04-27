#include "ObjectFactory.h"
namespace astron   // open namespace
{


ObjectFactory ObjectFactory::singleton;

BaseObjectType::BaseObjectType(const std::string &name)
{
	ObjectFactory::singleton.add_object_type(name, this);
}

void ObjectFactory::add_object_type(const std::string &name, BaseObjectType *factory)
{
	m_factories[name] = factory;
}

DistributedObject* ObjectFactory::instantiate_object(const std::string &dclass_name)
{
	if(m_factories.find(dclass_name) != m_factories.end())
	{
		return m_factories[dclass_name]->instantiate(dclass_name);
	}
	return NULL;
}


} // close namespace
