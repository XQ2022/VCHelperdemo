// VCHelperDlg.cpp : implementation file
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
// CAboutDlg dialog used for App About

static UINT nIndicators[] = {
	ID_SEPARATOR,
	ID_SEPARATOR,
	ID_SEPARATOR
};

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVCHelperDlg dialog

CVCHelperDlg::CVCHelperDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVCHelperDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVCHelperDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_hTreeItemRoot = NULL;

	m_nFilesCount = 0;
	m_nFilesSize  = 0;

	m_nProgressPos = 0;
}

void CVCHelperDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVCHelperDlg)
	DDX_Control(pDX, IDC_PROGRESS, m_progressCtrl);
	DDX_Control(pDX, IDC_TREE_DIRECTORY, m_treeDirectory);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CVCHelperDlg, CDialog)
	//{{AFX_MSG_MAP(CVCHelperDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TVN_ITEMEXPANDED, IDC_TREE_DIRECTORY, OnItemexpandedTreeDirectory)

	ON_MESSAGE( WM_SET_FINDFILEINFO, OnSetFindFileInfo )
	ON_MESSAGE( WM_SET_EXITTHREAD, OnSetExitThreadMsg )

	ON_COMMAND( IDC_BTN_DEL, OnBtnDel )
	ON_COMMAND( IDC_BTN_DEL_ALL, OnBtnDelAll )

	ON_COMMAND( IDC_BTN_STOP, OnBtnStop )
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVCHelperDlg message handlers

