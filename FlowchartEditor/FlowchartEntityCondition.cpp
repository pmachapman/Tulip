/* ==========================================================================
	CFlowchartEntityCondition

	Author :		Johan Rosengren, Abstrakt Mekanik AB

	Date :			2004-04-29

	Purpose :		CFlowchartEntityCondition, derived from CFlowchartEntity,
					represents a linkable condition box ("diamond"). The box
					can link from all four sides.

	Description :	The class implements the necessary functions for
					cloning and creation from text (Clone and
					CreateFromString). It draws a rectangle using the
					title attribute of CDiagramEntity as the text
					contents.

					Hit-testing is made with a region due to the non-
					rectangular shape of the object.

	Usage :			Create with CFlowchartControlFactory::CreateFromString

   ========================================================================*/
#include "stdafx.h"
#include "FlowchartEntityCondition.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CFlowchartEntityCondition::CFlowchartEntityCondition()
/* ============================================================
	Function :		CFlowchartEntityCondition::CFlowchartEntityCondition
	Description :	constructor

	Return :		void
	Parameters :	none

	Usage :

   ============================================================*/
{

	SetConstraints(CSize(128, 64), CSize(128, 64));
	SetType(_T("flowchart_condition"));

	CString title;
	if (title.LoadString(IDS_FLOWCHART_CONDITION) > 0)
	{
		SetTitle(title);
	}
}

CFlowchartEntityCondition::~CFlowchartEntityCondition()
/* ============================================================
	Function :		CFlowchartEntityCondition::~CFlowchartEntityCondition
	Description :	destructor

	Return :		void
	Parameters :	none

	Usage :

   ============================================================*/
{
}

void CFlowchartEntityCondition::Draw(CDC* dc, CRect rect)
/* ============================================================
	Function :		CFlowchartEntityCondition::Draw
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

	// The polygon
	CPoint pt[4];

	pt[0].x = rect.left;
	pt[0].y = rect.top + ((rect.bottom - rect.top) / 2);

	pt[1].x = rect.left + ((rect.right - rect.left) / 2);
	pt[1].y = rect.top;

	pt[2].x = rect.right;
	pt[2].y = rect.top + ((rect.bottom - rect.top) / 2);

	pt[3].x = rect.left + ((rect.right - rect.left) / 2);
	pt[3].y = rect.bottom;

	dc->Polygon(pt, 4);

	// The text
	int qwidth = rect.Width() / 4;
	int qheight = rect.Height() / 4;
	CRect r(rect.left + qwidth, rect.top + qheight, rect.right - qwidth, rect.bottom - qheight);

	CFont font;
	font.CreateFont(-round(12.0 * GetZoom()), 0, 0, 0, FW_NORMAL, 0, 0, 0, 0, 0, 0, 0, 0, _T("Courier New"));
	dc->SelectObject(&font);
	int mode = dc->SetBkMode(TRANSPARENT);

	dc->DrawText(GetTitle(), r, DT_NOPREFIX | DT_WORDBREAK | DT_CENTER);
	dc->SelectStockObject(DEFAULT_GUI_FONT);

	dc->SetBkMode(mode);
}

CDiagramEntity* CFlowchartEntityCondition::Clone()
/* ============================================================
	Function :		CFlowchartEntityCondition::Clone
	Description :	Clone this object to a new object.

	Return :		CDiagramEntity*	-	The new object.
	Parameters :	none

	Usage :			Call to create a clone of the object. The
					caller will have to delete the object.

   ============================================================*/
{

	CFlowchartEntityCondition* obj = new CFlowchartEntityCondition;
	obj->Copy(this);
	return obj;

}

CDiagramEntity* CFlowchartEntityCondition::CreateFromString(const CString& str)
/* ============================================================
	Function :		CFlowchartEntityCondition::CreateFromString
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

	CFlowchartEntityCondition* obj = new CFlowchartEntityCondition;
	if (!obj->FromString(str))
	{
		delete obj;
		obj = NULL;
	}

	return obj;

}

BOOL CFlowchartEntityCondition::BodyInRect(CRect rect) const
/* ============================================================
	Function :		CFlowchartEntityCondition::BodyInRect
	Description :	Used to see if any part of the object lies
					in rect.

	Return :		BOOL		-	TRUE if any part of the
									object lies inside rect.
	Parameters :	CRect rect	-	The rect to check.

	Usage :			Call to see if the object overlaps - for
					example - a selection rubberband.

   ============================================================*/
{

	BOOL result = FALSE;
	rect.NormalizeRect();
	CRect rectEntity = GetRect();

	CPoint pt[4];

	pt[0].x = rectEntity.left;
	pt[0].y = rectEntity.top + ((rectEntity.bottom - rectEntity.top) / 2);

	pt[1].x = rectEntity.left + ((rectEntity.right - rectEntity.left) / 2);
	pt[1].y = rectEntity.top;

	pt[2].x = rectEntity.right;
	pt[2].y = rectEntity.top + ((rectEntity.bottom - rectEntity.top) / 2);

	pt[3].x = rectEntity.left + ((rectEntity.right - rectEntity.left) / 2);
	pt[3].y = rectEntity.bottom;

	CRgn rgn;
	rgn.CreatePolygonRgn(pt, 4, ALTERNATE);

	if (rgn.RectInRegion(rect))
		result = TRUE;

	rgn.DeleteObject();

	return result;

}

int CFlowchartEntityCondition::GetHitCode(CPoint point) const
/* ============================================================
	Function :		CFlowchartEntityCondition::GetHitCode
	Description :	Returns the hit point constant for point.

	Return :		int				-	The hit point,
										DEHT_NONE if none.
	Parameters :	CPoint point	-	The point to check

	Usage :			Call to see in what part of the object point
					lies.

   ============================================================*/
{

	int result = CFlowchartEntity::GetHitCode(point);
	if (result == DEHT_BODY)
	{
		result = DEHT_NONE;

		CPoint pt[4];
		CRect rect = GetRect();

		pt[0].x = rect.left;
		pt[0].y = rect.top + ((rect.bottom - rect.top) / 2);

		pt[1].x = rect.left + ((rect.right - rect.left) / 2);
		pt[1].y = rect.top;

		pt[2].x = rect.right;
		pt[2].y = rect.top + ((rect.bottom - rect.top) / 2);

		pt[3].x = rect.left + ((rect.right - rect.left) / 2);
		pt[3].y = rect.bottom;

		CRgn rgn;
		rgn.CreatePolygonRgn(pt, 4, ALTERNATE);
		if (rgn.PtInRegion(point))
			result = DEHT_BODY;

		rgn.DeleteObject();
	}


	return result;

}
