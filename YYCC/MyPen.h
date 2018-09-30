// MyPen.h: interface for the MyPen class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYPEN_H__DA1BDD8D_09DA_49AC_9525_C38CF2F67B9C__INCLUDED_)
#define AFX_MYPEN_H__DA1BDD8D_09DA_49AC_9525_C38CF2F67B9C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



#define TUYA_FLAG_PREVIEW   1

class MyPen : public CObject  
{
public:	
	BOOL m_pChanged;
	char *m_lpszName;
	MyPen();
	virtual ~MyPen();	
	virtual void Draw(CDC *pDC,UINT x1,UINT y1,UINT x2,UINT y2,DWORD dwFlags = 0) = 0;
    LPCSTR GetName(){return m_lpszName;};


};

#endif // !defined(AFX_MYPEN_H__DA1BDD8D_09DA_49AC_9525_C38CF2F67B9C__INCLUDED_)
