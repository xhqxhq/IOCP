#include "stdafx.h"
#include "ClientInfoManager.h"

ClientInfoManager::ClientInfoManager():
m_dwClientCount(0)
{
    ::InitializeCriticalSection(&m_Locker);

}


ClientInfoManager::~ClientInfoManager()
{
    ::DeleteCriticalSection(&m_Locker);
}


tagClientInfo::tagClientInfo():
m_hClient(INVALID_HANDLE_VALUE)
{
    ZeroMemory(&m_skAddrClient, sizeof(sockaddr_in));
}


//************************************************************************
// 函数名称:  tagClientInfo::InitClientInfo
// 函数功能:  初始化客户端信息节点
// 所属权限:  public 
// 返回的值:  void
// 函数参数:  HANDLE hClient:客户端套接字
// 函数参数:  sockaddr_in & sockAddr:客户端地址信息结构体
// 注意事项:  1.该结构体对象构造完成后必须先使用本函数进行初始化
//************************************************************************
void tagClientInfo::InitClientInfo(HANDLE hClient, sockaddr_in & sockAddr)
{
    m_hClient = hClient;
    m_skAddrClient = sockAddr;
    ::InitializeCriticalSection(&m_Locker);
}




//************************************************************************
// 函数名称:  tagClientInfo::Lock
// 函数功能:  对该结构体对象进行上锁
// 所属权限:  public 
// 返回的值:  void
// 注意事项:  1.访问结构体时必须先调用本函数进行上锁,访问完成后在调用UnLock进行解锁
//************************************************************************
void tagClientInfo::Lock()
{
    ::EnterCriticalSection(&m_Locker);
}


//************************************************************************
// 函数名称:  tagClientInfo::UnLock
// 函数功能:  对结构体对象解锁
// 所属权限:  public 
// 返回的值:  void
// 注意事项:  1.使用结构体对象时先调用Lock进行上锁,使用完成后使用本函数解锁
//************************************************************************
void tagClientInfo::UnLock()
{
    ::LeaveCriticalSection(&m_Locker);
}


//************************************************************************
// 函数名称:  tagClientInfo::ReleaseClientInfo
// 函数功能:  释放客户端信息节点
// 所属权限:  public 
// 返回的值:  void
// 注意事项: 
//************************************************************************
void tagClientInfo::ReleaseClientInfo()
{
    ::closesocket((SOCKET)m_hClient);
    ::DeleteCriticalSection(&m_Locker);
}



//************************************************************************
// 函数名称:  ClientInfoManager::AddNewClientInfo
// 函数功能:  添加一个新的客户端信息
// 所属权限:  public 
// 返回的值:  BOOL
// 函数参数:  PCLIENT_INFO pClientInfo
// 注意事项: 
//************************************************************************
BOOL ClientInfoManager::AddNewClientInfo(PCLIENT_INFO pClientInfo)
{
    ::EnterCriticalSection(&m_Locker);
    m_lstClients.push_back(pClientInfo);
    m_dwClientCount++;
    ::LeaveCriticalSection(&m_Locker);
}


//************************************************************************
// 函数名称:  ClientInfoManager::RemoveClientInfo
// 函数功能:  删除一个客户端节点
// 所属权限:  public 
// 返回的值:  BOOL:成功返回TRUE,否则返回FALSE
// 函数参数:  PCLIENT_INFO pClientInfo:指向要删除的节点
// 注意事项: 
//************************************************************************
BOOL ClientInfoManager::RemoveClientInfo(PCLIENT_INFO pClientInfo)
{
    std::list<PCLIENT_INFO>::iterator itBegin;
    itBegin = m_lstClients.begin();
    std::list<PCLIENT_INFO>::iterator itEnd;
    itEnd = m_lstClients.end();

    while (itBegin != itEnd)
    {
        if (*itBegin == pClientInfo)
        {
            ::EnterCriticalSection(&m_Locker);
            (*itBegin)->Lock();
            m_lstClients.remove(pClientInfo);
            (*itBegin)->UnLock();
            pClientInfo->ReleaseClientInfo();
            delete pClientInfo;
            m_dwClientCount--;
            ::LeaveCriticalSection(&m_Locker);
        }
    }
}



