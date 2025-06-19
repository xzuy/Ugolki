// UgolkiField.cpp: файл реализации
//

#include "pch.h"

#define UGOLKIFIELD_CLASSNAME L"UgolkiField"
#define FIELDNUMBERSSPACE 20
#define TIMER_ID 101

// UgolkiField

IMPLEMENT_DYNAMIC(UgolkiField, CWnd)

UgolkiField::UgolkiField()
{
	gameParent = nullptr;
	this->RegisterClass();
	fieldSize = 8;
	CompWin = false;
}

UgolkiField::~UgolkiField()
{
}


BEGIN_MESSAGE_MAP(UgolkiField, CWnd)
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_DESTROY()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
END_MESSAGE_MAP()



// Обработчики сообщений UgolkiField




bool UgolkiField::RegisterClass()
{
	WNDCLASS wndcls;
	HINSTANCE hInst = AfxGetInstanceHandle();

	if (!(::GetClassInfo(hInst, UGOLKIFIELD_CLASSNAME, &wndcls)))
	{
		wndcls.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
		wndcls.lpfnWndProc = ::DefWindowProc;
		wndcls.cbClsExtra = wndcls.cbWndExtra = 0;
		wndcls.hInstance = hInst;
		wndcls.hIcon = NULL;
		wndcls.hCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
		wndcls.hbrBackground = (HBRUSH)(COLOR_3DFACE + 1);
		wndcls.lpszMenuName = NULL;
		wndcls.lpszClassName = UGOLKIFIELD_CLASSNAME;

		if (!AfxRegisterClass(&wndcls))
		{
			AfxThrowResourceException();
			return FALSE;
		}
	}
	return TRUE;
}

void UgolkiField::OnPaint()
{
	CPaintDC dc(this);

	if ((this->gameParent == nullptr) || (this->gameParent->GetBoard() == nullptr))
		return;
	CRect rect;
	GetClientRect(&rect);

	CPen ourPen;
	HGDIOBJ oldPen;
	ourPen.CreatePen(PS_SOLID, 2, RGB(0, 0, 0));

	oldPen = dc.SelectObject(ourPen);

	dc.MoveTo(0, 0);
	dc.LineTo(rect.right, 0);
	dc.LineTo(rect.right, rect.bottom);
	dc.LineTo(0, rect.bottom);
	dc.LineTo(0, 0);

	int hPartSize = (rect.right - FIELDNUMBERSSPACE) / 8;
	int vPartSize = (rect.right - FIELDNUMBERSSPACE) / 8;
	
	this->HighlightSelection(dc);

	for (int i = 0; i < 9; i++)
	{
		dc.MoveTo(FIELDNUMBERSSPACE + i * hPartSize, FIELDNUMBERSSPACE);
		dc.LineTo(FIELDNUMBERSSPACE + i * hPartSize, rect.bottom);
		dc.MoveTo(FIELDNUMBERSSPACE, FIELDNUMBERSSPACE + i * vPartSize);
		dc.LineTo(rect.right, i * vPartSize + FIELDNUMBERSSPACE);
	}

	dc.SetTextColor(RGB(255, 0, 0));
	dc.SetTextAlign(TA_TOP | TA_LEFT);
	dc.SetBkMode(TRANSPARENT);
	for (int i = 0; i < 8; i++)
	{
		CString str;
		str.Format(L"%d", i);
		dc.TextOutW(0, FIELDNUMBERSSPACE / 2 + vPartSize / 2 + i * vPartSize, str);
		dc.TextOutW(FIELDNUMBERSSPACE / 2 + hPartSize / 2 + i * hPartSize, 0, str);
	}

	for (int x = 0; x < 8; x++)
	{
		for (int y = 0; y < 8; y++)
		{
			if (this->gameParent->GetBoard()->GetCell(x,y) == CellType_W)
				this->DrawW(dc, this->GetRectFromField(x, y));
			if (this->gameParent->GetBoard()->GetCell(x, y) == CellType_B)
				this->DrawB(dc, this->GetRectFromField(x, y));
		}
	}

	dc.SelectObject(oldPen);
	ourPen.DeleteObject();
}


