// led_tool.h : main header file for the LED_TOOL application
//

#if !defined(AFX_LED_TOOL_H__6A913922_B24E_45D7_8002_8507B2925C40__INCLUDED_)
#define AFX_LED_TOOL_H__6A913922_B24E_45D7_8002_8507B2925C40__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CLed_toolApp:
// See led_tool.cpp for the implementation of this class
//

class CLed_toolApp : public CWinApp
{
public:
	CLed_toolApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLed_toolApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CLed_toolApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LED_TOOL_H__6A913922_B24E_45D7_8002_8507B2925C40__INCLUDED_)
