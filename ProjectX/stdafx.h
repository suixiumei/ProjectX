
// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

// �ر� MFC ��ĳЩ�����������ɷ��ĺ��Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS

//#include <atlbase.h>
//#include <atlapp.h>

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ
#include <afxdisp.h>        // MFC Automation classes


#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��

#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // �������Ϳؼ����� MFC ֧��

#include "resource.h"

#include <mshtmhst.h>
#include <mshtml.h>

#include <string>
#include <iostream>
#include <vector>
#include  <winINet.h>

#include <XLLuaRuntime.h>
#include <XLUE.h>
#include <XLGraphic.h>
#include <XLGraphicPlus.h>

#include "Transcode\Win32Transcode.h"
typedef Win32Transcode XLUETranscode;
#include "Transcode\Code.h"

#include "ChunkStruct.h"
#include "XML\Markup.h"
#include "..\Extract7z\Extract7z.h"
#pragma comment(lib, "Extract7z.lib")

#include "XLDownload\XLDownload.h"
#include "XLDownload\XLError.h"
#pragma comment(lib, "XLDownload.lib")
#include "COM\CommonFunc.h"
#include "COM\WebBrowserBase.h"
//#include "UI\TrayApp.h"
extern BOOL gc_isOnline;
extern CString gc_account;
extern CString gc_pass;
extern BOOL gc_autoLogin;
extern CStringArray* accountInfo;
extern CommonFunc com;
extern WebBrowserBase *gc_web;
extern CString gc_access_token;
extern HWND gc_hwnd;
extern CComPtr<IHTMLDocument2> gc_pIHTMLDocument2;
extern CComPtr<IDispatch> gc_pScript;
//extern TrayAPP app;
void OutputDebugStringX(CString outString);

