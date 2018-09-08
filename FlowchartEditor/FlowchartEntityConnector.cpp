/* ==========================================================================
	CFlowchartEntityConnector

	Author :		Johan Rosengren, Abstrakt Mekanik AB

	Date :			2004-04-29

	Purpose :		CFlowchartEntityConnector, derived from CFlowchartEntity,
					represents a linkable connector (a circle). The connector
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
#include "FlowchartEntityConnector.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CFlowchartEntityConnector::CFlowchartEntityConnector()
/* ============================================================
	Function :		CFlowchartEntityConnector::CFlowchartEntityConnector
	Description :	constructor

	Return :		void
	Parameters :	none

	Usage :

   ============================================================*/
{

	SetConstraints(CSize(32, 32), CSize(32, 32));
	SetType(_T("flowchart_connector"));

	CString title;
	title.LoadString(IDS_FLOWCHART_CONNECTOR);
	SetTitle(title);

}

CFlowchartEntityConnector::~CFlowchartEntityConnector()
/* ============================================================
	Function :		CFlowchartEntityConnector::~CFlowchartEntityConnector
	Description :	destructor

	Return :		void
	Parameters :	none

	Usage :

   ============================================================*/
{
}

void CFlowchartEntityConnector::Draw(CDC* dc, CRect rect)
/* ============================================================
	Function :		CFlowchartEntityConnector::Draw
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

	// The circle
	dc->Ellipse(rect);

	// The text
	CFont font;
	font.CreateFont(-round(12.0 * GetZoom()), 0, 0, 0, FW_NORMAL, 0, 0, 0, 0, 0, 0, 0, 0, _T("Courier New"));
	dc->SelectObject(&font);
	int mode = dc->SetBkMode(TRANSPARENT);

	dc->DrawText(GetTitle(), rect, DT_NOPREFIX | DT_SINGLELINE | DT_VCENTER | DT_CENTER);
	dc->SelectStockObject(DEFAULT_GUI_FONT);

	dc->SetBkMode(mode);
}

CDiagramEntity* CFlowchartEntityConnector::Clone()
/* ============================================================
	Function :		CFlowchartEntityConnector::Clone
	Description :	Clone this object to a new object.

	Return :		CDiagramEntity*	-	The new object.
	Parameters :	none

	Usage :			Call to create a clone of the object. The
					caller will have to delete the object.

   ============================================================*/
{

	CFlowchartEntityConnector* obj = new CFlowchartEntityConnector;
	obj->Copy(this);
	return obj;

}

CDiagramEntity* CFlowchartEntityConnector::CreateFromString(const CString& str)
/* ============================================================
	Function :		CFlowchartEntityConnector::CreateFromString
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

	CFlowchartEntityConnector* obj = new CFlowchartEntityConnector;
	if (!obj->FromString(str))
	{
		delete obj;
		obj = NULL;
	}

	return obj;

}

BOOL CFlowchartEntityConnector::BodyInRect(CRect rect) const
/* ============================================================
	Function :		CFlowchartEntityConnector::BodyInRect
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

	CRgn rgn;
	rgn.CreateEllipticRgn(rectEntity.left, rectEntity.top, rectEntity.right, rectEntity.bottom);
	if (rgn.RectInRegion(rect))
		result = TRUE;

	rgn.DeleteObject();

	return result;

}

int CFlowchartEntityConnector::GetHitCode(CPoint point) const
/* ============================================================
	Function :		CFlowchartEntityConnector::GetHitCode
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

		CRect rectEntity = GetRect();
		CRgn rgn;
		rgn.CreateEllipticRgn(rectEntity.left, rectEntity.top, rectEntity.right, rectEntity.bottom);
		if (rgn.PtInRegion(point))
			result = DEHT_BODY;

		rgn.DeleteObject();
	}

	return result;

}
