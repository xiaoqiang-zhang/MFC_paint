// YYCCDoc.h : interface of the CYYCCDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_YYCCDOC_H__3EC0D32F_958B_409E_AFDA_DCE02391ACD3__INCLUDED_)
#define AFX_YYCCDOC_H__3EC0D32F_958B_409E_AFDA_DCE02391ACD3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "MemDC.h"

class CYYCCDoc : public CDocument
{
protected: // create from serialization only
	CYYCCDoc();
	DECLARE_DYNCREATE(CYYCCDoc)

// Attributes
public:
		CString str;
		volatile LPCTSTR Routine;
		UINT WIDTH;
		UINT HEIGHT;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CYYCCDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	CMemDC * GetMemDC(){return m_pMemDC;}
	void SetMemDC();
    BOOL OnSaveDocument(LPCTSTR lpszPathName);
	BOOL OnOpenDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	int OnUpdateView(CDC *pDC);
	int OnUpdateView(CDC *pDC ,CRect rect);
	virtual ~CYYCCDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CString m_strContent;
    CMemDC* m_pMemDC;
// Generated message map functions
protected:
	//{{AFX_MSG(CYYCCDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_YYCCDOC_H__3EC0D32F_958B_409E_AFDA_DCE02391ACD3__INCLUDED_)
