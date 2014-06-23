#include "StdAfx.h"
#include "SystemManager.h"
#include "../Transcode/md5.h"

SystemManager* SystemManager::GetInstance()
{
	static SystemManager s_systemManager;

	return &s_systemManager;
}

CString SystemManager::GetSkinID()
{
	CString skin = com.com_method_GetStringINI(L"Pro", L"skin", L"1", (CString)com.com_method_GetResDir() + L"Config.ini");
	
	return skin;
}

CString SystemManager::GetTutorials()
{
	if (XLFS_FileExists((CString)com.com_method_GetResDir() + L"Tutorials.ini") != 0)	//检测没有教程情况下
	{
		return L"0";
	}
	else
	{
		DeleteFile((CString)com.com_method_GetResDir() + L"Tutorials.ini");
		return L"1";
	}
}

CString SystemManager::GetVersion()
{
	if (com.com_method_CheckOnline())
	{
		CString version;
		version.LoadStringW(IDS_VERSION);
		return version;
	}
	else
	{
		return L"";
	}
}

DWORD WINAPI CheckUpdateThread(LPVOID pParam)
{
	CString proID;
	proID.LoadString(IDS_PROID);
	CString pid;
	pid = L"p_id=" + proID + L"&RequestType=1";
	CString content = com.com_method_ReadPageByPost(Code_UnicodeToAnsi(pid), L"tj.dev.07073.com", L"/pm/sd_mgr/pts/web/sd_version_sign.php?");
	CString version;
	version.LoadString(IDS_VERSION);
	DOUBLE dcontent, dversion;
	dcontent = _tcstod(content, 0);
	dversion = _tcstod(version, 0);
	if (dcontent > dversion)
	{
		ShellExecute(NULL, L"open", (CString)com.com_method_GetResDir() + L"Update.exe", NULL, (CString)com.com_method_GetResDir() + L"update", SW_HIDE);
		exit(0);
	}
	else
	{
		OutputDebugStringX(L"远程版本：" + content);
		OutputDebugStringX(L"本地版本："+ version);
		ChunkStruct* cs = new ChunkStruct;
		cs->CheckUpdate.result = L"1";
		::PostMessage(gc_hwnd, WM_USER + 102, 0, (LPARAM)cs);
	}
	return 0;
}

void SystemManager::CheckUpdate()
{
	if (com.com_method_CheckOnline())
	{
		HANDLE hThread = CreateThread(NULL, 0, CheckUpdateThread, NULL, 0, NULL);
		CloseHandle(hThread);
	}
}

CString SystemManager::GetProcessIsNot(CString processName)
{
	if (com.com_method_AppIsStart(processName))
		return L"1";
	else
		return L"0";
}

CString SystemManager::CheckOnline()
{
	BOOL bl = com.com_method_CheckOnline();
	if (bl)
		return L"1";
	else
		return L"0";
}

CString SystemManager::GetINI(CString strOne, CString strTwo, CString strThree)
{
	return com.com_method_GetStringINI(strOne, strTwo, strThree, (CString)com.com_method_GetResDir() + L"Config.ini");
}

void SystemManager::SetINI(CString strOne, CString strTwo, CString strThree)
{
	com.com_method_WriteINI(strOne, strTwo, strThree, (CString)com.com_method_GetResDir() + L"Config.ini");
}

void SystemManager::OpenUrl(CString url)
{
	ShellExecute(NULL, L"open", url, NULL, NULL, SW_SHOWNORMAL);
}

DWORD WINAPI FuncStatisticsThread(LPVOID pParam)
{
	char* chstatics = (char*)pParam;
	CString proID;
	proID.LoadStringW(IDS_PROID);
	if (com.com_method_CheckOnline())
	{
		USES_CONVERSION;
		char * psText = T2A(proID + com.com_method_GetMacByCmd() + L"@#$YeyouMody$Tcom");
		MD5String(psText);
		if (Code_UTF8ToUnicode(chstatics) == "usage")
			proID = L"p_id=" + proID + L"&macAdd=" + com.com_method_GetMacByCmd() + L"&chk=" + (CString)MD5String(psText);
		else
			proID = L"p_id=" + proID + L"&f_id=" + Code_UTF8ToUnicode(chstatics) + L"&macAdd=" + com.com_method_GetMacByCmd() + L"&chk=" + (CString)MD5String(psText);
		com.com_method_ReadPageByPost(Code_UnicodeToAnsi(proID), L"tj.dev.07073.com", L"/pm/sd_mgr/pts/web/pm_usage.php?");
	}
	return 0;
}

void SystemManager::FuncStatis(char *buf)
{
	HANDLE hThread = CreateThread(NULL, 0, FuncStatisticsThread, buf, 0, NULL);
	CloseHandle(hThread);
}

void SystemManager::CopyCString(CString strCopy)
{
	//剪贴板操作(注意是unicode环境):
	HGLOBAL hClip;      //定义一个HGLOBAL句柄变量用来指向分配的内存块
	if (OpenClipboard(NULL))                             //打开剪贴板
	{
		EmptyClipboard();                            //将剪贴板内容清空
		hClip = GlobalAlloc(GMEM_MOVEABLE, (strCopy.GetLength()) * 2 + 2);  //在堆上分配可移动的全局内存块，程序返回一个内存句柄.这里注意因为你操作的unicode串,其中每个字符占2byte,所以要用字符数*2.否则,vs2008编译时能通过,运行是会报"堆损坏"错误.
		WCHAR * buff;                                 //定义指向宽字符型的指针变量
		buff = (WCHAR*)GlobalLock(hClip); //对分配的内存块进行加锁，将内存块句柄转化成一个指针,并将相应的引用计数器加1
		wcscpy_s(buff, strCopy.GetLength() + 1, strCopy);      //将用户输入的数据复制到指针变量中，实际上就是复制到分配的内存块中.注意这里使用的wcscpy_s函数有别于原来的strcpy函数
		GlobalUnlock(hClip);     //数据写入完毕，进行解锁操作，并将引用计数器数字减1           
		SetClipboardData(CF_UNICODETEXT, hClip); //设置剪贴板格式,注意是  CF_UNICODETEXT ---unicode文本
		CloseClipboard();            //关闭剪贴板，释放剪贴板资源的占用权   
	}
}

