#pragma once
#include <windows.h>
#include <list>

typedef struct tagClientInfo
{
public:
    HANDLE              m_hClient;   //客户端套接字
    sockaddr_in         m_skAddrClient; //客户端地址信息
    CRITICAL_SECTION    m_Locker;    //节点同步锁
public:
    void InitClientInfo(HANDLE hClient, sockaddr_in & sockAddr); //初始化客户端结构信息
    inline void Lock(); //对结构上锁
    inline void UnLock(); //对结构解锁
    void ReleaseClientInfo(); //释放该节点
    tagClientInfo();
}CLIENT_INFO, *PCLIENT_INFO;


class ClientInfoManager
{
public:
    BOOL AddNewClientInfo(PCLIENT_INFO pClientInfo); //添加一个新的客户端信息
    BOOL RemoveClientInfo(PCLIENT_INFO pClientInfo); //删除一个客户端节点
public:
    ClientInfoManager();
    virtual ~ClientInfoManager();
private:
    //客户端信息列表
    std::list<CLIENT_INFO*> m_lstClients; 

    //用于同步对m_lstClients的增删操作
    CRITICAL_SECTION        m_Locker;

    //客户端个数
    DWORD                   m_dwClientCount;
};

