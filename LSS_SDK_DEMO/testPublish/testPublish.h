// testPublish.h : main header file for the TESTPUBLISH application
//

#if !defined(AFX_TESTPUBLISH_H__2F8A82B2_BEA3_4156_8FF8_CBE918F1713F__INCLUDED_)
#define AFX_TESTPUBLISH_H__2F8A82B2_BEA3_4156_8FF8_CBE918F1713F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTestPublishApp:
// See testPublish.cpp for the implementation of this class
//

class CTestPublishApp : public CWinApp
{
public:
	CTestPublishApp();



// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestPublishApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTestPublishApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTPUBLISH_H__2F8A82B2_BEA3_4156_8FF8_CBE918F1713F__INCLUDED_)