void UgolkiField::OnLButtonDown(UINT nFlags, CPoint point)
{
	if ((this->gameParent->bGameInProgress)&& (this->gameParent->GetCurrentPlayer()->IsHuman())&& (!Click))
	{
		if ((this->gameParent == nullptr) || (this->gameParent->GetBoard() == nullptr))
			return;
		this->GetFieldPosition(point, x, y);
		Click = true;
	}
	else if ((this->gameParent->bGameInProgress) && (this->gameParent->GetCurrentPlayer()->IsHuman()) && Click)
	{
		if ((this->gameParent == nullptr) || (this->gameParent->GetBoard() == nullptr))
			return;
		this->GetFieldPosition(point, x2, y2);
		Click = false;
		if (this->gameParent->GetCurrentPlayer()->MakeMove(x, y, x2, y2) == true)
		{
			if (this->CheckEndCondition())
			{
				this->gameParent->SetGameProgress(false);
				this->Invalidate();
			}
			else
			{
				this->gameParent->TogglePlayer();
			}
			x = 0;
			y = 0;
			x2 = 0;
			y2 = 0;
			Click = false;
			this->Invalidate();
		}
	}
	CWnd::OnLButtonDown(nFlags, point);
}

void UgolkiField::DrawB(CPaintDC& dc, CRect rect)
{
	CPen pen;
	pen.CreatePen(PS_SOLID, 10, RGB(0, 0, 0));
	HGDIOBJ oldPen = dc.SelectObject(pen);
	CBrush brush;
	brush.CreateSolidBrush(RGB(40, 40, 40));
	HGDIOBJ oldbrush = dc.SelectObject(brush);
	dc.Ellipse(rect.left + 4, rect.top + 4, rect.right - 4, rect.bottom - 4);
	dc.SelectObject(oldPen);
	pen.DeleteObject();
	dc.SelectObject(oldbrush);
	brush.DeleteObject();
}

void UgolkiField::DrawW(CPaintDC& dc, CRect rect)
{
	CPen pen;
	pen.CreatePen(PS_SOLID, 10, RGB(255, 255, 255));
	HGDIOBJ oldPen = dc.SelectObject(pen);
	CBrush brush;
	brush.CreateSolidBrush(RGB(200, 200, 200));
	HGDIOBJ oldbrush = dc.SelectObject(brush);
	dc.Ellipse(rect.left + 4, rect.top + 4, rect.right - 4, rect.bottom - 4);
	dc.SelectObject(oldPen);
	pen.DeleteObject();
	dc.SelectObject(oldbrush);
	brush.DeleteObject();
}


void UgolkiField::GetFieldPosition(CPoint point, int& xpos, int& ypos)
{
	CRect rect;
	GetClientRect(&rect);
	int hPartSize = (rect.right - FIELDNUMBERSSPACE) / 8;
	int vPartSize = (rect.bottom - FIELDNUMBERSSPACE) / 8;
	for (int i = 0; i < 8; i++) {
		if ((point.x > FIELDNUMBERSSPACE + i * hPartSize)&&
			(point.x < FIELDNUMBERSSPACE * (i + 1) * hPartSize))
				xpos = i;
		if ((point.y > FIELDNUMBERSSPACE + i * vPartSize)&&
			(point.y < FIELDNUMBERSSPACE * (i + 1) * vPartSize))
				ypos = i;
	}
}




void UgolkiField::OnDestroy()
{
	CWnd::OnDestroy();
}


