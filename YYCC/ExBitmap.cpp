// ExBitmap.cpp
//
// Author : John R. Shaw (shawj2@earthlink.net)
// Date   : Aug 26 2003
//
// Copyright (C) 2003 John R. Shaw
// All rights reserved.
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed unmodified by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name is included. If 
// the source code in this file is used in any commercial application 
// then a simple email would be nice.
//
// Warranties and Disclaimers:
// THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND
// INCLUDING, BUT NOT LIMITED TO, WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.
// IN NO EVENT WILL JOHN R. SHAW BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY OR CONSEQUENTIAL DAMAGES,
// INCLUDING DAMAGES FOR LOSS OF PROFITS, LOSS OR INACCURACY OF DATA,
// INCURRED BY ANY PERSON FROM SUCH PERSON'S USAGE OF THIS SOFTWARE
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
//
// Please email bug reports, bug fixes, enhancements, requests and
// comments to: shawj2@earthlink.net
//

// Note: The strange comment styles are used for generating documentation.

// Note: Oct 3 2003 !!! Computer Crash Durring Compile !!!
// Lost this entire file, it was gone when computer was restarted.
// Recovered earlier version from zipped backup file. Unfortunatly
// the file was 5 days old. (%&@$...)

// Notes:
// Rotation can be accomplished using SetWorldTransform()
//    on WinNT 3.1 or later.
//
// SetViewportOrg(...) = Center of destination.
// The destination bitmap needs to be 4 x size of original.
// ( 4 x max(Origanal.Width(), Origanal.Height()) )
//
//                  -Y
//                   |
//       +-----------+-----------+
//       |           |           |
//       | -.......+ |         - |
//       |   +180    |   -90   : |
//       |   -180    |   +270  : |
//       |           |         + |
//       |           |           |
// -X <--+-----------+-----------+--> +X
//       |           |           |
//       | +         | Origanal  |
//       | : +90     | location  |
//       | : -270    | and size  |
//       | -         | +.......- |
//       |           |           |
//       +-----------+-----------+
//                   |
//                  +Y
//

#include "stdafx.h"

#include "CWorkDC.h"
#include "MyTrace.h"

#include "ExBitmap.h"

/////////////////////////////////////////////////////////////////////////////
// class CParBitmap()

/**
CParBitmap is a CBitmap derived class which is designed to hold a partial
bitmap. That is it holds a bitmap that represents only a part (sub-bitmap)
of the bitmap from which it was copied along with additional information.
*/

class CParBitmap: public CBitmap
{
	DECLARE_DYNAMIC(CParBitmap)

	CRect m_PosRect;	//!< Postion relative to the upper left courner of original.
	SIZE  m_SrcSize;	//!< Size of original source bitmap.

public:
	CParBitmap();
	~CParBitmap();

public:
	void SetPosRect(int left, int top, int right, int bottom);
	void SetPosRect(CRect& rcPos);
	CRect& GetPosRect();

	void SetSrcSize(int cx, int cy);
	void SetSrcSize(SIZE size);
	SIZE GetSrcSize();
};

//! Initializes members to default values.
inline CParBitmap::CParBitmap() : m_PosRect(0,0,0,0)
{ m_SrcSize.cx = m_SrcSize.cy = 0; }

//! Frees all resources used by this object.
inline CParBitmap::~CParBitmap()
{ if( GetSafeHandle() ) DeleteObject(); }

//! Sets the position of the partial bitmap relitive to original source bitmap.
inline void CParBitmap::
SetPosRect(int left, int top, int right, int bottom)
{ m_PosRect.SetRect(left,top, right,bottom); }
//! Sets the position of the partial bitmap relitive to original source bitmap.
inline void CParBitmap::SetPosRect(CRect& rcPos)
{ 
	MYTRACE(1,_T("CParBitmap::SetPosRect(rcPos(%d, %d, %d, %d))\n"),
		rcPos.left, rcPos.top, rcPos.right, rcPos.bottom);
	m_PosRect = rcPos;
	MYTRACE(-1,_T("CParBitmap::SetPosRect()\n"));
}

/**
Retrieves the positional rectangle of the partial bitmap
relitive to the upper left hand courner of the source bitmap.
\returns
Reference to positional rectangle.
*/
inline CRect& CParBitmap::GetPosRect()
{ return m_PosRect; }

//! Sets the size of the source bitmap from which the partial bitmap was copied.
inline void CParBitmap::SetSrcSize(int cx, int cy)
{
	MYTRACE(1,_T("CParBitmap::SetSrcSize(%d, %d)\n"), cx, cy);
	m_SrcSize.cx = cx; m_SrcSize.cy = cy;
	MYTRACE(-1,_T("CParBitmap::SetSrcSize()\n"));
}
//! Sets the size of the source bitmap from which the partial bitmap was copied.
inline void CParBitmap::SetSrcSize(SIZE size)
{ m_SrcSize.cx = size.cx, m_SrcSize.cy = size.cy; }

/**
Retrieves the size of the original bitmap that the partial bitmap was copied from.
\returns
Size of original source bitmap.
*/
inline SIZE CParBitmap::GetSrcSize()
{ return m_SrcSize; }

IMPLEMENT_DYNAMIC(CParBitmap,CBitmap)

/////////////////////////////////////////////////////////////////////////////
// class CExBitmap()

/**
CExBitmap is a CBitmap derived class designed to extend CBitmap capabilities for
use in an bitmap painting program:

1) Support for Undo/Redo list.

2) Support for multiple backup copies of bitmap.

3) Support for copying, expanding, and clearing bitmaps.

4) Support for rotating (90, 180, 270 degrees clockwise).

5) Support for Fliping (horizontaly and verticaly)

6) Support for loading, saving, and color-depth conversion (via CDibData class).
*/

IMPLEMENT_DYNAMIC(CExBitmap,CBitmap)

/**
Initializes members to default values.
*/
CExBitmap::CExBitmap() :
	m_nUndoRedoSize(10), m_nUndo(0), m_nRedo(0),
	m_nBack(0), m_nUndoTotal(0), m_bAutoUndo(FALSE)
{
	MYTRACE( 1,_T("CExBitmap::CExBitmap()\n"));
	MYTRACE(-1,_T("CExBitmap::CExBitmap()\n"));
}

/**
Frees all resources used by this object.
*/
CExBitmap::~CExBitmap()
{
	MYTRACE(1,_T("CExBitmap::~CExBitmap()\n"));
	DeleteAllObjects();
	MYTRACE(-1,_T("CExBitmap::~CExBitmap()\n"));
}

/**
Frees all resources used by this object.
*/
void CExBitmap::DeleteAllObjects()
{
	MYTRACE(1,_T("CExBitmap::RemoveAll()\n"));
	RemoveBackups();
	RemoveUndo();
	RemoveRedo();
	DeleteObject();
	MYTRACE(-1,_T("CExBitmap::RemoveAll()\n"));
}

/**
Copies a source bitmap to this bitmap object.
\returns
Nonzero on success; otherwise zero.
\par
Windows NT/ 2000: To get extended error information, call GetLastError.
*/
BOOL CExBitmap::CopyBitmap(
CBitmap& bmpSrc,				//!< Reference to source bitmap object.
BOOL	 bStretch/*=FALSE*/,	/**< Flag indicating that the source is to be
								     stretched from (xDest, yDest) to the
									 lower right hand courner of this bitmap
									 object. */
int		 xDest/*=0*/,			/**< The x-coordinate, of the upper left hand
								     courner, where the source bitmap is to be
									 copied. */
int		 yDest/*=0*/,			/**< The y-coordinate, of the upper left hand
								     courner, where the source bitmap is to be
									 copied. */
int		 xSrc/*=0*/,			/**< The x-coordinate, of the upper left hand
								     courner, of the source bitmap copied to
									 the destination bitmap. */
int		 ySrc/*=0*/				/**< The y-coordinate, of the upper left hand
								     courner, of the source bitmap copied to
									 the destination bitmap. */
)
{
	ASSERT_VALID(this);
	ASSERT(m_hObject != NULL);
	ASSERT(bmpSrc.GetSafeHandle() != NULL);

	MYTRACE(1,_T("CExBitmap::CopyBitmap(bmpSrc, %s, %d, %d, %d, %d)\n"),
		MYGETBOOLSTR(bStretch), xDest, yDest, xSrc, ySrc);

	BITMAP bmDest;
	if( !GetBitmap(&bmDest) )
	{
		MYTRACE(-1,_T("CExBitmap::CopyBitmap() = FALSE\n"));
		return FALSE;
	}

	if( xDest >= bmDest.bmWidth || yDest >= bmDest.bmHeight ||
		-xDest >= bmDest.bmWidth || -yDest >= bmDest.bmHeight )
	{
		MYTRACE(-1,_T("CExBitmap::CopyBitmap() = FALSE\n"));
		::SetLastError(ERROR_INVALID_PARAMETER);
		return FALSE;
	}

	// Adjust destination width & height
	if( 0 < xDest )
		bmDest.bmWidth -= xDest;
	if( 0 < yDest )
		bmDest.bmHeight -= yDest;

	::GdiFlush();	// See CreateDIBSection()

	BOOL bResult = FALSE;
	try
	{
		CWorkDC dcDest(NULL, this);
		CWorkDC dcSrc(NULL, &bmpSrc);

		// If we are not stretching
		if( !bStretch )
		{
			bResult = dcDest.BitBlt(
				xDest, yDest, bmDest.bmWidth, bmDest.bmHeight,
				&dcSrc, xSrc, ySrc, SRCCOPY);
	#ifdef _DEBUG
			BITMAP bmSrc;
			if( bmpSrc.GetBitmap(&bmSrc) )
			{
			MYTRACE(0,_T("BitBlt(%d, %d, %d ,%d,"
				         " dcSrc, %d , %d, [%d, %d,] SRCCOPY) = %s\n"),
				xDest, yDest, bmDest.bmWidth, bmDest.bmHeight,
				xSrc, ySrc, bmSrc.bmWidth, bmSrc.bmHeight,
				MYGETBOOLSTR(bResult));
			}
	#endif
		}
		else
		{
			BITMAP bmSrc;
			if( bmpSrc.GetBitmap(&bmSrc) )
			{
				if( xSrc < bmSrc.bmWidth && ySrc < bmSrc.bmHeight &&
					-xSrc < bmSrc.bmWidth && -ySrc < bmSrc.bmHeight )
				{
					// Adjust source width & height
					if( 0 < xSrc )
						bmSrc.bmWidth -= xSrc;
					if( 0 < ySrc )
						bmSrc.bmHeight -= ySrc;

					// Stretch source on to destination
					bResult = dcDest.StretchBlt(
						xDest, yDest, bmDest.bmWidth, bmDest.bmHeight,
						&dcSrc, xSrc, ySrc, bmSrc.bmWidth, bmSrc.bmHeight,
						SRCCOPY);
					MYTRACE(0,_T("StretchBlt(%d, %d, %d ,%d,"
								 " dcSrc, %d ,%d, %d, %d SRCCOPY) = %s\n"),
						xDest, yDest, bmDest.bmWidth, bmDest.bmHeight,
						xSrc, ySrc, bmSrc.bmWidth, bmSrc.bmHeight,
						MYGETBOOLSTR(bResult));
				}
			}
		}
	}
	catch(CResourceException* /*e*/)
	{
		MYTRACE(0,_T("Resource Exception\n"));
		bResult = FALSE;
	}
	MYTRACE(-1,_T("CExBitmap::CopyBitmap() = %s\n"),MYGETBOOLSTR(bResult));
	return bResult;
}

