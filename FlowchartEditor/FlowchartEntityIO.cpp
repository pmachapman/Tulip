/* ==========================================================================
	CFlowchartEntityIO

	Author :		Johan Rosengren, Abstrakt Mekanik AB

	Date :			2004-04-29

	Purpose :		CFlowchartEntityIO, derived from CFlowchartEntity,
					represents a linkable I/O-box (a parallellogram). The
					box can link from all four sides.

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
#include "FlowchartEntityIO.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CFlowchartEntityIO::CFlowchartEntityIO()
/* ============================================================
	Function :		CFlowchartEntityIO::CFlowchartEntityIO
	Description :	constructor

	Return :		void
	Parameters :	none

	Usage :

   ============================================================*/
{

	SetConstraints(CSize(128, 64), CSize(128, 64));
	SetType(_T("flowchart_IO"));

	CString title;
	title.LoadString(IDS_FLOWCHART_IO);
	SetTitle(title);

}

CFlowchartEntityIO::~CFlowchartEntityIO()
/* ============================================================
	Function :		CFlowchartEntityIO::~CFlowchartEntityIO
	Description :	destructor

	Return :		void
	Parameters :	none

	Usage :

   ============================================================*/
{
}

void CFlowchartEntityIO::Draw(CDC* dc, CRect rect)
/* ============================================================
	Function :		CFlowchartEntityIO::Draw
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

	CPoint pt[4];
	int cut = round((double)GetMarkerSize().cx * GetZoom());

	pt[0].x = rect.left + cut;
	pt[0].y = rect.top;

	pt[1].x = rect.right;
	pt[1].y = rect.top;

	pt[2].x = rect.right - cut;
	pt[2].y = rect.bottom;

	pt[3].x = rect.left;
	pt[3].y = rect.bottom;

	dc->Polygon(pt, 4);

	CRect r(rect.left + cut, rect.top + cut / 2, rect.right - cut, rect.bottom - cut / 2);
	CFont font;
	font.CreateFont(-round(12.0 * GetZoom()), 0, 0, 0, FW_NORMAL, 0, 0, 0, 0, 0, 0, 0, 0, _T("Courier New"));
	dc->SelectObject(&font);
	int mode = dc->SetBkMode(TRANSPARENT);
	dc->DrawText(GetTitle(), r, DT_NOPREFIX | DT_WORDBREAK | DT_CENTER);
	dc->SelectStockObject(DEFAULT_GUI_FONT);
	dc->SetBkMode(mode);

}

CDiagramEntity* CFlowchartEntityIO::Clone()
/* ============================================================
	Function :		CFlowchartEntityIO::Clone
	Description :	Clone this object to a new object.

	Return :		CDiagramEntity*	-	The new object.
	Parameters :	none

	Usage :			Call to create a clone of the object. The
					caller will have to delete the object.

   ============================================================*/
{

	CFlowchartEntityIO* obj = new CFlowchartEntityIO;
	obj->Copy(this);
	return obj;

}

CDiagramEntity* CFlowchartEntityIO::CreateFromString(const CString& str)
/* ============================================================
	Function :		CFlowchartEntityIO::CreateFromString
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

	CFlowchartEntityIO* obj = new CFlowchartEntityIO;
	if (!obj->FromString(str))
	{
		delete obj;
		obj = NULL;
	}

	return obj;

}
