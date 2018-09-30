// DrawLine.cpp: implementation of the DrawLine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "YYCC.h"
#include "DrawLine.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DrawLine::DrawLine()
{
	this->m_lpszName = "TOOL_LINE";
}

DrawLine::~DrawLine()
{

}
void DrawLine::Draw(CDC *pDC, UINT x1, 
						  UINT y1, UINT x2, UINT y2,DWORD dwFlags)
{
	pDC->MoveTo(x1,y1);
	pDC->LineTo(x2,y2);
}
