#include "repository/ObjectRepository.h"
#include "connection/InternalConnection.h"

class InternalRepository : public ObjectRepository, InternalConnection;