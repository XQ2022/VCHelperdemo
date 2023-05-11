// VCHelperDlg.h : header file
//

#if !defined(AFX_VCHELPERDLG_H__3CB0B374_E5A7_4B8F_BFC3_7F9228371F61__INCLUDED_)
#define AFX_VCHELPERDLG_H__3CB0B374_E5A7_4B8F_BFC3_7F9228371F61__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define	WM_SET_FINDFILEINFO	( WM_APP + 1 )
const UINT FLAG_FILENAME = 1;
const UINT FLAG_FILESIZE = 2;

#define	WM_SET_EXITTHREAD	( WM_APP + 2 )

/////////////////////////////////////////////////////////////////////////////
// CVCHelperDlg dialog

#include <afxmt.h>
#include "PSheetResult.h"

typedef struct _tagTHREADPARAM {
	CStringArray	strArrPath;
	CStringArray	strArrFile;
} THREADPARAM;

class CVCHelperDlg : public CDialog
{
// Construction
public:
	CVCHelperDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CVCHelperDlg)
	enum { IDD = IDD_VCHELPER_DIALOG };
	CProgressCtrl	m_progressCtrl;
	CTreeCtrl	m_treeDirectory;
	CPSheetResult	m_sheetResult;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVCHelperDlg)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CVCHelperDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();

	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnItemexpandedTreeDirectory(NMHDR* pNMHDR, LRESULT* pResult);

	afx_msg void OnSetFindFileInfo( WPARAM wParam, LPARAM lParam );
	afx_msg void OnSetExitThreadMsg( WPARAM wParam, LPARAM lParam );

	afx_msg void OnBtnDel();
	afx_msg void OnBtnDelAll();

	afx_msg void OnBtnStop();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void SetTreeListStyle();
	void GetDriveDir( HTREEITEM hItem = NULL );
	void GetLogicalDrives( HTREEITEM hParent = NULL );
	void AddSubDirectory( HTREEITEM hParent = NULL );

	CString GetFullPath( HTREEITEM hItem = NULL );

	BOOL CreateEmbededPSheet();
	void PositionEmbededPSheet();

	void GetSearchingPath( HTREEITEM hItem = NULL );

	void GetFindFileName();
	void FindFile();

	THREADPARAM m_threadParam;

	static UINT FindFileThreadFunc( LPVOID );
	static void FindFiles( CString strPath, CString strFileName );

	void AddStatusBar();

private:
	HTREEITEM m_hTreeItemRoot;

	CStringArray m_strArrPath;
	CStringArray m_strArrFile;

	UINT m_nFilesSize;
	UINT m_nFilesCount;

	int  m_nProgressPos;

	CStatusBar m_statusBar;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VCHELPERDLG_H__3CB0B374_E5A7_4B8F_BFC3_7F9228371F61__INCLUDED_)
