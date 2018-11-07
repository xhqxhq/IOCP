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
// ��������:  tagClientInfo::InitClientInfo
// ��������:  ��ʼ���ͻ�����Ϣ�ڵ�
// ����Ȩ��:  public 
// ���ص�ֵ:  void
// ��������:  HANDLE hClient:�ͻ����׽���
// ��������:  sockaddr_in & sockAddr:�ͻ��˵�ַ��Ϣ�ṹ��
// ע������:  1.�ýṹ���������ɺ������ʹ�ñ��������г�ʼ��
//************************************************************************
void tagClientInfo::InitClientInfo(HANDLE hClient, sockaddr_in & sockAddr)
{
    m_hClient = hClient;
    m_skAddrClient = sockAddr;
    ::InitializeCriticalSection(&m_Locker);
}




//************************************************************************
// ��������:  tagClientInfo::Lock
// ��������:  �Ըýṹ������������
// ����Ȩ��:  public 
// ���ص�ֵ:  void
// ע������:  1.���ʽṹ��ʱ�����ȵ��ñ�������������,������ɺ��ڵ���UnLock���н���
//************************************************************************
void tagClientInfo::Lock()
{
    ::EnterCriticalSection(&m_Locker);
}


//************************************************************************
// ��������:  tagClientInfo::UnLock
// ��������:  �Խṹ��������
// ����Ȩ��:  public 
// ���ص�ֵ:  void
// ע������:  1.ʹ�ýṹ�����ʱ�ȵ���Lock��������,ʹ����ɺ�ʹ�ñ���������
//************************************************************************
void tagClientInfo::UnLock()
{
    ::LeaveCriticalSection(&m_Locker);
}


//************************************************************************
// ��������:  tagClientInfo::ReleaseClientInfo
// ��������:  �ͷſͻ�����Ϣ�ڵ�
// ����Ȩ��:  public 
// ���ص�ֵ:  void
// ע������: 
//************************************************************************
void tagClientInfo::ReleaseClientInfo()
{
    ::closesocket((SOCKET)m_hClient);
    ::DeleteCriticalSection(&m_Locker);
}



//************************************************************************
// ��������:  ClientInfoManager::AddNewClientInfo
// ��������:  ���һ���µĿͻ�����Ϣ
// ����Ȩ��:  public 
// ���ص�ֵ:  BOOL
// ��������:  PCLIENT_INFO pClientInfo
// ע������: 
//************************************************************************
BOOL ClientInfoManager::AddNewClientInfo(PCLIENT_INFO pClientInfo)
{
    ::EnterCriticalSection(&m_Locker);
    m_lstClients.push_back(pClientInfo);
    m_dwClientCount++;
    ::LeaveCriticalSection(&m_Locker);
}


//************************************************************************
// ��������:  ClientInfoManager::RemoveClientInfo
// ��������:  ɾ��һ���ͻ��˽ڵ�
// ����Ȩ��:  public 
// ���ص�ֵ:  BOOL:�ɹ�����TRUE,���򷵻�FALSE
// ��������:  PCLIENT_INFO pClientInfo:ָ��Ҫɾ���Ľڵ�
// ע������: 
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



