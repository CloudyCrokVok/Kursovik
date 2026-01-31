// KURSACH.h: основной файл заголовка для приложения KURSACH
//
#pragma once

#ifndef __AFXWIN_H__
#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"       // основные символы


// CKURSACHApp:
// Сведения о реализации этого класса: KURSACH.cpp
//

class CKURSACHApp : public CWinApp
{
public:
	CKURSACHApp() noexcept;


	// Переопределение
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	// Реализация
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

public:
	// Добавляем обработчики для меню Сборка
	afx_msg void OnParametrs();
	afx_msg void OnSborka();
};

extern CKURSACHApp theApp;
