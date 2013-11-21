#pragma once
#include <string>
namespace astron { // open namespace


class DistributedObject
{
	public:
		~DistributedObject();

		inline std::string get_dclass_name()
		{
			return m_dclass_name;
		}

	protected:
		DistributedObject(std::string dclass_name);

	private:
		std::string m_dclass_name;
};


} // close namespace
