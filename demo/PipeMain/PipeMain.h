// pipiFather.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CPipeMainApp:
// See pipiFather.cpp for the implementation of this class
//

class CPipeMainApp : public CWinApp
{
public:
	CPipeMainApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CPipeMainApp theApp;