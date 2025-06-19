
// MFCUgolkiTestDlg.h: файл заголовка
//

#pragma once
#include "UgolkiField.h"
#include "pch.h"
// Диалоговое окно CMFCUgolkiTestDlg
class CMFCUgolkiTestDlg : public CDialogEx
{
// Создание
public:
	CMFCUgolkiTestDlg(CWnd* pParent = nullptr);	// стандартный конструктор
// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCUGOLKITEST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;
	UgolkiField uuField;
	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton3();
	void CreateBoard();
	void CreatePlayer1(CString name, int type);
	void CreatePlayer2(CString name, int type);
	Board* GetBoard();
	UPlayer* GetCurrentPlayer();
	UPlayer* GetPlayer1();
	UPlayer* GetPlayer2();
	void SetGameProgress(bool bGameInProgress);
	void TogglePlayer();
	bool bGameInProgress = false;
	bool inProgress = false;
	void SetGameInProgress(bool inProgress);
private:
	Board* board;
	UPlayer* Player1;
	UPlayer* Player2;
	UPlayer* CurrentPlayer;
	void CleanUp();
	CButton mStartStopButton;
	void UpdateName();
public:
	CString strPlayerTurn;
private:
	CString WhiteTurns;
	CString BlackTurns;
};
