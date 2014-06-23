#include "StdAfx.h"
#include <afxinet.h>
#include <tlhelp32.h>
#include <psapi.h>
#pragma comment(lib,"psapi.lib")
#pragma comment(lib, "Netapi32.lib")
#include "..\Transcode\md5.h"
using namespace std;

BOOL CommonFunc::com_method_Init()
{	
	CString protitle;
	protitle.LoadString(IDS_TITLE);
	com_method_ElevatePrivileges();
	CString str = (CString)com_method_GetResDir();
	//OutputDebugStringX(str);
	if (XLFS_DirExists((CString)com_method_GetResDir() + L"Temp") != 0)
		CreateDirectory((CString)com_method_GetResDir() + L"Temp", NULL);
	HANDLE m_hMutex = CreateMutex(NULL, TRUE, protitle);
	if (m_hMutex == NULL)
	{
		return FALSE;
	}
	//如果程序已经存在并且正在运行
	if (GetLastError() ==ERROR_ALREADY_EXISTS)
	{
		HWND hProgramWnd = ::FindWindow(NULL, protitle);
		if (hProgramWnd)
		{
			WINDOWPLACEMENT* pWndpl =NULL;
			WINDOWPLACEMENT   wpm; 
			pWndpl =&wpm;
			GetWindowPlacement(hProgramWnd,&wpm);
			if (pWndpl)	
			{
				//将运行的程序窗口还原成正常状态
				pWndpl->showCmd= SW_SHOWNORMAL;
				::SetWindowPlacement(hProgramWnd,pWndpl);
				SetWindowPos(hProgramWnd,HWND_NOTOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
			}
		}
		//关闭进程互斥体
		CloseHandle(m_hMutex);
		m_hMutex = NULL;
		return FALSE;
	}
	com.com_method_CheckUpdate();
	
	return TRUE;
}

void CommonFunc::com_method_ElevatePrivileges()
{
	HMODULE hDll = ::LoadLibrary(L"ntdll.dll"); 
	typedef int (__stdcall * type_RtlAdjustPrivilege)(int, bool, bool, int*);
	type_RtlAdjustPrivilege RtlAdjustPrivilege = (type_RtlAdjustPrivilege)GetProcAddress(hDll, "RtlAdjustPrivilege");
	int nEn = 0;
	RtlAdjustPrivilege(0x14,TRUE,FALSE,&nEn);
	FreeLibrary(hDll);
}

void CommonFunc::com_method_ClearType()
{

}

CString CommonFunc::com_method_GetStringINI(CString lpAppName,CString lpKeyName,CString lpDefault,CString lpFileName)
{
	CString lpReturnedString;
	GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpReturnedString.GetBuffer(65534),65534,lpFileName);
	lpReturnedString.ReleaseBuffer();
	return lpReturnedString;
}

void CommonFunc::com_method_WriteINI(CString lpAppName,CString lpKeyName,CString lpString,CString lpFileName)
{
	WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName);
}

int CommonFunc::com_method_FindLastPos(CString src, CString tar)
{
	int result = src.FindOneOf(tar);
	if(-1 == result )
	{
		return -1;
	}

	//递归处理剩余串
	return 1 + result + com_method_FindLastPos(src.Right( src.GetLength() - 1 - result), tar);
}

WCHAR* CommonFunc::com_method_GetResDir()
{
	static WCHAR wszModulePath[MAX_PATH];
	GetModuleFileNameW(NULL,wszModulePath,MAX_PATH);
	PathAppend(wszModulePath, L"..\\");
	return wszModulePath;
}