/**
Copies a source bitmap to this bitmap object.
\returns
Nonzero on success; otherwise zero.
\par
Windows NT/ 2000: To get extended error information, call GetLastError.
*/
BOOL CExBitmap::CopyBitmap(
CBitmap&	bmpSrc,	//!< Reference to source bitmap object.
CRect&		rcSrc,	//!< Source rectangle: sub-rectangle of source bitmap.
CRect&		rcDest	//!< Destination rectangle: sub-rectangle of destination bitmap.
)
{
	ASSERT_VALID(this);
	ASSERT(m_hObject != NULL);
	ASSERT(bmpSrc.GetSafeHandle() != NULL);

	MYTRACE(1,_T("CExBitmap::CopyBitmap(bmpSrc, sRECT(%d, %d, %d, %d), dRECT(%d, %d, %d, %d))\n"),
		rcSrc.left, rcSrc.top, rcSrc.right, rcSrc.bottom,
		rcDest.left, rcDest.top, rcDest.right, rcDest.bottom);

	BITMAP bmDest;
	if( !GetBitmap(&bmDest) )
	{
		MYTRACE(-1,_T("CExBitmap::CopyBitmap() = FALSE\n"));
		return FALSE;
	}

	::GdiFlush();	// See CreateDIBSection()

	BOOL bResult = FALSE;
	try
	{
		CWorkDC dcDest(NULL, this);
		CWorkDC dcSrc(NULL, &bmpSrc);

		if( rcSrc != rcDest )
		{
			bResult = dcDest.StretchBlt(
				rcDest.left, rcDest.top, rcDest.Width(), rcDest.Height(),
				&dcSrc, rcSrc.left, rcSrc.top, rcSrc.Width(), rcSrc.Height(),
				SRCCOPY);
			MYTRACE(0,_T("StretchBlt(%d, %d, %d ,%d,"
				         " dcSrc, %d , %d, %d, %d, SRCCOPY) = %s\n"),
				rcDest.left, rcDest.top, rcDest.Width(), rcDest.Height(),
				rcSrc.left, rcSrc.top, rcSrc.Width(), rcSrc.Height(),
				MYGETBOOLSTR(bResult));
		}
		else
		{
			bResult = dcDest.BitBlt(
				rcDest.left, rcDest.top, rcDest.Width(), rcDest.Height(),
				&dcSrc, rcSrc.left, rcSrc.top, SRCCOPY);
			MYTRACE(0,_T("BitBlt(%d, %d, %d ,%d,"
				         " dcSrc, %d , %d, [%d, %d,] SRCCOPY) = %s\n"),
				rcDest.left, rcDest.top, rcDest.Width(), rcDest.Height(),
				rcSrc.left, rcSrc.top, rcSrc.Width(), rcSrc.Height(),
				MYGETBOOLSTR(bResult));
		}
	}
	catch(CResourceException* /*e*/)
	{
		MYTRACE(0,_T("Resource Exception\n"));
		bResult = FALSE;
	}

	MYTRACE(-1,_T("CExBitmap::CopyBitmap() = %s\n"),MYGETBOOLSTR(bResult));
	return bResult;
}

/**
Expands/Srinks size of this objects attached bitmap.
\returns
Nonzero on success; otherwise zero.
\par
Windows NT/ 2000: To get extended error information, call GetLastError.
*/
BOOL CExBitmap::ExpandBitmap(
int		 dx,			//!< New width.
int		 dy,			//!< New height.
COLORREF clrFill/*=0*/	//!< Color used to fill expanded portion of bitmap.
)
{
	ASSERT_VALID(this);
	ASSERT(m_hObject != NULL);

	MYTRACE(1,_T("CExBitmap::ExpandBitmap(%d, %d)\n"), dx, dy);

	if( !GetSafeHandle() )
	{
		MYTRACE(-1,_T("CExBitmap::ExpandBitmap() = FALSE\n"));
		::SetLastError(ERROR_INVALID_HANDLE);
		return FALSE;
	}

	if( !dx && !dy )
	{
		MYTRACE(-1,_T("CExBitmap::ExpandBitmap() = TRUE\n"));
		::SetLastError(ERROR_INVALID_PARAMETER);
		return TRUE;
	}

	// Get new bitmap size
	BITMAP bmOld;
	if( !GetBitmap(&bmOld) )
	{
		MYTRACE(-1,_T("CExBitmap::ExpandBitmap() = FALSE\n"));
		return FALSE;
	}

	::GdiFlush();	// See CreateDIBSection()

	// Caculate new bitmap size
	CRect rcPrev(0, 0, bmOld.bmWidth, bmOld.bmHeight);
	int newWidth  = bmOld.bmWidth + dx;
	int newHeight = bmOld.bmHeight + dy;

	if( newWidth < 1 || newHeight < 1 )
	{
		MYTRACE(-1,_T("CExBitmap::ExpandBitmap() = FALSE\n"));
		::SetLastError(ERROR_INVALID_PARAMETER);
		return FALSE;
	}

	BOOL bResult = FALSE;
	try
	{
		// Create expanded bitmap
		CExBitmap bmpNew;
		{
			CWorkDC dcThis(NULL, this);
			if( !bmpNew.CreateCompatibleBitmap(
				&dcThis, newWidth, newHeight) )
			{
				MYTRACE(-1,_T("CExBitmap::ExpandBitmap() = FALSE\n"));
				return FALSE;
			}
			MYTRACE(0,_T("THIS Size(%d, %d)\n"),
				bmOld.bmWidth, bmOld.bmHeight);
			MYTRACE(0,_T("bmpNew.CreateCompatibleBitmap(dcThis, %d, %d) = TRUE\n"),
				newWidth, newHeight);
		}

		// Copy this bitmap to new bitmap
		bmpNew.CopyBitmap(this);

		// Detach new handle from new bitmap object
		HGDIOBJ hNew = bmpNew.Detach();

		// Delete this bitmap
		DeleteObject();

		// Attach new handle to this bitmap
		Attach(hNew);

		// Fill in background
		if( newWidth >  bmOld.bmWidth || newHeight > bmOld.bmHeight )
		{
			CWorkDC dcWork(NULL, this);
			if( newWidth >  bmOld.bmWidth )
			{
				dcWork.FillSolidRect(
					bmOld.bmWidth, 0,
					newWidth - bmOld.bmWidth, newHeight, clrFill);
			}
			if( newHeight > bmOld.bmHeight )
			{
				dcWork.FillSolidRect(
					0, bmOld.bmHeight,
					bmOld.bmWidth, newHeight - bmOld.bmHeight, clrFill);
			}
		}
		bResult = TRUE;
	}
	catch(CResourceException* /*e*/)
	{
		MYTRACE(0,_T("Resource Exception\n"));
	}

	MYTRACE(-1,_T("CExBitmap::ExpandBitmap() = TRUE\n"));
	return bResult;
}

