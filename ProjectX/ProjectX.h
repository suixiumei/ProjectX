
// ProjectX.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//


#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������

class ProjectX_App : public CWinApp
{
public:
// ��д
public:
	virtual BOOL InitInstance();
// ʵ��
	DECLARE_MESSAGE_MAP()
};

extern ProjectX_App theApp;