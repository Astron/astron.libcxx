#include "repository/ObjectRepository.h"
#include "connection/ClientConnection.h"

class ClientRepository : public ObjectRepository, ClientConnection;