/**
Flips bitmap attached to this object.
\returns
Nonzero on success; otherwise zero.
\par
Windows NT/ 2000: To get extended error information, call GetLastError.
*/
BOOL CExBitmap::Flip(
int nFlip/* = 0*/	//!< 0 = Horizontal, 1 = Vertical, 2 = Horizontal & Vertical.
)
{
	ASSERT_VALID(this);
	ASSERT(m_hObject != NULL);

	MYTRACE(1,_T("CExBitmap::Flip(%s)\n"),
		(nFlip == 0) ? _T("Horizontal") : (nFlip == 1) ? _T("Vertical"): _T("180 Degrees"));
	if( !m_hObject )
	{
		MYTRACE(-1,_T("CExBitmap::Flip() = FALSE\n"));
		::SetLastError(ERROR_INVALID_HANDLE);
		return FALSE;
	}

	// Get this bitmaps info.
	BITMAP bm;
	if( !GetBitmap(&bm) )
	{
		MYTRACE(-1,_T("CExBitmap::Flip() = FALSE\n"));
		return FALSE;
	}

	::GdiFlush();	// See CreateDIBSection()

	if( m_bAutoUndo )
		SaveUndo();

	// Create display compatible DC and
	// select this bitmap into it.
	BOOL bResult = FALSE;
	try
	{
		CWorkDC dcDest(NULL, this);

		// Create source bitmap compatible with this
		CExBitmap bmpSrc;
		if( !bmpSrc.CreateCompatibleBitmap(&dcDest, bm.bmWidth, bm.bmHeight) )
		{
			MYTRACE(-1,_T("CExBitmap::Flip() = FALSE\n"));
			return FALSE;
		}
		MYTRACE(0,_T("CreateCompatibleBitmap(dcDest, %d, %d) = TRUE\n"),
			bm.bmWidth, bm.bmHeight);

		// Create display compatible DC and
		// select source bitmap into it.
		CWorkDC dcSrc(NULL, &bmpSrc);

		// Copy this bitmap to source bitmap
		dcSrc.BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &dcDest, 0, 0, SRCCOPY);

		// Set mapping mode that allows for customization
		dcDest.SetMapMode(MM_ANISOTROPIC);

		// Safety: Let user figure out what is wrong!
		nFlip %= 3;

		// Horizontal flip
		if( nFlip < 1 )
		{
			// Axis
			// +X <---+
			//        |
			//       +Y
			dcDest.SetViewportOrg(bm.bmWidth - 1, 0);
			dcDest.SetViewportExt(-1, 1);
		}
		// Vetical flip
		else if( nFlip < 2 )
		{
			// Axis
			// +Y
			//  |
			//  +---> +X
			dcDest.SetViewportOrg(0, bm.bmHeight - 1);
			dcDest.SetViewportExt(1, -1);
		}
		// Horizontal & Vitical flip (same as 180 degrees rotation)
		else
		{
			// Axis
			//       +Y
			//        |
			// +X <---+
			dcDest.SetViewportOrg(bm.bmWidth - 1, bm.bmHeight - 1);
			dcDest.SetViewportExt(-1, -1);
		}

		// Copy source to new destination origin
		bResult = dcDest.BitBlt(0, 0, bm.bmWidth, bm.bmHeight,
			&dcSrc, 0, 0, SRCCOPY);

		// OK: Remember CWorkDC is self cleaning
		bmpSrc.DeleteObject();
	}
	catch(CResourceException* /*e*/)
	{
		MYTRACE(0,_T("Resource Exception\n"));
		bResult = FALSE;
	}

	if( m_bAutoUndo && bResult )
		Backup();

	MYTRACE(-1,_T("CExBitmap::Flip() = %s\n"),MYGETBOOLSTR(bResult));
	return bResult;
}

//#define WINNT31_ROTATE
#ifdef WINNT31_ROTATE

/**
Rotates bitmap attached to this object, using SetWorldTransform().
(Win2000 and above)
\returns
Nonzero on success; otherwise zero.
\par
Windows NT/ 2000: To get extended error information, call GetLastError.
*/
BOOL CExBitmap::Rotate(
int nRotations/*= 0*/	//!< 0 = +90 degrees, 1 = +180 degrees, 2 = -90 degrees.
)
{
	ASSERT_VALID(this);
	ASSERT(m_hObject != NULL);

	// Safety: Let user figure out what is wrong!
	nRotations %= 3;

	MYTRACE(1,_T("CExBitmap::Rotate(%s Degrees)\n"),
		(nRotations == 0) ? _T("90") : (nRotations == 1) ? _T("180"): _T("270"));

	if( !m_hObject )
	{
		MYTRACE(-1,_T("CExBitmap::Rotate() = FALSE\n"));
		::SetLastError(ERROR_INVALID_HANDLE);
		return FALSE;
	}

	// Rotate 180 degrees
	if( nRotations == 1 )
#ifdef _DEBUG
	{
		BOOL bResult = Flip(2);
		MYTRACE(-1,_T("CExBitmap::Rotate() = %s\n"),MYGETBOOLSTR(bResult));
		return bResult;
	}
#else
		return Flip(2);
#endif

	::GdiFlush();

	if( m_bAutoUndo )
		SaveUndo();

	BOOL bResult = FALSE;
	try
	{
		// Get size of final destination
		SIZE size = GetSize();
		size.cy ^= size.cx; size.cx ^= size.cy; size.cy ^= size.cx;

		// Rotate bitmap
		CExBitmap bmpTemp;
		CExBitmap bmpDest;
		{
			CWorkDC dcSrc(NULL, this);

			// Create temporary destination bitmap
			// This is needed to prevent part of our
			// bitmap from being cut off durring rotation.
			int cMax = max(size.cy, size.cx);
			if( !bmpTemp.CreateCompatibleBitmap(&dcSrc, cMax, cMax) )
			{
				MYTRACE(-1,_T("CExBitmap::Rotate() = FALSE\n"));
				return FALSE;
			}
			MYTRACE(0,_T("CreateCompatibleBitmap(bmpTemp, %d, %d) = TRUE\n"),
				cMax, cMax);

			// Select bitmap into destination DC
			CWorkDC dcDest(NULL, &bmpTemp);

			// Prepare destination DC for blit rotation
			if( !SetGraphicsMode(dcDest.GetSafeHdc(), GM_ADVANCED) )
			{
				MYTRACE(-1,_T("CExBitmap::Rotate() = FALSE\n"));
				return FALSE;
			}

			if( !dcDest.SetMapMode(MM_ANISOTROPIC) )
			{
				MYTRACE(-1,_T("CExBitmap::Rotate() = FALSE\n"));
				return FALSE;
			}

			// Set Origin & World transform
			XFORM xForm;
			int xOrg = 0, yOrg = 0;
			::ZeroMemory(&xForm, sizeof(XFORM));

			if( nRotations == 0 )
			{
				// Rotation +90 degrees
				//
				// Case 1:
				//
				//                 (0,0)
				// -X <--+-----------+-------+---+--> +X
				//       |           |       |   |
				//       |     B     |       |   |
				//       |           |   A   |   |
				//       +-----------+       |   |
				//       |           |       |   |
				//       +-----------+-------+---+
				//       |           |
				//       |          +Y
				//       |
				//       | <- Final destinations x-origin.
				//
				// Case 2:
				//                 (0,0)
				// -X <--+---+-------+-----------+--> +X
				//       |   |       |           |
				//       |   |       |     A     |
				//       |   |   B   |           |
				//       |   |       +-----------+
				//       |   |       |           |
				//       +---+-------+-----------+
				//       |           |
				//       |          +Y
				//       |
				//       | <- Final destinations x-origin.
				//
				// Need to adjust destination origin to compenstate.
				//
				xOrg = cMax;
				if( cMax > size.cx )
					xOrg -= cMax - size.cx;

				// Rotate from A to B
				xForm.eM12 = (FLOAT)  1.0; 
				xForm.eM21 = (FLOAT) -1.0; 
			}
			else
			{
				// Rotation -90 degrees
				//
				// Case 1:
				//
				//       -Y
				//       |
				//       +-----------+ <- Final destination y-origin.
				//       |           |
				//       +-----------+
				//       |           |
				//       |     B     |
				//       |           |
				// (0,0) +-------+---+--> +X
				//       |       |   |
				//       |       |   |
				//       |   A   |   |
				//       |       |   |
				//       |       |   |
				//       +-------+---+
				//       | 
				//      +Y
				//
				// Need to adjust destination origin to compenstate.
				//
				// Case 2:
				//
				//      -Y
				//       |
				//       +-------+---+ <- Final destination y-origin.
				//       |       |   |
				//       |       |   |
				//       |   B   |   |
				//       |       |   |
				//       |       |   |
				// (0,0) +-------+---+--> +X
				//       |           |
				//       |     A     |
				//       |           |
				//       +-----------+
				//       |           |
				//       +-----------+
				//       |
				//      +Y
				//
				yOrg = cMax;
				if( cMax > size.cy )
					yOrg -= cMax - size.cy;

				// 90 degrees counter-clockwise
				xForm.eM12 = (FLOAT) -1.0; 
				xForm.eM21 = (FLOAT)  1.0; 
			}

			dcDest.SetViewportOrg(xOrg, yOrg);
			SetWorldTransform(dcDest.GetSafeHdc(), &xForm); 

			// Blit with rotation from A to B
			dcDest.BitBlt(0, 0, cMax, cMax, &dcSrc, 0, 0, SRCCOPY);

			// Create rotated destination bitmap object
			if( !bmpDest.CreateCompatibleBitmap(&dcSrc, size.cx, size.cy) )
			{
				MYTRACE(-1,_T("CExBitmap::Rotate() = FALSE\n"));
				return FALSE;
			}
			MYTRACE(0,_T("CreateCompatibleBitmap(dcDest, %d, %d) = TRUE\n"),
				size.cx, size.cy);
		}

		// Final copy to destination.
		// I do not like the need for this,
		// it indicates and error in thinking!
		if( !bmpDest.CopyBitmap(bmpTemp) )
		{
			MYTRACE(-1,_T("CExBitmap::Rotate() = FALSE\n"));
			return FALSE;
		}

		// Delete contents of this bitmap object
		DeleteObject();

		// Attach destination bitmap handle to this bitmap object
		bResult = Attach(bmpDest.Detach());
	}
	catch(CResourceException* /*e*/)
	{
		MYTRACE(0,_T("Resource Exception\n"));
	}

	if( m_bAutoUndo && bResult )
		Backup();

	MYTRACE(-1,_T("CExBitmap::Rotate() = TRUE\n"));
	return bResult;
}

