#pragma  once

#include <windows.h>

class CIOCP
{
public:
    CIOCP();
    ~CIOCP();
    //创建一个完成端口对象
    BOOL CreateIoCompletionPortObj(DWORD dwNumberOfCurrentThreads = 0);

    //将一个设备对象或者套接字对象和已存在的IO完成端口进行绑定
    BOOL AssociateDeviceObjWithPortObj(HANDLE hDevice, ULONG_PTR CompKey);

    //向完成端口发送一个已经完成的IO请求
    BOOL PostQueuedIoCompletionStatus(ULONG_PTR CompletionKey,
                                      DWORD dwNumBytes = 0,
                                      OVERLAPPED* pOverlapped = NULL);

private:
    HANDLE m_hIoCompletionPortObj;
};


