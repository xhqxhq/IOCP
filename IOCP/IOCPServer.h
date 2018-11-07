#pragma once
#include "IOCP.h"
#include "ClientInfoManager.h"


class IOCPServer
{
public:
    
public:
    IOCPServer();
    virtual ~IOCPServer();
private:
    CIOCP               m_Iocp;
    ClientInfoManager   m_ClientManager;
};

