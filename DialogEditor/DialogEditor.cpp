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

void CDialogEditor::DrawGrid(CDC* dc, CRect /*rect*/, double zoom) const
{
	COLORREF gridcol = GetGridColor();

	dc->SelectStockObject(BLACK_PEN);

	int stepx = GetVirtualSize().cx / GetGridSize().cx;
	int stepy = GetVirtualSize().cy / GetGridSize().cy;

	for (int x = 0; x <= stepx; x++)
		for (int y = 0; y <= stepy; y++)
			dc->SetPixel(round((double)(GetGridSize().cx * x) * zoom), round((double)(GetGridSize().cy * y) * zoom), gridcol);
}
