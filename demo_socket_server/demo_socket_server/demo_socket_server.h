
// demo_socket_server.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// Cdemo_socket_serverApp: 
// �йش����ʵ�֣������ demo_socket_server.cpp
//

class Cdemo_socket_serverApp : public CWinApp
{
public:
	Cdemo_socket_serverApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern Cdemo_socket_serverApp theApp;