BOOL CommonFunc::com_method_CopyDir(CString strSrcPath, CString strDstPath)
{ // 创建目标文件夹

	CreateDirectory(strDstPath,NULL);
	CFileFind finder;
	// 打开指定的文件夹进行搜索
	BOOL bWorking = finder.FindFile(strSrcPath + "\\" + "*.*"); 
	while(bWorking)
	{
		// 从当前目录搜索文件
		bWorking = finder.FindNextFile();
		CString strFileName = finder.GetFileName();
		CString strSrc = strSrcPath + "\\" + strFileName;
		CString strDst = strDstPath + "\\" + strFileName;
		// 判断搜索到的是不是"."和".."目录
		if(!finder.IsDots())
		{
			// 判断搜索到的目录是否是文件夹
			if(finder.IsDirectory())
			{
				// 如果是文件夹的话，进行递归
				if(!com_method_CopyDir(strSrc, strDst)) 
					return FALSE;
			}
			else
			{
				// 如果是文件，进行复制
				if(!CopyFile(strSrc, strDst, FALSE))
					return FALSE;
			}
		}
	}       
	return TRUE;
}

void CommonFunc::com_method_Split(CString content, CString pattern, CStringArray& strlist)
{
	int iPos = content.Find(pattern), iStart = 0;
	while (iPos != -1)
	{
		strlist.Add(content.Mid(iStart, iPos - iStart));
		iStart = iPos + pattern.GetLength();
		iPos = content.Find(pattern, iStart);
	}

	if (iStart <= content.GetLength())
		strlist.Add(content.Mid(iStart));
}

void CommonFunc::com_method_DelFolder(CString lpszFromPath)
{
	SHFILEOPSTRUCT shfileop; 
	shfileop.hwnd = NULL; 
	shfileop.wFunc = FO_DELETE ; 
	shfileop.fFlags = FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR | FOF_NOERRORUI | FOF_SILENT; 
	lpszFromPath += '\0'; //Convert of CString to LPCTSTR 结尾必须添加截止符号才能生效 
	shfileop.pFrom = lpszFromPath; 
	shfileop.pTo = L""; 
	shfileop.lpszProgressTitle = L""; 
	shfileop.fAnyOperationsAborted = TRUE; 
	SHFileOperation(&shfileop);
}

CString CommonFunc::com_method_ReadPageByPost(char* requestData,CString siteUrl, CString pageUrl)
{
	try
	{
		CString   strResultStr;
		CInternetSession   session;
		CHttpConnection   *pConnection;
		CHttpFile   *pFile;

		//与服务器建立连接 
		pConnection = session.GetHttpConnection(siteUrl);
		pFile = pConnection->OpenRequest(CHttpConnection::HTTP_VERB_POST, pageUrl);
		pFile->AddRequestHeaders(_T("Content-Type: application/x-www-form-urlencoded"));
		pFile->AddRequestHeaders(_T("FromOpen:YeyouMody"));
		pFile->SendRequest(NULL, NULL, requestData, (DWORD)strlen(requestData));

		//最终写入文件的字符串
		CString content;
		CString temp;
		//读取网页内容
		while (pFile->ReadString(content))
		{
			temp = temp + content;
		}
		USES_CONVERSION;
		content = W2A(Code_UTF8ToUnicode((char*)temp.GetBuffer()));
		content = content.Trim();

		pFile->Close();
		session.Close();
		content.ReleaseBuffer();
		temp.ReleaseBuffer();
		return content;
	}
	catch (CMemoryException* e)
	{
		e->Delete();
		return L"";
	}
	catch (CFileException* e)
	{
		e->Delete();
		return L"";
	}
	catch (CException* e)
	{
		e->Delete();
		return L"";
	}
}

 CString CommonFunc::com_method_GetRegeditByCLASSES_ROOT(CString subKey,CString valueName)
 {
	HKEY hKEY;
	//打开与路径 data_Set相关的hKEY
	LPCTSTR data_Set=subKey;
	LONG return0=::RegOpenKeyEx(HKEY_CLASSES_ROOT,data_Set,0,KEY_READ,&hKEY);
	if(return0!=ERROR_SUCCESS)
	{
		 return L"";
	}
	LPBYTE data_Value=new BYTE[MAX_PATH];
	DWORD type_1=REG_SZ;//定义数据类型
	DWORD cbData_1=MAX_PATH;//定义数据长度

	LONG return1=::RegQueryValueEx(hKEY,valueName,NULL,&type_1,data_Value,&cbData_1);
	if(return1!=ERROR_SUCCESS)
	{
		return L"";
	}
	CString strValue;
	strValue.Format(L"%s",data_Value);
	delete[] data_Value; 
	::RegCloseKey(hKEY);

	return strValue;
 }

 CString CommonFunc::com_method_GetRegeditByCURRENT_USER(CString subKey, CString valueName)
 {
	 HKEY hKEY;
	 //打开与路径 data_Set相关的hKEY
	 LPCTSTR data_Set = subKey;
	 LONG return0 = ::RegOpenKeyEx(HKEY_CURRENT_USER, data_Set, 0, KEY_READ, &hKEY);
	 if (return0 != ERROR_SUCCESS)
	 {
		 return L"";
	 }
	 LPBYTE data_Value = new BYTE[MAX_PATH];
	 DWORD type_1 = REG_SZ;//定义数据类型
	 DWORD cbData_1 = MAX_PATH;//定义数据长度

	 LONG return1 = ::RegQueryValueEx(hKEY, valueName, NULL, &type_1, data_Value, &cbData_1);
	 if (return1 != ERROR_SUCCESS)
	 {
		 return L"";
	 }
	 CString strValue;
	 strValue.Format(L"%s", data_Value);
	 delete[] data_Value;
	 ::RegCloseKey(hKEY);

	 return strValue;
 }

 void CommonFunc::com_method_SetRegedit(HKEY hKeyParent,LPCTSTR lpszkeyName,LPCTSTR pszkeyValue,LPCTSTR pszValueName)
 {
	 CRegKey rKey;
	 if(rKey.Open(hKeyParent,lpszkeyName)==ERROR_SUCCESS)
	 {
		 rKey.SetValue(pszkeyValue,pszValueName);
	 }
	 rKey.Close();
 }

 CString CommonFunc::com_method_DeleteRegedit(HKEY hKeyParent,LPCTSTR lpszkeyName,LPCTSTR pszkeyValue)
 {
	 CRegKey rKey;
	 if(rKey.Open(hKeyParent,lpszkeyName)==ERROR_SUCCESS)
	 {
		 rKey.DeleteValue(pszkeyValue);
	 }
	 rKey.Close();
	 return L"1";
 }

