#pragma once

#include <iostream>
#include <vector>
#include "nb30.h"

#define C1 77
#define C2 73

typedef struct _ASTAT_
{
	ADAPTER_STATUS adapt;
	NAME_BUFFER    NameBuff[30];
}ASTAT, *PASTAT;
typedef struct AdapterStrct
{
	int nIndex;			//网卡编号
	CString strMac;		//Mac地址（字符串形式）
}ADPTSTRCT;
class CommonFunc
{
public:
	/************************************************************************/
	/* Pro                                                                     */
	/************************************************************************/
	static BOOL com_method_Init();			//主程初始化函数
	static void com_method_CheckUpdate();			//检测更新
	static WCHAR* com_method_GetResDir();			//获取主程当前目录 带有'\\'
	static BOOL com_method_AppIsStart(CString filename);		//判断指定进程是否存在
	static CString com_method_Login(CString name, CString pass);		//登录账号
	static CString com_method_GetStringINI(CString lpAppName,CString lpKeyName,CString lpDefault,CString lpFileName);			//获得配置：指定键值
	static void com_method_WriteINI(CString lpAppName,CString lpKeyName,CString lpString,CString lpFileName);			//设置配置：指定键值
	static int com_method_FindLastPos(CString src, CString tar);		//查找指定字符在字符串最后出现位置
	static void com_method_Split(CString content, CString pattern, CStringArray& strlist);			//分割字符串
	static BOOL com_method_CopyDir(CString strSrcPath, CString strDstPath);			//复制文件夹
	static void com_method_DelFolder(CString lpszFromPath);			//删除文件夹
	static void com_method_ChunkBolt(CString fileName,const char* funName,int numbers,...);			//Chunk-LUA
	static void com_method_ChunkBoltTable(CString fileName, const char* funName, CStringArray& strlist);			//Chunk-LUA-TABLE
	static BOOL com_method_DownXML(CString downurl, CString filename);			//下载XML
	static CString com_method_ResolveHTMLXML(CString value);			//解析HTMLXML

	/************************************************************************/
	/* Sys                                                                     */
	/************************************************************************/
	static void com_method_ElevatePrivileges();			//提升主程系统权限
	static void com_method_ClearType();
	static BOOL com_method_CheckOnline();			//检查本机联网状态
	static CString com_method_GetMacByCmd();	//获得本机MAC地址
	static CString com_method_GetRegeditByCLASSES_ROOT(CString subKey,CString valueName);		//获得注册表CLASSES_ROOT键值
	static CString com_method_GetRegeditByCURRENT_USER(CString subKey, CString valueName);		//获得注册表CURRENT_USER键值
	static void com_method_SetRegedit(HKEY hKeyParent,LPCTSTR lpszkeyName,LPCTSTR pszkeyValue,LPCTSTR pszValueName);			//设置注册表指定键值
	static CString com_method_DeleteRegedit(HKEY hKeyParent,LPCTSTR lpszkeyName,LPCTSTR pszkeyValue);			//删除注册表指定键值
	CString com_method_ReadPageByPost(char* requestData,CString siteUrl, CString pageUrl);			//POST方式访问URL
};

