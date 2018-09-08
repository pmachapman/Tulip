/* ==========================================================================
	CFlowchartLinkableLineSegment

	Author :		Johan Rosengren, Abstrakt Mekanik AB

	Date :			2004-04-29

	Purpose :		CFlowchartLinkableLineSegment is a CFlowchartEntity-
					derived class, representing a line that can be linked
					to other CFlowchartEntity-derived objects.

	Description :	The implementation is based on CDiagramLine, even though
					not derived from it. The class allows two link point,
					LINK_START and LINK_END - the top-left and bottom-right
					corners.

	Usage :			Create with CFlowchartControlFactory::CreateFromString

   ========================================================================*/
#include "stdafx.h"
#include "FlowchartLinkableLineSegment.h"
#include "../DiagramEditor/DiagramLine.h"

   //////////////////////////////////////////
   // LineDDA callbacks from CDiagramLine
   //
VOID CALLBACK HitTest(int X, int Y, LPARAM data);
VOID CALLBACK HitTestRect(int X, int Y, LPARAM data);

CFlowchartLinkableLineSegment::CFlowchartLinkableLineSegment()
/* ============================================================
	Function :		CFlowchartLinkableLineSegment::CFlowchartLinkableLineSegment
	Description :	constructor

	Return :		void
	Parameters :	none

	Usage :

   ============================================================*/
{

	SetMinimumSize(CSize(-1, -1));
	SetMaximumSize(CSize(-1, -1));
	SetType(_T("flowchart_arrow"));
	SetTitle(_T(""));

}

CFlowchartLinkableLineSegment::~CFlowchartLinkableLineSegment()
/* ============================================================
	Function :		CFlowchartLinkableLineSegment::~CFlowchartLinkableLineSegment
	Description :	destructor

	Return :		void
	Parameters :	none

	Usage :

   ============================================================*/
{
}

CDiagramEntity* CFlowchartLinkableLineSegment::Clone()
/* ============================================================
	Function :		CFlowchartLinkableLineSegment::Clone
	Description :	Clone this object to a new object.

	Return :		CDiagramEntity*	-	The new object.
	Parameters :	none

	Usage :			Call to create a clone of the object. The
					caller will have to delete the object.

   ============================================================*/
{

	CFlowchartLinkableLineSegment* obj = new CFlowchartLinkableLineSegment;
	obj->Copy(this);
	return obj;

}

void CFlowchartLinkableLineSegment::Draw(CDC* dc, CRect rect)
/* ============================================================
	Function :		CFlowchartLinkableLineSegment::Draw
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
	dc->SelectStockObject(BLACK_BRUSH);

	// Draw line
	dc->MoveTo(rect.TopLeft());
	dc->LineTo(rect.BottomRight());

	// Draw title
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

CDiagramEntity* CFlowchartLinkableLineSegment::CreateFromString(const CString& str)
/* ============================================================
	Function :		CFlowchartLinkableLineSegment::CreateFromString
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

	CFlowchartLinkableLineSegment* obj = new CFlowchartLinkableLineSegment;
	if (!obj->FromString(str))
	{
		delete obj;
		obj = NULL;
	}

	return obj;

}

int CFlowchartLinkableLineSegment::GetHitCode(CPoint point) const
/* ============================================================
	Function :		CFlowchartLinkableLineSegment::GetHitCode
	Description :	Returns the hit point constant for point.

	Return :		int				-	The hit point,
										DEHT_NONE if none.
	Parameters :	CPoint point	-	The point to check

	Usage :			Call to see in what part of the object point
					lies.

   ============================================================*/
{

	int result = DEHT_NONE;

	CRect rect = GetRect();

	hitParams hit;
	hit.hit = FALSE;
	hit.x = point.x;
	hit.y = point.y;

	LineDDA((int)GetLeft(),
		(int)GetTop(),
		(int)GetRight(),
		(int)GetBottom(),
		HitTest,
		(LPARAM)&hit);

	if (hit.hit)
		result = DEHT_BODY;

	CRect rectTest;

	rectTest = GetSelectionMarkerRect(DEHT_TOPLEFT, rect);
	if (rectTest.PtInRect(point))
		result = DEHT_TOPLEFT;

	rectTest = GetSelectionMarkerRect(DEHT_BOTTOMRIGHT, rect);
	if (rectTest.PtInRect(point))
		result = DEHT_BOTTOMRIGHT;

	return result;

}

