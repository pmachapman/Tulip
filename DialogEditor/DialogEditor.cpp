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
	Function :		CFlowchartEditor::CFlowchartEditor
	Description :	constructor

	Return :		void
	Parameters :	none

	Usage :

   ============================================================*/
{
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
	CStdGrfx::drawframed3dBox(dc, rect);
}

void CDialogEditor::ExportEMF(const CString& filename)
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
	if (dlg.DoModal() == IDOK)
	{

		CClientDC	dc(this);
		CMetaFileDC	metaDC;
		CRect rect(0, 0,
			GetVirtualSize().cx,
			GetVirtualSize().cy);

		// Himetric rect
		CRect r(0, 0, 8 * 2540, 11 * 2540);

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