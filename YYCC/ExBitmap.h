// ExBitmap.h
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

#ifndef __EXBITMAP_H__
#define __EXBITMAP_H__

#include "CDibData.h"

/////////////////////////////////////////////////////////////////////////////
// class CExBitmap

class CExBitmap: public CBitmap
{
	DECLARE_DYNAMIC(CExBitmap)

	// Members variables used to support Undos, Redos, and Backups.
	CObArray m_UndoObjArray;	//!< Undo object array.
	CObArray m_RedoObjArray;	//!< Redo object array.
	CObArray m_BackObjArray;	//!< Backup object array.
	int m_nUndoRedoSize;		//!< Size of both Undo and Redo arrays (default: 10).
	int m_nUndo;				//!< Number of objects stored in Undo array.
	int m_nRedo;				//!< Number of objects stored in Redo array.
	int m_nBack;				//!< Active Backup index.
	int m_nUndoTotal;			//!< Total number Undos needed before original is restored.
	BOOL m_bAutoUndo;			//!< Automatic Undo and Backup saving for Flip() and Rotate().

private:
	// Private helper members
	void RemoveUndoRedoAt(CObArray& obArray, int nIndex, int nCount = 1);
	BOOL SaveUndoRedo(CRect* prcSrc,BOOL bRedo);
	BOOL RestoreUndoRedo(BOOL bRedo,COLORREF clrFill=0);
	BOOL SaveRedo(CRect* prcSrc = NULL);
#ifdef TESTING_UNDOREDO
	SIZE DrawUndoRedo(CDC* pDC, int x, int y,COLORREF clrBack,BOOL bRedo);
#endif
	BOOL SaveBackup(CRect& rcSrc,COLORREF clrFill=0);

public:
	// Constructor(s)/Destuctor(s)
	CExBitmap();
	~CExBitmap();

	void DeleteAllObjects();

public:

	// Information
	SIZE GetSize();

	// Bitamp drawing
	SIZE Draw(CDC* pDC, int x, int y, int  nMag=1);
	BOOL CExBitmap::TransBlt(
	  CDC* pdcDest, int xDest, int yDest,
	  int nWidth, int hHeight, COLORREF crTrans);
#ifdef TESTING_UNDOREDO // Used to help with debugging
	SIZE DrawBackup(CDC* pDC, int x, int y);
	SIZE DrawUndo(CDC* pDC, int x, int y,COLORREF clrBack);
	SIZE DrawRedo(CDC* pDC, int x, int y,COLORREF clrBack);
#endif

	// Bitmap manipulation
	BOOL CopyBitmap(
		CBitmap& bmpSrc,BOOL bStretch=FALSE,
		int xDest=0,int yDest=0,
		int xSrc=0,int ySrc=0);
	BOOL CopyBitmap(
		CBitmap* bmpSrc,BOOL bStretch=FALSE,
		int xDest=0,int yDest=0,
		int xSrc=0,int ySrc=0);

	BOOL CopyBitmap(
		CBitmap& bmpSrc,CRect &rcSrc,CRect& rcDest);
	BOOL CopyBitmap(
		CBitmap* bmpSrc,CRect &rcSrc,CRect& rcDest);

	virtual BOOL ExpandBitmap(int dx,int dy,COLORREF clrFill=0);
	BOOL ClearBitmap(COLORREF clrFill,CRect* pRect = NULL);

		// { 0,1,2 -> Horizontal,Vertical,Horizontal + Vertical }
	BOOL Flip(int nFlip = 0);
	virtual BOOL FlipHorizontal();
	virtual BOOL FlipVertical();
		 // { 0,1,2 -> 90,180,270 } clockwise rotation
	virtual BOOL Rotate(int nRotations = 0);

	// Backup members
	int UseBackup(int nBack);
	BOOL Backup();
	BOOL RestoreBackup(CRect& rcDest,BOOL bUseDestRect=FALSE);
	void RemoveBackups();
	int GetBackupSize();
	BOOL IsBackupValid();

