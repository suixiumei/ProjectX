// D:\360yunpan\VC项目\ProjectX\ProjectX\UI\TrayAPP.cpp : 实现文件
//

#include "stdafx.h"
#include "..\ProjectX.h"
#include "TrayAPP.h"
#include "afxdialogex.h"

#define WM_ICON_NOTIFY WM_USER+10
// TrayAPP 对话框

IMPLEMENT_DYNAMIC(TrayAPP, CDialog)

TrayAPP::TrayAPP(CWnd* pParent /*=NULL*/)
	: CDialog(TrayAPP::IDD, pParent)
{

}

TrayAPP::~TrayAPP()
{
}

void TrayAPP::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(TrayAPP, CDialog)
	ON_WM_TIMER()
	ON_MESSAGE(WM_ICON_NOTIFY, OnTrayNotification)
END_MESSAGE_MAP()


// TrayAPP 消息处理程序
void TrayAPP::LoadTrayICON()
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	trayTitle.LoadStringW(IDS_TITLE);

	//托盘图标
	CWnd * pCWnd;
	pCWnd = FromHandle(m_hWnd);
	m_TrayIcon.Create(pCWnd, WM_ICON_NOTIFY, trayTitle + L"魔笛", m_hIcon, NULL);
}

LRESULT TrayAPP::OnTrayNotification(WPARAM wParam, LPARAM lParam)
{
	if (LOWORD(lParam) == WM_RBUTTONUP)
	{
		//右键点击
		CPoint pos;
		GetCursorPos(&pos);
		CString strX, strY;
		strX.Format(L"%d", pos.x);
		strY.Format(L"%d", pos.y);
		com.com_method_ChunkBolt(L"resultMainHelper.lua", "resultTrayMenu", 2, strX, strY);
	}
	else if (LOWORD(lParam) == WM_LBUTTONUP)
	{
		//左键点击
		/*SetTimer(1, 300, 0);
		lbuttonCurrent++;*/
		CPoint pos;
		GetCursorPos(&pos);
		CString strX, strY;
		strX.Format(L"%d", pos.x);
		strY.Format(L"%d", pos.y);
		com.com_method_ChunkBolt(L"resultMainHelper.lua", "resultTrayMenu", 2, strX, strY);
	}
	return 1;
}

void TrayAPP::OnDestroy()
{
	m_TrayIcon.RemoveIcon();
	CDialog::OnDestroy();
}

void TrayAPP::OnTimer(UINT nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == 1)
	{
		KillTimer(1);
		lbuttonCurrent = 0;
		//OutputDebugStringX(L"托盘图标左键点击");
		com.com_method_ChunkBolt(L"resultMainHelper.lua", "Restore", 0);
	}
	CDialog::OnTimer(nIDEvent);
}