#else

/**
Rotates bitmap attached to this object.
\returns
Nonzero on success; otherwise zero.
\par
Windows NT/ 2000: To get extended error information, call GetLastError.
*/
BOOL CExBitmap::Rotate(
int nRotations/*= 0*/	//!< 0 = +90 degrees, 1 = +180 degrees, 2 = -90 degrees.
)
{
	ASSERT_VALID(this);
	ASSERT(m_hObject != NULL);

	// Safety: Let user figure out what is wrong!
	nRotations %= 3;

	MYTRACE(1,_T("CExBitmap::Rotate(%s Degrees)\n"),
		(nRotations == 0) ? _T("90") : (nRotations == 1) ? _T("180"): _T("270"));

	if( !m_hObject )
	{
		MYTRACE(-1,_T("CExBitmap::Rotate() = FALSE\n"));
		::SetLastError(ERROR_INVALID_HANDLE);
		return FALSE;
	}

	// Flip 180 degrees
	if( nRotations == 1 )
#ifdef _DEBUG
	{
		BOOL bResult = Flip(2);
		MYTRACE(-1,_T("CExBitmap::Rotate() = %s\n"),MYGETBOOLSTR(bResult));
		return bResult;
	}
#else
		return Flip(2);
#endif

	::GdiFlush();

	if( m_bAutoUndo )
		SaveUndo();

	// Create source DIB
	CDibData dibSrc;
	if( !dibSrc.CreateDIB(this) )
	{
		MYTRACE(-1,_T("CExBitmap::Rotate() = FALSE\n"));
		return FALSE;
	}

	int x, y, t;
	int cx = dibSrc.GetWidth();
	int cy = abs(dibSrc.GetHeight());

	BOOL bResult = FALSE;
	try
	{
		// Create rotated destination bitmap bits object
		CExBitmap bmpDest;
		{
			CWorkDC dcDest(NULL, this);
			if( !bmpDest.CreateCompatibleBitmap(&dcDest, cy, cx) )
			{
				MYTRACE(-1,_T("CExBitmap::Rotate() = FALSE\n"));
				return FALSE;
			}
		}
		MYTRACE(0,_T("CreateCompatibleBitmap(dcDest, %d, %d) = TRUE\n"),
			cy, cx);

		CDibData dibDest;
		if( !dibDest.CreateDIB(&bmpDest) )
		{
			MYTRACE(-1,_T("CExBitmap::Rotate() = FALSE\n"));
			return FALSE;
		}

		// Rotate right (90 degrees)
		if( nRotations == 0 )
		{
			for( x = 0; x < cx; ++x )
			{
				t = cy - 1;
				for( y = 0; y < cy; ++y, --t )
					dibDest.CopyPixelValue( t, x, dibSrc, x, y);
			}
		}
		// Rotate left (90 degrees)
		else
		{
			for( y = 0; y < cy; ++y )
			{
				t = cx - 1;
				for( x = 0; x < cx; ++x, --t )
					dibDest.CopyPixelValue(y, t, dibSrc, x, y);
			}
		}

		// Copy destination bits to destination bitmap
		if( !dibDest.SetDIBits(&bmpDest) )
		{
			MYTRACE(-1,_T("CExBitmap::Rotate() = FALSE\n"));
			return FALSE;
		}

		// Delete contents of this bitmap object
		DeleteObject();

		// Attach destination bitmap handle to this bitmap object
		bResult = Attach(bmpDest.Detach());
	}
	catch(CResourceException* /*e*/)
	{
		MYTRACE(0,_T("Resource Exception\n"));
	}

	if( m_bAutoUndo && bResult )
		Backup();

	MYTRACE(-1,_T("CExBitmap::Rotate() = TRUE\n"));
	return bResult;
}

#endif

/////////////////////////////////////////////////////////////////////////////
//

/**
Clears a bitmap or part of bitmap using given color.
\returns
Nonzero on success; otherwise zero.
\remarks
If pRect is NULL then the entire bitmap is cleared.
*/
BOOL CExBitmap::ClearBitmap(
COLORREF	clrFill,			//!< Color to use when clearing bitmap.
CRect*		pRect/* = NULL*/	//!< Pointer to rectangle area to clear.
)
{
	ASSERT_VALID(this);
	ASSERT(m_hObject != NULL);

	BOOL bResult = FALSE;
	BITMAP bmpDest;
	if( GetBitmap(&bmpDest) )
	{
		::GdiFlush();	// See CreateDIBSection()
		CRect rcClear(0, 0, bmpDest.bmWidth, bmpDest.bmHeight);
		if( pRect )
			rcClear = *pRect;
		try
		{
			CWorkDC dcWork(NULL, this);
			dcWork.FillSolidRect(&rcClear, clrFill);
			bResult = TRUE;
		}
		catch(CResourceException* /*e*/)
		{
			MYTRACE(0,_T("Resource Exception\n"));
		}
	}

	return bResult;
}

/////////////////////////////////////////////////////////////////////////////
// Backup related functions

/**
\returns
Nonzero if backup object exists and contains a valid object.
*/
BOOL CExBitmap::IsBackupValid()
{
	ASSERT_VALID(this);

	int nSize = m_BackObjArray.GetSize();
	if( nSize < 1 )
		return FALSE;

	if( nSize < m_nBack )	// Should not happen
		return FALSE;

	CParBitmap* pBack = DYNAMIC_DOWNCAST(CParBitmap, m_BackObjArray[m_nBack]);
	if( !pBack )
		return FALSE;

	// create back up bitmap if needed
	return( pBack->GetSafeHandle() != NULL );
}

/**
Sets which backup image to use when SaveBackup() is called.
\returns
Actual index of backup selected or -1.
*/
int CExBitmap::UseBackup(
int nBack	//!< Index of backup to use.
)
{
	ASSERT_VALID(this);
	MYTRACE(1,_T("CExBitmap::UseBackup(%d)\n"), nBack);

	if( 0 > nBack )
		nBack = 0;

	int nSize = m_BackObjArray.GetSize();
	if( nBack >= nSize )
	{
		// Add new partial bitmap object to backup array.
		try
		{
			nBack = m_BackObjArray.Add(new CParBitmap());
		}
		catch( CMemoryException* /*e*/ )
		{
			MYTRACE(0,_T("m_BackObjArray.Add(new CParBitmap()) : "
				"OUT OR MEMORY\n"));
			return -1;
		}
		MYTRACE(0,_T("m_BackObjArray.Add(%p) = %d\n"), m_BackObjArray[nBack], nBack);
	}
	// Backup array must contain a valid partial bitmap pointer, 
	// at given index, or SaveBackup() and RestoreBackup() will fail.
	else if( !DYNAMIC_DOWNCAST(CParBitmap, m_BackObjArray[nBack]) )
	{
		m_BackObjArray.SetAt(nBack,new CParBitmap());
		MYTRACE(0,_T("m_BackObjArray.SetAt(%d, %p)\n"), nBack, m_BackObjArray[nBack]);
	}

	m_nBack = nBack;
	MYTRACE(-1,_T("CExBitmap::UseBackup() = %d\n"), nBack);
	return m_nBack;
}

