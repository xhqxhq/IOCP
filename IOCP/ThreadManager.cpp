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
// 函数名称:  ThreadManager::InitThreads
// 函数功能:  创建dwThreadsCount个线程
// 所属权限:  public 
// 返回的值:  BOOL:TRUE表示成功,否则失败
// 函数参数:  DWORD dwThreadsCount:要创建的线程个数
// 注意事项: 
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
        //如果有线程创建失败则关闭所有创建成功的线程在退出
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
// 函数名称:  ThreadManager::ThreadCallBack
// 函数功能:  线程回调函数
// 所属权限:  private static 
// 返回的值:  DWORD __stdcall
// 函数参数:  void * pParam
// 注意事项: 
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
