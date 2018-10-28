#include "stdafx.h"
#include "IOCP.h"


CIOCP::CIOCP():
m_hIoCompletionPortObj(INVALID_HANDLE_VALUE)
{
}


CIOCP::~CIOCP()
{
}



//************************************************************************
// 函数名称:  CIOCP::CreateIoCompletionPortObj
// 函数功能:  创建一个完成端口对象
// 所属权限:  public 
// 返回的值:  BOOL:成功返回TRUE,否则返回FALSE
// 函数参数:  DWORD dwNumberOfCurrentThreads:最大允许的线程数
// 注意事项:  
//************************************************************************
BOOL CIOCP::CreateIoCompletionPortObj(DWORD dwNumberOfCurrentThreads /*= 0*/)
{
  HANDLE hIoCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, dwNumberOfCurrentThreads);
  if (hIoCompletionPort == NULL)
  {
    return FALSE;
  }

  m_hIoCompletionPortObj = hIoCompletionPort;
  return TRUE;
}



//************************************************************************
// 函数名称:  CIOCP::AssociateDeviceObjWithPortObj
// 函数功能:  将一个设备对象或者套接字对象和已存在的IO完成端口进行绑定
// 所属权限:  public 
// 返回的值:  BOOL 成功返回TRUE,否则返回FALSE
// 函数参数:  HANDLE hDevice:设备或套接字对象句柄
// 函数参数:  ULONG_PTR CompKey:完成键
// 注意事项: 
//************************************************************************
BOOL CIOCP::AssociateDeviceObjWithPortObj(HANDLE hDevice, ULONG_PTR CompKey)
{
  HANDLE hIoCompletionPort = CreateIoCompletionPort(hDevice, m_hIoCompletionPortObj, CompKey, 0);
  if (hIoCompletionPort != m_hIoCompletionPortObj)
  {
    return FALSE;
  }
  return TRUE;
}


//************************************************************************
// 函数名称:  CIOCP::PostQueuedIoCompletionStatus
// 函数功能:  向完成端口发送一个已经完成的IO请求
// 所属权限:  public 
// 返回的值:  BOOL 成功返回TRUE,否则返回FALSE
// 函数参数:  ULONG_PTR CompletionKey
// 函数参数:  DWORD dwNumBytes:
// 函数参数:  OVERLAPPED * pOverlapped
// 注意事项: 
//************************************************************************
BOOL CIOCP::PostQueuedIoCompletionStatus(ULONG_PTR CompletionKey, DWORD dwNumBytes /*= 0*/, OVERLAPPED* pOverlapped /*= NULL*/)
{
  BOOL bRet = FALSE;
  bRet = PostQueuedCompletionStatus(m_hIoCompletionPortObj, dwNumBytes, CompletionKey, pOverlapped);
  return bRet;
}
