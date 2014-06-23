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
	if (XLFS_FileExists((CString)com.com_method_GetResDir() + L"Tutorials.ini") != 0)	//���û�н̳������
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
		OutputDebugStringX(L"Զ�̰汾��" + content);
		OutputDebugStringX(L"���ذ汾��"+ version);
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
	//���������(ע����unicode����):
	HGLOBAL hClip;      //����һ��HGLOBAL�����������ָ�������ڴ��
	if (OpenClipboard(NULL))                             //�򿪼�����
	{
		EmptyClipboard();                            //���������������
		hClip = GlobalAlloc(GMEM_MOVEABLE, (strCopy.GetLength()) * 2 + 2);  //�ڶ��Ϸ�����ƶ���ȫ���ڴ�飬���򷵻�һ���ڴ���.����ע����Ϊ�������unicode��,����ÿ���ַ�ռ2byte,����Ҫ���ַ���*2.����,vs2008����ʱ��ͨ��,�����ǻᱨ"����"����.
		WCHAR * buff;                                 //����ָ����ַ��͵�ָ�����
		buff = (WCHAR*)GlobalLock(hClip); //�Է�����ڴ����м��������ڴ����ת����һ��ָ��,������Ӧ�����ü�������1
		wcscpy_s(buff, strCopy.GetLength() + 1, strCopy);      //���û���������ݸ��Ƶ�ָ������У�ʵ���Ͼ��Ǹ��Ƶ�������ڴ����.ע������ʹ�õ�wcscpy_s�����б���ԭ����strcpy����
		GlobalUnlock(hClip);     //����д����ϣ����н����������������ü��������ּ�1           
		SetClipboardData(CF_UNICODETEXT, hClip); //���ü������ʽ,ע����  CF_UNICODETEXT ---unicode�ı�
		CloseClipboard();            //�رռ����壬�ͷż�������Դ��ռ��Ȩ   
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
	//OutputDebugStringX(L"�����ַ���תΪƴ������ĸ��");
	//OutputDebugStringX(sPY);
	return sPY;
}

CString SystemManager::CovertToUTF8URLEncode(CString sCh)
{
	CString sUrl = getUTF8URLEncode(sCh);
	//OutputDebugStringX(L"ת��Ϊutf8��urlencode���أ�");
	//OutputDebugStringX(sUrl);
	return sUrl;
}

void SystemManager::GetScreenSize(int &nScreenX, int&nScreenY)
{
	//���������
	int nWorkSpaceX = 0;
	int nWorkSpaceY = 0;

	//��ȡȫ�����
 	int nfullScreenX = GetSystemMetrics(SM_CXSCREEN);
	int nfullScreenY = GetSystemMetrics(SM_CYSCREEN);

	//����������ľ��   
	HWND TaskWnd = FindWindow(L"Shell_TrayWnd", NULL);
	CRect rect;
	//�������ĳߴ�   
	GetWindowRect(TaskWnd, &rect);

	int nTaskScreenX = rect.Width();
	int nTaskScreenY = rect.Height();

	wchar_t* strTaskSide = L"";
	wchar_t* strTaskVisible = L"";
	//�����������  
	if (rect.top==0 && rect.left == 0 && rect.right<rect.bottom)
	{
		strTaskSide = L"�����������";
		strTaskVisible = L"�������ɼ�";

		nWorkSpaceX = nfullScreenX - nTaskScreenX;
		nWorkSpaceY = nfullScreenY;
	}
	else if (rect.top == 0 && rect.left < 0 && rect.right<rect.bottom)
	{
		strTaskSide = L"�����������";
		strTaskVisible = L"���������ɼ�";

		nWorkSpaceX = nfullScreenX;
		nWorkSpaceY = nfullScreenY;
	}
	//�������ڶ���
	else if (rect.top == 0 && rect.left == 0 && rect.right > rect.bottom)
	{
		strTaskSide = L"�������ڶ���";
		strTaskVisible = L"�������ɼ�";

		nWorkSpaceX = nfullScreenX;
		nWorkSpaceY = nfullScreenY - nTaskScreenY;
	}
	else if (rect.top < 0 && rect.left == 0 && rect.right > rect.bottom)
	{
		strTaskSide = L"�������ڶ���";
		strTaskVisible = L"���������ɼ�";

		nWorkSpaceX = nfullScreenX;
		nWorkSpaceY = nfullScreenY;
	}
	//������������
	else if (rect.top != 0 && rect.left == 0 && rect.bottom == nfullScreenY)
	{
		strTaskSide = L"������������";
		strTaskVisible = L"�������ɼ�";

		nWorkSpaceX = nfullScreenX;
		nWorkSpaceY = nfullScreenY - nTaskScreenY;
	}
	else if (rect.top != 0 && rect.left == 0 && rect.bottom > nfullScreenY)
	{
		strTaskSide = L"������������";
		strTaskVisible = L"���������ɼ�";

		nWorkSpaceX = nfullScreenX;
		nWorkSpaceY = nfullScreenY;
	}
	//���������ұ�
	else if (rect.top == 0 && rect.left != 0 && rect.right == nfullScreenX)
	{
		strTaskSide = L"���������ұ�";
		strTaskVisible = L"�������ɼ�";

		nWorkSpaceX = nfullScreenX - nTaskScreenX;
		nWorkSpaceY = nfullScreenY;
	}
	else if (rect.top == 0 && rect.left != 0 && rect.right > nfullScreenX)
	{
		strTaskSide = L"���������ұ�";
		strTaskVisible = L"���������ɼ�";

		nWorkSpaceX = nfullScreenX;
		nWorkSpaceY = nfullScreenY;
	}


	//��Ļ���
	nScreenX = nWorkSpaceX;
	nScreenY = nWorkSpaceY;

	CString outStr1;
	outStr1.Format(L"ȫ���� = %d,ȫ���� = %d,�������� = %d,�������� = %d", nfullScreenX, nfullScreenY, nTaskScreenX, nTaskScreenY);
	//OutputDebugStringX(outStr1);

	CString outStr2;
	outStr2.Format(L"������λ�ã�%s", strTaskSide);
	//OutputDebugStringX(outStr2);

	CString outStr3;
	outStr3.Format(L"�������Ƿ�ɼ���%s", strTaskVisible);
	//OutputDebugStringX(outStr3);

	CString outStr4;
	outStr4.Format(L"��Ļ�� = %d,��Ļ�� = %d", nScreenX, nScreenY);
	//OutputDebugStringX(outStr4);
}