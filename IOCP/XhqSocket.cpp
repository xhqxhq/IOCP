#include "XhqSocket.h"


CXhqSocket::CXhqSocket():
m_socket(INVALID_SOCKET)
{
}


CXhqSocket::~CXhqSocket()
{

}



//************************************************************************
// ��������: CXhqSocket::StartUp
// ����Ȩ��: public 
// ��������: װ��Winsock��
// ����ֵ:   �ɹ�����true,ʧ�ܷ���false
// ����:     BYTE majorVersion:Winsock������汾��
// ����:     BYTE minVersion:Winsock��Ĵΰ汾��
// ����:      WSADATA * wsaData:����Winsock��汾��Ϣ,ΪNULLʱ��ʾ������
// ע��:     1.WSAClean�����������Զ�����
//************************************************************************
bool CXhqSocket::StartUp(IN BYTE majorVersion, IN BYTE minVersion, OUT WSADATA * pWsaData)
{
  int nRet = 0;

  WSADATA data;
  nRet = ::WSAStartup(MAKEWORD(minVersion, majorVersion), &data);
  if (pWsaData != NULL)
  {
    memset(pWsaData, 0, sizeof(WSADATA));
    memcpy(pWsaData, &data, sizeof(WSADATA));
  }

  return (nRet == 0) ? true:false;
}



//************************************************************************
// ��������: CXhqSocket::GetAllLocalHostIPAndHostName
// ����Ȩ��: public static 
// ��������: ��ȡ����������IP��ַ����������
// ����ֵ:   �ɹ�����true,ʧ�ܷ���false
// ����:     OUT hostent * & ipList��������������IP��ַ�б�
// ע��:    
//************************************************************************
bool CXhqSocket::GetAllLocalHostIP(OUT hostent *& ipList)
{
  //��ȡ������������
  char szHostName[MAX_PATH] = { 0 };
  int nRet = ::gethostname(szHostName, MAX_PATH);

  if (nRet == SOCKET_ERROR)
  {
    return false;
  }

  //ͨ���������ƻ�ȡIP��ַ��Ϣ
  ipList = ::gethostbyname(szHostName);
  return (ipList == NULL) ? true : false;
}



//************************************************************************
// ��������: CXhqSocket::CreateSocket
// ����Ȩ��: public 
// ��������: �����׽���
// ����ֵ:   �ɹ�����true,ʧ�ܷ���false
// ����:     IN int type:�׽�������
// ����:     IN int protocol:Э������
// ע��:     
//************************************************************************
bool CXhqSocket::CreateSocket(IN int type, IN int protocol /*= IPPROTO_IP*/)
{
  m_socket = ::socket(AF_INET, type, protocol);
  if (m_socket == INVALID_SOCKET)
  {
    return false;
  }
  return true;
}


//************************************************************************
// ��������: CXhqSocket::CloseSocket
// ����Ȩ��: public 
// ��������: �ر��׽���
// ����ֵ:   �ɹ�����true,ʧ�ܷ���false
// ע��:     
//************************************************************************
bool CXhqSocket::CloseSocket()
{
  if (m_socket != INVALID_SOCKET)
  {
    int nRet = ::closesocket(m_socket);
    if (nRet == 0)
    {
      m_socket = INVALID_SOCKET;
      return true;
    }
    return false;
  }
  return false;
}