BOOL CVCHelperDlg::OnInitDialog()
{//////////////////////////////////////////////////////////////////////////////
// 函数名称: OninitDialog
// 内容简介: Initialize the control here
// 功    能: Initialize dialog
// 参    数: void
// 返    回: BOOL
//	( TRUE ) Initialize succeessed : ( FALSE ) Initialize failed
// 作    者: L.F.
// 开始日期: 2005.08.13
///////////////////////////////////////////////////////////////////////////////

	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	SetTreeListStyle();

	m_hTreeItemRoot = m_treeDirectory.InsertItem( _T( "我的电脑" ) );
	ASSERT( m_hTreeItemRoot );

	GetLogicalDrives( m_hTreeItemRoot );
	GetDriveDir( m_hTreeItemRoot );

	m_treeDirectory.Expand( m_hTreeItemRoot, TVE_EXPAND );

	VERIFY( CreateEmbededPSheet() );
	PositionEmbededPSheet();

	m_progressCtrl.SetRange( 0, 100 );

	( ( CButton* ) GetDlgItem( IDC_BTN_DEL ) )->EnableWindow( FALSE );
	( ( CButton* ) GetDlgItem( IDC_BTN_DEL_ALL ) )->EnableWindow( FALSE );

	AddStatusBar();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CVCHelperDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CVCHelperDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CVCHelperDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CVCHelperDlg::SetTreeListStyle()
{//////////////////////////////////////////////////////////////////////////////
// 函数名称: SetTreeListStyle
// 内容简介: Set the style for the CTreeList control
// 功    能: 略
// 参    数: void
// 返    回: void
// 作    者: L.F.
// 开始日期: 2005.08.13
///////////////////////////////////////////////////////////////////////////////

	DWORD dwStyle = GetWindowLong( m_treeDirectory.m_hWnd, GWL_STYLE );

	dwStyle |= TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT;

	SetWindowLong( m_treeDirectory.m_hWnd, GWL_STYLE, dwStyle );
}

void CVCHelperDlg::GetDriveDir( HTREEITEM hItem /* = NULL */ )
{//////////////////////////////////////////////////////////////////////////////
// 函数名称: GetDriveDir
// 内容简介: Add the sub-item to the tree control
//	The sub-item is the sub directory to the current drive.
// 功    能: Add sub-item
// 参    数:
//	HTREEITEM hItem: The parent item to the sub-item
// 返    回: void
// 作    者: L.F.
// 开始日期: 2005.08.13
///////////////////////////////////////////////////////////////////////////////

	ASSERT( hItem );

	HTREEITEM hChild = m_treeDirectory.GetChildItem( hItem );
	while ( hChild )
	{
		CString strDir = m_treeDirectory.GetItemText( hChild );
		if ( strDir.Right( 1 ) != _T( "\\" ) )
			strDir += _T( "\\" );

		strDir += _T( "*.*" );

		CFileFind file;

		BOOL bContinue = file.FindFile( strDir );
		while ( bContinue )
		{
			bContinue = file.FindNextFile();
			if ( file.IsDirectory() && ! file.IsDots() )
			{
				m_treeDirectory.InsertItem( file.GetFileName(), hChild );
			}
		}

		GetDriveDir( hChild );
		hChild = m_treeDirectory.GetNextItem( hChild, TVGN_NEXT ); 
	}
}

void CVCHelperDlg::GetLogicalDrives( HTREEITEM hParent /* = NULL */ )
{//////////////////////////////////////////////////////////////////////////////
// 函数名称: GetLogicalDrives
// 内容简介: Get the logical drivers
// 功    能: 略
// 参    数:
//	HTREEITEM hParent: The parent drivers for all logical drivers
// 返    回: void
// 作    者: L.F.
// 开始日期: 2005.08.13
///////////////////////////////////////////////////////////////////////////////

	ASSERT( hParent );

	DWORD dwAllDriveStrings = GetLogicalDriveStrings( 0, NULL );
	ASSERT( dwAllDriveStrings >= 1 );

	LPTSTR lpDriveStrings = NULL;
	lpDriveStrings = new TCHAR[ dwAllDriveStrings + sizeof( _T( "" ) ) ];
	ASSERT( lpDriveStrings != NULL );

	// Save the original address for the drive strings...
	LPTSTR lpTmp = lpDriveStrings;

	GetLogicalDriveStrings( dwAllDriveStrings, lpDriveStrings );

	size_t szDriveString = lstrlen( lpDriveStrings );
	while ( szDriveString > 1 )
	{
		m_treeDirectory.InsertItem( lpDriveStrings, hParent );
		lpDriveStrings += szDriveString + 1;
		szDriveString = lstrlen( lpDriveStrings );
	}

	lpDriveStrings = lpTmp;
	if ( lpDriveStrings )
	{
		delete lpDriveStrings;
		lpDriveStrings = NULL;
	}
	ASSERT( lpDriveStrings == NULL );
}

void CVCHelperDlg::OnItemexpandedTreeDirectory( NMHDR* pNMHDR, LRESULT* pResult ) 
{//////////////////////////////////////////////////////////////////////////////
// 函数名称: OnItemexpandedTreeDirectory
// 内容简介: Set the sub tree directory
// 功    能: Handle the tree control expand message
// 参    数:
//	NMHDR* pNMHDR:
//	LRESULT* pResult: Reference to MSDN
// 返    回: void
// 作    者: L.F.
// 开始日期: 2005.08.14
///////////////////////////////////////////////////////////////////////////////

	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here

	TVITEM tvItem = pNMTreeView->itemNew;
	if ( tvItem.hItem == m_hTreeItemRoot )
		return;

	HTREEITEM hChild = m_treeDirectory.GetChildItem( tvItem.hItem );

	while ( hChild )
	{
		AddSubDirectory( hChild );
		hChild = m_treeDirectory.GetNextItem( hChild, TVGN_NEXT );
	}

	*pResult = 0;
}

void CVCHelperDlg::AddSubDirectory( HTREEITEM hParent /* = NULL */ )
{//////////////////////////////////////////////////////////////////////////////
// 函数名称: AddSubDirectory
// 内容简介: Add the sub-directory to the selected item
// 功    能: 略
// 参    数:
//	HTREEITEM hParent: The selected item handle
// 返    回: void
// 作    者: L.F.
// 开始日期: 2005.08.14
///////////////////////////////////////////////////////////////////////////////
	
	ASSERT( hParent );

	// Delete the previous added child items
	HTREEITEM hChild = m_treeDirectory.GetChildItem( hParent );
	while ( hChild )
	{
		m_treeDirectory.DeleteItem( hChild );
		hChild = m_treeDirectory.GetChildItem( hParent );
	}

	while ( hParent )
	{
		CString strPath = GetFullPath( hParent );
		if ( strPath.Right( 1 ) != "\\" )
			strPath += "\\";
		strPath += _T( "*.*" );

		CFileFind file;

		BOOL bContinue = file.FindFile( strPath );
		while ( bContinue )
		{
			bContinue = file.FindNextFile();
			if ( file.IsDirectory() && ! file.IsDots() )
			{
				m_treeDirectory.InsertItem( file.GetFileName(), hParent );
			}
		}

		hParent = m_treeDirectory.GetNextItem( hParent, TVGN_NEXT );
	}
}

CString CVCHelperDlg::GetFullPath( HTREEITEM hItem /* = NULL */ )
{//////////////////////////////////////////////////////////////////////////////
// 函数名称: GetFullPath
// 内容简介: Get the full path on the current selected item
// 功    能: 略
// 参    数:
//	HTREEITEM hItem: The current selected item
// 返    回:
//	( CString ) : The full path
// 作    者: L.F.
// 开始日期: 2005.08.14
///////////////////////////////////////////////////////////////////////////////

	ASSERT( hItem );

	CString strTmp	= _T( "" );
	CString strPath	= _T( "" );

	while ( hItem != m_hTreeItemRoot )
	{
		strTmp = m_treeDirectory.GetItemText( hItem );
		if ( strTmp.Right( 1 ) != _T( "\\" ) )
			strTmp += _T( "\\" );
		strPath = strTmp + strPath;

		hItem = m_treeDirectory.GetParentItem( hItem );
	}

	return strPath;
}

BOOL CVCHelperDlg::CreateEmbededPSheet()
{//////////////////////////////////////////////////////////////////////////////
// 函数名称: CreateEmbededPSheet
// 内容简介: Create a embeded property sheet on the dialog
// 功    能: 略
// 参    数: void
// 返    回: BOOL
//	( TRUE ) Creation succeessful ! : ( FALSE ) Creation failed !
// 作    者: L.F.
// 开始日期: 2005.08.16
///////////////////////////////////////////////////////////////////////////////

	if ( ! m_sheetResult.Create( this, WS_CHILD | WS_VISIBLE ) )
	{
		TRACE0( "EMBEDED PROPERTY SHEET CREATION FAILED\n" );
		return FALSE;
	}

	m_sheetResult.ModifyStyle( 0, WS_TABSTOP );
	m_sheetResult.ModifyStyleEx( 0, WS_EX_CONTROLPARENT );

	ModifyStyle( 0, WS_EX_CONTROLPARENT );

	return TRUE;
}

void CVCHelperDlg::PositionEmbededPSheet()
{//////////////////////////////////////////////////////////////////////////////
// 函数名称: PositionEmbededPSheet
// 内容简介: Position the embeded property sheet
// 功    能: 略
// 参    数: void
// 返    回: void
// 作    者: L.F.
// 开始日期: 2005.08.16
///////////////////////////////////////////////////////////////////////////////

	CWnd* pWnd = GetDlgItem( IDC_STATIC_SHEET );
	ASSERT_VALID( pWnd );

	CRect rect;
	pWnd->GetWindowRect( & rect );
	ScreenToClient( & rect );

	m_sheetResult.MoveWindow( & rect );
}

void CVCHelperDlg::OnOK() 
{//////////////////////////////////////////////////////////////////////////////
// 函数名称: OnOk
// 内容简介: Searching the path. Save the file result in the result property sheet.
// 功    能: Handle the identifier IDOK
// 参    数: void
// 返    回: void
// 作    者: L.F.
// 开始日期: 2005.08.24
///////////////////////////////////////////////////////////////////////////////

	// TODO: Add extra validation here
	m_strArrPath.RemoveAll();
	m_strArrFile.RemoveAll();

	GetSearchingPath( m_hTreeItemRoot );
	m_sheetResult.SetActivePage( & m_sheetResult.m_pageFindResult );
	m_sheetResult.m_pageFindResult.DeleteAllItems();

	GetFindFileName();
	FindFile();

	m_nProgressPos = 0;
	m_progressCtrl.SetPos( m_nProgressPos );
	// Disable the default behavior...
	// CDialog::OnOK();
}

UINT CVCHelperDlg::FindFileThreadFunc( LPVOID lParam )
{//////////////////////////////////////////////////////////////////////////////
// 函数名称: ThreadFunc
// 内容简介: The thread function. which respondes the file searching.
// 功    能: 略
// 参    数:
//	LPVOID lParam: The thread function parameter.
// 返    回: UINT
//	( 0 ) Normal exit : ( Not 0 ) Error exit
// 作    者: L.F.
// 开始日期: 2005.08.26
///////////////////////////////////////////////////////////////////////////////

	if ( lParam == NULL )
	{
		AfxEndThread( NULL );
	}

	THREADPARAM* threadParam = ( THREADPARAM* ) lParam;

	for ( int i = 0; i < threadParam->strArrPath.GetSize(); i++ )
	{
		CString strPath = threadParam->strArrPath.GetAt( i );
		TRACE0( strPath + "\n" );

		for ( int j = 0; j < threadParam->strArrFile.GetSize(); j++ )
		{
			CString strFile	= threadParam->strArrFile.GetAt( j );
			FindFiles( strPath, strFile );
		}
	}

	// The thread end...
	AfxGetMainWnd()->SendMessage( WM_SET_EXITTHREAD );
	return 0;
}

void CVCHelperDlg::GetSearchingPath( HTREEITEM hItem /* = NULL */ )
{//////////////////////////////////////////////////////////////////////////////
// 函数名称: GetSearchingPath
// 内容简介: Get the searching path and save in the string array
// 功    能: 略
// 参    数: void
// 返    回: void
// 作    者: L.F.
// 开始日期: 2005.08.27
///////////////////////////////////////////////////////////////////////////////

	ASSERT( hItem );
	HTREEITEM hChildItem = m_treeDirectory.GetChildItem( hItem );

	while ( hChildItem )
	{
		// If the parent item was selected
		// No need to check the child item if was selected...
		if ( m_treeDirectory.GetCheck( hChildItem ) )
		{
			if ( GetFullPath( hChildItem ) != _T( "" ) )
			{
				TRACE1( "The selected path is: %s\n", GetFullPath( hChildItem ) );
				m_strArrPath.Add( GetFullPath( hChildItem ) );
			}
		}
		else
		{
			GetSearchingPath( hChildItem );
		}

		hChildItem = m_treeDirectory.GetNextItem( hChildItem, TVGN_NEXT );
	}
}

void CVCHelperDlg::FindFile()
{//////////////////////////////////////////////////////////////////////////////
// 函数名称: FindFile
// 内容简介: Creating thread function and finding file
// 功    能: 略
// 参    数: void
// 返    回: void
// 作    者: L.F.
// 开始日期: 2005.08.30
///////////////////////////////////////////////////////////////////////////////

	m_threadParam.strArrPath.RemoveAll();
	m_threadParam.strArrFile.RemoveAll();

	for ( int i = 0; i < m_strArrPath.GetSize(); i++ )
		m_threadParam.strArrPath.Add( m_strArrPath.GetAt( i ) );

	for ( i = 0; i < m_strArrFile.GetSize(); i++ )
		m_threadParam.strArrFile.Add( m_strArrFile.GetAt( i ) );

	CWinThread* pThread = AfxBeginThread( \
				CVCHelperDlg::FindFileThreadFunc, \
				& m_threadParam, \
				THREAD_PRIORITY_NORMAL, \
				0, \
				CREATE_SUSPENDED );
	ASSERT_VALID( pThread );
	pThread->ResumeThread();

	( ( CButton* ) GetDlgItem( IDOK ) )->EnableWindow( FALSE );
	( ( CButton* ) GetDlgItem( IDC_BTN_DEL ) )->EnableWindow( FALSE );
	( ( CButton* ) GetDlgItem( IDC_BTN_DEL_ALL ) )->EnableWindow( FALSE );
}

void CVCHelperDlg::GetFindFileName()
{//////////////////////////////////////////////////////////////////////////////
// 函数名称: GetFindFileName
// 内容简介: Get the find file name from the list control items
// 功    能: 略
// 参    数: void
// 返    回: void
// 作    者: L.F.
// 开始日期: 2005.08.30
///////////////////////////////////////////////////////////////////////////////

	m_sheetResult.m_pageFileType.GetItemFileType( m_strArrFile );
}

void CVCHelperDlg::FindFiles( CString strPath, CString strFileName )
{//////////////////////////////////////////////////////////////////////////////
// 函数名称: FindFiles
// 内容简介: Recures searching files from the defined path.
// 功    能: 略
// 参    数:
//	CString& strPath: The current searching path
//	CString& strFileName: The file type.
// 返    回: void
// 作    者: L.F.
// 开始日期: 2005.08.31
///////////////////////////////////////////////////////////////////////////////

	if ( strPath.Right( 1 ) != _T( "\\" ) )
		strPath += _T( "\\" );

	strPath += _T( "*.*" );

	CFileFind file;

	BOOL bContinue = file.FindFile( strPath );
	while ( bContinue )
	{
		bContinue = file.FindNextFile();
		if ( file.IsDirectory() && ! file.IsDots() )
		{
			FindFiles( file.GetFilePath(), strFileName );
		}
		else if ( ! file.IsDots() )
		{
			if ( file.GetFileName().Find( strFileName.Right( 4 ) ) != -1 )
			{
				TCHAR strInfo[ 512 ] = { '\0' };

				wsprintf( strInfo, "%s", file.GetFilePath() );
				AfxGetMainWnd()->SendMessage( \
							WM_SET_FINDFILEINFO, \
							FLAG_FILENAME, \
							( LPARAM ) strInfo );

				ZeroMemory( strInfo, sizeof( strInfo ) );
				wsprintf( strInfo, "%d", file.GetLength() / 1024 );
				AfxGetMainWnd()->SendMessage( \
							WM_SET_FINDFILEINFO, \
							FLAG_FILESIZE, \
							( LPARAM ) strInfo );
			}
		} // End [ else if ( ! file.IsDots() ) ]
	} // End [ while ( bContinue ) ]
}

void CVCHelperDlg::OnSetFindFileInfo( WPARAM wParam, LPARAM lParam )
{//////////////////////////////////////////////////////////////////////////////
// Name: OnSetFindFileInfo
// Description:
//	Get the thread find file information and set text to the list control
//	 in the list control on the find result property sheet.
// Effect:
//	Handle the self defined message WM_SET_FINDFILEINFO.
// Parameters: void
// Resturn: void
// Author: L.F.
// Date: 2005.09.06
///////////////////////////////////////////////////////////////////////////////

	m_nProgressPos++;
	m_progressCtrl.SetPos( 	m_nProgressPos % 100 );

	LPCTSTR lpStrInfo = ( LPCTSTR ) lParam;

	static CString strFileName = _T( "" );

	switch( wParam )
	{
	case FLAG_FILENAME:
		strFileName = lpStrInfo;
		TRACE1( "Name: %s\n", lpStrInfo );
		m_statusBar.SetPaneText( 0, lpStrInfo );
		m_nFilesCount++;
		break;
	case FLAG_FILESIZE:
		ASSERT( strFileName );
		m_sheetResult.m_pageFindResult.AddListItem( strFileName, lpStrInfo );
		TRACE1( "Size: %s\n", lpStrInfo );
		m_nFilesSize += atol( lpStrInfo );
		break;
	default:
		break;
	}
}

void CVCHelperDlg::OnSetExitThreadMsg( WPARAM wParam, LPARAM lParam )
{//////////////////////////////////////////////////////////////////////////////
// Name: OnSetExitThreadMsg
// Description:
//	The thread return.
// Effect:
//	Handle the self-defined message WM_SET_THREADEXIT
// Parameters:
//	WPARAM wParam: = 0
//	LPARAM lParam: = 0
// Return: void
// Author: L.F.
// Date: 2005.09.13
///////////////////////////////////////////////////////////////////////////////

	m_progressCtrl.SetPos( 100 );

	TCHAR strInfo[ 256 ] = { '\0' };
	wsprintf( strInfo, _T( "COMPLETE : FILES %d SIZE %d K" ), m_nFilesCount, m_nFilesSize );

	m_statusBar.SetPaneText( 0, strInfo );
	TRACE0( "Thread return !\n" );

	m_nFilesCount = 0;
	m_nFilesSize  = 0;

	( ( CButton* ) GetDlgItem( IDOK ) )->EnableWindow( TRUE );
	( ( CButton* ) GetDlgItem( IDC_BTN_DEL ) )->EnableWindow( TRUE );
	( ( CButton* ) GetDlgItem( IDC_BTN_DEL_ALL ) )->EnableWindow( TRUE );

	m_progressCtrl.SetPos( 0 );
}

void CVCHelperDlg::OnBtnDel()
{//////////////////////////////////////////////////////////////////////////////
// NAME: OnBtnDel
// DESCRIPTION:
//	Delete the selected files in the list control.
// EFFECT:
//	Handle the button IDC_BTN_DEL
// PARAMETERS: None
// RETURN: void
// AUTHOR: L.F.
// DATE: 2005.09.15
///////////////////////////////////////////////////////////////////////////////

	m_sheetResult.m_pageFindResult.DeleteFiles();
}

void CVCHelperDlg::OnBtnDelAll()
{//////////////////////////////////////////////////////////////////////////////
// NAME: OnBtnDelAll
// DESCRIPTION:
//	Delete all files in the list control.
// EFFECT:
//	Handle the button IDC_BTN_DEL_ALL
// PARAMETERS: None
// RETURN: void
// AUTHOR: L.F.
// DATE: 2005.09.15
///////////////////////////////////////////////////////////////////////////////

	m_sheetResult.m_pageFindResult.DeleteAllFiles();
}

void CVCHelperDlg::OnCancel()
{//////////////////////////////////////////////////////////////////////////////
// NAME: OnCalcel
// DESCRIPTION:
//	Quit when the ID IDCANCEL button was clicked !.
//	 Clean the resource the dialog allocates.
// EFFECT:
//	virtual function. Handle the ID IDCANCEL.
// PARAMETERS: None
// RETURN: void
// AUTHOR: L.F.
// DATE: 2005.09.15
///////////////////////////////////////////////////////////////////////////////

	TRACE0( "The program is quit !\n" );

	CDialog::OnCancel();
}

void CVCHelperDlg::AddStatusBar()
{//////////////////////////////////////////////////////////////////////////////
// Name: AddStatusBar
// Description:
//	Add the status bar control to the dialog.
// Effect:
//	Call by OnInitDialog( ... )
// Parameters: None
// Return: void
// Author: L.F.
// Date: 2005.09.21
///////////////////////////////////////////////////////////////////////////////

	m_statusBar.CreateEx( this );
	m_statusBar.SetIndicators( nIndicators, sizeof( nIndicators ) / sizeof( UINT ) );

	CRect rectClient;
	GetClientRect( & rectClient );

	m_statusBar.SetPaneInfo( 0, ID_SEPARATOR, SBPS_STRETCH, rectClient.Width() / 2 );
	m_statusBar.SetPaneInfo( 1, ID_SEPARATOR, SBPS_STRETCH, rectClient.Width() / 4 );
	m_statusBar.SetPaneInfo( 2, ID_SEPARATOR, SBPS_STRETCH, 0 );

	RepositionBars( AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, ID_INDICATOR_NUM );
}

void CVCHelperDlg::OnBtnStop()
{//////////////////////////////////////////////////////////////////////////////
// Name: OnBtnStop
// Description:
//	Stop the searching procedure...
// Effect:
//	Handle the WM_COMMAND -> IDC_BTN_STOP
// Parameters: None
// Return: void
// Author: L.F.
// Date: 2005.09.24
///////////////////////////////////////////////////////////////////////////////

}