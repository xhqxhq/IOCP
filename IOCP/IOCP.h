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
    BOOL AssociateDeviceObjWithPortObj(HANDLE hDevice, ULONG_PTR CompKey = NULL);

    //����ɶ˿ڷ���һ���Ѿ���ɵ�IO����
    BOOL PostQueuedIoCompletionStatus(ULONG_PTR CompletionKey,
                                      DWORD dwNumBytes = 0,
                                      OVERLAPPED* pOverlapped = NULL);

    //����ɶ˿ڻ�ȡһ���Ѿ���ɵ�IO����
    BOOL GetQueuedIoCompletionStatus(LPDWORD lpNumberOfBytesTransferred,
                                     PULONG_PTR lpCompletionKey,
                                     LPOVERLAPPED * lpOverlapped,
                                     DWORD dwMilliseconds);
private:
    HANDLE m_hIoCompletionPortObj;  //��ɶ˿ڶ���
};


