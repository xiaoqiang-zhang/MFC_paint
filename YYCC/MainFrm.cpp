// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "YYCC.h"

#include "MainFrm.h"
#include "DrawCircle.h"
#include "DrawLine.h"
#include "DrawRect.h"
#include "DrawBitmap.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	for( int i = 0; i < sizeof(m_pPaintTools)/sizeof(m_pPaintTools[0]); i++)
	{
       m_pPaintTools[i] = NULL;
	}
	this->m_pPaintTools[0] = new DrawRect();
	this->m_pPaintTools[1] = new DrawCircle();
	this->m_pPaintTools[2] = new DrawLine();
    this->m_pPaintTools[3] = new DrawBitmap();
	
}

CMainFrame::~CMainFrame()
{
	for( int i = 0; i < sizeof(m_pPaintTools)/sizeof(m_pPaintTools[0]); i++)
	{
       if(m_pPaintTools[i] != NULL)
		   delete m_pPaintTools[i];
	   m_pPaintTools[i] = NULL;
	}
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{

	CRect rect,rect1;
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_PEN))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	m_wndToolBar.SetWindowText(_T("Pen"));
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	if (!m_ownToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_ownToolBar.LoadToolBar(IDR_COLOR))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}


	m_ownToolBar.SetWindowText(_T("Color"));
	m_ownToolBar.EnableDocking(CBRS_ALIGN_ANY);
//	CRect rect;
	m_ownToolBar.GetWindowRect(&rect);
	rect.OffsetRect(1, 0);
	this->RecalcLayout();    
	DockControlBar(&m_ownToolBar, AFX_IDW_DOCKBAR_TOP, &rect);

	if (!m_lineToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_lineToolBar.LoadToolBar(IDR_LINEWIDTH))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}


	m_lineToolBar.SetWindowText(_T("Width"));
	m_lineToolBar.EnableDocking(CBRS_ALIGN_ANY);
//	CRect rect;
	m_lineToolBar.GetWindowRect(&rect);
	rect1.OffsetRect(1, 0);
	this->RecalcLayout();    
	DockControlBar(&m_lineToolBar, AFX_IDW_DOCKBAR_TOP, &rect1);



	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	
	m_wndReBar.Create(this);



	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void CMainFrame::SetStatusCursor(int x, int y)
{

	TCHAR szMsg[MAX_PATH];
	sprintf(szMsg,"%d,%d",x,y);
	this->m_wndStatusBar.SetPaneText(2,szMsg);
}

void CMainFrame::SetStatusTool(LPCTSTR szTool)
{
	this->m_wndStatusBar.SetPaneText(1,szTool);
}

void CMainFrame::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here

	
}