	// Undo/Redo members
	void SetUndoSize(int nNewSize=10);
	BOOL SaveUndo(CRect* prcSrc = NULL);
	BOOL Undo(COLORREF clrFill=0);
	BOOL Redo(COLORREF clrFill=0);
	void RemoveUndo();
	void RemoveRedo();
	int GetUndoSize() const;
	int GetRedoSize() const;
	BOOL IsModified() const;
	BOOL EnableAutoUndo(BOOL bEnable/* = TRUE*/);

	// Saving/Loading image
	BOOL SaveDIB(LPCTSTR lpszPathName, BOOL bCompress = FALSE,
		WORD wBitsPerPixel = 0, BOOL bUseOptimalPalette = FALSE);
	BOOL LoadDIB(LPCTSTR lpszPathName);
};

/**
  Enable automatic undo saving for Rotate() and Flip().
\returns Previous value.
*/
inline BOOL CExBitmap::EnableAutoUndo(BOOL bEnable = TRUE)
{
	ASSERT_VALID(this);
	BOOL bFlag = m_bAutoUndo; 
	m_bAutoUndo = bEnable;
	return( bFlag );
}

/** \returns Actual size of backup object array */
inline int CExBitmap::GetBackupSize()
{ ASSERT_VALID(this); return m_BackObjArray.GetSize(); }

/** \returns Number of items stored in undo object array */
inline int CExBitmap::GetUndoSize() const
{ ASSERT_VALID(this); return m_nUndo; }
/** \returns Number of items stored in redo object array */
inline int CExBitmap::GetRedoSize() const
{ ASSERT_VALID(this); return m_nRedo; }
/**
\returns Nonzero if bitmap has been modified.
\remarks
Test for the total number of undos needed to restore bitmap to original state.
*/
inline BOOL CExBitmap::IsModified() const
{ ASSERT_VALID(this); return( m_nUndoTotal > 0 ); }

/**
Saves a partial bitmap to the undo array.
\return
Nonzero on success; otherwise zero.
*/
inline BOOL CExBitmap::SaveUndo(CRect* prcSrc/* = NULL*/)
{ return SaveUndoRedo(prcSrc,FALSE); }
/**
Saves a partial bitmap to the redo array.
\return
Nonzero on success; otherwise zero.
*/
inline BOOL CExBitmap::SaveRedo(CRect* prcSrc/* = NULL*/)
{ return SaveUndoRedo(prcSrc,TRUE); }

/**
Restores part of the bitmap stored in the undo array.
\return
Nonzero on success; otherwise zero.
*/
inline BOOL CExBitmap::Undo(COLORREF clrFill/*=0*/)
{ return RestoreUndoRedo(FALSE,clrFill); }
/**
Restores part of the bitmap stored in the redo array.
\return
Nonzero on success; otherwise zero.
*/
inline BOOL CExBitmap::Redo(COLORREF clrFill/*=0*/)
{ return RestoreUndoRedo(TRUE,clrFill); }

/**
Copies a source bitmap to this bitmap object.
\return
Nonzero on success; otherwise zero.
*/
inline BOOL CExBitmap::CopyBitmap(
	CBitmap* bmpSrc,BOOL bStretch/*=FALSE*/,
	int xDest/*=0*/,int yDest/*=0*/,
	int xSrc/*=0*/,int ySrc/*=0*/)
{
	ASSERT_VALID(bmpSrc);
	return CopyBitmap(*bmpSrc,bStretch,xDest,yDest,xSrc,ySrc);
}

/**
Copies a source bitmap to this bitmap object.
\return
Nonzero on success; otherwise zero.
*/
inline BOOL CExBitmap::CopyBitmap(
	CBitmap* bmpSrc,CRect &rcSrc,CRect& rcDest)
{
	ASSERT_VALID(bmpSrc);
	return CopyBitmap(*bmpSrc,rcSrc,rcDest);
}

/**
Flips bitmap attached to this object, horizontaly.
\return
Nonzero on success; otherwise zero.
*/
inline BOOL CExBitmap::FlipHorizontal() { return Flip(); }
/**
Flips bitmap attached to this object, verticaly.
\return
Nonzero on success; otherwise zero.
*/
inline BOOL CExBitmap::FlipVertical()   { return Flip(1); }

#endif // __EXBITMAP_H__
