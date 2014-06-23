#include "TrayIcon.h"
#pragma once


// TrayAPP 对话框

class TrayAPP : public CDialog
{
	DECLARE_DYNAMIC(TrayAPP)

public:
	TrayAPP(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~TrayAPP();

// 对话框数据
	enum { IDD = IDD_DIALOG_TRAYICON };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	HICON   m_hIcon;
	CString trayTitle;
	CTrayIcon m_TrayIcon;
	afx_msg LRESULT OnTrayNotification(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDestroy();
	void LoadTrayICON();

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	int lbuttonCurrent;

	NOTIFYICONDATA nid;
};