CString CommonFunc::com_method_Login(CString name, CString pass)
{
	CString encodename, encodepass;
	CStringEncodeBase64(name, encodename);
	CStringEncodeBase64(pass, encodepass);
	encodename.Replace(L"=", L"_");
	encodename.Replace(L"+", L"__");
	encodename.Replace(L"/", L"___");
	encodepass.Replace(L"=", L"_");
	encodepass.Replace(L"+", L"__");
	encodepass.Replace(L"/", L"___");

	CString content;
	try
	{
		if (com_method_CheckOnline())
		{
			content = com.com_method_ReadPageByPost("1=1", L"me.07073.com", L"/service/jsonLogin_/" + encodename + L"/" + encodepass + L"/?");
		}
		else
		{
			content = L"";
		}
	}
	catch (...)
	{
		content = L"";
	}
	return content;
}

DWORD WINAPI CheckOnlineUpdateThread(LPVOID pParam)
{
	CString proID;
	proID.LoadString(IDS_PROID);
	CString pid;
	pid = L"p_id=" + proID + L"&RequestType=1";
	CString content = com.com_method_ReadPageByPost(Code_UnicodeToAnsi(pid), L"tj.dev.07073.com", L"/pm/sd_mgr/pts/web/sd_version_sign.php?");
	CString version;
	version.LoadString(IDS_VERSION);
	DOUBLE dcontent, dversion;
	dcontent = _tcstod(content,0);
	dversion = _tcstod(version, 0);
	if (dcontent > dversion)
	{
		ShellExecute(NULL, L"open", (CString)com.com_method_GetResDir() + L"Update.exe", NULL, (CString)com.com_method_GetResDir() + L"update", SW_HIDE);
		exit(0);
	}
	return 0;
}

void CommonFunc::com_method_CheckUpdate()
{
	if (com.com_method_CheckOnline())
	{
		HANDLE hThread = CreateThread(NULL, 0, CheckOnlineUpdateThread, NULL, 0, NULL);
		CloseHandle(hThread);
	}
}

