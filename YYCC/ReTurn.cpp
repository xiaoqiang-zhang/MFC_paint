// ReTurn.cpp: implementation of the ReTurn class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "YYCC.h"
#include "ReTurn.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ReTurn::ReTurn(MyPen*      myPen,
			   CPoint pStartPoint,
			CPoint	pEndPoint,
			COLORREF pReTurnColor,
			int pReTurnWidth,
			int ReTurnStyle)
{
		m_myPen				=		myPen;
		m_StartPoint.x		=		pStartPoint.x;
		m_StartPoint.y		=		pStartPoint.y;
		m_EndPoint			=		pEndPoint;
		m_pReTurnColor		=		pReTurnColor;
		m_pReTurnWidth		=		pReTurnWidth;
		m_ReTurnStyle			=		ReTurnStyle;
}

ReTurn::~ReTurn()
{	

}
