// VCHelper.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "VCHelper.h"
#include "VCHelperDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVCHelperApp

BEGIN_MESSAGE_MAP(CVCHelperApp, CWinApp)
	//{{AFX_MSG_MAP(CVCHelperApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVCHelperApp construction

CVCHelperApp::CVCHelperApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CVCHelperApp object

CVCHelperApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CVCHelperApp initialization

BOOL CVCHelperApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

	SetDialogBkColor( RGB( 120, 180, 255 ) );

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CVCHelperDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

void CVCHelperApp::GetInitFilePath( CString& strPath )
{//////////////////////////////////////////////////////////////////////////////
// 函数名称: GetInitFilePath
// 内容简介: Get the initialize setup file name
// 功    能: 略
// 参    数: CString&
//	It is to save the full path name of the file.
// 返    回: void
// 作    者: L.F.
// 开始日期: 2005.08.21
///////////////////////////////////////////////////////////////////////////////

	static LPCTSTR strFile = _T( "\\Setup.ini" );

	TCHAR strInitPath[ MAX_PATH ] = { '\0' };

	GetCurrentDirectory( MAX_PATH, strInitPath );
	lstrcat( strInitPath, strFile );

	strPath = strInitPath;
}
