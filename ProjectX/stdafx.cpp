
// stdafx.cpp : 只包括标准包含文件的源文件
// ProjectX.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"

BOOL gc_isOnline = TRUE;
CString gc_account;
CString gc_pass;
BOOL gc_autoLogin = FALSE;
CStringArray* accountInfo = new CStringArray;
CommonFunc com;
WebBrowserBase *gc_web;
HWND gc_hwnd = NULL;
CComPtr<IHTMLDocument2> gc_pIHTMLDocument2 = NULL;
CComPtr<IDispatch> gc_pScript = NULL;
//TrayAPP app;
void OutputDebugStringX(CString outString)
{
	OutputDebugString(L"ProjectX:" + outString);
}