BOOL CommonFunc::com_method_AppIsStart(CString filename)   
{   
	HANDLE hProcessSnap=NULL;   
	PROCESSENTRY32 pe32={0};   
	hProcessSnap=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);   
	if(hProcessSnap==(HANDLE)-1)   
	{     
		return FALSE;
	}   
	pe32.dwSize=sizeof(PROCESSENTRY32);   
	if(Process32First(hProcessSnap,&pe32))   
	{   
		do   
		{   
			if((!lstrcmp((LPCWSTR)pe32.szExeFile,filename)))//取进程模块名与你的应用程序名比较   
			{   
				//检测到相同，然后进行你的处理。
				CloseHandle(hProcessSnap);
				return TRUE; 
			}
		}   
		while(Process32Next(hProcessSnap,&pe32));   
	}   
	else   
	{   
		CloseHandle(hProcessSnap);
		return FALSE;
	}   
	CloseHandle(hProcessSnap);
	return FALSE;
}

BOOL CommonFunc::com_method_CheckOnline()
{
	BOOL   m_bOnline;//是否在线   
	DWORD   flags;//上网方式
	m_bOnline=InternetGetConnectedState(&flags,0);   
	if(m_bOnline)//在线 
	{  
		int b; 
		if ( flags& INTERNET_CONNECTION_MODEM )
		{
			b = flags& INTERNET_CONNECTION_MODEM;
		}  
		else if ( flags&INTERNET_CONNECTION_LAN )
		{
			b = flags&INTERNET_CONNECTION_LAN; 
		}
		else if ( flags& INTERNET_CONNECTION_PROXY )
		{
			b = flags& INTERNET_CONNECTION_PROXY;
		}
		switch(b)   
		{   
		case   INTERNET_CONNECTION_MODEM   :   
			m_bOnline = TRUE;
			break;   
		case   INTERNET_CONNECTION_LAN   :   
			m_bOnline = TRUE;
			break;   
		case   INTERNET_CONNECTION_PROXY   :   
			m_bOnline = TRUE;
			break;   
		}   
		if(flags&&INTERNET_CONNECTION_MODEM_BUSY==INTERNET_CONNECTION_MODEM_BUSY)   
			m_bOnline = TRUE;
	}   
	else   
		m_bOnline = FALSE;
	
	if (!gc_isOnline)
		return FALSE;
	else
		return m_bOnline;
}

CString GetOneMac(int AdapterIndex)
{
	NCB ncb;
	UCHAR uRetCode;
	ASTAT Adapter;
	memset(&ncb, 0, sizeof(ncb));
	ncb.ncb_command = NCBRESET;
	ncb.ncb_lana_num = AdapterIndex;   // 指定网卡号

	//首先对选定的网卡发送一个NCBRESET命令，以便进行初始化 
	uRetCode = Netbios(&ncb);

	memset(&ncb, 0, sizeof(ncb));
	ncb.ncb_command = NCBASTAT;
	ncb.ncb_lana_num = AdapterIndex; 	// 指定网卡号

	strcpy((char *)ncb.ncb_callname, "*");
	// 指定返回的信息存放的变量 
	ncb.ncb_buffer = (unsigned char *)&Adapter;
	ncb.ncb_length = sizeof(Adapter);

	// 发送NCBASTAT命令以获取网卡的信息 
	uRetCode = Netbios(&ncb);
	// 把网卡MAC地址格式化成常用的16进制形式，如0010-A4E4-5802 
	CString strMacAddr;
	if (uRetCode == 0)
	{
		strMacAddr.Format(L"%02X%02X%02X%02X%02X%02X",
			Adapter.adapt.adapter_address[0],
			Adapter.adapt.adapter_address[1],
			Adapter.adapt.adapter_address[2],
			Adapter.adapt.adapter_address[3],
			Adapter.adapt.adapter_address[4],
			Adapter.adapt.adapter_address[5]);
	}
	return strMacAddr;
}

