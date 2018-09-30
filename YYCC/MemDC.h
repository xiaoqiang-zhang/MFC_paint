// MemDC.h: interface for the CMemDC class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MEMDC_H__587C7783_BC50_4FBB_92D6_283524E2CC16__INCLUDED_)
#define AFX_MEMDC_H__587C7783_BC50_4FBB_92D6_283524E2CC16__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "ExBitmap.h"
class CMemDC : public CDC  
{
public:
	CMemDC(int width,int height,LPCTSTR lpszFileName,CDC *pOrigPC = NULL, COLORREF background = RGB(0,0,0));
	CMemDC(LPCTSTR lpszFileName, CDC *pOrigDC = NULL);
	CBitmap* Close();
	CBitmap* GetBitmap(){return m_pBitmap;};
	virtual ~CMemDC();
private:
	CExBitmap *m_pBitmap;
    CBitmap *m_pOldBmp;

};

#endif // !defined(AFX_MEMDC_H__587C7783_BC50_4FBB_92D6_283524E2CC16__INCLUDED_)
