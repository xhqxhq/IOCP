#pragma once
#include <winsock2.h>
#include<mswsock.h>
#include "../../XhqBuff.h"
#pragma  comment(lib,"WS2_32")
#pragma  comment(lib,"mswsock")
#define IN          //说明性宏:表示该参数为传入参数
#define OUT         //说明性宏:表示该参数为传出参数
#define IN_OUT      //说明性宏:表示该参数为传入传出参数

class CXhqSocket
{
public:
  CXhqSocket();

  virtual ~CXhqSocket();

  //装载Winsock库,参数:主版本,次版本,Dll库的详细信息
  static bool StartUp(IN BYTE majorVersion, IN BYTE minVersion, OUT WSADATA * pWsaData = NULL);

  //释放Winsock库
  static bool CleanUp(){ ::WSACleanup(); return true; };
  //获取本机的所有IP地址
  static bool GetAllLocalHostIP(OUT hostent *& ipList);

  //创建套接字
  bool CreateSocket(IN int type, IN int protocol = IPPROTO_IP);

  //绑定套接字
  int BindSocket(IN char* pIpAddress = NULL,IN int nPortNum = 0);

  //监听
  int ListenSocket();

  //接收新的连接
  SOCKET AccpetConnect(IN_OUT sockaddr_in & newConSocket);

  //连接到指定IP
  int Connect(IN sockaddr_in & DstAddr);

  //接收数据
  int RecvData(CXhqBuff & buff,int nFlags);

  //发送数据
  int SendData(SOCKET dstSocket, CXhqBuff & buff, int nFlags = MSG_OOB);

  //关闭套接字
  bool CloseSocket();
private:
  SOCKET m_socket;
  sockaddr_in m_sockAddr;
};

