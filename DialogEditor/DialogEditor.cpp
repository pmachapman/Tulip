// CDialogEditor.cpp : implementation file
//

#include "stdafx.h"
#include "StdGrfx.h"
#include "DialogEditor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogEditor

CDialogEditor::CDialogEditor()
/* ============================================================
	Function :		CDialogEditor::CFlowchartEditor
	Description :	constructor

	Return :		void
	Parameters :	none

	Usage :

   ============================================================*/
{
	SetBackgroundColor(::GetSysColor(COLOR_3DFACE));
	ShowGrid(FALSE);
	SetSnapToGrid(TRUE);
	SetRestraints(RESTRAINT_VIRTUAL);
}

CDialogEditor::~CDialogEditor()
/* ============================================================
	Function :		CDialogEditor::~CFlowchartEditor
	Description :	destructor

	Return :		void
	Parameters :	none

	Usage :

   ============================================================*/
{
}

void CDialogEditor::DrawBackground(CDC* dc, CRect rect, double /*zoom*/) const
{
	GetBackgroundColor(); // Called to ensure a correct value is saved
	CStdGrfx::drawframed3dBox(dc, rect);
}

void CDialogEditor::ExportEMF(const CString& filename, const bool noDialog)
/* ============================================================
	Function :		CDialogEditor::ExportEMF
	Description :	Exports the current diagram to an EMF-file.
	Access :

	Return :		void
	Parameters :	none

	Usage :			Call to export the diagram to an EMF file
					for - for example - inclusion into a
					word-processing document.

   ============================================================*/
{

	CFileDialog dlg(FALSE, _T("emf"), filename, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("Enhanced MetaFile (*.emf)|*.emf|All Files (*.*)|*.*||"));
	if (noDialog || dlg.DoModal() == IDOK)
	{

		CClientDC	dc(this);
		CMetaFileDC	metaDC;
		CRect rect(0, 0,
			GetVirtualSize().cx,
			GetVirtualSize().cy);

		// Determine the picture frame dimensions.
		// iWidthMM is the display width in millimeters.
		// iHeightMM is the display height in millimeters.
		// iWidthPels is the display width in pixels.
		// iHeightPels is the display height in pixels

		int iWidthMM = GetDeviceCaps(dc, HORZSIZE);
		int iHeightMM = GetDeviceCaps(dc, VERTSIZE);
		int iWidthPels = GetDeviceCaps(dc, HORZRES);
		int iHeightPels = GetDeviceCaps(dc, VERTRES);

		// Convert client coordinates to .01-mm units.
		// Use iWidthMM, iWidthPels, iHeightMM, and
		// iHeightPels to determine the number of
		// .01-millimeter units per pixel in the x-
		//  and y-directions.
		CRect r(
			(rect.left * iWidthMM * 100) / iWidthPels,
			(rect.top * iHeightMM * 100) / iHeightPels,
			(rect.right * iWidthMM * 100) / iWidthPels,
			(rect.bottom * iHeightMM * 100) / iHeightPels);

		metaDC.CreateEnhanced(&dc, dlg.GetPathName(), &r, _T("DialogEditor Drawing"));
		metaDC.SetAttribDC(dc.m_hDC);

		SetRedraw(FALSE);
		COLORREF col = GetBackgroundColor();

		SetBackgroundColor(RGB(255, 255, 255));
		Print(&metaDC, rect, 1);
		SetBackgroundColor(col);
		SetRedraw(TRUE);
		RedrawWindow();

		::DeleteEnhMetaFile(metaDC.CloseEnhanced());

	}

}