CRect UgolkiField::GetRectFromField(int x, int y)
{
	CRect rect;
	CRect rectWnd;
	GetClientRect(&rectWnd);
	int hPartSize = (rectWnd.right - FIELDNUMBERSSPACE) / 8;
	int vPartSize = (rectWnd.bottom - FIELDNUMBERSSPACE) / 8;
	rect.left = FIELDNUMBERSSPACE + x * hPartSize + 1;
	rect.top = FIELDNUMBERSSPACE + y * vPartSize + 1;
	rect.right = FIELDNUMBERSSPACE + ((x + 1) * hPartSize - 1);
	rect.bottom = FIELDNUMBERSSPACE + ((y + 1) * vPartSize - 1);
	return rect;
}


void UgolkiField::HighlightSelection(CPaintDC& dc)
{
	if ((nSelectedX < 0) || (nSelectedY < 0))
		return;
	CRect rect = GetRectFromField(nSelectedX, nSelectedY);
	CBrush brush;
	brush.CreateSolidBrush(RGB(125, 249, 255));
	HGDIOBJ oldbrush = dc.SelectObject(brush);
	dc.Rectangle(rect);
	dc.SelectObject(oldbrush);
	brush.DeleteObject();
}

bool UgolkiField::CheckEndCondition()
{
	if (this->gameParent->GetBoard()->CheckEndCondition() && !CompWin)
	{
		if (this->gameParent->GetBoard()->IsWVictory())
		{
			if (!this->gameParent->GetPlayer1()->IsHuman())
				CompWin = true;
			CString str;
			str.Format(L"Белый игрок победил! Поздравляю, %s", this->gameParent->GetCurrentPlayer()->GetName());
			if (AfxMessageBox(str) == IDOK)
				CompWin = false;
		}
		if (this->gameParent->GetBoard()->IsBVictory())
		{
			if (!this->gameParent->GetPlayer2()->IsHuman())
				CompWin = true;
			CString str;
			str.Format(L"Черный игрок победил! Поздравляю, %s", this->gameParent->GetCurrentPlayer()->GetName());
			if (AfxMessageBox(str) == IDOK)
				CompWin = false;
		}
		else
		{
			AfxMessageBox(L"Ничья!");
		}
		return true;
	}
	return false;
}

void UgolkiField::SetGameInProgress(bool inProgress)
{
	if (bGameInProgress != inProgress)
	{
		this->bGameInProgress = inProgress;
		this->gameParent->SetGameInProgress(inProgress);
		if (inProgress)
		{
			SetTimer(TIMER_ID, 100, NULL);
		}
		else
			KillTimer(TIMER_ID);
	}
}


void UgolkiField::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	int x, y;
	GetFieldPosition(point, x, y);
	if ((nSelectedX != x) || (nSelectedY != y))
	{
		nSelectedX = x;
		nSelectedY = y;
		this->Invalidate();
	}
	TRACKMOUSEEVENT tme;
	tme.cbSize = sizeof(tme);
	tme.hwndTrack = this->m_hWnd;
	tme.dwFlags = TME_LEAVE;
	tme.dwHoverTime = HOVER_DEFAULT;
	TrackMouseEvent(&tme);
	CWnd::OnMouseMove(nFlags, point);
}

void UgolkiField::OnMouseLeave()
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	nSelectedX = -1;
	nSelectedY = -1;
	this->Invalidate();
	CWnd::OnMouseLeave();
}

void UgolkiField::SetGameParent(CMFCUgolkiTestDlg* p)
{
	this->gameParent = p;
}

void UgolkiField::OnTimer(UINT_PTR nIDEvent)
{
	if ((bGameInProgress) && (!this->gameParent->GetCurrentPlayer()->IsHuman()))
	{
		if ((this->gameParent == nullptr) || (this->gameParent->GetBoard() == nullptr)) 
			return;
		if (this->gameParent->GetCurrentPlayer()->MakeMove(0, 0, 0, 0) == true)
		{
			if (this->CheckEndCondition())
			{
				this->SetGameInProgress(false);
				this->Invalidate();
			}
			else
			{
				this->gameParent->TogglePlayer();
			}
		}
		if (this->CheckEndCondition())
		{
			this->SetGameInProgress(false);
			this->Invalidate();
		}
	}
	CWnd::OnTimer(nIDEvent);
}