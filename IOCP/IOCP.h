#pragma  once

#include <windows.h>

class CIOCP
{
public:
    CIOCP();
    ~CIOCP();
    //����һ����ɶ˿ڶ���
    BOOL CreateIoCompletionPortObj(DWORD dwNumberOfCurrentThreads = 0);

    //��һ���豸��������׽��ֶ�����Ѵ��ڵ�IO��ɶ˿ڽ��а�
    BOOL AssociateDeviceObjWithPortObj(HANDLE hDevice, ULONG_PTR CompKey);

    //����ɶ˿ڷ���һ���Ѿ���ɵ�IO����
    BOOL PostQueuedIoCompletionStatus(ULONG_PTR CompletionKey,
                                      DWORD dwNumBytes = 0,
                                      OVERLAPPED* pOverlapped = NULL);

private:
    HANDLE m_hIoCompletionPortObj;
};


