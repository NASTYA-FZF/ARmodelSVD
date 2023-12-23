
// ARmodelSVD.h: главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CARmodelSVDApp:
// Сведения о реализации этого класса: ARmodelSVD.cpp
//

class CARmodelSVDApp : public CWinApp
{
public:
	CARmodelSVDApp();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CARmodelSVDApp theApp;
