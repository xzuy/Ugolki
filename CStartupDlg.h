#pragma once
#include "afxdialogex.h"
#include "pch.h"
#include "MFCUgolkiTestDlg.h"

// Диалоговое окно CStartupDlg

class CStartupDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CStartupDlg)

public:
	CStartupDlg(CWnd* pParent = nullptr);   // стандартный конструктор
	virtual ~CStartupDlg();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_STARTDIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()
private:
	CString strPlayerName1;
	CString strPlayerName2;
	CComboBox mTypePlayer1;
	CComboBox mTypePlayer2;
	CMFCUgolkiTestDlg* dlgParent;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCancel();
	void SetDlgParent(CMFCUgolkiTestDlg* p);
	afx_msg void OnBnClickedStartbutton();
};