CString CommonFunc::com_method_GetMacByCmd()
{
	NCB Ncb;
	UCHAR uRetCode;
	LANA_ENUM lenum;
	int i = 0;
	CString macStr;

	memset(&Ncb, 0, sizeof(Ncb));
	Ncb.ncb_command = NCBENUM;
	Ncb.ncb_buffer = (UCHAR *)&lenum;
	Ncb.ncb_length = sizeof(lenum);

	//向网卡发送NCBENUM命令，以获取当前机器的网卡信息，如有多少个网卡、每张网卡的编号等 
	uRetCode = Netbios(&Ncb);
	//获得所有网卡信息
	for (i = 0; i < lenum.length; i++)
	{
		macStr = GetOneMac(lenum.lana[i]);
	}

	return macStr;
}

void CommonFunc::com_method_ChunkBolt(CString fileName,const char* funName,int numbers,...)
{
	CString luaFileName;
	luaFileName = L"xar@file://"+(CString)com.com_method_GetResDir()+L"View.xar$";
	XL_LRT_ENV_HANDLE hEnv = XLLRT_GetEnv(NULL);
	XL_LRT_RUNTIME_HANDLE hRuntime = XLLRT_GetRuntime(hEnv, NULL);
	lua_State* luaState = XLLRT_GetLuaState(hRuntime);
	long nowTop = lua_gettop(luaState);
	XL_LRT_CHUNK_HANDLE hThisChunk = NULL;
	XLLRT_CreateChunkFromModule(fileName, (CString)com.com_method_GetResDir()+L"View\\layout\\Interface\\Mutual\\CToLua\\"+fileName, funName, &hThisChunk);
	if ( hThisChunk != NULL )
	{
		long lRet = XLLRT_PrepareChunk(hRuntime, hThisChunk);
		if (lRet == 0)
		{
			va_list ap;
			va_start ( ap, numbers );
			for ( int i= 0; i< numbers; i++ )
			{
				lua_pushstring(luaState, Code_UnicodeToUtf8(va_arg (ap, CString)));
			}
			va_end (ap);
			if (long lCallRet = XLLRT_LuaCall(luaState, numbers,  0, luaFileName))
			{
				//MessageBoxA(NULL, XLLRT_GetLastError(luaState), 0, 0);
			}
		}
		else
		{
			//MessageBoxA(NULL, XLLRT_GetLastError(luaState), 0, 0);
		}

		XLLRT_ReleaseChunk(hThisChunk);
	}

	XLLRT_ReleaseRunTime(hRuntime);
	XLLRT_ReleaseEnv(hEnv);

	// 恢复之前的栈顶位置
	lua_settop(luaState, nowTop);
}

void CommonFunc::com_method_ChunkBoltTable(CString fileName, const char* funName, CStringArray& strlist)
{
	CString luaFileName;
	luaFileName = L"xar@file://" + (CString)com.com_method_GetResDir() + L"View.xar$";
	XL_LRT_ENV_HANDLE hEnv = XLLRT_GetEnv(NULL);
	XL_LRT_RUNTIME_HANDLE hRuntime = XLLRT_GetRuntime(hEnv, NULL);
	lua_State* luaState = XLLRT_GetLuaState(hRuntime);
	long nowTop = lua_gettop(luaState);
	XL_LRT_CHUNK_HANDLE hThisChunk = NULL;
	XLLRT_CreateChunkFromModule(fileName, (CString)com.com_method_GetResDir() + L"View\\layout\\Interface\\Mutual\\CToLua\\" + fileName, funName, &hThisChunk);
	if (hThisChunk != NULL)
	{
		long lRet = XLLRT_PrepareChunk(hRuntime, hThisChunk);
		if (lRet == 0)
		{
			lua_newtable(luaState);
			for (int i = 0; i < strlist.GetCount(); i++)
			{
				lua_pushstring(luaState, Code_UnicodeToUtf8(strlist.GetAt(i)));
				lua_rawseti(luaState, -2, i+1);
			}

			if (long lCallRet = XLLRT_LuaCall(luaState, 1, 0, luaFileName))
			{
				//MessageBoxA(NULL, XLLRT_GetLastError(luaState), 0, 0);
			}
		}
		else
		{
			//MessageBoxA(NULL, XLLRT_GetLastError(luaState), 0, 0);
		}

		XLLRT_ReleaseChunk(hThisChunk);
	}

	XLLRT_ReleaseRunTime(hRuntime);
	XLLRT_ReleaseEnv(hEnv);

	// 恢复之前的栈顶位置
	lua_settop(luaState, nowTop);
}

