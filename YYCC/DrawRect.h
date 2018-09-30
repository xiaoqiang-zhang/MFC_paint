// DrawRect.h: interface for the DrawRect class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAWRECT_H__4AF85A4B_0A41_4BE0_B650_1E0D178EC86E__INCLUDED_)
#define AFX_DRAWRECT_H__4AF85A4B_0A41_4BE0_B650_1E0D178EC86E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyPen.h"

class DrawRect : public MyPen  
{
public:
	DrawRect();
	virtual ~DrawRect();
	void Draw(CDC *pDC,UINT x1,UINT y1,UINT x2,UINT y2,DWORD dwFlags = 0);
};

#endif // !defined(AFX_DRAWRECT_H__4AF85A4B_0A41_4BE0_B650_1E0D178EC86E__INCLUDED_)
