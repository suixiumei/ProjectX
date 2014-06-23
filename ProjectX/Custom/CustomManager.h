#pragma once

class CustomManager
{
public:
	static CustomManager* GetInstance();

	void ClearCache();

public:	//WebBrowser_Method
	void WebObjInit(IWebBrowser2* lpWB);
	void RegeditCallFunc();
	void CallJScript(CComBSTR JSFunc);
};
