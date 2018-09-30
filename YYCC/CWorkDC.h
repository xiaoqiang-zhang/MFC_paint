#ifndef _CWORKDC_H
#define _CWORKDC_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// CWorkDC.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWorkDC

class CWorkDC : public CDC
{
	DECLARE_DYNAMIC(CWorkDC)

	int m_nSavedDC;

	// main constructor
	void BuildWorkDC(CDC*);

// Constructors
public:
	CWorkDC(CDC*);
	CWorkDC(CDC*,CBrush*);
	CWorkDC(CDC*,CPen*);
	CWorkDC(CDC*,CFont*);
	CWorkDC(CDC*,CRgn*);
	CWorkDC(CDC*,CBitmap*);
	CWorkDC(CDC*,CBitmap*,CPen*);
	CWorkDC(CDC*,CBitmap*,CBrush*);
	CWorkDC(CDC*,CBitmap*,CFont*);
	CWorkDC(CDC*,CBitmap*,CPen*,CBrush*);
	CWorkDC(CDC*,CBitmap*,CPen*,CBrush*,CFont*);


// Implementation
public:
	virtual ~CWorkDC();

// Device-Context Functions
	//virtual BOOL RestoreDC();
};

#endif // _CWORKDC_H
