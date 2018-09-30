// DrawLine.h: interface for the DrawLine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAWLINE_H__B4F5AF8E_17CA_42F5_BFF8_CFAFB8A374AD__INCLUDED_)
#define AFX_DRAWLINE_H__B4F5AF8E_17CA_42F5_BFF8_CFAFB8A374AD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyPen.h"

class DrawLine  : public MyPen
{
public:
	DrawLine();
	virtual ~DrawLine();
	void Draw(CDC *pDC,UINT x1,UINT y1,UINT x2,UINT y2,DWORD dwFlags = 0);

};

#endif // !defined(AFX_DRAWLINE_H__B4F5AF8E_17CA_42F5_BFF8_CFAFB8A374AD__INCLUDED_)