//************************************************************************
// ��������: CXhqSocket::BindSocket
// ����Ȩ��: public 
// ��������: Ϊm_socket��IP��ַ�Ͷ˿ں�
// ����ֵ:   int:�ο�bind��������ֵ
// ����:     IN char * pIpAddress:���ʮ���Ƶ�IP��ַ(����"192.168.0.1"),ΪNULLʱϵͳ���Զ�ʹ�õ�ǰ
//                                �������õ�IP��ַ.
// ����:     IN int nPortNum:�˿ں�,Ϊ0ʱ��ʾ��ϵͳ�Զ�����
// ע��:     
//************************************************************************
int CXhqSocket::BindSocket(IN char* pIpAddress /*= NULL*/, IN int nPortNum /*= 0*/)
{
  int nRet = 0;
  memset(&m_sockAddr, 0, sizeof(m_sockAddr));
  
  if (pIpAddress == NULL)
  {
    m_sockAddr.sin_addr.S_un.S_addr = INADDR_ANY;
  }
  else
  {
    m_sockAddr.sin_addr.S_un.S_addr = inet_addr(pIpAddress);
  }

  m_sockAddr.sin_port = htons(nPortNum);
  m_sockAddr.sin_family = AF_INET;

  nRet = ::bind(m_socket, (const sockaddr*)&m_sockAddr, sizeof(sockaddr_in));

  return nRet;
}



//************************************************************************
// ��������: CXhqSocket::ListenSocket
// ����Ȩ��: public 
// ��������: ����m_socket�׽���
// ����ֵ:   int
// ע��:     
//************************************************************************
int CXhqSocket::ListenSocket()
{
  int nRet = ::listen(m_socket, SOMAXCONN);
  return nRet;
}



//************************************************************************
// ��������: CXhqSocket::AccpetConnect
// ����Ȩ��: public 
// ��������: �����µ�����
// ����ֵ:   SOCKET:ͬaccept
// ����:     IN_OUT sockaddr_in & newConSocket
// ע��:     
//************************************************************************
SOCKET CXhqSocket::AccpetConnect(IN_OUT sockaddr_in & newConSocket)
{
  SOCKET newSocket = INVALID_SOCKET;
  int nLen = sizeof(sockaddr_in);
  newSocket = ::accept(m_socket, (sockaddr*)&newConSocket, &nLen);
  return newSocket;
}


//************************************************************************
// ��������: CXhqSocket::RecvData
// ����Ȩ��: public 
// ��������: ���շ��͸�m_socket������
// ����ֵ:   int
// ����:     CXhqBuff & buff:���ص�����
// ����:     int nFlags:�ο�recv������flags�Ĳ���˵��
// ע��:     
//************************************************************************
int CXhqSocket::RecvData(CXhqBuff & buff, int nFlags)
{
  int nRet = 0;
  int nBuffSize = 0;
  char* pBuff = (char*)buff.GetBuffPointer();
  nBuffSize = buff.GetBuffSize();
  nRet = ::recv(m_socket, pBuff, nBuffSize, nFlags);
  if (nRet > 0)
  {
    buff.SetBuffDataLen(nRet);
  }

  return nRet;
}



//************************************************************************
// ��������: CXhqSocket::SendData
// ����Ȩ��: public 
// ��������: 
// ����ֵ:   int:�ο�send
// ����:     SOCKET dstSocket:���ݽ��շ����׽���
// ����:     CXhqBuff & buff:���Ҫ�������ݵĻ���ȡ
// ����:     int nFlags:�ο�send
// ע��:     
//************************************************************************
int CXhqSocket::SendData(SOCKET dstSocket, CXhqBuff & buff, int nFlags /*= MSG_OOB*/)
{
  int nRet = 0;
  char* pDataBuff = (char*)buff.GetBuffPointer();
  int nDataLen = buff.GetDataLen();
  nRet = ::send(dstSocket, pDataBuff, nDataLen,nFlags);
  return nRet;
}



//************************************************************************
// ��������: CXhqSocket::Connect
// ����Ȩ��: public 
// ��������: ���ӵ�ָ��IP
// ����ֵ:   int:�ο�connect
// ����:     IN sockaddr_in & DstAddr
// ע��:     
//************************************************************************
int CXhqSocket::Connect(IN sockaddr_in & DstAddr)
{
  int nRet = 0;
  nRet = ::connect(m_socket, (sockaddr*)&DstAddr, sizeof(sockaddr_in));
  return nRet;
}





