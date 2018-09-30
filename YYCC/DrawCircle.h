// DrawCircle.h: interface for the DrawCircle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAWCIRCLE_H__043F9CAB_B5CB_456D_A052_E4A044BF977F__INCLUDED_)
#define AFX_DRAWCIRCLE_H__043F9CAB_B5CB_456D_A052_E4A044BF977F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyPen.h"

class DrawCircle : public MyPen  
{
public:
	DrawCircle();
	virtual ~DrawCircle();
	void Draw(CDC *pDC,UINT x1,UINT y1,UINT x2,UINT y2,DWORD dwFlags = 0);
};

#endif // !defined(AFX_DRAWCIRCLE_H__043F9CAB_B5CB_456D_A052_E4A044BF977F__INCLUDED_)
