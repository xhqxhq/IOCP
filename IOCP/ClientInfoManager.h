#pragma once
#include <windows.h>
#include <list>

typedef struct tagClientInfo
{
public:
    HANDLE              m_hClient;   //�ͻ����׽���
    sockaddr_in         m_skAddrClient; //�ͻ��˵�ַ��Ϣ
    CRITICAL_SECTION    m_Locker;    //�ڵ�ͬ����
public:
    void InitClientInfo(HANDLE hClient, sockaddr_in & sockAddr); //��ʼ���ͻ��˽ṹ��Ϣ
    inline void Lock(); //�Խṹ����
    inline void UnLock(); //�Խṹ����
    void ReleaseClientInfo(); //�ͷŸýڵ�
    tagClientInfo();
}CLIENT_INFO, *PCLIENT_INFO;


class ClientInfoManager
{
public:
    BOOL AddNewClientInfo(PCLIENT_INFO pClientInfo); //���һ���µĿͻ�����Ϣ
    BOOL RemoveClientInfo(PCLIENT_INFO pClientInfo); //ɾ��һ���ͻ��˽ڵ�
public:
    ClientInfoManager();
    virtual ~ClientInfoManager();
private:
    //�ͻ�����Ϣ�б�
    std::list<CLIENT_INFO*> m_lstClients; 

    //����ͬ����m_lstClients����ɾ����
    CRITICAL_SECTION        m_Locker;

    //�ͻ��˸���
    DWORD                   m_dwClientCount;
};

