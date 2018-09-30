// cdlgproc.cpp : implementation file
//

#include "stdafx.h"
#include "YYCC.h"
#include "cdlgproc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// cdlgproc dialog


cdlgproc::cdlgproc(CWnd* pParent /*=NULL*/)
	: CDialog(cdlgproc::IDD, pParent)
{
	//{{AFX_DATA_INIT(cdlgproc)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void cdlgproc::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(cdlgproc)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(cdlgproc, CDialog)
	//{{AFX_MSG_MAP(cdlgproc)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// cdlgproc message handlers
