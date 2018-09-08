/* ==========================================================================
	CFlowchartEntity

	Author :		Johan Rosengren, Abstrakt Mekanik AB

	Date :			2004-04-29

	Purpose :		CFlowchartEntity is a base class for linked objects used
					by a CDiagramEditor.

	Description :	The class contains a few virtual functions that derived
					classes can implement:
					AllowLink and GetLinkPosition. AllowLink returns what
					link types are allowed, one or more of the following
					ORed together:
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

					GetLinkPosition will return the position of a link type.
					The coordinates are not zoomed, so if they should be
					used for drawing, they must be multiplied with the
					current zoom factor of the editor.

	Usage :			Use this class as a base class for linked objects.

   ========================================================================*/
#include "stdafx.h"
#include "FlowchartEntity.h"
#include "../UMLEditor/LinkFactory.h"
#include "../DiagramEditor/Tokenizer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CFlowchartEntity::CFlowchartEntity()
/* ============================================================
	Function :		CFlowchartEntity::CFlowchartEntity
	Description :	constructor

	Return :		void
	Parameters :	none

	Usage :

   ============================================================*/
{

	// Setting fixed size
	SetConstraints(CSize(128, 64), CSize(128, 64));
	SetType(_T("flowchart_entity"));

	SetPropertyDialog(&m_dlg, CPropertyDialog::IDD);

	SetName(CLinkFactory::GetID());

	SetMoved(FALSE);

}

CFlowchartEntity::~CFlowchartEntity()
/* ============================================================
	Function :		CFlowchartEntity::~CFlowchartEntity
	Description :	destructor

	Return :		void
	Parameters :	none

	Usage :

   ============================================================*/
{

	if (m_dlg.m_hWnd)
		m_dlg.DestroyWindow();

}

CDiagramEntity* CFlowchartEntity::Clone()
/* ============================================================
	Function :		CFlowchartEntity::Clone
	Description :	Clone this object to a new object.

	Return :		CDiagramEntity*	-	The new object.
	Parameters :	none

	Usage :			Call to create a clone of the object. The
					caller will have to delete the object.

   ============================================================*/
{

	CFlowchartEntity* obj = new CFlowchartEntity;
	obj->Copy(this);
	return obj;

}

CDiagramEntity* CFlowchartEntity::CreateFromString(const CString& str)
/* ============================================================
	Function :		CFlowchartEntity::CreateFromString
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

	CFlowchartEntity* obj = new CFlowchartEntity;
	if (!obj->FromString(str))
	{
		delete obj;
		obj = NULL;
	}

	return obj;

}

int CFlowchartEntity::GetHitCode(CPoint point) const
/* ============================================================
	Function :		CFlowchartEntity::GetHitCode
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

	if (rect.PtInRect(point))
		result = DEHT_BODY;

	return result;

}

void CFlowchartEntity::DrawSelectionMarkers(CDC* dc, CRect rect) const
/* ============================================================
	Function :		CFlowchartEntity::DrawSelectionMarkers
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

	rectSelect = GetSelectionMarkerRect(DEHT_LEFTMIDDLE, rect);
	dc->Rectangle(rectSelect);

	rectSelect = GetSelectionMarkerRect(DEHT_TOPMIDDLE, rect);
	dc->Rectangle(rectSelect);

	rectSelect = GetSelectionMarkerRect(DEHT_RIGHTMIDDLE, rect);
	dc->Rectangle(rectSelect);

	rectSelect = GetSelectionMarkerRect(DEHT_BOTTOMMIDDLE, rect);
	dc->Rectangle(rectSelect);

	dc->SelectStockObject(BLACK_BRUSH);

}

HCURSOR CFlowchartEntity::GetCursor(int /*hit*/) const
/* ============================================================
	Function :		CFlowchartEntity::GetCursor
	Description :	Returns the cursor for the given hit point.

	Return :		HCURSOR	-	The cursor to show
	Parameters :	int hit	-	The hit point constant (DEHT_)
								to get the cursor for.

	Usage :			Call to get the cursor for a specific hit
					point constant.

   ============================================================*/
{

	HCURSOR cursor = LoadCursor(NULL, IDC_SIZEALL);
	return cursor;

}

int CFlowchartEntity::AllowLink()
/* ============================================================
	Function :		CFlowchartEntity::AllowLink
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

	return LINK_ALL;

}

void CFlowchartEntity::SetMoved(BOOL moved)
/* ============================================================
	Function :		CFlowchartEntity::SetMoved
	Description :	Sets the moved-flag of the object.

	Return :		void
	Parameters :	BOOL moved	-	TRUE if the object is moved
									while moving linked objects,
									FALSE if not.

	Usage :			To avoid infinite recursion while moving
					linked objects, we use a flag to mark this
					object as already moved.

   ============================================================*/
{

	m_moved = moved;

}

BOOL CFlowchartEntity::GetMoved()
/* ============================================================
	Function :		CFlowchartEntity::GetMoved
	Description :	Gets the moved-flag of the object.

	Return :		BOOL	-	TRUE if the object is moved
									while moving linked objects,
									FALSE if not.

	Parameters :	none

	Usage :			To avoid infinite recursion while moving
					linked objects, we use a flag to mark this
					object as already moved.

   ============================================================*/
{

	return m_moved;

}

CPoint CFlowchartEntity::GetLinkPosition(int type)
/* ============================================================
	Function :		CFlowchartEntity::GetLinkPosition
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
	case LINK_LEFT:
		point.x = rect.left;
		point.y = rect.top + round((double)rect.Height() / 2);
		break;
	case LINK_RIGHT:
		point.x = rect.right;
		point.y = rect.top + round((double)rect.Height() / 2);
		break;
	case LINK_TOP:
		point.x = rect.left + round((double)rect.Width() / 2);
		point.y = rect.top;
		break;
	case LINK_BOTTOM:
		point.x = rect.left + round((double)rect.Width() / 2);
		point.y = rect.bottom;
		break;
	}

	return point;

}
