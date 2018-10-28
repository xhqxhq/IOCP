// IOCP.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "IOCP.h"
#include "XhqSocket.h"
int _tmain(int argc, _TCHAR* argv[])
{
  CXhqSocket::StartUp(2, 2);
  CIOCP Iocp;
  Iocp.CreateIoCompletionPortObj();
  SOCKET server;
  SOCKADDR_IN skAddr;
  DWORD dwRetBytes = 0;
  server = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  skAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
  skAddr.sin_port = htons(5166);
  int nRet = ::bind(server, (sockaddr*)&skAddr, sizeof(skAddr));
	
  BOOL bRet = Iocp.AssociateDeviceObjWithPortObj((HANDLE)server);
  nRet = ::listen(server, 200);
  
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
  CXhqSocket::CleanUp();

  return 0;
}

