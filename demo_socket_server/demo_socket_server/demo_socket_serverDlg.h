
// demo_socket_serverDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"

#include "ServerSocket.h"
class CServerSocket;

// Cdemo_socket_serverDlg �Ի���
class Cdemo_socket_serverDlg : public CDialogEx
{
// ����
public:
	Cdemo_socket_serverDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DEMO_SOCKET_SERVER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	bool m_connect;
	int m_userCount;
	CServerSocket* listenSocket;
	CPtrList m_clientList;
	void AddClient(void);
	void RemoveClient(CServerSocket* pSocket);
	void RecvData(CServerSocket* pSocket);
	void UpdateEvent(CString str);
	BOOL WChar2MByte(LPCWSTR srcBuff, LPSTR destBuff, int nlen);
	void SendMSG(CString str);       // ������Ϣ�������ͻ���
	
public:
	CEdit m_CEPortNum;
	CEdit m_CELog;
	CStatic m_CSEUserNum;
	afx_msg void OnBnClickedButtonServerstart();
};
