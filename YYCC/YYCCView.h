// YYCCView.h : interface of the CYYCCView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_YYCCVIEW_H__B86650C0_61DE_4B7A_8681_12B17516B34A__INCLUDED_)
#define AFX_YYCCVIEW_H__B86650C0_61DE_4B7A_8681_12B17516B34A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include  "MyPen.h"
#include "MainFrm.h"
#include "ReTurn.h"

class CYYCCView : public CView
{
protected: // create from serialization only
	CYYCCView();
	DECLARE_DYNCREATE(CYYCCView)
	MyPen* m_pCurPaintTool;

	CPen			*m_pCurPen;
	BOOL			m_bChangedPen;
	COLORREF		m_pColor;
	int			    m_pWidth;
	int				m_Style;
	CPtrArray       m_ReturnArray;

// Attributes
public:
	CYYCCDoc* GetDocument();
	void onPen(UINT nID);
	void onColor(UINT nID);
	void onLineWidth(UINT nID);
	void onReturn();
	int CreateNewPen(int nStyle, int nWIdth, COLORREF cir);
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CYYCCView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CYYCCView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
   CPoint m_ptStart,m_ptOld;
   CPoint m_ptButtonUp;
// Generated message map functions
protected:
	//{{AFX_MSG(CYYCCView)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in YYCCView.cpp
inline CYYCCDoc* CYYCCView::GetDocument()
   { return (CYYCCDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_YYCCVIEW_H__B86650C0_61DE_4B7A_8681_12B17516B34A__INCLUDED_)
