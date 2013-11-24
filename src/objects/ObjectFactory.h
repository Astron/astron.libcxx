#pragma once
#include "DistributedObject.h"
#include <unordered_map>
namespace astron   // open namespace
{


class BaseObjectType
{
	public:
		virtual DistributedObject* instantiate(std::string name) = 0;
	protected:
		BaseObjectType(const std::string &name);
};

template <class T>
class ObjectType : public BaseObjectType
{
	public:
		ObjectType(const std::string &name) : BaseObjectType(name)
		{
		}

		virtual DistributedObject* instantiate(std::string name)
		{
			return new T(name);
		}
};

class ObjectFactory
{
	public:
		DistributedObject* instantiate_object(const std::string &dclass_name);
		static ObjectFactory singleton;

		void add_object_type(const std::string &name, BaseObjectType *factory);
	private:
		std::unordered_map<std::string, BaseObjectType*> m_factories;
};


} // close namespace
