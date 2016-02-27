
// demo_socket_serverDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "demo_socket_server.h"
#include "demo_socket_serverDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Cdemo_socket_serverDlg 对话框



Cdemo_socket_serverDlg::Cdemo_socket_serverDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DEMO_SOCKET_SERVER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Cdemo_socket_serverDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_PortNumber, m_CEPortNum);
	DDX_Control(pDX, IDC_EDIT_Log, m_CELog);
	DDX_Control(pDX, IDC_STATIC_NUM, m_CSEUserNum);
}

BEGIN_MESSAGE_MAP(Cdemo_socket_serverDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_StartServer, &Cdemo_socket_serverDlg::OnBnClickedButtonServerstart)
END_MESSAGE_MAP()


// Cdemo_socket_serverDlg 消息处理程序

BOOL Cdemo_socket_serverDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void Cdemo_socket_serverDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void Cdemo_socket_serverDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR Cdemo_socket_serverDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void Cdemo_socket_serverDlg::AddClient(void)
{
	CServerSocket *pSocket = new CServerSocket;
	pSocket->m_pDlg = this;
	listenSocket->Accept(*pSocket);
	pSocket->AsyncSelect(FD_READ | FD_WRITE | FD_CLOSE);
	m_clientList.AddTail(pSocket);

	CString str;
	m_userCount = m_clientList.GetCount();
	str.Format(L"%d", m_userCount);
	m_CSEUserNum.SetWindowTextW(str);

	UpdateData(false);
	UpdateEvent(_T("用户连接服务器."));
	SendMSG(_T("Hello!"));
}

void Cdemo_socket_serverDlg::RemoveClient(CServerSocket * pSocket)
{
	POSITION nPos = m_clientList.GetHeadPosition();
	POSITION nTmpPos = nPos;
	while (nPos)
	{
		CServerSocket* pSockItem = (CServerSocket*)m_clientList.GetNext(nPos);
		if (pSockItem->m_hSocket == pSocket->m_hSocket)
		{
			pSockItem->Close();
			delete pSockItem;
			m_clientList.RemoveAt(nTmpPos);
			m_userCount = m_clientList.GetCount();
			UpdateData(false);
			UpdateEvent(_T("用户离开."));
			return;
		}
		nTmpPos = nPos;
	}
}

void Cdemo_socket_serverDlg::RecvData(CServerSocket * pSocket)
{
	char* pData = NULL;
	pData = new char[1024];
	memset(pData, 0, sizeof(char) * 1024);
	UCHAR leng = 0;
	CString str;
	if (pSocket->Receive(pData, 1024, 0) != SOCKET_ERROR)
	{
		str = pData;
		//ControlPC(str);     // 依据指令控制电脑
		SendMSG(str);     // 转发数据给所有用户，包括发送数据的用户
	}
	delete pData;
	pData = NULL;
}

void Cdemo_socket_serverDlg::UpdateEvent(CString str)
{
	CString string;
	CTime time = CTime::GetCurrentTime();
	// 获取系统当前时间
	str += _T("\r\n");
	// 用于换行显示日志
	string = time.Format(_T("%Y/%m/%d %H:%M:%S  ")) + str;
	// 格式化当前时间
	int lastLine = m_CELog.LineIndex(m_CELog.GetLineCount() - 1);
	//m_CELog.LineIndex
	//获取编辑框最后一行索引
	m_CELog.SetSel(lastLine + 1, lastLine + 2, 0);
	//选择编辑框最后一行
	m_CELog.ReplaceSel(string);                                                             //替换所选那一行的内容
}

BOOL Cdemo_socket_serverDlg::WChar2MByte(LPCWSTR srcBuff, LPSTR destBuff, int nlen)
{
	int n = 0;
	n = WideCharToMultiByte(CP_OEMCP, 0, srcBuff, -1, destBuff, 0, 0, FALSE);
	if (n<nlen)return FALSE;
	WideCharToMultiByte(CP_OEMCP, 0, srcBuff, -1, destBuff, nlen, 0, FALSE);
	return TRUE;

}

void Cdemo_socket_serverDlg::SendMSG(CString str)
{
	char *pSend = new char[str.GetLength()];
	memset(pSend, 0, str.GetLength()*sizeof(char));
	if (!WChar2MByte(str.GetBuffer(0), pSend, str.GetLength()))
	{
		AfxMessageBox(_T("字符转换失败"));
		delete pSend;
		return;
	}
	POSITION nPos = m_clientList.GetHeadPosition();
	while (nPos)
	{
		CServerSocket* pTemp = (CServerSocket*)m_clientList.GetNext(nPos);
		pTemp->Send(pSend, str.GetLength());
	}
	delete pSend;
}




void Cdemo_socket_serverDlg::OnBnClickedButtonServerstart()
{
	// TODO: 在此添加控件通知处理程序代码
	WCHAR text_buff[32];
	CString str;
	int	m_port;

	//从编辑框获取端口号
	m_CEPortNum.GetWindowTextW(str);
	m_CEPortNum.GetWindowTextW(text_buff, m_CEPortNum.GetWindowTextLengthW() + 1);
	m_port = _wtoi(text_buff);
	if (m_port < 1024 || m_port > 65535)
	{
		MessageBox(L"端口号范围为1024~65535",L"错误",MB_OK | MB_ICONERROR);
		m_CEPortNum.SetFocus();
		return;
	}

	if (m_connect)
	{
		delete listenSocket;
		listenSocket = NULL;
		m_connect = false;
		SetDlgItemText(IDC_StartServer, _T("打开服务器"));
		UpdateEvent(_T("系统关闭服务器."));
		return;
	}
	listenSocket = new CServerSocket();
	listenSocket->m_pDlg = this;
	// 指定对话框为主对话框，不能少了这句
	UpdateData(true);

	if (!listenSocket->Create(m_port))// 创建服务器的套接字，IP地址默认本机IP		
	{
		AfxMessageBox(_T("创建套接字错误！"));
		listenSocket->Close();
		return;
	}
	if (!listenSocket->Listen())
	{
		AfxMessageBox(_T("监听失败！"));
		listenSocket->Close();
		return;
	}
	m_connect = true;
	SetDlgItemText(IDC_StartServer, _T("关闭服务器"));
	UpdateEvent(_T("系统打开服务器."));
}
