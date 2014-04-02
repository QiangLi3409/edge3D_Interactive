
// edge3D Studio.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// Cedge3DStudioApp:
// See edge3D Studio.cpp for the implementation of this class
//

class Cedge3DStudioApp : public CWinApp
{
public:
	Cedge3DStudioApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern Cedge3DStudioApp theApp;
extern CSize g_sizeImageAnalyzer;