// ReTurn.h: interface for the ReTurn class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RETURN_H__1D6F8AE5_CFA9_4BAF_8DEE_1FBA0422A710__INCLUDED_)
#define AFX_RETURN_H__1D6F8AE5_CFA9_4BAF_8DEE_1FBA0422A710__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "MyPen.h"

class ReTurn  
{
public:
	ReTurn(	MyPen*      myPen,
			CPoint pStartPoint,
			CPoint	pEndPoint,
			COLORREF m_pReTurnColor,
			int m_pReTurnWidth,
			int m_ReTurnStyle);
	virtual ~ReTurn();
public:
	MyPen*      m_myPen;
	CPoint		m_StartPoint;
	CPoint		m_EndPoint;
	COLORREF	m_pReTurnColor;
	int			m_pReTurnWidth;
	int			m_ReTurnStyle;

};

#endif // !defined(AFX_RETURN_H__1D6F8AE5_CFA9_4BAF_8DEE_1FBA0422A710__INCLUDED_)
