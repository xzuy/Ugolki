#pragma once
#include "pch.h"

// UgolkiField
class CMFCUgolkiTestDlg;

class UgolkiField : public CWnd
{
	DECLARE_DYNAMIC(UgolkiField)
protected:
	DECLARE_MESSAGE_MAP()
public:
	UgolkiField();
	virtual ~UgolkiField();
	afx_msg void OnPaint();
//	afx_msg void OnTimer(UINT_PTR nIDEvent);
	int fieldSize;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	void DrawW(CPaintDC& dc, CRect rect);
	void DrawB(CPaintDC& dc, CRect rect);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	void SetGameParent(CMFCUgolkiTestDlg* p);
	afx_msg void OnDestroy();
	int x, y, x2, y2;
	bool CompWin;
	bool bGameInProgress = false;
	void SetGameInProgress(bool inProgress);
private:
	bool RegisterClass();
	void GetFieldPosition(CPoint point, int& xpos, int& ypos);
	CRect GetRectFromField(int x, int y);
	int nSelectedX;
	int nSelectedY;
	CMFCUgolkiTestDlg* gameParent;
	void HighlightSelection(CPaintDC& dc);
	bool CheckEndCondition();
	bool Click, Cell;
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};