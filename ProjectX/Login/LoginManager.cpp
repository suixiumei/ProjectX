#include "StdAfx.h"
#include "LoginManager.h"

LoginManager* LoginManager::GetInstance()
{
	static LoginManager s_loginManager;

	return &s_loginManager;
}

DWORD WINAPI LoginThread(LPVOID pParam)
{
	/*OutputDebugStringX(L"账号：" + gc_account);
	OutputDebugStringX(L"密码：" + gc_pass);*/

	CString name, pass;
	name = gc_account;
	pass = gc_pass;
	if (com.com_method_GetStringINI(L"Pro", L"pass", L"", (CString)com.com_method_GetResDir() + L"Config.ini") == pass)
	{
		pass = Code_Decrypt(pass, 07073);
	}

	CString result = com.com_method_Login(name, pass);
	//OutputDebugStringX(L"result：" + result); 
	if ((result.Find(L"1") != -1) && (result != ""))
	{
		gc_account = name;
		gc_pass = pass;
		if (!gc_autoLogin)
		{
			com.com_method_WriteINI(L"Pro", L"account", name, (CString)com.com_method_GetResDir() + L"Config.ini");
			com.com_method_WriteINI(L"Pro", L"pass", L"", (CString)com.com_method_GetResDir() + L"Config.ini");
			com.com_method_WriteINI(L"Pro", L"login", L"0", (CString)com.com_method_GetResDir() + L"Config.ini");
		}
		else
		{
			com.com_method_WriteINI(L"Pro", L"account", name, (CString)com.com_method_GetResDir() + L"Config.ini");
			com.com_method_WriteINI(L"Pro", L"pass", Code_Encrypt(pass, 07073), (CString)com.com_method_GetResDir() + L"Config.ini");
			com.com_method_WriteINI(L"Pro", L"login", L"1", (CString)com.com_method_GetResDir() + L"Config.ini");
		}

		if (com.com_method_DownXML(L"http://me.07073.com/service/jsonLoginStauts/xml/", L"login"))
		{
			CString stautsInfo, nickName, maxexp, exp, level, avatar;

			CMarkup xml;
			xml.Load((CString)com.com_method_GetResDir() + L"Temp\\login.xml");
			xml.ResetMainPos();
			xml.FindElem(L"api");
			xml.IntoElem();
			xml.FindElem(L"el");
			maxexp = xml.GetData();
			xml.FindElem(L"es");
			exp = xml.GetData();
			xml.FindElem(L"l");
			level = xml.GetData();
			xml.FindElem(L"i");
			avatar = xml.GetData();

			CString content;
			CFile file;
			unsigned char buff[1024];
			file.Open((CString)com.com_method_GetResDir()+_T("Temp\\login.xml"), CFile::modeRead);
			file.Read(buff, sizeof(buff));
			file.Close();
			content = (CString)buff;
			content = content.Mid(content.Find(L"<username>") + 10);
			content = content.Left(content.Find(L"</username>"));
			nickName = content;
			DeleteFile((CString)com.com_method_GetResDir() + L"Temp\\login.xml");

			if (URLDownloadToFile(NULL, avatar, (CString)com.com_method_GetResDir() + L"Temp\\avatar.jpg", NULL, NULL) == S_OK)
			{
				DeleteUrlCacheEntry(avatar);
				CImage image;
				image.Load((CString)com.com_method_GetResDir() + _T("Temp\\avatar.jpg"));
				image.Save((CString)com.com_method_GetResDir() + _T("Temp\\avatar.png"));
				DeleteFile((CString)com.com_method_GetResDir() + L"Temp\\avatar.jpg");
			}

			/*OutputDebugStringX(L"昵称：" + nickName);
			OutputDebugStringX(L"金币：999");
			OutputDebugStringX(L"等级：" + level);
			OutputDebugStringX(L"经验：" + exp);
			OutputDebugStringX(L"最大经验：" + maxexp);*/
			ChunkStruct* cs = new ChunkStruct;
			accountInfo->RemoveAll();
			accountInfo->Add(gc_account);
			accountInfo->Add(nickName);
			accountInfo->Add(L"999");
			accountInfo->Add(level);
			accountInfo->Add(exp);
			accountInfo->Add(maxexp);
			::PostMessage(gc_hwnd, WM_USER + 100, 0, (LPARAM)cs);
		}
	}
	else
	{
		gc_account = L"";
		gc_pass = L"";
		gc_autoLogin = FALSE;
		result = L"";
		com.com_method_WriteINI(L"Pro", L"pass", L"", (CString)com.com_method_GetResDir() + L"Config.ini");
		ChunkStruct* cs = new ChunkStruct;
		accountInfo->RemoveAll();
		::PostMessage(gc_hwnd, WM_USER + 1001, 0, (LPARAM)cs);
	}

	return 0;
}

void LoginManager::Login()
{
	HANDLE hThread = CreateThread(NULL, 0, LoginThread, NULL, 0, NULL);
	CloseHandle(hThread);
}

DWORD WINAPI LogoutThread(LPVOID pParam)
{
	gc_account = L"";
	gc_pass = L"";
	com.com_method_ReadPageByPost("1=1", L"me.07073.com", L"/service/jsonLogout/?");
	return 0;
}

void LoginManager::Logout()
{
	HANDLE hThread = CreateThread(NULL, 0, LogoutThread, NULL, 0, NULL);
	CloseHandle(hThread);
}