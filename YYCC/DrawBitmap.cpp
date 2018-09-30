// DrawBitmap.cpp: implementation of the DrawBitmap class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "YYCC.h"
#include "DrawBitmap.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DrawBitmap::DrawBitmap()
{
	this->m_lpszName = "TOOL_BITMAP";

}

DrawBitmap::~DrawBitmap()
{


}
void DrawBitmap::Draw(CDC * pDC,UINT x1,UINT y1,UINT x2,UINT y2,DWORD dwFlags)
{
	

	Graphics *graphics = Graphics::FromHDC(pDC->m_hDC);

	Pen      pen(Color(255, 0, 0, 255));//tou ming du/yan se su du
	if(m_pChanged)
	{
  		CFileDialog dlg(TRUE,
						_T("jpg"),
						NULL,
						OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
						_T("(*.jpg)|*.jpg||"));	
		if (dlg.DoModal() == IDOK)
		{
			str = dlg.GetPathName();
		}
		MultiByteToWideChar(CP_ACP,0,str,-1,wcstring,256);
	}
	m_pChanged = FALSE;
	Image img(wcstring);
   
	graphics->DrawImage(&img,(int)x2,(int)y2);
	delete graphics;

}