HCURSOR CFlowchartLinkableLineSegment::GetCursor(int hit) const
/* ============================================================
	Function :		CFlowchartLinkableLineSegment::GetCursor
	Description :	Returns the cursor for the given hit point.

	Return :		HCURSOR	-	The cursor to show
	Parameters :	int hit	-	The hit point constant (DEHT_)
								to get the cursor for.

	Usage :			Call to get the cursor for a specific hit
					point constant.

   ============================================================*/
{

	HCURSOR cursor = NULL;
	switch (hit)
	{
	case DEHT_BODY:
		cursor = LoadCursor(NULL, IDC_SIZEALL);
		break;
	case DEHT_TOPLEFT:
		cursor = LoadCursor(NULL, IDC_SIZEALL);
		break;
	case DEHT_BOTTOMRIGHT:
		cursor = LoadCursor(NULL, IDC_SIZEALL);
		break;
	}

	return cursor;

}

void CFlowchartLinkableLineSegment::DrawSelectionMarkers(CDC* dc, CRect rect) const
/* ============================================================
	Function :		CFlowchartLinkableLineSegment::DrawSelectionMarkers
	Description :	Draws the selection markers for the
					object.

	Return :		void
	Parameters :	CDC* dc		-	The CDC to draw to
					CRect rect	-	The real object rectangle.

	Usage :			rect is the true rectangle (zoomed) of the
					object.

   ============================================================*/
{

	CRect rectSelect;

	dc->SelectStockObject(BLACK_PEN);
	CBrush greenBrush;
	greenBrush.CreateSolidBrush(RGB(0, 255, 0));
	dc->SelectObject(&greenBrush);

	rectSelect = GetSelectionMarkerRect(DEHT_TOPLEFT, rect);
	dc->Rectangle(rectSelect);

	rectSelect = GetSelectionMarkerRect(DEHT_BOTTOMRIGHT, rect);
	dc->Rectangle(rectSelect);

	dc->SelectStockObject(BLACK_BRUSH);

}

void CFlowchartLinkableLineSegment::SetRect(CRect rect)
/* ============================================================
	Function :		CFlowchartLinkableLineSegment::SetRect
	Description :	Sets the rect of the object.

	Return :		void
	Parameters :	CRect rect	-

	Usage :			Overriden to avoid normalization.

   ============================================================*/
{

	SetLeft(rect.left);
	SetTop(rect.top);
	SetRight(rect.right);
	SetBottom(rect.bottom);

}

BOOL CFlowchartLinkableLineSegment::BodyInRect(CRect rect) const
/* ============================================================
	Function :		CFlowchartLinkableLineSegment::BodyInRect
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
	hitParamsRect hit;
	hit.rect = rect;
	hit.hit = FALSE;

	LineDDA((int)GetLeft(),
		(int)GetTop(),
		(int)GetRight(),
		(int)GetBottom(),
		HitTestRect,
		(LPARAM)&hit);

	if (hit.hit)
		result = TRUE;

	return result;

}

CPoint CFlowchartLinkableLineSegment::GetLinkPosition(int type)
/* ============================================================
	Function :		CFlowchartLinkableLineSegment::GetLinkPosition
	Description :	Returns the position of a link.

	Return :		CPoint		-	The position of the link,
									-1, -1 if the link is not
									allowed.
	Parameters :	int type	-	The type of the link.

	Usage :			The possible link types are:
					LINK_TOP		Links are allowed to the
									top of the object.
					LINK_BOTTOM		Links are allowed to the
									bottom.
					LINK_LEFT		Links are allowed to the
									left.
					LINK_RIGHT		Links are allowed to the
									right.
					LINK_START		Links are allowed to the
									start of a line (normally
									the top-left corner of
									the non-normalized bounding
									rect).
					LINK_END		Links are allowed to the
									end of a line (normally the
									bottom-right corner of the
									non-normalized bounding
									rect).

   ============================================================*/
{

	CPoint point(-1, -1);
	CRect rect = GetRect();

	switch (type)
	{
	case LINK_START:
		point.x = rect.left;
		point.y = rect.top;
		break;
	case LINK_END:
		point.x = rect.right;
		point.y = rect.bottom;
		break;
	}

	return point;

}

int CFlowchartLinkableLineSegment::AllowLink()
/* ============================================================
	Function :		CFlowchartLinkableLineSegment::AllowLink
	Description :	Returns the allowed link types for this
					object.

	Return :		int		-	The allowed link types
	Parameters :	none

	Usage :			Call this function to get the link types
					allowed for this object. Several link-types
					can be ORed together. The possible link
					types are:

					LINK_TOP		Links are allowed to the
									top of the object.
					LINK_BOTTOM		Links are allowed to the
									bottom.
					LINK_LEFT		Links are allowed to the
									left.
					LINK_RIGHT		Links are allowed to the
									right.
					LINK_ALL		Links are allowed to all
									of the above.
					LINK_START		Links are allowed to the
									start of a line (normally
									the top-left corner of
									the non-normalized bounding
									rect).
					LINK_END		Links are allowed to the
									end of a line (normally the
									bottom-right corner of the
									non-normalized bounding
									rect).

   ============================================================*/
{

	return LINK_START | LINK_END;

}

