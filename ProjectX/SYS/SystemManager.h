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
	CString CovertChineseToPY(CString sCh); //将一段传入的中文转换为拼音首字母并返回
	CString CovertToUTF8URLEncode(CString sCh); //将一段传入的sCh转换为utf8的urlencode并返回
	void GetScreenSize(int &nScreenX, int&nScreenY);//获取屏幕宽高：去掉任务栏后
};
