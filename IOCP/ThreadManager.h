#pragma once
#include <list>
#include <windows.h>
#include "IOCP.h"
typedef struct tagThreadInfo
{
    DWORD m_dwThreadId;     //�߳�ID
    HANDLE m_hThread;       //�߳̾��
    BOOL  m_bIsTerminate;   //�Ƿ���ֹ�߳�
    CIOCP m_Iocp;          //��ɶ˿ڶ���(���߳���Ҫ�ȴ�����ɶ˿ڶ���)
}THREAD_INFO,*PTHREAD_INFO;


class ThreadManager
{
public:
    BOOL InitThreads(DWORD dwThreadsCount); //��ʼ���߳�
public:
    ThreadManager();
    virtual ~ThreadManager();
private:
    std::list<PTHREAD_INFO> m_lstThreadInfo;
private:
    static DWORD __stdcall ThreadCallBack(void* pParam); //�̻߳ص�����
};

