// DrawBitmap.h: interface for the DrawBitmap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAWBITMAP_H__A76FCBBA_7E7A_492B_8B21_5694EA1201CE__INCLUDED_)
#define AFX_DRAWBITMAP_H__A76FCBBA_7E7A_492B_8B21_5694EA1201CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyPen.h"

class DrawBitmap : public MyPen  
{
public:
	CString str;
	wchar_t wcstring[256];
	DrawBitmap();

	virtual ~DrawBitmap();
	void Draw(CDC * pDC,UINT x1,UINT y1,UINT x2,UINT y2,DWORD dwFlags = 0);
	void Draw(CDC *pDC,UINT x1,UINT y1,UINT x2,UINT y2,DWORD dwFlags = 0,wchar_t *wstr =NULL);

};

#endif // !defined(AFX_DRAWBITMAP_H__A76FCBBA_7E7A_492B_8B21_5694EA1201CE__INCLUDED_)
