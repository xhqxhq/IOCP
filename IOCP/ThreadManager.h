#pragma once
#include <list>
#include <windows.h>
#include "IOCP.h"
typedef struct tagThreadInfo
{
    DWORD m_dwThreadId;     //线程ID
    HANDLE m_hThread;       //线程句柄
    BOOL  m_bIsTerminate;   //是否终止线程
    CIOCP m_Iocp;          //完成端口对象(该线程所要等待的完成端口对象)
}THREAD_INFO,*PTHREAD_INFO;


class ThreadManager
{
public:
    BOOL InitThreads(DWORD dwThreadsCount); //初始化线程
public:
    ThreadManager();
    virtual ~ThreadManager();
private:
    std::list<PTHREAD_INFO> m_lstThreadInfo;
private:
    static DWORD __stdcall ThreadCallBack(void* pParam); //线程回调函数
};

