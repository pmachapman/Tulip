/* ==========================================================================
	CFlowchartEntityTerminator

	Author :		Johan Rosengren, Abstrakt Mekanik AB

	Date :			2004-04-29

	Purpose :		CFlowchartEntityTerminator, derived from CFlowchartEntity,
					represents a linkable terminator. The terminator can link
					from all four sides.

	Description :	The class implements the necessary functions for cloning
					and creation from text (Clone and CreateFromString). It
					draws a rectangle using the title attribute of
					CDiagramEntity as the text contents.

	Usage :			Create with CFlowchartControlFactory::CreateFromString

   ========================================================================*/

#include "stdafx.h"
#include "FlowchartEntityTerminator.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CFlowchartEntityTerminator::CFlowchartEntityTerminator()
/* ============================================================
	Function :		CFlowchartEntityTerminator::CFlowchartEntityTerminator
	Description :	constructor

	Return :		void
	Parameters :	none

	Usage :

   ============================================================*/
{

	SetConstraints(CSize(128, 32), CSize(128, 32));
	SetType(_T("flowchart_start"));

	CString title;
	title.LoadString(IDS_FLOWCHART_TERMINATOR);
	SetTitle(title);

}

CFlowchartEntityTerminator::~CFlowchartEntityTerminator()
/* ============================================================
	Function :		CFlowchartEntityTerminator::~CFlowchartEntityTerminator
	Description :	destructor

	Return :		void
	Parameters :	none

	Usage :

   ============================================================*/
{
}

CDiagramEntity* CFlowchartEntityTerminator::Clone()
/* ============================================================
	Function :		CFlowchartEntityTerminator::Clone
	Description :	Clone this object to a new object.

	Return :		CDiagramEntity*	-	The new object.
	Parameters :	none

	Usage :			Call to create a clone of the object. The
					caller will have to delete the object.

   ============================================================*/
{

	CFlowchartEntityTerminator* obj = new CFlowchartEntityTerminator;
	obj->Copy(this);
	return obj;

}

CDiagramEntity* CFlowchartEntityTerminator::CreateFromString(const CString& str)
/* ============================================================
	Function :		CFlowchartEntityTerminator::CreateFromString
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

	CFlowchartEntityTerminator* obj = new CFlowchartEntityTerminator;
	if (!obj->FromString(str))
	{
		delete obj;
		obj = NULL;
	}

	return obj;

}

void CFlowchartEntityTerminator::Draw(CDC* dc, CRect rect)
/* ============================================================
	Function :		CFlowchartEntityTerminator::Draw
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

	dc->SelectStockObject(BLACK_PEN);
	dc->SelectStockObject(WHITE_BRUSH);

	CSize sz = GetMarkerSize();
	CPoint pt;
	pt.x = round((double)sz.cx * GetZoom());
	pt.y = round((double)sz.cy * GetZoom());
	dc->RoundRect(rect, pt);

	CFont font;
	font.CreateFont(-round(12.0 * GetZoom()), 0, 0, 0, FW_NORMAL, 0, 0, 0, 0, 0, 0, 0, 0, _T("Courier New"));
	dc->SelectObject(&font);
	int mode = dc->SetBkMode(TRANSPARENT);
	dc->DrawText(GetTitle(), rect, DT_NOPREFIX | DT_SINGLELINE | DT_VCENTER | DT_CENTER);
	dc->SelectStockObject(DEFAULT_GUI_FONT);
	dc->SetBkMode(mode);

}

