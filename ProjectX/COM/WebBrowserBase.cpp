#include "stdafx.h"
#include "WebBrowserBase.h"

WebBrowserBase::~WebBrowserBase()
{
}

DISPID WebBrowserBase::FindId(IDispatch *pObj, LPOLESTR pName)
{
	DISPID id = 0;
	if (pObj==NULL)
	{
		id = -1;
	}
	else
	{
		if (FAILED(pObj->GetIDsOfNames(IID_NULL, &pName, 1, LOCALE_SYSTEM_DEFAULT, &id))) id = -1;
	}

	return id;
}

HRESULT WebBrowserBase::InvokeMethod(IDispatch *pObj, LPOLESTR pName, VARIANT *pVarResult, VARIANT *p, int cArgs)
{
	DISPID dispid = FindId(pObj, pName);
	if (dispid == -1) return E_FAIL;

	DISPPARAMS ps;
	ps.cArgs = cArgs;
	ps.rgvarg = p;
	ps.cNamedArgs = 0;
	ps.rgdispidNamedArgs = NULL;

	return pObj->Invoke(dispid, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, &ps, pVarResult, NULL, NULL);
}

HRESULT WebBrowserBase::GetProperty(IDispatch *pObj, LPOLESTR pName, VARIANT *pValue)
{
	DISPID dispid = FindId(pObj, pName);
	if (dispid == -1) return E_FAIL;
		
	DISPPARAMS ps;
	ps.cArgs = 0;
	ps.rgvarg = NULL;
	ps.cNamedArgs = 0;
	ps.rgdispidNamedArgs = NULL;

	return pObj->Invoke(dispid, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_PROPERTYGET, &ps, pValue, NULL, NULL);
}

HRESULT WebBrowserBase::SetProperty(IDispatch *pObj, LPOLESTR pName, VARIANT *pValue)
{
	DISPID dispid = FindId(pObj, pName);

	if (dispid == -1) return E_FAIL;

	DISPPARAMS ps;
	ps.cArgs = 1;
	ps.rgvarg = pValue;
	ps.cNamedArgs = 0;
	ps.rgdispidNamedArgs = NULL;

	return pObj->Invoke(dispid, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_PROPERTYPUT, &ps, NULL, NULL, NULL);
}

CString WebBrowserBase::GetButtonCallC(CString buttonID, JsFunction_Callback *m_pCallback)
{
	//获取页面DOC元素
	CVariant document;
	GetProperty(gc_pScript, L"document", &document);

	//获取网页指定元素
	VARIANT params[1];
	params[0].vt = VT_BSTR;
	params[0].bstrVal = buttonID.AllocSysString();
	CVariant button;
	InvokeMethod(document.pdispVal, L"getElementById", &button, params, 1);

	CVariant h_value;
	GetProperty(button.pdispVal, L"h", &h_value);
	//return ((CString)h_value);
	
	//处理网页元素的onclick事件
	params[0].vt = VT_DISPATCH;
	params[0].pdispVal = new JsFunction(m_pCallback, (LPVOID)(LPCTSTR)((CString)h_value));
	SetProperty(button.pdispVal, L"onclick", params);

	return NULL;
}