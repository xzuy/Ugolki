
// MFCUgolkiTestDlg.cpp: файл реализации
//

#include "pch.h"
#include "framework.h"
#include "MFCUgolkiTest.h"
#include "MFCUgolkiTestDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Диалоговое окно CAboutDlg используется для описания сведений о приложении

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Диалоговое окно CMFCUgolkiTestDlg



CMFCUgolkiTestDlg::CMFCUgolkiTestDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCUGOLKITEST_DIALOG, pParent)
	, strPlayerTurn(_T(""))
	, WhiteTurns(_T(""))
	, BlackTurns(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	this->Player1 = nullptr;
	this->Player2 = nullptr;
	this->CurrentPlayer = nullptr;
	this->board = nullptr;
	inProgress = true;
}

void CMFCUgolkiTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_UGOLKIFIELD, uuField);
	DDX_Control(pDX, IDC_BUTTON3, mStartStopButton);
	DDX_Text(pDX, IDC_STATIC1, strPlayerTurn);
	DDX_Text(pDX, IDC_STATICWHITE, WhiteTurns);
	DDX_Text(pDX, IDC_STATICBLACK, BlackTurns);
}

BEGIN_MESSAGE_MAP(CMFCUgolkiTestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON3, &CMFCUgolkiTestDlg::OnBnClickedButton3)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// Обработчики сообщений CMFCUgolkiTestDlg

BOOL CMFCUgolkiTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Добавление пункта "О программе..." в системное меню.

	// IDM_ABOUTBOX должен быть в пределах системной команды.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию

	uuField.SetGameParent(this);

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

void CMFCUgolkiTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CMFCUgolkiTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CMFCUgolkiTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMFCUgolkiTestDlg::OnBnClickedButton3()
{
	if (bGameInProgress)
	{
		CleanUp();
		this->SetGameInProgress(false);
		this->Invalidate();
	}
	else
	{
		CStartupDlg startdlg;
		startdlg.SetDlgParent(this);
		int nRes = static_cast<int>(startdlg.DoModal());
		if (nRes == IDOK)
		{
			CurrentPlayer = Player1;
			this->SetGameInProgress(true);
			this->UpdateData();
			this->Invalidate();
		}
	}
}

void CMFCUgolkiTestDlg::CreateBoard()
{
	this->board = new Board();
}

void CMFCUgolkiTestDlg::CreatePlayer1(CString name, int type)
{
	if (type == 0)
	{
		this->Player1 = new CPlayer();
	}
	else
	{
		this->Player1 = new HPlayer();
	}
	this->Player1->SetupPlayer(name, CellType_W, this->board);
}

void CMFCUgolkiTestDlg::CreatePlayer2(CString name, int type)
{
	if (type == 0)
	{
		this->Player2 = new CPlayer();
	}
	else
	{
		this->Player2 = new HPlayer();
	}
	this->Player2->SetupPlayer(name, CellType_B, this->board);
}

Board* CMFCUgolkiTestDlg::GetBoard()
{
	return board;
}

UPlayer* CMFCUgolkiTestDlg::GetCurrentPlayer()
{
	return CurrentPlayer;
}

UPlayer* CMFCUgolkiTestDlg::GetPlayer1()
{
	return Player1;
}

UPlayer* CMFCUgolkiTestDlg::GetPlayer2()
{
	return Player2;
}

void CMFCUgolkiTestDlg::SetGameProgress(bool bGameInProgress)
{
	this->bGameInProgress = bGameInProgress;
}

void CMFCUgolkiTestDlg::TogglePlayer()
{
	if (this->CurrentPlayer != Player1)
		this->CurrentPlayer = Player1;
	else
		this->CurrentPlayer = Player2;
	UpdateName();
}

void CMFCUgolkiTestDlg::CleanUp()
{
	if (Player1 != nullptr)
	{
		delete Player1;
		Player1 = nullptr;
	}
	if (Player2 != nullptr)
	{
		delete Player2;
		Player2 = nullptr;
	}
	if (board != nullptr)
	{
		delete board;
		board = nullptr;
	}
	CurrentPlayer = nullptr;
}

void CMFCUgolkiTestDlg::SetGameInProgress(bool inProgress)
{
	this->bGameInProgress = inProgress;
	this->uuField.SetGameInProgress(inProgress);
	if (inProgress)
	{
		mStartStopButton.SetWindowTextW(L"Остановить");
	}
	else
	{
		mStartStopButton.SetWindowTextW(L"Запустить");
		CleanUp();
	}
}

void CMFCUgolkiTestDlg::UpdateName()
{
	CString str;
	str.Format(L"Ход игрока: %s", (CString)this->GetCurrentPlayer()->GetName());
	strPlayerTurn = str;
	str.Format(L"Белые сходили: %d", this->GetBoard()->GetCountW());
	WhiteTurns = str;
	str.Format(L"Черные сходили: %d", this->GetBoard()->GetCountB());
	BlackTurns = str;
	UpdateData(FALSE);
}

void CMFCUgolkiTestDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	CleanUp();
}

