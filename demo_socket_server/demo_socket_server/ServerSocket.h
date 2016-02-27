#pragma once
#include "afxsock.h"
#include "demo_socket_serverDlg.h"

class Cdemo_socket_serverDlg;

class CServerSocket :
	public CSocket
{
public:
	Cdemo_socket_serverDlg* m_pDlg;
	CServerSocket();
	~CServerSocket();
	virtual void OnAccept(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
};

