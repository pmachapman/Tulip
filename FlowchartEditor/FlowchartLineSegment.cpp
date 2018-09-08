/* ==========================================================================
	CFlowchartLineSegment

	Author :		Johan Rosengren, Abstrakt Mekanik AB

	Date :			2004-04-29

	Purpose :		CFlowchartLineSegment represents a dotted line and is
					derived from CDiagramLine.

	Description :	The class implements a minimum amount of functionality -
					drawing, cloning and creating from string.

	Usage :			Create with CFlowchartControlFactory::CreateFromString

   ========================================================================*/
#include "stdafx.h"
#include "FlowchartLineSegment.h"
#include "../DiagramEditor/DiagramLine.h"

   /////////////////////////////////////////////////////////////////////////////
   // LinDDA prototypes - for hit-testing

VOID CALLBACK HitTest(int X, int Y, LPARAM data);
VOID CALLBACK HitTestRect(int X, int Y, LPARAM data);

/////////////////////////////////////////////////////////////////////////////
// CFlowchartLineSegment

CFlowchartLineSegment::CFlowchartLineSegment()
/* ============================================================
	Function :		CFlowchartLineSegment::CFlowchartLineSegment
	Description :	constructor

	Return :		void
	Parameters :	none

	Usage :

   ============================================================*/
{

	SetMinimumSize(CSize(-1, -1));
	SetType(_T("flowchart_line"));
	SetTitle(_T(""));

	SetPropertyDialog(&m_dlg, CPropertyDialog::IDD);

}

CFlowchartLineSegment::~CFlowchartLineSegment()
/* ============================================================
	Function :		CFlowchartLineSegment::~CFlowchartLineSegment
	Description :	destructor

	Return :		void
	Parameters :	none

	Usage :

   ============================================================*/
{

	if (m_dlg.m_hWnd)
		m_dlg.DestroyWindow();

}

CDiagramEntity* CFlowchartLineSegment::Clone()
/* ============================================================
	Function :		CFlowchartLineSegment::Clone
	Description :	Clone this object to a new object.

	Return :		CDiagramEntity*	-	The new object.
	Parameters :	none

	Usage :			Call to create a clone of the object. The
					caller will have to delete the object.

   ============================================================*/

{

	CFlowchartLineSegment* obj = new CFlowchartLineSegment;
	obj->Copy(this);
	return obj;
}

void CFlowchartLineSegment::Draw(CDC* dc, CRect rect)
/* ============================================================
	Function :		CFlowchartLineSegment::Draw
	Description :	Draws the object.

	Return :		void
	Parameters :	CDC* dc		-	The CDC to draw to.
					CRect rect	-	The real rectangle of the
									object.

	Usage :			The function should clean up all selected
					objects. Note that the CDC is a memory CDC,
					so creating a memory CDC in this function
					will probably not speed up the function.

   ============================================================*/
{

	CPen pen(PS_DASH, 0, RGB(0, 0, 0));
	dc->SelectObject(&pen);
	dc->SelectStockObject(BLACK_BRUSH);

	dc->MoveTo(rect.TopLeft());
	dc->LineTo(rect.BottomRight());

	dc->SelectStockObject(BLACK_PEN);

	CString str = GetTitle();
	if (str.GetLength())
	{
		CFont font;
		font.CreateFont(-round(12.0 * GetZoom()), 0, 0, 0, FW_NORMAL, 0, 0, 0, 0, 0, 0, 0, 0, _T("Courier New"));
		dc->SelectObject(&font);
		int mode = dc->SetBkMode(TRANSPARENT);

		CRect rectTemp(rect);
		rectTemp.NormalizeRect();
		int cy = round(14.0 * GetZoom());
		int cut = round((double)GetMarkerSize().cx * GetZoom() / 2);
		CRect r(rect.right - cut, rect.top, rect.right - (rectTemp.Width() + cut), rect.bottom);
		if (rect.top == rect.bottom)
		{
			CRect r(rect.left, rect.top - (cy + cut), rect.right, rect.bottom);
			r.NormalizeRect();
			dc->DrawText(str, r, DT_NOPREFIX | DT_SINGLELINE | DT_VCENTER | DT_CENTER);
		}
		else
		{
			CRect r(rect.right - cut, rect.top, rect.right - (cy * str.GetLength() + cut), rect.bottom);
			r.NormalizeRect();
			dc->DrawText(str, r, DT_NOPREFIX | DT_SINGLELINE | DT_VCENTER | DT_RIGHT);
		}

		dc->SelectStockObject(DEFAULT_GUI_FONT);
		dc->SetBkMode(mode);

	}

}

CDiagramEntity* CFlowchartLineSegment::CreateFromString(const CString& str)
/* ============================================================
	Function :		CFlowchartLineSegment::CreateFromString
	Description :	Static factory function that creates and
					returns an instance of this class if str
					is a valid representation.

	Return :		CDiagramEntity*		-	The object, or NULL
											if str is not a
											representation of
											this type.
	Parameters :	const CString& str	-	The string to create
											from.

	Usage :			Can be used as a factory for text file loads.
					Each object type should have its own
					version - the default one is a model
					implementation.

   ============================================================*/
{

	CFlowchartLineSegment* obj = new CFlowchartLineSegment;
	if (!obj->FromString(str))
	{
		delete obj;
		obj = NULL;
	}

	return obj;

}