BOOL CommonFunc::com_method_DownXML(CString downurl, CString filename)
{
	CString szRand;
	szRand.Format(L"?skq=%d", GetTickCount());
	downurl += szRand;
	try
	{
		if (URLDownloadToFile(NULL, downurl, (CString)com.com_method_GetResDir() + L"Temp\\" + filename + ".xml", 0, NULL) == S_OK)
		{
			DeleteUrlCacheEntry(downurl);

			return TRUE;
		}
		return FALSE;
	}
	catch (...)
	{
		return FALSE;
	}
}


char  convert(wchar_t  n);
bool  In(wchar_t  start, wchar_t  end, wchar_t  code);
void CNtoPY(const char* szCn, char* outEn);

CString CommonFunc::com_method_ChiniseToPY(CString sCh)
{
	//char sCh[100] = "我爱中国";  //  输入的字符串
	char  oEn[100] = { 0 };
	char* tempStr = Code_UnicodeToAnsi(sCh);
	CNtoPY(tempStr, oEn);
	CString oPY;
	oPY.Format(L"%s", Code_UTF8ToUnicode(oEn));
	return oPY;
}
char  convert(wchar_t  n)
{
	if (In(0xB0A1, 0xB0C4, n))  return  'a';

	if (In(0XB0C5, 0XB2C0, n))  return  'b';

	if (In(0xB2C1, 0xB4ED, n))  return  'c';

	if (In(0xB4EE, 0xB6E9, n))  return  'd';

	if (In(0xB6EA, 0xB7A1, n))  return  'e';

	if (In(0xB7A2, 0xB8c0, n))  return  'f';

	if (In(0xB8C1, 0xB9FD, n))  return  'g';

	if (In(0xB9FE, 0xBBF6, n))  return  'h';

	if (In(0xBBF7, 0xBFA5, n))  return  'j';

	if (In(0xBFA6, 0xC0AB, n))  return  'k';

	if (In(0xC0AC, 0xC2E7, n))  return  'l';

	if (In(0xC2E8, 0xC4C2, n))  return  'm';

	if (In(0xC4C3, 0xC5B5, n))  return  'n';

	if (In(0xC5B6, 0xC5BD, n))  return  'o';

	if (In(0xC5BE, 0xC6D9, n))  return  'p';

	if (In(0xC6DA, 0xC8BA, n))  return  'q';

	if (In(0xC8BB, 0xC8F5, n))  return  'r';

	if (In(0xC8F6, 0xCBF0, n))  return  's';

	if (In(0xCBFA, 0xCDD9, n))  return  't';

	if (In(0xCDDA, 0xCEF3, n))  return  'w';

	if (In(0xCEF4, 0xD188, n))  return  'x';

	if (In(0xD1B9, 0xD4D0, n))  return  'y';

	if (In(0xD4D1, 0xD7F9, n))  return  'z';

	return  '\0';
}

bool  In(wchar_t  start, wchar_t  end, wchar_t  code)
{
	if (code >= start  &&  code <= end)
	{
		return  true;
	}

	return  false;
}

void CNtoPY(const char* szCn, char* outEn)
{
	int szLen = strlen(szCn);
	char  chr[3] = { 0 };
	wchar_t  wchr = 0;

	for (int i = 0, j = 0; i < (szLen / 2); ++i)
	{
		memset(chr, 0x00, sizeof(chr));

		chr[0] = szCn[j++];

		chr[1] = szCn[j++];

		chr[2] = '\0';

		//  单个字符的编码  如：'我'  =  0xced2

		wchr = 0;

		wchr = (chr[0] & 0xff) << 8;

		wchr |= (chr[1] & 0xff);

		outEn[i] = convert(wchr);
	}
}