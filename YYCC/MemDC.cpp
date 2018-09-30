// MemDC.cpp: implementation of the CMemDC class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "YYCC.h"
#include "MemDC.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMemDC::CMemDC(int width, int height,LPCTSTR lpszFileName,CDC *pOrigDC /* = NULL */, 
                      COLORREF background /*= RGB(255,255,255) */)
					  :  CDC(),
    m_pBitmap(NULL),
    m_pOldBmp(NULL)
{
	CWindowDC dc(NULL); // screen
	// create new bitmap
	if (!pOrigDC)
	{
		pOrigDC = &dc;
	}
	  		

	m_pBitmap = (CExBitmap*)new CBitmap();
	m_pBitmap->LoadDIB(lpszFileName);


	// make compatible device context and select bitmap into it
	VERIFY(CreateCompatibleDC(pOrigDC));
	m_pOldBmp = (CExBitmap*)SelectObject(m_pBitmap);
	CDC::SetMapMode(pOrigDC->GetMapMode());

	// paint background in bitmap
	background = RGB(255,255,255);
	FillSolidRect(0, 0, width, height, background);
}
CMemDC::CMemDC(LPCTSTR lpszFileName, CDC *pOrigDC)
{
	CWindowDC dc(NULL); // screen
	// create new bitmap
	if (!pOrigDC)
	{
		pOrigDC = &dc;
	}

	m_pBitmap = new CExBitmap();

	m_pBitmap->LoadDIB(lpszFileName);


	VERIFY(CreateCompatibleDC(pOrigDC));
	m_pOldBmp = SelectObject(m_pBitmap);
	CDC::SetMapMode(pOrigDC->GetMapMode());


}

CMemDC::~CMemDC()
{
	if (m_pBitmap) {
		CBitmap *pBitmap;
		pBitmap = Close();
		delete pBitmap;
		pBitmap = NULL;
	}
}
CBitmap *CMemDC::Close()
{
	ASSERT(m_pBitmap);
	CExBitmap *pBitmap = m_pBitmap;
	SelectObject(m_pOldBmp);
	m_pBitmap = NULL;
	return pBitmap;
}