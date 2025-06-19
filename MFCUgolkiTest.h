
// MFCUgolkiTest.h: главный файл заголовка для приложения PROJECT_NAME
//

#pragma once
#include "pch.h"

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CMFCUgolkiTestApp:
// Сведения о реализации этого класса: MFCUgolkiTest.cpp
//

class CMFCUgolkiTestApp : public CWinApp
{
public:
	CMFCUgolkiTestApp();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CMFCUgolkiTestApp theApp;
