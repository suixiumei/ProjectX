
// ProjectX.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "ProjectX.h"

#include "Login/LuaLoginManager.h"
#include "SYS/LuaSystemManager.h"
#include "Custom/LuaCustomManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//#define ID_HOTKEY_SHOW_HIDE     (WM_USER+1001)

BEGIN_MESSAGE_MAP(ProjectX_App, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

bool InitXLUE()
{
	XLFS_Init();

	XLGraphicParam param;
	XL_PrepareGraphicParam(&param);
	param.textType = XLTEXT_TYPE_FREETYPE;
	XL_InitGraphicLib(&param);
	XL_SetFreeTypeEnabled(TRUE);
	XLUE_InitLoader(NULL);
	XLGraphicPlusParam plusParam;
	XLGP_PrepareGraphicPlusParam(&plusParam);
	XLGP_InitGraphicPlus(&plusParam);

	CString path;
	path = L"xar@file://" + (CString)com.com_method_GetResDir() + L"View.xar$";
	XLFS_MountDir((CString)com.com_method_GetResDir() + L"View", path, 0, 0);

	return true;
}

bool InitLuaCore()
{
	XL_LRT_ENV_HANDLE hEnv = XLLRT_GetEnv(NULL);
	LuaSystemManager::RegisterSelf(hEnv);
	LuaLoginManager::RegisterSelf(hEnv);
	LuaCustomManager::RegisterSelf(hEnv);

	XLLRT_ReleaseEnv(hEnv);

	return true;
}

void UninitXLUE()
{
	//退出流程
	XLUE_Uninit(NULL);
	XLUE_UninitLuaHost(NULL);
	XL_UnInitGraphicLib();
	XLUE_UninitHandleMap(NULL);
	XLFS_Uninit();
}

bool LoadMainXAR()
{
	long result = 0;
	//设置XAR的搜索路径
	result = XLUE_AddXARSearchPath(com.com_method_GetResDir());
	//加载主XAR,此时会执行该XAR的启动脚本onload.lua
	result = XLUE_LoadXAR(Code_UnicodeToUtf8((CString)com.com_method_GetResDir() + L"View"));

	if(result != 0)
	{
		return false;
	}
	return true;
}

ProjectX_App theApp;

BOOL ProjectX_App::InitInstance()
{
	HRESULT hRes = ::OleInitialize(NULL);
	ATLASSERT(SUCCEEDED(hRes));
	if(!InitXLUE())
	{
		return false;
	}

	if (!com.com_method_Init())
	{
		return false;
	}
	
	if (!InitLuaCore())
	{
		return false;
	}

	if(!LoadMainXAR())
	{
		return false;
	}

	CString boltid;
	boltid.LoadString(IDS_PROBOLTID);
	gc_hwnd = HWND(XLUE_GetHostWndWindowHandle(XLUE_GetHostWndByID(Code_UnicodeToUtf8(boltid))));

	MSG msg;
	// 主消息循环:
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (msg.message == WM_USER + 100)	//账号登录成功
		{
			ChunkStruct* cs = (ChunkStruct*)msg.lParam;
			com.com_method_ChunkBoltTable(L"resultMainHelper.lua", "resultGetLogin", *accountInfo);
			free(cs);
		}
		if (msg.message == WM_USER + 1001)	//账号登录失败
		{
			ChunkStruct* cs = (ChunkStruct*)msg.lParam;
			com.com_method_ChunkBolt(L"resultMainHelper.lua", "resultFailedGetLogin", 0);
			free(cs);
		}
		if (msg.message == WM_USER + 102)	//检测更新
		{
			ChunkStruct* cs = (ChunkStruct*)msg.lParam;
			com.com_method_ChunkBolt(L"resultMainHelper.lua", "resultCheckUpdate", 1, cs->CheckUpdate.result);
			free(cs);
		}
		if (msg.message == WM_USER + 103)	//清除缓存
		{
			ChunkStruct* cs = (ChunkStruct*)msg.lParam;
			com.com_method_ChunkBolt(L"resultMainHelper.lua", "resultClearCache", 1, cs->ClearCache.result);
			free(cs);
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	UninitXLUE();
	::OleUninitialize();

	return (int) msg.wParam;
} 