// DrawRect.cpp: implementation of the DrawRect class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "YYCC.h"
#include "DrawRect.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DrawRect::DrawRect()
{
	this->m_lpszName = "TOOL_RECT";
}

DrawRect::~DrawRect()
{

}
void DrawRect::Draw(CDC *pDC, UINT x1, 
						  UINT y1, UINT x2, UINT y2,DWORD dwFlags)
{
	pDC->Rectangle(x1,y1,x2,y2);
}
