
// Kolebaniya.h: главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CKolebaniyaApp:
// Сведения о реализации этого класса: Kolebaniya.cpp
//

class CKolebaniyaApp : public CWinApp
{
public:
	CKolebaniyaApp();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CKolebaniyaApp theApp;
