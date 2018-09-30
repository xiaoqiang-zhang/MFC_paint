// YYCCView.cpp : implementation of the CYYCCView class
//

#include "stdafx.h"
#include "YYCC.h"

#include "YYCCDoc.h"
#include "YYCCView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CYYCCView

IMPLEMENT_DYNCREATE(CYYCCView, CView)

BEGIN_MESSAGE_MAP(CYYCCView, CView)
	//{{AFX_MSG_MAP(CYYCCView)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND_RANGE(ID_WIDTH1,ID_WIDTH10,onLineWidth)
	ON_COMMAND_RANGE(ID_BLACK,ID_LIGHTBLUE,onColor)
	ON_COMMAND_RANGE(ID_FANG,ID_BITMAP,onPen)
	ON_COMMAND(ID_RETURN, onReturn)
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CYYCCView construction/destruction

CYYCCView::CYYCCView()
{
	// TODO: add construction code here
	this->m_pCurPaintTool = NULL;
	m_pCurPen = NULL;
	m_bChangedPen = TRUE;
	m_pWidth = 1;
	m_pColor = RGB(0,0,0);
	m_Style = PS_SOLID;


}

CYYCCView::~CYYCCView()
{
}

BOOL CYYCCView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CYYCCView drawing

void CYYCCView::OnDraw(CDC* pDC)
{
	CYYCCDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code

	CRect rect;

    GetClientRect(rect);

    pDoc->OnUpdateView(GetDC(),rect);

}

/////////////////////////////////////////////////////////////////////////////
// CYYCCView printing

BOOL CYYCCView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CYYCCView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CYYCCView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CYYCCView diagnostics

#ifdef _DEBUG
void CYYCCView::AssertValid() const
{
	CView::AssertValid();
}

void CYYCCView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CYYCCDoc* CYYCCView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CYYCCDoc)));
	return (CYYCCDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CYYCCView message handlers

void CYYCCView::onPen(UINT nID) 
{
	// TODO: Add your command handler code here
	UINT nIndex = nID - ID_FANG;


	
	if(nIndex < 10)
	{
		TRACE1("Changing tool to %d\r\n", nIndex);
			
	    m_pCurPaintTool = ((CMainFrame *) AfxGetMainWnd())->m_pPaintTools[nIndex];

		TRACE2("Changing tool to [%d] &s \r\n", nIndex,m_pCurPaintTool->GetName());

		m_pCurPaintTool->m_pChanged = TRUE;

	    ((CMainFrame*)AfxGetMainWnd())->SetStatusTool(
			m_pCurPaintTool->GetName());
	}
}

void CYYCCView::onLineWidth(UINT nID)
{
	UINT nIndex = nID - ID_WIDTH1;

	if(m_pCurPen == NULL)
	{
		m_pCurPen = new CPen();
	}
	m_pWidth = nIndex + 1;
	
	CreateNewPen(PS_SOLID,m_pWidth,m_pColor);
}

void CYYCCView::onReturn()
{
	CPen * pOldPen = NULL;
	CPen *m_RetNewPen = NULL;
	int style = 0;
	int width = 0;
	COLORREF color = 0;
	MyPen * mypen = NULL;
	UINT n = m_ReturnArray.GetSize();
	CPoint start ;
    CPoint end;



    GetDocument()->SetMemDC();
	if(n > 0)
	{
		m_ReturnArray.RemoveAt(n - 1);
		n  = n-1;
		for(UINT i = 0; i < n ;i++)
		{

				style = ((ReTurn *)(m_ReturnArray.GetAt(i)))->m_ReTurnStyle;
				width = ((ReTurn *)(m_ReturnArray.GetAt(i)))->m_pReTurnWidth;
				color = ((ReTurn *)(m_ReturnArray.GetAt(i)))->m_pReTurnColor;
			m_RetNewPen = new CPen();

			m_RetNewPen->CreatePen(style,width,color);

			mypen =  ((ReTurn *)(m_ReturnArray.GetAt(i)))->m_myPen;

			pOldPen = GetDocument()->GetMemDC()->SelectObject(m_RetNewPen);

			start = ((ReTurn *)(m_ReturnArray.GetAt(i)))->m_StartPoint;

			end  = ((ReTurn *)(m_ReturnArray.GetAt(i)))->m_EndPoint;

			mypen->Draw(GetDocument()->GetMemDC(), 
						start.x, 
					    start.y, 
						end.x, 
						end.y); 
		m_RetNewPen->DeleteObject();
		
		}

			this->Invalidate();
			ReleaseCapture();

	}
}

void CYYCCView::onColor(UINT nID) 
{
	UINT nIndex = nID - ID_BLACK;
	if(m_pCurPen == NULL)
	{
		m_pCurPen = new CPen();
	}


	switch (nIndex )
	{
	case 0:	
		m_pColor = RGB(0,0,0);//black
		break;
	case 1:
		m_pColor = RGB(255,0,0);//red
		break;
	case 2:
		m_pColor = RGB(0,255,0);//green
		break;
	case 3:
		m_pColor = RGB(0,0,255);//blue
		break;
	case 4:
		m_pColor = RGB(255,255,0);//yellow
		break;
	case 5:
		m_pColor = RGB(0,0,139);//ID_DEEPBLUE
		break;
	case 6:
		m_pColor = RGB(	255,192,203);//ID_PINK
		break;
	case 7:
		m_pColor = RGB(255,255,255);//ID_WHITE
		break;
	case 8:
		m_pColor = RGB(	139,0,0);//ID_DEEPRED
		break;
	case 9:
		m_pColor = RGB(222,184,135);//ID_ZONG
		break;
	case 10:
		m_pColor = RGB(0,128,0);//ID_DEEPGREEN
		break;
	case 11:
		m_pColor = RGB(	135,206,235);//ID_DEEPGREEN
		break;
	default:
		m_pColor = RGB(0,0,0);
		break;
	    
	}
	CreateNewPen(PS_SOLID,m_pWidth,m_pColor);

}

void CYYCCView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_ptStart = point;

    m_ptOld.x=-1;//erase,
    SetCapture();
	

	CView::OnLButtonDown(nFlags, point);
}

