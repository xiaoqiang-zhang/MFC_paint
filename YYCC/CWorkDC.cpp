// CWorkDC.cpp : implementation file
//
#include "stdafx.h"
#include "CWorkDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWorkDC

void CWorkDC::BuildWorkDC(CDC* pDC)
{
	m_nSavedDC = 0;
	if( !CreateCompatibleDC(pDC) )
		AfxThrowResourceException();
	m_nSavedDC = SaveDC();
}

CWorkDC::CWorkDC(CDC* pDC)
{
	BuildWorkDC(pDC);
}

CWorkDC::CWorkDC(CDC* pDC,CBrush* pBrush)
{
	BuildWorkDC(pDC);
	SelectObject(pBrush);
}

CWorkDC::CWorkDC(CDC* pDC,CPen* pPen)
{
	BuildWorkDC(pDC);
	SelectObject(pPen);
}

CWorkDC::CWorkDC(CDC* pDC,CFont* pFont)
{
	BuildWorkDC(pDC);
	SelectObject(pFont);
}

CWorkDC::CWorkDC(CDC* pDC,CBitmap* pBitmap)
{
	BuildWorkDC(pDC);
	SelectObject(pBitmap);
}

CWorkDC::CWorkDC(CDC* pDC,CBitmap* pBitmap,CPen* pPen)
{
	BuildWorkDC(pDC);
	SelectObject(pBitmap);
	SelectObject(pPen);
}

CWorkDC::CWorkDC(CDC* pDC,CBitmap* pBitmap,CBrush* pBrush)
{
	BuildWorkDC(pDC);
	SelectObject(pBitmap);
	SelectObject(pBrush);
}

CWorkDC::CWorkDC(CDC* pDC,CBitmap* pBitmap,CFont* pFont)
{
	BuildWorkDC(pDC);
	SelectObject(pBitmap);
	SelectObject(pFont);
}

CWorkDC::CWorkDC(CDC* pDC,CBitmap* pBitmap,CPen* pPen,CBrush* pBrush)
{
	BuildWorkDC(pDC);
	SelectObject(pBitmap);
	SelectObject(pPen);
	SelectObject(pBrush);
}

CWorkDC::CWorkDC(CDC* pDC,CBitmap* pBitmap,CPen* pPen,CBrush* pBrush,CFont* pFont)
{
	BuildWorkDC(pDC);
	SelectObject(pBitmap);
	SelectObject(pPen);
	SelectObject(pBrush);
	SelectObject(pFont);
}

CWorkDC::~CWorkDC()
{
	if( m_hDC != NULL )
	{
		if( m_nSavedDC )
			CDC::RestoreDC(m_nSavedDC);
		::DeleteDC(Detach());
	}
}

IMPLEMENT_DYNAMIC(CWorkDC, CDC)
