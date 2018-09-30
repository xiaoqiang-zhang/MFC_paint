// MyPen.cpp: implementation of the MyPen class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "YYCC.h"
#include "MyPen.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MyPen::MyPen()
{
	m_pChanged = TRUE;
}

MyPen::~MyPen()
{

}

void Draw(CDC * pDC,UINT x1,UINT y1,UINT x2,UINT y2,DWORD dwFlags )
{

}
