// YYCCDoc.cpp : implementation of the CYYCCDoc class
//

#include "stdafx.h"
#include "YYCC.h"

#include "YYCCDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CYYCCDoc

IMPLEMENT_DYNCREATE(CYYCCDoc, CDocument)

BEGIN_MESSAGE_MAP(CYYCCDoc, CDocument)
	//{{AFX_MSG_MAP(CYYCCDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CYYCCDoc construction/destruction

CYYCCDoc::CYYCCDoc()
{
	// TODO: add one-time construction code here
	m_pMemDC  = NULL;
	WIDTH   =  2000;
	HEIGHT   = 2000;
	Routine = 0;

}

CYYCCDoc::~CYYCCDoc()
{
}

BOOL CYYCCDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	if(m_pMemDC ==NULL)
	{
		CFileDialog dlg(TRUE,
						_T("bmp"),
						NULL,
						OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
						_T("(*.bmp)|*.bmp||"));	
		if (dlg.DoModal() == IDOK)
		{
			str = dlg.GetPathName();
			Routine = str.GetBuffer(str.GetLength());
		}
		this->m_pMemDC = new CMemDC(WIDTH,HEIGHT,Routine);
	}
	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CYYCCDoc serialization

void CYYCCDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CYYCCDoc diagnostics

#ifdef _DEBUG
void CYYCCDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CYYCCDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CYYCCDoc commands


int CYYCCDoc::OnUpdateView(CDC *pDC ,CRect rect)
{


	BOOL bRet = FALSE;

	pDC->FillSolidRect(&rect, RGB(0,0,0));

	bRet = pDC->BitBlt(rect.left, rect.top, 
			   rect.Width(),  rect.Height(),
				m_pMemDC, rect.left, rect.top, SRCCOPY); //bit block transfer
	//jiang memdc copy to pDC
	return (bRet==FALSE) ? S_OK : E_FAIL;
}

int CYYCCDoc::OnUpdateView(CDC *pDC )
{

	CRect rect(0, 0, 500,100);

	pDC->DrawText(this->m_strContent, rect, 0);

	return 0;
}

BOOL CYYCCDoc::OnSaveDocument(LPCTSTR lpszPathName)
{

    
   CExBitmap  *pBitmap = (CExBitmap*)this->m_pMemDC->GetBitmap();
	return pBitmap->SaveDIB(lpszPathName);
}

BOOL CYYCCDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	if(m_pMemDC == NULL)
		this->m_pMemDC = new CMemDC(lpszPathName);

	return TRUE;
}
void CYYCCDoc::SetMemDC()
{
	if(m_pMemDC != NULL)
	{
		m_pMemDC->DeleteDC();
		this->m_pMemDC = new CMemDC(WIDTH,HEIGHT,str.GetBuffer(str.GetLength()));
	}
}