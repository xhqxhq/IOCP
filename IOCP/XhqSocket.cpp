#include "XhqSocket.h"


CXhqSocket::CXhqSocket():
m_socket(INVALID_SOCKET)
{
}


CXhqSocket::~CXhqSocket()
{

}



//************************************************************************
// 函数名称: CXhqSocket::StartUp
// 访问权限: public 
// 函数功能: 装载Winsock库
// 返回值:   成功返回true,失败返回false
// 参数:     BYTE majorVersion:Winsock库的主版本号
// 参数:     BYTE minVersion:Winsock库的次版本号
// 参数:      WSADATA * wsaData:接收Winsock库版本信息,为NULL时表示不接收
// 注意:     1.WSAClean由析构函数自动调用
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
// 函数名称: CXhqSocket::GetAllLocalHostIPAndHostName
// 访问权限: public static 
// 函数功能: 获取本机的所有IP地址和主机名称
// 返回值:   成功返回true,失败返回false
// 参数:     OUT hostent * & ipList：传出本地所有IP地址列表
// 注意:    
//************************************************************************
bool CXhqSocket::GetAllLocalHostIP(OUT hostent *& ipList)
{
  //获取本地主机名称
  char szHostName[MAX_PATH] = { 0 };
  int nRet = ::gethostname(szHostName, MAX_PATH);

  if (nRet == SOCKET_ERROR)
  {
    return false;
  }

  //通过主机名称获取IP地址信息
  ipList = ::gethostbyname(szHostName);
  return (ipList == NULL) ? true : false;
}



//************************************************************************
// 函数名称: CXhqSocket::CreateSocket
// 访问权限: public 
// 函数功能: 创建套接字
// 返回值:   成功返回true,失败返回false
// 参数:     IN int type:套接字类型
// 参数:     IN int protocol:协议类型
// 注意:     
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
// 函数名称: CXhqSocket::CloseSocket
// 访问权限: public 
// 函数功能: 关闭套接字
// 返回值:   成功返回true,失败返回false
// 注意:     
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
// 函数名称: CXhqSocket::BindSocket
// 访问权限: public 
// 函数功能: 为m_socket绑定IP地址和端口号
// 返回值:   int:参考bind函数返回值
// 参数:     IN char * pIpAddress:点分十进制的IP地址(例如"192.168.0.1"),为NULL时系统会自动使用当前
//                                主机配置的IP地址.
// 参数:     IN int nPortNum:端口号,为0时表示由系统自动分配
// 注意:     
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
// 函数名称: CXhqSocket::ListenSocket
// 访问权限: public 
// 函数功能: 监听m_socket套接字
// 返回值:   int
// 注意:     
//************************************************************************
int CXhqSocket::ListenSocket()
{
  int nRet = ::listen(m_socket, SOMAXCONN);
  return nRet;
}



//************************************************************************
// 函数名称: CXhqSocket::AccpetConnect
// 访问权限: public 
// 函数功能: 接收新的连接
// 返回值:   SOCKET:同accept
// 参数:     IN_OUT sockaddr_in & newConSocket
// 注意:     
//************************************************************************
SOCKET CXhqSocket::AccpetConnect(IN_OUT sockaddr_in & newConSocket)
{
  SOCKET newSocket = INVALID_SOCKET;
  int nLen = sizeof(sockaddr_in);
  newSocket = ::accept(m_socket, (sockaddr*)&newConSocket, &nLen);
  return newSocket;
}


//************************************************************************
// 函数名称: CXhqSocket::RecvData
// 访问权限: public 
// 函数功能: 接收发送给m_socket的数据
// 返回值:   int
// 参数:     CXhqBuff & buff:返回的数据
// 参数:     int nFlags:参考recv函数的flags的参数说明
// 注意:     
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
// 函数名称: CXhqSocket::SendData
// 访问权限: public 
// 函数功能: 
// 返回值:   int:参考send
// 参数:     SOCKET dstSocket:数据接收方的套接字
// 参数:     CXhqBuff & buff:存放要发送数据的缓冲取
// 参数:     int nFlags:参考send
// 注意:     
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
// 函数名称: CXhqSocket::Connect
// 访问权限: public 
// 函数功能: 连接到指定IP
// 返回值:   int:参考connect
// 参数:     IN sockaddr_in & DstAddr
// 注意:     
//************************************************************************
int CXhqSocket::Connect(IN sockaddr_in & DstAddr)
{
  int nRet = 0;
  nRet = ::connect(m_socket, (sockaddr*)&DstAddr, sizeof(sockaddr_in));
  return nRet;
}





