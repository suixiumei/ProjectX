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
	int nIndex;			//�������
	CString strMac;		//Mac��ַ���ַ�����ʽ��
}ADPTSTRCT;
class CommonFunc
{
public:
	/************************************************************************/
	/* Pro                                                                     */
	/************************************************************************/
	static BOOL com_method_Init();			//���̳�ʼ������
	static void com_method_CheckUpdate();			//������
	static WCHAR* com_method_GetResDir();			//��ȡ���̵�ǰĿ¼ ����'\\'
	static BOOL com_method_AppIsStart(CString filename);		//�ж�ָ�������Ƿ����
	static CString com_method_Login(CString name, CString pass);		//��¼�˺�
	static CString com_method_GetStringINI(CString lpAppName,CString lpKeyName,CString lpDefault,CString lpFileName);			//������ã�ָ����ֵ
	static void com_method_WriteINI(CString lpAppName,CString lpKeyName,CString lpString,CString lpFileName);			//�������ã�ָ����ֵ
	static int com_method_FindLastPos(CString src, CString tar);		//����ָ���ַ����ַ���������λ��
	static void com_method_Split(CString content, CString pattern, CStringArray& strlist);			//�ָ��ַ���
	static BOOL com_method_CopyDir(CString strSrcPath, CString strDstPath);			//�����ļ���
	static void com_method_DelFolder(CString lpszFromPath);			//ɾ���ļ���
	static void com_method_ChunkBolt(CString fileName,const char* funName,int numbers,...);			//Chunk-LUA
	static void com_method_ChunkBoltTable(CString fileName, const char* funName, CStringArray& strlist);			//Chunk-LUA-TABLE
	static BOOL com_method_DownXML(CString downurl, CString filename);			//����XML
	static CString com_method_ResolveHTMLXML(CString value);			//����HTMLXML

	/************************************************************************/
	/* Sys                                                                     */
	/************************************************************************/
	static void com_method_ElevatePrivileges();			//��������ϵͳȨ��
	static void com_method_ClearType();
	static BOOL com_method_CheckOnline();			//��鱾������״̬
	static CString com_method_GetMacByCmd();	//��ñ���MAC��ַ
	static CString com_method_GetRegeditByCLASSES_ROOT(CString subKey,CString valueName);		//���ע���CLASSES_ROOT��ֵ
	static CString com_method_GetRegeditByCURRENT_USER(CString subKey, CString valueName);		//���ע���CURRENT_USER��ֵ
	static void com_method_SetRegedit(HKEY hKeyParent,LPCTSTR lpszkeyName,LPCTSTR pszkeyValue,LPCTSTR pszValueName);			//����ע���ָ����ֵ
	static CString com_method_DeleteRegedit(HKEY hKeyParent,LPCTSTR lpszkeyName,LPCTSTR pszkeyValue);			//ɾ��ע���ָ����ֵ
	CString com_method_ReadPageByPost(char* requestData,CString siteUrl, CString pageUrl);			//POST��ʽ����URL
};

