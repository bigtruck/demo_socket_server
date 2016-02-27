
// demo_socket_serverDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "demo_socket_server.h"
#include "demo_socket_serverDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// Cdemo_socket_serverDlg �Ի���



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


// Cdemo_socket_serverDlg ��Ϣ�������

BOOL Cdemo_socket_serverDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void Cdemo_socket_serverDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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
	UpdateEvent(_T("�û����ӷ�����."));
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
			UpdateEvent(_T("�û��뿪."));
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
		//ControlPC(str);     // ����ָ����Ƶ���
		SendMSG(str);     // ת�����ݸ������û��������������ݵ��û�
	}
	delete pData;
	pData = NULL;
}

void Cdemo_socket_serverDlg::UpdateEvent(CString str)
{
	CString string;
	CTime time = CTime::GetCurrentTime();
	// ��ȡϵͳ��ǰʱ��
	str += _T("\r\n");
	// ���ڻ�����ʾ��־
	string = time.Format(_T("%Y/%m/%d %H:%M:%S  ")) + str;
	// ��ʽ����ǰʱ��
	int lastLine = m_CELog.LineIndex(m_CELog.GetLineCount() - 1);
	//m_CELog.LineIndex
	//��ȡ�༭�����һ������
	m_CELog.SetSel(lastLine + 1, lastLine + 2, 0);
	//ѡ��༭�����һ��
	m_CELog.ReplaceSel(string);                                                             //�滻��ѡ��һ�е�����
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
		AfxMessageBox(_T("�ַ�ת��ʧ��"));
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	WCHAR text_buff[32];
	CString str;
	int	m_port;

	//�ӱ༭���ȡ�˿ں�
	m_CEPortNum.GetWindowTextW(str);
	m_CEPortNum.GetWindowTextW(text_buff, m_CEPortNum.GetWindowTextLengthW() + 1);
	m_port = _wtoi(text_buff);
	if (m_port < 1024 || m_port > 65535)
	{
		MessageBox(L"�˿ںŷ�ΧΪ1024~65535",L"����",MB_OK | MB_ICONERROR);
		m_CEPortNum.SetFocus();
		return;
	}

	if (m_connect)
	{
		delete listenSocket;
		listenSocket = NULL;
		m_connect = false;
		SetDlgItemText(IDC_StartServer, _T("�򿪷�����"));
		UpdateEvent(_T("ϵͳ�رշ�����."));
		return;
	}
	listenSocket = new CServerSocket();
	listenSocket->m_pDlg = this;
	// ָ���Ի���Ϊ���Ի��򣬲����������
	UpdateData(true);

	if (!listenSocket->Create(m_port))// �������������׽��֣�IP��ַĬ�ϱ���IP		
	{
		AfxMessageBox(_T("�����׽��ִ���"));
		listenSocket->Close();
		return;
	}
	if (!listenSocket->Listen())
	{
		AfxMessageBox(_T("����ʧ�ܣ�"));
		listenSocket->Close();
		return;
	}
	m_connect = true;
	SetDlgItemText(IDC_StartServer, _T("�رշ�����"));
	UpdateEvent(_T("ϵͳ�򿪷�����."));
}
