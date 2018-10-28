#pragma once
#include <winsock2.h>
#include<mswsock.h>
#include "../../XhqBuff.h"
#pragma  comment(lib,"WS2_32")
#pragma  comment(lib,"mswsock")
#define IN          //˵���Ժ�:��ʾ�ò���Ϊ�������
#define OUT         //˵���Ժ�:��ʾ�ò���Ϊ��������
#define IN_OUT      //˵���Ժ�:��ʾ�ò���Ϊ���봫������

class CXhqSocket
{
public:
  CXhqSocket();

  virtual ~CXhqSocket();

  //װ��Winsock��,����:���汾,�ΰ汾,Dll�����ϸ��Ϣ
  static bool StartUp(IN BYTE majorVersion, IN BYTE minVersion, OUT WSADATA * pWsaData = NULL);

  //�ͷ�Winsock��
  static bool CleanUp(){ ::WSACleanup(); return true; };
  //��ȡ����������IP��ַ
  static bool GetAllLocalHostIP(OUT hostent *& ipList);

  //�����׽���
  bool CreateSocket(IN int type, IN int protocol = IPPROTO_IP);

  //���׽���
  int BindSocket(IN char* pIpAddress = NULL,IN int nPortNum = 0);

  //����
  int ListenSocket();

  //�����µ�����
  SOCKET AccpetConnect(IN_OUT sockaddr_in & newConSocket);

  //���ӵ�ָ��IP
  int Connect(IN sockaddr_in & DstAddr);

  //��������
  int RecvData(CXhqBuff & buff,int nFlags);

  //��������
  int SendData(SOCKET dstSocket, CXhqBuff & buff, int nFlags = MSG_OOB);

  //�ر��׽���
  bool CloseSocket();
private:
  SOCKET m_socket;
  sockaddr_in m_sockAddr;
};

