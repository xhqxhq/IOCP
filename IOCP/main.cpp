// IOCP.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <list>
#include "IOCP.h"
#include "XhqSocket.h"
#define  OP_ACCEPT 1
typedef struct tagMyOverLapped
{
    WSAOVERLAPPED m_WsaOverLapped;
    int           m_nOperation;
    SOCKET        m_ClientSocket;
}MYOVERLAPPED;

int _tmain(int argc, _TCHAR* argv[])
{
  CXhqSocket::StartUp(2, 2);
  CIOCP Iocp;
  Iocp.CreateIoCompletionPortObj();
  SOCKET server;

  SOCKADDR_IN skAddr;
  DWORD dwRetBytes = 0;
  char chBuff[1024] = { 0 };
  DWORD dwRecvBytes = 0;
  server = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

  std::list<MYOVERLAPPED*> lstClient;
  skAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
  skAddr.sin_port = htons(5166);
  int nRet = ::bind(server, (sockaddr*)&skAddr, sizeof(skAddr));
	
  BOOL bRet = Iocp.AssociateDeviceObjWithPortObj((HANDLE)server);

  
  LPFN_ACCEPTEX lpAcceptEx = NULL;
  GUID GuidAccpetEx = WSAID_ACCEPTEX;
  nRet = WSAIoctl(server,
                  SIO_GET_EXTENSION_FUNCTION_POINTER,
                  &GuidAccpetEx,
                  sizeof(GUID),
                  &lpAcceptEx,
                  sizeof(LPFN_ACCEPTEX),
                  &dwRetBytes,
                  NULL,
                  NULL);

  if (nRet != 0)
  {
    return -1;
  }

  nRet = ::listen(server, 200);
  bool bIsReciveConnect = true;
  MYOVERLAPPED* pNewClient = NULL; new MYOVERLAPPED;

  while (true)
  {
    if (bIsReciveConnect)
    {
        SOCKET client;
        client = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        pNewClient = new MYOVERLAPPED;
        pNewClient->m_ClientSocket = client;
        pNewClient->m_nOperation = OP_ACCEPT;
        bRet = lpAcceptEx(server,
                          client,
                          chBuff,
                          1024 - (sizeof(SOCKADDR_IN) + 16) * 2,
                          sizeof(SOCKADDR_IN) + 16,
                          sizeof(SOCKADDR_IN) + 16,
                          &dwRecvBytes,
                          &pNewClient->m_WsaOverLapped);
        if (!bRet)
        {
            printf("AccpetEx Failed Error Code:[%d]", GetLastError());
        }
    }
  }

  CXhqSocket::CleanUp();

  return 0;
}