void SystemManager::Quit()
{
	//app.OnDestroy();
	//exit(0);
	//gc_pIHTMLDocument2 = NULL;
	gc_pScript = NULL;
	::PostQuitMessage(0);
}

CString SystemManager::CovertChineseToPY(CString sCh)
{
	CString sPY = CommonFunc::com_method_ChiniseToPY(sCh);
	//OutputDebugStringX(L"汉字字符串转为拼音首字母：");
	//OutputDebugStringX(sPY);
	return sPY;
}

CString SystemManager::CovertToUTF8URLEncode(CString sCh)
{
	CString sUrl = getUTF8URLEncode(sCh);
	//OutputDebugStringX(L"转换为utf8的urlencode返回：");
	//OutputDebugStringX(sUrl);
	return sUrl;
}

void SystemManager::GetScreenSize(int &nScreenX, int&nScreenY)
{
	//工作区宽高
	int nWorkSpaceX = 0;
	int nWorkSpaceY = 0;

	//获取全屏宽高
 	int nfullScreenX = GetSystemMetrics(SM_CXSCREEN);
	int nfullScreenY = GetSystemMetrics(SM_CYSCREEN);

	//获得任务栏的句柄   
	HWND TaskWnd = FindWindow(L"Shell_TrayWnd", NULL);
	CRect rect;
	//任务栏的尺寸   
	GetWindowRect(TaskWnd, &rect);

	int nTaskScreenX = rect.Width();
	int nTaskScreenY = rect.Height();

	wchar_t* strTaskSide = L"";
	wchar_t* strTaskVisible = L"";
	//任务栏在左边  
	if (rect.top==0 && rect.left == 0 && rect.right<rect.bottom)
	{
		strTaskSide = L"任务栏在左边";
		strTaskVisible = L"任务栏可见";

		nWorkSpaceX = nfullScreenX - nTaskScreenX;
		nWorkSpaceY = nfullScreenY;
	}
	else if (rect.top == 0 && rect.left < 0 && rect.right<rect.bottom)
	{
		strTaskSide = L"任务栏在左边";
		strTaskVisible = L"任务栏不可见";

		nWorkSpaceX = nfullScreenX;
		nWorkSpaceY = nfullScreenY;
	}
	//任务栏在顶部
	else if (rect.top == 0 && rect.left == 0 && rect.right > rect.bottom)
	{
		strTaskSide = L"任务栏在顶部";
		strTaskVisible = L"任务栏可见";

		nWorkSpaceX = nfullScreenX;
		nWorkSpaceY = nfullScreenY - nTaskScreenY;
	}
	else if (rect.top < 0 && rect.left == 0 && rect.right > rect.bottom)
	{
		strTaskSide = L"任务栏在顶部";
		strTaskVisible = L"任务栏不可见";

		nWorkSpaceX = nfullScreenX;
		nWorkSpaceY = nfullScreenY;
	}
	//任务栏在下面
	else if (rect.top != 0 && rect.left == 0 && rect.bottom == nfullScreenY)
	{
		strTaskSide = L"任务栏在下面";
		strTaskVisible = L"任务栏可见";

		nWorkSpaceX = nfullScreenX;
		nWorkSpaceY = nfullScreenY - nTaskScreenY;
	}
	else if (rect.top != 0 && rect.left == 0 && rect.bottom > nfullScreenY)
	{
		strTaskSide = L"任务栏在下面";
		strTaskVisible = L"任务栏不可见";

		nWorkSpaceX = nfullScreenX;
		nWorkSpaceY = nfullScreenY;
	}
	//任务栏在右边
	else if (rect.top == 0 && rect.left != 0 && rect.right == nfullScreenX)
	{
		strTaskSide = L"任务栏在右边";
		strTaskVisible = L"任务栏可见";

		nWorkSpaceX = nfullScreenX - nTaskScreenX;
		nWorkSpaceY = nfullScreenY;
	}
	else if (rect.top == 0 && rect.left != 0 && rect.right > nfullScreenX)
	{
		strTaskSide = L"任务栏在右边";
		strTaskVisible = L"任务栏不可见";

		nWorkSpaceX = nfullScreenX;
		nWorkSpaceY = nfullScreenY;
	}


	//屏幕宽高
	nScreenX = nWorkSpaceX;
	nScreenY = nWorkSpaceY;

	CString outStr1;
	outStr1.Format(L"全屏宽 = %d,全屏高 = %d,任务栏宽 = %d,任务栏高 = %d", nfullScreenX, nfullScreenY, nTaskScreenX, nTaskScreenY);
	//OutputDebugStringX(outStr1);

	CString outStr2;
	outStr2.Format(L"任务栏位置：%s", strTaskSide);
	//OutputDebugStringX(outStr2);

	CString outStr3;
	outStr3.Format(L"任务栏是否可见：%s", strTaskVisible);
	//OutputDebugStringX(outStr3);

	CString outStr4;
	outStr4.Format(L"屏幕宽 = %d,屏幕高 = %d", nScreenX, nScreenY);
	//OutputDebugStringX(outStr4);
}