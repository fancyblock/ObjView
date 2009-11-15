// OBJ_VIEW.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// COBJ_VIEWApp:
// See OBJ_VIEW.cpp for the implementation of this class
//

class COBJ_VIEWApp : public CWinApp
{
public:
	COBJ_VIEWApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation
	DECLARE_MESSAGE_MAP()
};

extern COBJ_VIEWApp theApp;