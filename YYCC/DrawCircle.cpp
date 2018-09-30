// DrawCircle.cpp: implementation of the DrawCircle class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "YYCC.h"
#include "DrawCircle.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DrawCircle::DrawCircle()
{
    this->m_lpszName = "TOOL_CIRCLE";
}

DrawCircle::~DrawCircle()
{

}
void DrawCircle::Draw(CDC *pDC, UINT x1, 
						  UINT y1, UINT x2, UINT y2,DWORD dwFlags)
{
	pDC->Ellipse(x1,y1,x2,y2);
}
