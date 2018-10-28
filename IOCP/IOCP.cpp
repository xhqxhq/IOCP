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
// ��������:  CIOCP::CreateIoCompletionPortObj
// ��������:  ����һ����ɶ˿ڶ���
// ����Ȩ��:  public 
// ���ص�ֵ:  BOOL:�ɹ�����TRUE,���򷵻�FALSE
// ��������:  DWORD dwNumberOfCurrentThreads:���������߳���
// ע������:  
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
// ��������:  CIOCP::AssociateDeviceObjWithPortObj
// ��������:  ��һ���豸��������׽��ֶ�����Ѵ��ڵ�IO��ɶ˿ڽ��а�
// ����Ȩ��:  public 
// ���ص�ֵ:  BOOL �ɹ�����TRUE,���򷵻�FALSE
// ��������:  HANDLE hDevice:�豸���׽��ֶ�����
// ��������:  ULONG_PTR CompKey:��ɼ�
// ע������: 
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
// ��������:  CIOCP::PostQueuedIoCompletionStatus
// ��������:  ����ɶ˿ڷ���һ���Ѿ���ɵ�IO����
// ����Ȩ��:  public 
// ���ص�ֵ:  BOOL �ɹ�����TRUE,���򷵻�FALSE
// ��������:  ULONG_PTR CompletionKey
// ��������:  DWORD dwNumBytes:
// ��������:  OVERLAPPED * pOverlapped
// ע������: 
//************************************************************************
BOOL CIOCP::PostQueuedIoCompletionStatus(ULONG_PTR CompletionKey, DWORD dwNumBytes /*= 0*/, OVERLAPPED* pOverlapped /*= NULL*/)
{
  BOOL bRet = FALSE;
  bRet = PostQueuedCompletionStatus(m_hIoCompletionPortObj, dwNumBytes, CompletionKey, pOverlapped);
  return bRet;
}