/**
Saves a backup copy of the currently attached bitmap.
\returns
Nonzero on success; otherwise zero.
\par
Windows NT/ 2000: To get extended error information, call GetLastError.
\remarks
	Uses full bitmap instead of partial bitmap to reduce
	the number bitmap reallocations.
*/
BOOL CExBitmap::SaveBackup(
CRect&		rcSrc,			//!< Rectangular area to copy to backup bitmap.
COLORREF	clrFill/*=0*/	//!< Color used to fill in unused area.
)
{
	ASSERT_VALID(this);
	ASSERT(m_hObject != NULL);
	MYTRACE(1,_T("CExBitmap::SaveBackup(sRECT(%d, %d, %d, %d))\n"),
		rcSrc.left, rcSrc.top, rcSrc.right, rcSrc.bottom);

	BITMAP bmSrc;
	if( !GetBitmap(&bmSrc) )
	{
		MYTRACE(-1,_T("CExBitmap::SaveBackup() = FALSE\n"));
		return FALSE;
	}

	::GdiFlush();	// See CreateDIBSection()

	// Safety
	if( UseBackup(m_nBack) < 0 )
	{
		MYTRACE(-1,_T("CExBitmap::SaveBackup() = FALSE\n"));
		return FALSE;
	}

	CParBitmap* pBack = DYNAMIC_DOWNCAST(CParBitmap, m_BackObjArray[m_nBack]);
	if( !pBack )
	{
		MYTRACE(-1,_T("CExBitmap::SaveBackup() = FALSE\n"));
		return FALSE;
	}

	// create back up bitmap if needed
	if( !pBack->GetSafeHandle() )
	{
		try
		{
			CWorkDC dcSrc(NULL, this);
			if( !pBack->CreateCompatibleBitmap(
				&dcSrc, bmSrc.bmWidth, bmSrc.bmHeight) )
			{
				MYTRACE(-1,_T("CExBitmap::SaveBackup() = FALSE\n"));
				return FALSE;
			}
		}
		catch(CResourceException* /*e*/)
		{
			MYTRACE(0,_T("Resource Exception\n"));
			return FALSE;
		}
		MYTRACE(0,_T("CreateCompatibleBitmap(dcSrc, %d, %d) = TRUE\n"),
			bmSrc.bmWidth, bmSrc.bmHeight);
	}

	// back up source
	if( pBack->GetSafeHandle() )
	{
		BITMAP bmBack;
		if( !pBack->GetBitmap(&bmBack) )
		{
			MYTRACE(-1,_T("CExBitmap::SaveBackup() = FALSE\n"));
			return FALSE;
		}

		CExBitmap exBmp;
		exBmp.Attach(pBack->Detach());
		if( bmBack.bmHeight != bmSrc.bmHeight
			|| bmBack.bmWidth != bmSrc.bmWidth )
		{
			pBack->SetPosRect(0, 0, 0, 0);
			pBack->SetSrcSize(0, 0);
			if( !exBmp.ExpandBitmap(
				bmSrc.bmWidth  - bmBack.bmWidth,
				bmSrc.bmHeight - bmBack.bmHeight, clrFill) )
			{
				pBack->Attach(exBmp.Detach());
				MYTRACE(-1,_T("CExBitmap::SaveBackup() = FALSE\n"));
				return FALSE;
			}
		}

		// Copy this to backup
		BOOL bResult = exBmp.CopyBitmap(*this, rcSrc, rcSrc);
		if( bResult )
		{
			pBack->SetPosRect(rcSrc);
			pBack->SetSrcSize(GetSize());
		}

		pBack->Attach(exBmp.Detach());
		MYTRACE(-1,_T("CExBitmap::SaveBackup() = %s\n"),MYGETBOOLSTR(bResult));
		return bResult;
	}

	MYTRACE(-1,_T("CExBitmap::SaveBackup() = FALSE\n"));
	return FALSE;
}

/**
Back up the entire bitmap attached to this object.
\returns
Nonzero on success; otherwise zero.
\par
Windows NT/ 2000: To get extended error information, call GetLastError.
*/
BOOL CExBitmap::Backup()
{
	ASSERT_VALID(this);
	ASSERT(m_hObject != NULL);
	MYTRACE(1,_T("CExBitmap::Backup()\n"));

	BITMAP bmSrc;
	if( !GetBitmap(&bmSrc) )
	{
		MYTRACE(-1,_T("CExBitmap::SaveBackup() = FALSE\n"));
		return FALSE;
	}

	CRect rcSrc(0, 0, bmSrc.bmWidth, bmSrc.bmHeight);
#ifdef _DEBUG
	BOOL bResult = SaveBackup(rcSrc);;
	MYTRACE(-1,_T("CExBitmap::SaveBackup() = %s\n"), MYGETBOOLSTR(bResult));
	return bResult;
#else
	return SaveBackup(rcSrc);
#endif
}

/**
Restores part of the attached bitmap from the currently selected backup bitmap.
\returns
Nonzero on success; otherwise zero.
\par
Windows NT/ 2000: To get extended error information, call GetLastError.
*/
BOOL CExBitmap::RestoreBackup(
CRect&	rcDest,					/**< [in/out] Reference to the destination
								     rectangle to restore, contains the
									 restored rectangles coordinates upon
									 return. */
BOOL	bUseDestRect/*=FALSE*/	/**< If nonzero then rcDest is used as the source
								     and destination rectangle. If zero rcDest
									 is retrieved from the backup source. */
)
{
	ASSERT_VALID(this);
	ASSERT(m_hObject != NULL);
	MYTRACE(1,_T("CExBitmap::RestoreBackup(dRECT(%d, %d, %d, %d))\n"),
		rcDest.left, rcDest.top, rcDest.right, rcDest.bottom);

	if( m_nBack < 0 || m_BackObjArray.GetUpperBound() < m_nBack )
	{
		MYTRACE(-1,_T("CExBitmap::RestoreBackup() = FALSE\n"));
		return FALSE;
	}

	CParBitmap* pBack = DYNAMIC_DOWNCAST(CParBitmap, m_BackObjArray[m_nBack]);
	if( pBack && pBack->GetSafeHandle() )
	{
		BOOL bResult = FALSE;
		CExBitmap exBmp;
		exBmp.Attach(pBack->GetSafeHandle());
		if( bUseDestRect )
		{
			bResult = CopyBitmap(exBmp, rcDest, rcDest);
		}
		else
		{
			CRect rcBack = pBack->GetPosRect();
			bResult = CopyBitmap(exBmp,rcBack, rcBack);
			rcDest = rcBack;
		}
		exBmp.Detach();
		MYTRACE(-1,_T("CExBitmap::RestoreBackup() = %s\n"),
			MYGETBOOLSTR(bResult));
		return bResult;
	}
	MYTRACE(-1,_T("CExBitmap::RestoreBackup() = FALSE\n"));
	return FALSE;
}

