#pragma once
#include "stdafx.h"

class CVariant : public VARIANT
{
public:
	CVariant()
	{
		VariantInit(this);
	}
	CVariant(int i)
	{
		VariantInit(this);
		this->vt = VT_I4;
		this->intVal = i;
	}
	CVariant(float f)
	{
		VariantInit(this);
		this->vt = VT_R4;
		this->fltVal = f;
	}
	CVariant(LPOLESTR s)
	{
		VariantInit(this);
		this->vt = VT_BSTR;
		this->bstrVal = s;
	}
	CVariant(IDispatch *disp)
	{
		VariantInit(this);
		this->vt = VT_DISPATCH;
		this->pdispVal = disp;
	}

	~CVariant()
	{
		VariantClear(this);
	}
};

void CppCall()
{
	MessageBox(NULL, L"��������CppCall", L"��ʾ(C++)", 0);
}

class ClientCall :public IDispatch
{
	long _refNum;
public:
	ClientCall()
	{
		_refNum = 1;
	}
	~ClientCall(void)
	{
	}
public:

	// IUnknown Methods

	STDMETHODIMP QueryInterface(REFIID iid, void**ppvObject)
	{
		*ppvObject = NULL;
		if (iid == IID_IUnknown)	*ppvObject = this;
		else if (iid == IID_IDispatch)	*ppvObject = (IDispatch*)this;
		if (*ppvObject)
		{
			AddRef();
			return S_OK;
		}
		return E_NOINTERFACE;
	}

	STDMETHODIMP_(ULONG) AddRef()
	{
		return ::InterlockedIncrement(&_refNum);
	}

	STDMETHODIMP_(ULONG) Release()
	{
		::InterlockedDecrement(&_refNum);
		if (_refNum == 0)
		{
			delete this;
		}
		return _refNum;
	}

	// IDispatch Methods

	HRESULT _stdcall GetTypeInfoCount(
		unsigned int * pctinfo)
	{
		return E_NOTIMPL;
	}

	HRESULT _stdcall GetTypeInfo(
		unsigned int iTInfo,
		LCID lcid,
		ITypeInfo FAR* FAR* ppTInfo)
	{
		return E_NOTIMPL;
	}

	HRESULT _stdcall GetIDsOfNames(
		REFIID riid,
		OLECHAR FAR* FAR* rgszNames,
		unsigned int cNames,
		LCID lcid,
		DISPID FAR* rgDispId
		)
	{
		if (lstrcmp(rgszNames[0], L"CppCall") == 0)
		{
			//��ҳ����window.external.CppCallʱ����������������ȡCppCall��ID
			*rgDispId = 100;
		}
		return S_OK;
	}

	HRESULT _stdcall Invoke(
		DISPID dispIdMember,
		REFIID riid,
		LCID lcid,
		WORD wFlags,
		DISPPARAMS* pDispParams,
		VARIANT* pVarResult,
		EXCEPINFO* pExcepInfo,
		unsigned int* puArgErr
		)
	{
		if (dispIdMember == 100)
		{
			//��ҳ����CppCallʱ������ݻ�ȡ����ID����Invoke����
			CppCall();
		}
		return S_OK;
	}
};

typedef void _stdcall JsFunction_Callback(LPVOID pParam);

class JsFunction :public IDispatch
{
	long _refNum;
	JsFunction_Callback *m_pCallback;
	LPVOID m_pParam;
public:
	JsFunction(JsFunction_Callback *pCallback, LPVOID pParam)
	{
		_refNum = 1;
		m_pCallback = pCallback;
		m_pParam = pParam;
		OutputDebugString(L"X:" + (CString)(LPCTSTR)m_pParam);
	}
	~JsFunction(void)
	{
	}
public:

	// IUnknown Methods

	STDMETHODIMP QueryInterface(REFIID iid, void**ppvObject)
	{
		*ppvObject = NULL;

		if (iid == IID_IUnknown)	*ppvObject = this;
		else if (iid == IID_IDispatch)	*ppvObject = (IDispatch*)this;

		if (*ppvObject)
		{
			AddRef();
			return S_OK;
		}
		return E_NOINTERFACE;
	}

	STDMETHODIMP_(ULONG) AddRef()
	{
		return ::InterlockedIncrement(&_refNum);
	}

	STDMETHODIMP_(ULONG) Release()
	{
		::InterlockedDecrement(&_refNum);
		if (_refNum == 0)
		{
			delete this;
		}
		return _refNum;
	}

	// IDispatch Methods

	HRESULT _stdcall GetTypeInfoCount(
		unsigned int * pctinfo)
	{
		return E_NOTIMPL;
	}

	HRESULT _stdcall GetTypeInfo(
		unsigned int iTInfo,
		LCID lcid,
		ITypeInfo FAR* FAR* ppTInfo)
	{
		return E_NOTIMPL;
	}

	HRESULT _stdcall GetIDsOfNames(
		REFIID riid,
		OLECHAR FAR* FAR* rgszNames,
		unsigned int cNames,
		LCID lcid,
		DISPID FAR* rgDispId
		)
	{
		//���˷ѽ���ǣ���ҳ���ú�����call����ʱ��û�е���GetIDsOfNames��ȡcall��ID������ֱ�ӵ���Invoke
		return E_NOTIMPL;
	}

	HRESULT _stdcall Invoke(
		DISPID dispIdMember,
		REFIID riid,
		LCID lcid,
		WORD wFlags,
		DISPPARAMS* pDispParams,
		VARIANT* pVarResult,
		EXCEPINFO* pExcepInfo,
		unsigned int* puArgErr
		)
	{
		m_pCallback(m_pParam);
		return S_OK;
	}
};

class WebBrowserBase:
	public IDispatch,
	public IOleClientSite,
	public IOleInPlaceSite,
	public IOleInPlaceFrame,
	public IDocHostUIHandler
{
public:
	ClientCall *pClientCall;
	WebBrowserBase()
	{
		pClientCall = new ClientCall();
	}
	~WebBrowserBase();

	static DISPID FindId(IDispatch *pObj, LPOLESTR pName);
	static HRESULT InvokeMethod(IDispatch *pObj, LPOLESTR pName, VARIANT *pVarResult, VARIANT *p, int cArgs);
	static HRESULT GetProperty(IDispatch *pObj, LPOLESTR pName, VARIANT *pValue);
	static HRESULT SetProperty(IDispatch *pObj, LPOLESTR pName, VARIANT *pValue);
	static CString GetButtonCallC(CString buttonID, JsFunction_Callback *m_pCallback);
public:
	//IDocHostUIHandler
	virtual HRESULT STDMETHODCALLTYPE GetExternal(IDispatch **ppDispatch)
	{
		//��дGetExternal����һ��ClientCall����
		*ppDispatch = pClientCall;
		return S_OK;
	}
};

