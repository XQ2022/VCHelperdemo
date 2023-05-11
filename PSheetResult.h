#if !defined(AFX_PSHEETRESULT_H__3656EC57_6AAC_4EB7_B7CC_8185951EFDEF__INCLUDED_)
#define AFX_PSHEETRESULT_H__3656EC57_6AAC_4EB7_B7CC_8185951EFDEF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PSheetResult.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPSheetResult

#include "PageFileType.h"
#include "PageFindResult.h"

class CPSheetResult : public CPropertySheet
{
	DECLARE_DYNAMIC(CPSheetResult)

// Construction
public:
	CPSheetResult();

// Attributes
public:
	CPageFileType	m_pageFileType;
	CPageFindResult	m_pageFindResult;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPSheetResult)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPSheetResult();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPSheetResult)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PSHEETRESULT_H__3656EC57_6AAC_4EB7_B7CC_8185951EFDEF__INCLUDED_)