void CYYCCView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CPen * pOldPen = NULL;

	m_ptButtonUp = point;
	if(this->m_pCurPaintTool != NULL)
	{

		if(this->m_pCurPen != NULL)//pen not null
		{
			pOldPen = GetDocument()->GetMemDC()->SelectObject(m_pCurPen);
           
		}			

		m_pCurPaintTool->Draw(GetDocument()->GetMemDC(), 
	    this->m_ptStart.x, this->m_ptStart.y, point.x, point.y); 

		ReTurn *m_Return = new ReTurn(m_pCurPaintTool,m_ptStart,point,m_pColor,m_pWidth,m_Style);
        m_ReturnArray.Add(m_Return);

		this->Invalidate();//clear pic process hen ji;nei cun zhon mei you yan se suo yi hua chu lai de mei you yan se

	}
	ReleaseCapture();
	CView::OnLButtonUp(nFlags, point);
}

void CYYCCView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	((CMainFrame*)AfxGetMainWnd())->SetStatusCursor(point.x, point.y);


	if( (nFlags & MK_LBUTTON) != 0 && this->m_pCurPaintTool != NULL)
	{
		CDC* pDC = GetDC();

		int   nOldMode = pDC->GetROP2();
		pDC->SetROP2(SRCINVERT); 

		if(m_ptOld.x >=0 )//erase =-1to erase
		{
			m_pCurPaintTool->Draw(pDC, this->m_ptStart.x, m_ptStart.y, m_ptOld.x, m_ptOld.y,TUYA_FLAG_PREVIEW );
		}

		m_pCurPaintTool->Draw(pDC, this->m_ptStart.x, m_ptStart.y, point.x, point.y,TUYA_FLAG_PREVIEW);

		m_ptOld = point;

		pDC->SetROP2(nOldMode);
	}
	CView::OnMouseMove(nFlags, point);
}
int CYYCCView::CreateNewPen(int nStyle, int nWIdth, COLORREF cir)
{

	    if(m_pCurPen !=NULL)
		{
			m_pCurPen->DeleteObject();	
		}

		m_pCurPen = new CPen();

		m_pCurPen->CreatePen(nStyle,nWIdth,cir);
	
		m_pColor = cir;
		m_pWidth = nWIdth;
		m_Style = nStyle;

		return S_OK;

}