/**
Destroys all backups currently stored in backup array
*/
void CExBitmap::RemoveBackups()
{
	ASSERT_VALID(this);
	int nSize = m_BackObjArray.GetSize();
	CParBitmap* pBack;
	for( int i = 0; i < nSize; ++i )
	{
		pBack = DYNAMIC_DOWNCAST(CParBitmap, m_BackObjArray[i]);
		if( pBack && pBack->GetSafeHandle() )
		{
			pBack->SetPosRect(0, 0, 0, 0);
			delete pBack;
			m_BackObjArray[i] = NULL;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// Undo/Redo functions

/**
Removes a specified number of items from given object array.
*/
void CExBitmap::RemoveUndoRedoAt(
CObArray&	obArray,		//!< Reference to internal object array.
int			nIndex,			//!< Index of first object to be removed.
int			nCount /* = 1*/	//!< Number of objects to be removed.
)
{
	ASSERT_VALID(this);
	if( 0 <= nIndex && 0 < nCount
		&& nIndex <= obArray.GetUpperBound()
		&& (nIndex+nCount) <= obArray.GetSize() )
	{
		CParBitmap* pbmp;
		for( int i = 0; i < nCount; ++i )
		{
			pbmp = DYNAMIC_DOWNCAST(CParBitmap, obArray[nIndex+i]);
			if( pbmp )
			{
				delete pbmp;
				obArray[nIndex+i] = NULL;
			}
		}

		// Move items down
		obArray.RemoveAt(nIndex, nCount);
	}

	// Restore size (if it has changed)
	obArray.SetSize(m_nUndoRedoSize);
}

/**
Set the sizes of the undo/redo arrays.
*/
void CExBitmap::SetUndoSize(
int nNewSize/*=10*/	//!< The new size of the undo/redo arrays.
)
{
	ASSERT_VALID(this);

	// Remove all if new size <= 0
	if( nNewSize <= 0 )
		nNewSize = 0;

	m_nUndoRedoSize = nNewSize;

	// Move elements down to maintain proper order
	// Note: RemoveUndoRedoAt() insures proper size is maintained
	RemoveUndoRedoAt(m_UndoObjArray, 0, m_nUndo - nNewSize);
	RemoveUndoRedoAt(m_RedoObjArray, 0, m_nRedo - nNewSize);
}

/**
Saves a partial bitmap to the undo or redo array.
\returns
Nonzero on success; otherwise zero.
\par
Windows NT/ 2000: To get extended error information, call GetLastError.
\remarks
If bRedo is nonzero then it copies from attached bitmap to redo array,
otherwise it copies from current backup bitmap to undo array.
*/
BOOL CExBitmap::SaveUndoRedo(
CRect*	prcSrc, /**< Rectangle representing the portion of the bitmap to be
				     copied to the undo or redo array. */
BOOL	bRedo	//!< Redo copy flag.
)
{
	ASSERT_VALID(this);
	ASSERT(m_hObject != NULL);
	MYTRACE(1,_T("CExBitmap::SaveUndoRedo(%s)\n"),
		bRedo ? _T("REDO") : _T("UNDO"));

	::GdiFlush();	// See CreateDIBSection()

	// Create backup if one does not exists
	if( m_BackObjArray.GetSize() <= m_nBack || !IsBackupValid() )
	{
		SIZE size = GetSize();
		CRect rcBack(0, 0, size.cx, size.cy);
		if( !SaveBackup(rcBack) )
		{
			MYTRACE(-1,_T("CExBitmap::SaveUndoRedo() : SaveBackup() = FALSE\n"));
			return FALSE;
		}
	}

	// If saving redo source is this else source is backup
	CBitmap* pSrc;
	if( bRedo )
	{
		pSrc = DYNAMIC_DOWNCAST(CBitmap, this);
		MYTRACE(0,_T("pSrc = THIS;\n"));
	}
	else
	{
		pSrc = DYNAMIC_DOWNCAST(CBitmap, m_BackObjArray[m_nBack]);
		MYTRACE(0,_T("pSrc = BACKUP[%d];\n"), m_nBack);
	}

	ASSERT_VALID(pSrc);
	ASSERT(pSrc->GetSafeHandle() != NULL);

	if( !pSrc )
	{
		MYTRACE(-1,_T("CExBitmap::SaveUndoRedo() = FALSE\n"));
		return FALSE;
	}

	// Get source bitmap size
	BITMAP bmSrc;
	if( !pSrc->GetBitmap(&bmSrc) )
	{
		MYTRACE(-1,_T("CExBitmap::SaveUndoRedo() : GetBitmap() = FALSE\n"));
		return FALSE;
	}

	CRect rcSrc;
	if( !prcSrc )
	{
		rcSrc.SetRect(0, 0, bmSrc.bmWidth, bmSrc.bmHeight);
	}
	else
	{
		if( 0 >= prcSrc->Width() || 0 >= prcSrc->Height() )
		{
			MYTRACE(-1,_T("CExBitmap::SaveUndoRedo() : Width(%d) or Height(%d) <= 0 = FALSE\n"),
				prcSrc->Width(), prcSrc->Height());
			return FALSE;
		}

		rcSrc = *prcSrc;
		// Image rotated ???
		if( rcSrc.Height() > bmSrc.bmHeight || rcSrc.Width() > bmSrc.bmWidth )
		{
			rcSrc.SetRect(0, 0, bmSrc.bmWidth, bmSrc.bmHeight);
			MYTRACE(0,_T("WAS IMAGE ROTATED EARLIER?\n"));
		}
		else
		{
			// Do not accede actual source size
			/**/
			if( rcSrc.top < 0 )
				rcSrc.top = 0;
			if( rcSrc.left < 0 )
				rcSrc.left = 0;
			if( rcSrc.bottom > bmSrc.bmHeight )
				rcSrc.bottom = bmSrc.bmHeight;
			if( rcSrc.right > bmSrc.bmWidth )
				rcSrc.right = bmSrc.bmWidth;
			/**/
		}
	}

	//
	CObArray& obArray = bRedo ? m_RedoObjArray : m_UndoObjArray;
	int& nCount = bRedo ? m_nRedo : m_nUndo;

	// Make sure object array is the correct size
	obArray.SetSize(m_nUndoRedoSize);

	// Move array elements down if maximumn size exceeded
	if( obArray.GetSize() <= nCount )
	{
		RemoveUndoRedoAt(obArray, 0, 1);
		--nCount;
	}

	// Allocate new partial bitmap
	CParBitmap* pbmpDest;
	try
	{
		pbmpDest = new CParBitmap(); // can throw CMemoryException
	}
	catch( CMemoryException* /*e*/ )
	{
		MYTRACE(0,_T("m_BackObjArray.Add(new CParBitmap()) : "
			"OUT OR MEMORY\n"));
		return FALSE;
	}
	BOOL bResult = FALSE;

	// Create a screen compatible bitmap
	try
	{
		CWorkDC dcSrc(NULL, pSrc);
		if( pbmpDest->CreateCompatibleBitmap(
			&dcSrc, rcSrc.Width(), rcSrc.Height()) )
		{
			MYTRACE(0,
				_T("pbmpDest->CreateCompatibleBitmap(dcSrc, %d, %d) = TRUE\n"),
				rcSrc.Width(), rcSrc.Height());

			CRect rcDest(0, 0, rcSrc.Width(), rcSrc.Height());
			CWorkDC dcDest(NULL, pbmpDest);
			bResult = dcDest.BitBlt(
				0, 0, rcSrc.Width(), rcSrc.Height(),
				&dcSrc, rcSrc.left, rcSrc.top, SRCCOPY);

			MYTRACE(0,_T("dcDest.BitBlt(%d, %d, %d, %d,"
				         " dcSrc, %d, %d, [%d, %d,] SRCCOPY\n"),
				0, 0, rcSrc.Width(), rcSrc.Height(),
				rcSrc.left, rcSrc.top, rcSrc.Width(), rcSrc.Height());

			if( bResult )
			{
				pbmpDest->SetPosRect(rcSrc);
				pbmpDest->SetSrcSize(bmSrc.bmWidth, bmSrc.bmHeight);
				obArray[nCount++] = pbmpDest;
				if( !bRedo )
					++m_nUndoTotal;
			}
		}
	}
	catch(CResourceException* /*e*/)
	{
		MYTRACE(0,_T("Resource Exception\n"));
		bResult = FALSE;
	}

	MYTRACE(-1,_T("CExBitmap::SaveUndoRedo() = %s\n"),MYGETBOOLSTR(bResult));
	return bResult;
}

/**
Restores part of the bitmap stored in the undo or redo array.
\returns
Nonzero on success; otherwise zero.
\par
Windows NT/ 2000: To get extended error information, call GetLastError.
*/
BOOL CExBitmap::RestoreUndoRedo(
BOOL		bRedo,			/**< If nonzero then restore from redo array,
							     otherwise restore from undo array. */
COLORREF	clrFill/*=0*/	/**< Color used to fill unused prortion of bitmap,
							     if any */
)
{
	ASSERT_VALID(this);
	ASSERT(m_hObject != NULL);
	MYTRACE(1,_T("CExBitmap::RestoreUndoRedo(%s)\n"),
		bRedo ? _T("REDO") : _T("UNDO"));

	CObArray& obArray = bRedo ? m_RedoObjArray : m_UndoObjArray;
	int& nCount = bRedo ? m_nRedo : m_nUndo;

	// Make sure count is within range
	if( nCount <= 0 || obArray.GetSize() < nCount )
	{
		MYTRACE(-1,_T("CExBitmap::RestoreUndoRedo() = FALSE\n"));
		return FALSE;
	}

	// Get pointer to partial bitmap
	CParBitmap* pbmpSrc = DYNAMIC_DOWNCAST(CParBitmap, obArray[nCount - 1]);
	if( !pbmpSrc )
	{
		MYTRACE(-1,_T("CExBitmap::RestoreUndoRedo() = FALSE\n"));
		return FALSE;
	}
#ifdef _DEBUG
	if( bRedo )
		MYTRACE(0,_T("pbmpSrc = REDO[%d];\n"), m_nRedo-1);
	else
		MYTRACE(0,_T("pbmpSrc = UNDO[%d];\n"), m_nUndo-1);
#endif

	// Get position where partial bitmap came from
	CRect rcPos = pbmpSrc->GetPosRect();

	// Save to undo or redo
	if( bRedo )
	{
		if( SaveUndo(&rcPos) )
		{
			// Restore origanal source size
			/*CParBitmap* pUndo = DYNAMIC_DOWNCAST(
				CParBitmap, m_UndoObjArray[m_nUndo - 1]);
			if( pUndo )
				pUndo->SetSrcSize(pbmpSrc->GetSrcSize());*/
		}
	}
	else
	{
		if( SaveRedo(&rcPos) )
		{
			// Restore origanal source size
			/*CParBitmap* pRedo = DYNAMIC_DOWNCAST(
				CParBitmap, m_RedoObjArray[m_nRedo - 1]);
			if( pRedo )
				pRedo->SetSrcSize(pbmpSrc->GetSrcSize());*/
		}
	}

	// Expand this bitmap if needed
	{
		SIZE sizeThis = GetSize();
		SIZE sizeSrc = pbmpSrc->GetSrcSize();
		MYTRACE(0,_T("This Size(%d, %d) : Source Size(%d, %d)\n"),
			sizeThis.cx, sizeThis.cy, sizeSrc.cx, sizeSrc.cy);
		if( sizeThis.cx != sizeSrc.cx || sizeThis.cy != sizeSrc.cy )
		{
			if( !ExpandBitmap(
				sizeSrc.cx - sizeThis.cx,
				sizeSrc.cy - sizeThis.cy, clrFill) )
			{
				MYTRACE(-1,_T("CExBitmap::RestoreUndoRedo() = FALSE\n"));
				return FALSE;
			}
		}
	}

	// Copy partial bitmap to this bitmap
	if( CopyBitmap(*pbmpSrc, FALSE, rcPos.left, rcPos.top, 0, 0) )
	{

		obArray[--nCount] = NULL;
		delete pbmpSrc;
		if( !bRedo )
			--m_nUndoTotal;

		// Update background bit map to reflect change
		SaveBackup(rcPos, clrFill);
		MYTRACE(-1,_T("CExBitmap::RestoreUndoRedo() = TRUE\n"));
		return TRUE;
	}

	MYTRACE(-1,_T("CExBitmap::RestoreUndoRedo() = FALSE\n"));
	return FALSE;
}

/**
Removes the last undo item from undo array.
*/
void CExBitmap::RemoveUndo()
{
	ASSERT_VALID(this);
	RemoveUndoRedoAt(m_UndoObjArray, 0, m_nUndo);
	m_nUndo = 0;
}

/**
Removes the last redo item from redo array.
*/
void CExBitmap::RemoveRedo()
{
	ASSERT_VALID(this);
	RemoveUndoRedoAt(m_RedoObjArray, 0, m_nRedo);
	m_nRedo = 0;
}

/////////////////////////////////////////////////////////////////////////////
//

/**
Draws this bitmap on to given DC.
\returns
The total size of the image drawn.
*/
SIZE CExBitmap::Draw(
CDC* pDC,		//!< Pointer to CDC.
int  x,			//!< x-coordinate.
int  y,			//!< y-coordinate.
int  nMag/*=1*/ //!< magnification.
)
{
	ASSERT_VALID(this);
	//ASSERT(m_hObject != NULL);
	SIZE size = { 0, 0 };
	if( pDC && GetSafeHandle() )
	{
		BITMAP bmBack;
		try
		{
			CWorkDC dcTemp(pDC, this);
			if( GetBitmap(&bmBack) )
			{
				size.cx = bmBack.bmWidth;
				size.cy = bmBack.bmHeight;
				if( nMag < 2 )
				{
					pDC->BitBlt(
						x, y, bmBack.bmWidth, bmBack.bmHeight,
						&dcTemp, 0, 0, SRCCOPY);
				}
				else
				{
					size.cx *= nMag;
					size.cy *= nMag;
					pDC->StretchBlt(
						x, y, size.cx, size.cy,
						&dcTemp, 0, 0, bmBack.bmWidth, bmBack.bmHeight, SRCCOPY);
				}
			}
		}
		catch(CResourceException* /*e*/)
		{
			MYTRACE(0,_T("Resource Exception\n"));
		}
	}
	return size;
}

/**
Get size of attached bitmap.
\returns
	Size of attached bitmap, if any.
*/
SIZE CExBitmap::GetSize()
{
	ASSERT_VALID(this);
	ASSERT(m_hObject != NULL);
	SIZE size = { 0, 0 };
	BITMAP bm;
	if( GetSafeHandle() && GetBitmap(&bm) )
	{
		size.cx = bm.bmWidth;
		size.cy = bm.bmHeight;
	}
	return size;
}

/////////////////////////////////////////////////////////////////////////////
//

#ifdef TESTING_UNDOREDO
/**
Draws all images in backup array, from top to bottom. This is only available
if code module is compiled with TESTING_UNDOREDO defined. The purpose of this
function is to add in testing of this module.
\returns
The total size of the area to which the images were drawn.
\remarks
The rectangular area drawn to is excluded from furthur drawing.
*/
SIZE CExBitmap::DrawBackup(
CDC* pDC,	//!< Pointer to CDC.
int  x,		//!< x-coordinate.
int  y		//!< y-coordinate.
)
{
	SIZE size = { 0, 0 };
	if( pDC )
	{
		BITMAP bmBack;
		try
		{
			CWorkDC dcTemp(pDC);
			int nSize = m_BackObjArray.GetSize();
			CParBitmap* pBack;
			for( int i = 0; i < nSize; ++i )
			{
				pBack = DYNAMIC_DOWNCAST(CParBitmap, m_BackObjArray[i]);
				if( pBack && pBack->GetSafeHandle() && pBack->GetBitmap(&bmBack) )
				{
					dcTemp.SelectObject(pBack);
					pDC->BitBlt(
						x, y, bmBack.bmWidth, bmBack.bmHeight,
						&dcTemp, 0, 0, SRCCOPY);

					// Exclude bitmap rectangle from furthur drawing
					pDC->ExcludeClipRect(x, y, x + bmBack.bmWidth, y + bmBack.bmHeight);

					bmBack.bmHeight +=  5;
					if( size.cx < bmBack.bmWidth )
						size.cx = bmBack.bmWidth;
					size.cy += bmBack.bmHeight;
					y += bmBack.bmHeight;
				}
			}
		}
		catch(CResourceException* /*e*/)
		{
			MYTRACE(0,_T("Resource Exception\n"));
		}
	}
	return size;
}

/**
Draws all images in Undo & Redo arrays, from top to bottom. This is only available
if code module is compiled with TESTING_UNDOREDO defined. The purpose of this
function is to add in testing of this module.
\returns
The total size of the area to which the images were drawn.
\remarks
The rectangular area of each bitmap drawn to is excluded from furthur drawing.
*/
SIZE CExBitmap::DrawUndoRedo(
CDC*	 pDC,		//!< Pointer to CDC.
int		 x,			//!< x-coordinate.
int		 y,			//!< y-coordinate.
COLORREF clrBack,	//!< Color used to fill non-image area.
BOOL	 bRedo)		//!< Nonzero if drawing Redo array, else Undo array.
{
	SIZE size = { 0, 0 };
	try
	{
		CWorkDC dcTemp(pDC);
		COLORREF clrOldBack = pDC->GetBkColor();
		BITMAP bmTemp;
		SIZE sizeSrc;

		CObArray& obArray = bRedo ? m_RedoObjArray : m_UndoObjArray;
		int& nCount = bRedo ? m_nRedo : m_nUndo;

		for( int i = 0; i < nCount; ++i )
		{
			CParBitmap* pSrc = DYNAMIC_DOWNCAST(CParBitmap, obArray[i]);
			if( !pSrc )
				break;

			if( !pSrc->GetBitmap(&bmTemp) )
				break;

			sizeSrc = pSrc->GetSrcSize();

			// Draw partial bitmap
			dcTemp.SelectObject(pSrc);
			pDC->BitBlt(
				x + pSrc->m_PosRect.left,
				y + pSrc->m_PosRect.top + size.cy,
				pSrc->m_PosRect.Width(), pSrc->m_PosRect.Height(),
				&dcTemp, 0, 0, SRCCOPY);

			// Exclude partial bitmap rectangle from furthur drawing
			pDC->ExcludeClipRect(
				x + pSrc->m_PosRect.left,
				y + pSrc->m_PosRect.top + size.cy,
				x + pSrc->m_PosRect.left + pSrc->m_PosRect.Width(),
				y + pSrc->m_PosRect.top + size.cy + pSrc->m_PosRect.Height());

			// Draw background rectangle
			pDC->FillSolidRect(
				x, y + size.cy, sizeSrc.cx, sizeSrc.cy, clrBack);

			// Exclude background rectangle from furthur drawing
			pDC->ExcludeClipRect(
				x,
				y + size.cy,
				x + sizeSrc.cx,
				y + size.cy + sizeSrc.cy);

			if( size.cx < sizeSrc.cx )
				size.cx = sizeSrc.cx;

			size.cy += sizeSrc.cy + 5;
		}
		pDC->SetBkColor(clrOldBack);
	}
	catch(CResourceException* /*e*/)
	{
		MYTRACE(0,_T("Resource Exception\n"));
	}
	return size;
}

/**
Draws all images in Undo arrays from top to bottom. This is only available
if code module is compiled with TESTING_UNDOREDO defined. The purpose of this
function is to add in testing of this module.
\returns
The total size of the area to which the images were drawn.
\remarks
The rectangular area of each bitmap drawn to is excluded from furthur drawing.
*/
SIZE CExBitmap::DrawUndo(
CDC*	 pDC,		//!< Pointer to CDC.
int		 x,			//!< x-coordinate.
int		 y,			//!< y-coordinate.
COLORREF clrBack	//!< Color used to fill non-image area.
)
{
	return DrawUndoRedo(pDC, x, y, clrBack, FALSE);
}

/**
Draws all images in Redo arrays from top to bottom. This is only available
if code module is compiled with TESTING_UNDOREDO defined. The purpose of this
function is to add in testing of this module.
\returns
The total size of the area to which the images were drawn.
\remarks
The rectangular area of each bitmap drawn to is excluded from furthur drawing.
*/
SIZE CExBitmap::DrawRedo(
CDC*	 pDC,		//!< Pointer to CDC.
int		 x,			//!< x-coordinate.
int		 y,			//!< y-coordinate.
COLORREF clrBack	//!< Color used to fill non-image area.
)
{
	return DrawUndoRedo(pDC, x, y, clrBack, TRUE);
}
#endif

//////////////////////////////////////////////////////////////////////////////////////////
//  Transparent Blit for Windows 3.x and above
//////////////////////////////////////////////////////////////////////////////////////////

// Used to support TransparentBlt()
// on WinNT 4.0, Win2000, Win98, WinMe.
// Win31 and Win95 uses internal method.
typedef BOOL (WINAPI* LPTRANSPARENTBLT)(
	HDC, int, int, int, int, HDC, int, int, int, int, UINT);
class CTransBlit
{
public:
	HMODULE m_hMSIModule;
	LPTRANSPARENTBLT m_pTransBlt;
	CTransBlit();
	~CTransBlit();
} CExBitmap_TransBlt;

CTransBlit::CTransBlit() : m_hMSIModule(NULL), m_pTransBlt(NULL)
{
	MYTRACE(1,_T("CTransBlit::CTransBlit()\n"));
	// Note: This works on WindowsMe and is supposed to work on
	// Win98 and above.
	m_hMSIModule = LoadLibrary(_T("Msimg32.dll")); 
	if( m_hMSIModule ) 
	{
		m_pTransBlt = (LPTRANSPARENTBLT)
			GetProcAddress(m_hMSIModule, "TransparentBlt"); 
	}

	MYTRACE(0,_T("LoadLibrary(_T(\"Msimg32.dll\")) = %p\n"), m_hMSIModule);
	MYTRACE(-1,_T("CTransBlit::CTransBlit()\n"));
}

CTransBlit::~CTransBlit()
{
	MYTRACE(1,_T("CTransBlit::~CTransBlit()\n"));
	if( m_hMSIModule ) 
	{
#ifdef _DEBUG
		BOOL bResult = FreeLibrary(m_hMSIModule);
		MYTRACE(0,_T("FreeLibrary(%p) = %s\n"),
			m_hMSIModule, MYGETBOOLSTR(bResult));
#else
		FreeLibrary(m_hMSIModule);
#endif
	}
	m_hMSIModule = NULL;
	m_pTransBlt  = NULL;
	MYTRACE(-1,_T("CTransBlit::~CTransBlit()\n"));
}

/**
Preforms a transparent bitblit to destination device.
\returns
Nonzero on success; otherwise zero.
\par
Windows NT/ 2000: To get extended error information, call GetLastError.
\remarks
Supports WinNT 4.0, Win2000, Win98, WinMe, Win31, and Win95.
*/
BOOL CExBitmap::TransBlt(
  CDC* pdcDest,    //!< pointer to destination CDC.
  int xDest,       //!< x-coord of destination upper-left corner.
  int yDest,       //!< y-coord of destination upper-left corner.
  int nWidth,      //!< width of destination rectangle.
  int hHeight,     //!< height of destination rectangle.
  COLORREF crTrans //!< color to make transparent.
)
{
	ASSERT_VALID(this);
	ASSERT(m_hObject != NULL);
	ASSERT_VALID(pdcDest);

	// Get Bitmap Information
	BITMAP bmSrc;
	if( !GetBitmap(&bmSrc) )
		return FALSE;

	::GdiFlush();	// See CreateDIBSection()

	// Create temporary DC and Select Bitmap into it
	try
	{
		CWorkDC dcSrc(pdcDest, this);

		// Win 98 and above and WinNT 4.0 and above
		if( CExBitmap_TransBlt.m_pTransBlt ) 
		{ 
			return (CExBitmap_TransBlt.m_pTransBlt)(
				pdcDest->GetSafeHdc(), xDest, yDest,
				nWidth, hHeight,
				dcSrc.GetSafeHdc(), 0, 0,
				bmSrc.bmWidth, bmSrc.bmHeight,
				crTrans);
		}

		// Win 3.x and above
		// Note: This technique may not necessarily work on printer devices.

		// Create temporary Bitmaps
		CBitmap bmpMask;
		if( !bmpMask.CreateBitmap(
			bmSrc.bmWidth, bmSrc.bmHeight, 1, 1, NULL) )
		{
			return FALSE;
		}

		CBitmap bmpTemp;
		if( !bmpTemp.CreateCompatibleBitmap(
			pdcDest, bmSrc.bmWidth, bmSrc.bmHeight) )
		{
			return FALSE;
		}

		// Create temporary DCs and select bitmaps into them
		CWorkDC dcTemp(pdcDest, &bmpTemp);
		CWorkDC dcMask(pdcDest, &bmpMask);

		// Use temporary DC to reduce flicker
		if( dcTemp.BitBlt(0, 0, bmSrc.bmWidth, bmSrc.bmHeight,
			pdcDest, xDest, yDest, SRCCOPY) )
		{
			// If we made it here there should be no more possible problems

			// Create mask using crTransparent
			dcSrc.SetBkColor(crTrans);
			dcMask.BitBlt(0, 0, bmSrc.bmWidth, bmSrc.bmHeight,
				&dcSrc, 0,0, SRCCOPY); 

			// Draw bitmap using mask
			dcTemp.BitBlt(0, 0, bmSrc.bmWidth, bmSrc.bmHeight,
				&dcSrc, 0, 0, SRCINVERT); 
			dcTemp.BitBlt(0, 0, bmSrc.bmWidth, bmSrc.bmHeight,
				&dcMask, 0, 0, SRCAND); 
			dcTemp.BitBlt(0, 0, bmSrc.bmWidth, bmSrc.bmHeight,
				&dcSrc, 0, 0, SRCINVERT); 

			// Copy Temporary DC to Destination DC
			pdcDest->BitBlt(xDest, yDest, nWidth, hHeight,
				&dcTemp, 0, 0, SRCCOPY); 
		}
	}
	catch(CResourceException* /*e*/)
	{
		MYTRACE(0,_T("Resource Exception\n"));
		return FALSE;
	}
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////

/**
Save bitmap to disk.
\returns
Nonzero on success; otherwise zero.
\par
Windows NT/ 2000: To get extended error information, call GetLastError.
\remarks
The bCompress flag only applies to 4 and 8 bits-per-pixel images.
The bOptimize flag only applies if conversion from higher to lower
number of bits-per-pixel, recomended for palettized bitmaps.
*/
BOOL CExBitmap::SaveDIB(
LPCTSTR lpszPathName,		//!< Null teminated string containing file path.
BOOL bCompress/* = FALSE*/,	//!< Flag indicating if image is to be compressed.
WORD wBitsPerPixel/* = 0*/,	//!< Color depth at which to save image (conversion).
BOOL bOptimize/* = FALSE*/	//!< Flag indicating color pallete optimization.
)
{
	ASSERT_VALID(this);
	ASSERT(m_hObject != NULL);
	ASSERT(lpszPathName != NULL);
	MYTRACE(1,_T("CExBitmap::SaveDIB(%s, %s, %d, %s)\n"), lpszPathName,
		MYGETBOOLSTR(bCompress), wBitsPerPixel, MYGETBOOLSTR(bOptimize));

	if( !m_hObject )
	{
		MYTRACE(-1,_T("CExBitmap::SaveDIB() = FALSE\n"));
		return FALSE;
	}

	if( !lpszPathName )
	{
		MYTRACE(-1,_T("CExBitmap::SaveDIB() = FALSE\n"));
		::SetLastError(ERROR_INVALID_PARAMETER);
		return FALSE;
	}

	BOOL bResult = FALSE;
	CDibData* pConvDibData = NULL;
	HBITMAP hBitmap = NULL;
	CDibData dibData;

	try
	{
		BITMAP bm;
		if( !GetBitmap(&bm) )
		{
			MYTRACE(-1,_T("CExBitmap::SaveDIB() = FALSE\n"));
			return FALSE;
		}

		// Save normaly ?
		if( wBitsPerPixel < 1 || wBitsPerPixel == (bm.bmBitsPixel * bm.bmPlanes) )
		{
			bResult = dibData.SaveDIB(lpszPathName, this, bCompress);
		}
		// Else create uncompressed DIB
		else if( dibData.CreateDIB(this) )
		{
			// Try to convert DIB
			pConvDibData = dibData.GetConvertedDIB(wBitsPerPixel, bOptimize);
			if( !pConvDibData )
			{
				bResult = dibData.SaveDIB(lpszPathName, NULL, bCompress);
			}
			else
			{
				bResult = pConvDibData->SaveDIB(lpszPathName, NULL, bCompress);
				delete pConvDibData;
				pConvDibData = NULL;

				if( bResult )
				{
					// I do not like this method !
					// Load converted DIB
					MYTRACE(0,_T("Reloading converted DIB from file\n"));
					dibData.DeleteObject();
					hBitmap = dibData.LoadDIB(lpszPathName);
					::DeleteObject(Detach());
					Attach(hBitmap);
				}
			}
		}
	}
	catch( CFileException* /*e*/)
	{
		MYTRACE(0,_T("File Exception\n"));
		bResult = FALSE;
	}
	catch( CMemoryException* /*e*/ )
	{
		MYTRACE(0,_T("Memory Exception\n"));
		bResult = FALSE;
	}
	catch( CResourceException* /*e*/ )
	{
		MYTRACE(0,_T("Resource Exception\n"));
		bResult = FALSE;
	}

	if( !bResult )
	{
		if( hBitmap )
			::DeleteObject(HGDIOBJ(hBitmap));
		delete pConvDibData;
	}

	MYTRACE(-1,_T("CExBitmap::SaveDIB() = %s\n"), MYGETBOOLSTR(bResult));
	return bResult;
}

/**
Loads bitmap from disk.
\returns
Nonzero on success; otherwise zero.
\par
Windows NT/ 2000: To get extended error information, call GetLastError.
*/
BOOL CExBitmap::LoadDIB(
LPCTSTR lpszPathName	//!< Null teminated string containing file path.
)
{
	ASSERT_VALID(this);
	ASSERT(m_hObject == NULL);
	ASSERT(lpszPathName != NULL);
	MYTRACE(1,_T("CExBitmap::LoadDIB()\n"));

	if( m_hObject )
	{
		MYTRACE(-1,_T("CExBitmap::LoadDIB() = FALSE\n"));
		return FALSE;
	}

	if( !lpszPathName )
	{
		MYTRACE(-1,_T("CExBitmap::LoadDIB() = FALSE\n"));
		::SetLastError(ERROR_INVALID_PARAMETER);
		return FALSE;
	}

	CDibData dibData;
	HBITMAP hImage =  dibData.LoadDIB(lpszPathName);
	if( hImage )
		Attach(hImage);

	MYTRACE(-1,_T("CExBitmap::LoadDIB() = %s\n"),
		MYGETBOOLSTR(hImage != NULL));
	return( hImage != NULL );
}

/////////////////////////////////////////////////////////////////////////////
