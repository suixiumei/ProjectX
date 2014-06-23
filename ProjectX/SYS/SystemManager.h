#pragma once


class SystemManager
{
public:
	static SystemManager* GetInstance();

	CString GetSkinID();
	CString GetTutorials();
	CString GetVersion();
	void CheckUpdate();
	CString GetProcessIsNot(CString processName);
	CString CheckOnline();
	CString GetINI(CString strOne, CString strTwo, CString strThree);
	void SetINI(CString strOne, CString strTwo, CString strThree);
	void OpenUrl(CString url);
	void FuncStatis(char *buf);
	void CopyCString(CString strCopy);
	void Quit();
	CString CovertChineseToPY(CString sCh); //��һ�δ��������ת��Ϊƴ������ĸ������
	CString CovertToUTF8URLEncode(CString sCh); //��һ�δ����sChת��Ϊutf8��urlencode������
	void GetScreenSize(int &nScreenX, int&nScreenY);//��ȡ��Ļ��ߣ�ȥ����������
};
