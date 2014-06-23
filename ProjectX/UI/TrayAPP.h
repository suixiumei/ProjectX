#include "TrayIcon.h"
#pragma once


// TrayAPP �Ի���

class TrayAPP : public CDialog
{
	DECLARE_DYNAMIC(TrayAPP)

public:
	TrayAPP(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~TrayAPP();

// �Ի�������
	enum { IDD = IDD_DIALOG_TRAYICON };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
