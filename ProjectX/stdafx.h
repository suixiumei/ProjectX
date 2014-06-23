
// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常可放心忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

//#include <atlbase.h>
//#include <atlapp.h>

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展
#include <afxdisp.h>        // MFC Automation classes


#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持

#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // 功能区和控件条的 MFC 支持

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

