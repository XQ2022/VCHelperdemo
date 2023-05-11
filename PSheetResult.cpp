// PSheetResult.cpp : implementation file
//

#include "stdafx.h"
#include "VCHelper.h"
#include "PSheetResult.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPSheetResult

IMPLEMENT_DYNAMIC(CPSheetResult, CPropertySheet)

CPSheetResult::CPSheetResult()
{
	AddPage( & m_pageFileType );
	AddPage( & m_pageFindResult );
}

CPSheetResult::~CPSheetResult()
{
}


BEGIN_MESSAGE_MAP(CPSheetResult, CPropertySheet)
	//{{AFX_MSG_MAP(CPSheetResult)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPSheetResult message handlers
