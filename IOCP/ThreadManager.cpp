#include "stdafx.h"
#include "ThreadManager.h"
#include <process.h>
ThreadManager::ThreadManager()
{
}


ThreadManager::~ThreadManager()
{
}


//************************************************************************
// ��������:  ThreadManager::InitThreads
// ��������:  ����dwThreadsCount���߳�
// ����Ȩ��:  public 
// ���ص�ֵ:  BOOL:TRUE��ʾ�ɹ�,����ʧ��
// ��������:  DWORD dwThreadsCount:Ҫ�������̸߳���
// ע������: 
//************************************************************************
BOOL ThreadManager::InitThreads(DWORD dwThreadsCount)
{
    if (dwThreadsCount == 0)
    {
        return FALSE;
    }

    BOOL bIsFailed = FALSE;
    for (int iIndex = 0; iIndex < dwThreadsCount; iIndex++)
    {
        unsigned int dwThreadId = 0;
        int nRet = _beginthreadex(NULL, 0,
                                  (_beginthreadex_proc_type)ThreadManager::ThreadCallBack,
                                  NULL,
                                  CREATE_SUSPENDED,
                                  &dwThreadId);
        if (nRet == 0)
        {
            bIsFailed = TRUE;
            break;
        }

        PTHREAD_INFO pNewThread = new THREAD_INFO;
        pNewThread->m_dwThreadId = dwThreadId;
        pNewThread->m_hThread = (HANDLE)nRet;
        pNewThread->m_bIsTerminate = FALSE;
        m_lstThreadInfo.push_back(pNewThread);
    }


    if (bIsFailed)
    {
        //������̴߳���ʧ����ر����д����ɹ����߳����˳�
        while (!m_lstThreadInfo.empty())
        {
            PTHREAD_INFO pCurrThread = NULL;
            m_lstThreadInfo.push_back(pCurrThread);
            if (pCurrThread != NULL)
            {
                
            }
        }
    }
    return TRUE;
}





//************************************************************************
// ��������:  ThreadManager::ThreadCallBack
// ��������:  �̻߳ص�����
// ����Ȩ��:  private static 
// ���ص�ֵ:  DWORD __stdcall
// ��������:  void * pParam
// ע������: 
//************************************************************************
DWORD __stdcall ThreadManager::ThreadCallBack(void* pParam)
{
    PTHREAD_INFO pThisThread = (PTHREAD_INFO)pParam;
    
    DWORD dwNumOfBytes = 0;
    PULONG_PTR pCompeleteKey = NULL;
    pThisThread->m_Iocp.GetQueuedIoCompletionStatus(&dwNumOfBytes,
                                                    pCompeleteKey,
        )
    
}
