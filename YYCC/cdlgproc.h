#if !defined(AFX_CDLGPROC_H__B01594CA_F940_4B99_894D_2ADC8D9394E7__INCLUDED_)
#define AFX_CDLGPROC_H__B01594CA_F940_4B99_894D_2ADC8D9394E7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// cdlgproc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// cdlgproc dialog

class cdlgproc : public CDialog
{
// Construction
public:
	cdlgproc(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(cdlgproc)
	enum { IDD = IDD_DIALOG3 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(cdlgproc)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(cdlgproc)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDLGPROC_H__B01594CA_F940_4B99_894D_2ADC8D9394E7__INCLUDED_)
