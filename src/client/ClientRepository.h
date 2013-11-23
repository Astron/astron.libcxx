#pragma once
#include "ClientConnection.h"
#include "objects/ObjectRepository.h"
namespace astron   // open namespace
{


class ClientRepository : public ObjectRepository, public ClientConnection
{
};


} // close namespace