
// demo_socket_serverDlg.h : 头文件
//

#pragma once
#include "afxwin.h"

#include "ServerSocket.h"
class CServerSocket;

// Cdemo_socket_serverDlg 对话框
class Cdemo_socket_serverDlg : public CDialogEx
{
// 构造
public:
	Cdemo_socket_serverDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DEMO_SOCKET_SERVER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
	void SendMSG(CString str);       // 发送消息给各个客户端
	
public:
	CEdit m_CEPortNum;
	CEdit m_CELog;
	CStatic m_CSEUserNum;
	afx_msg void OnBnClickedButtonServerstart();
};
