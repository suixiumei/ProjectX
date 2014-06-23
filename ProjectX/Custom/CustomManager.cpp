#include "StdAfx.h"
#include "CustomManager.h"
#include "..\SYS\SystemManager.h"

CustomManager* CustomManager::GetInstance()
{
	static CustomManager s_customManager;

	return &s_customManager;
}

DWORD WINAPI ClearCacheThread(LPVOID pParam)
{
	try
	{
		//ҳ���ļ�������
		HANDLE hEntry;
		LPINTERNET_CACHE_ENTRY_INFO lpCacheEntry = NULL;
		DWORD dwEntrySize = 0;
		hEntry = FindFirstUrlCacheEntry(NULL, NULL, &dwEntrySize);
		lpCacheEntry = (LPINTERNET_CACHE_ENTRY_INFO) new char[dwEntrySize];
		hEntry = FindFirstUrlCacheEntry(NULL, lpCacheEntry, &dwEntrySize);
		if (hEntry)
		{
			do
			{
				DeleteUrlCacheEntry(lpCacheEntry->lpszSourceUrlName);
				dwEntrySize = 0;
				FindNextUrlCacheEntry(hEntry, NULL, &dwEntrySize);
				delete[] lpCacheEntry;
				lpCacheEntry = (LPINTERNET_CACHE_ENTRY_INFO) new char[dwEntrySize];
			} while (FindNextUrlCacheEntry(hEntry, lpCacheEntry, &dwEntrySize));
		}
		ChunkStruct* cs = new ChunkStruct;
		cs->ClearCache.result = L"1";
		::PostMessage(gc_hwnd, WM_USER + 103, 0, (LPARAM)cs);
		
		return 0;
	}
	catch (...)
	{
		return 0;
	}
	return 0;
}

void CustomManager::ClearCache()
{
	HANDLE hThread = CreateThread(NULL, 0, ClearCacheThread, NULL, 0, NULL);
	CloseHandle(hThread);
}

void CustomManager::WebObjInit(IWebBrowser2* lpWB)
{
	//��ʼ��web�ؼ�ָ��
	gc_pScript = NULL;
	if (!gc_pIHTMLDocument2)
	{
		IDispatch *pdispDoc = NULL;
		lpWB->get_Document(&pdispDoc);
		if (!pdispDoc)
			return;
		pdispDoc->QueryInterface(IID_IHTMLDocument2, (void **)&gc_pIHTMLDocument2);
		pdispDoc->Release();
	}
	gc_pIHTMLDocument2->get_Script(&gc_pScript);

	//ע��ҳ��Ԫ�ػص�����
	RegeditCallFunc();

	//ȫ��DOC�ͷ�
	gc_pIHTMLDocument2 = NULL;
}

//��¼
static void _stdcall stdcall_unlogin(LPVOID pParam)
{
	com.com_method_ChunkBolt(L"resultMainHelper.lua", "resultOpenLogin", 0);
}

//ÿ��ǩ��
static void _stdcall stdcall_dailyAttendance(LPVOID pParam)
{
	com.com_method_ChunkBolt(L"resultMainHelper.lua", "resultMRQDclick", 0);
}

//�����ˮ
static void _stdcall stdcall_goldExpenses(LPVOID pParam)
{
	com.com_method_ChunkBolt(L"resultMainHelper.lua", "resultJBLSclick", 0);
}

CString jxyx_button_h;
//������Ϸ
static void _stdcall stdcall_test(LPVOID pParam)
{
	OutputDebugStringX((CString)(LPCTSTR)pParam);
	//com.com_method_ChunkBolt(L"resultMainHelper.lua", "resultJBLSclick", ,);
}

//ע��ص�����
void CustomManager::RegeditCallFunc()
{
	//ע��
	gc_web->GetButtonCallC(L"unlogin_button", stdcall_unlogin);
	gc_web->GetButtonCallC(L"mrqd_button", stdcall_dailyAttendance);
	gc_web->GetButtonCallC(L"ckfl_button", stdcall_goldExpenses);
	jxyx_button_h = gc_web->GetButtonCallC(L"jxyx_button", stdcall_test);
}

/*************************************************
Author:JohnWang
Date:2014/06/19 16:02
Describe:������ҳJS����
Parament:
**************************************************/
void CustomManager::CallJScript(CComBSTR JSFunc)
{
	/*************************************************
	Author:JohnWang
	Date:2014/06/18 17:40
	Describe:���вΣ�/�޲�/�з���ֵ C����JS
	Parament:
	**************************************************/
	//DISPPARAMS dispparams;
	//memset(&dispparams, 0, sizeof(dispparams));
	//dispparams.cArgs = 2;   // ���ò�������
	//dispparams.rgvarg = new VARIANTARG[2]; // ��ʼ����������
	//dispparams.rgvarg[0].vt = VT_BSTR; // ��һ������������
	//dispparams.rgvarg[0].bstrVal = ::SysAllocString(L"One Dream"); //��һ��������ֵ
	//dispparams.rgvarg[1].vt = VT_BSTR;   //�ڶ�������������
	//dispparams.rgvarg[1].bstrVal = ::SysAllocString(L"One World "); //�ڶ���������ֵ

	//VARIANT ret;
	//gc_web->InvokeMethod(gc_pScript, JSFunc, &ret, dispparams, 2);
	//((CComVariant)ret).ChangeType(VT_BSTR);
	//CString strResult = ((CComVariant)ret).bstrVal;
	//AfxMessageBox(strResult);

	/*************************************************
	Author:JohnWang
	Date:2014/06/18 17:40
	Describe:�в�/�޲�/�޷���ֵ C����JS
	Parament:
	**************************************************/
	/*CComPtr<IHTMLWindow2> lpTempIHTMLwindow2 = NULL;
	gc_pIHTMLDocument2->get_parentWindow(&lpTempIHTMLwindow2);
	VARIANT lvar;
	lvar.vt = VT_EMPTY;
	lpTempIHTMLwindow2->execScript(JSFunc, L"Javascript", &lvar);*/
}