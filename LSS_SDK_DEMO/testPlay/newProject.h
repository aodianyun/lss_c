// newProject.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CnewProjectApp:
// �йش����ʵ�֣������ newProject.cpp
//

class CnewProjectApp : public CWinApp
{
public:
	CnewProjectApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CnewProjectApp theApp;
