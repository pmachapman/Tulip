/* ==========================================================================
	CFlowchartEntityBox

	Author :		Johan Rosengren, Abstrakt Mekanik AB

	Date :			2004-04-29

	Purpose :		CFlowchartEntityBox, derived from CFlowchartEntity,
					represents a simple linkable box. The box can link
					from all four sides.

	Description :	The class implements the necessary functions for
					cloning and creation from text (Clone and
					CreateFromString). It draws a rectangle using the
					title attribute of CDiagramEntity as the text
					contents.

	Usage :			Create with CFlowchartControlFactory::CreateFromString

   ========================================================================*/

#include "stdafx.h"
#include "FlowchartEntityBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CFlowchartEntityBox::CFlowchartEntityBox()
/* ============================================================
	Function :		CFlowchartEntityBox::CFlowchartEntityBox
	Description :	constructor

	Return :		void
	Parameters :	none

	Usage :

   ============================================================*/
{

	SetConstraints(CSize(128, 64), CSize(128, 64));
	SetType(_T("flowchart_box"));
	CString title;
	if (title.LoadString(IDS_FLOWCHART_BOX) > 0)
	{
		SetTitle(title);
	}
}

CFlowchartEntityBox::~CFlowchartEntityBox()
/* ============================================================
	Function :		CFlowchartEntityBox::~CFlowchartEntityBox
	Description :	destructor

	Return :		void
	Parameters :	none

	Usage :

   ============================================================*/
{
}

void CFlowchartEntityBox::Draw(CDC* dc, CRect rect)
/* ============================================================
	Function :		CFlowchartEntityBox::Draw
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

	int cut = round((double)GetMarkerSize().cx * GetZoom());
	CRect r(rect.left + cut, rect.top + cut / 2, rect.right - cut, rect.bottom - cut / 2);

	dc->SelectStockObject(BLACK_PEN);
	dc->SelectStockObject(WHITE_BRUSH);
	int mode = dc->SetBkMode(TRANSPARENT);

	dc->Rectangle(rect);

	CFont font;
	font.CreateFont(-round(12.0 * GetZoom()), 0, 0, 0, FW_NORMAL, 0, 0, 0, 0, 0, 0, 0, 0, _T("Courier New"));
	dc->SelectObject(&font);

	dc->DrawText(GetTitle(), r, DT_NOPREFIX | DT_WORDBREAK | DT_CENTER);

	dc->SelectStockObject(DEFAULT_GUI_FONT);
	dc->SetBkMode(mode);

}

CDiagramEntity* CFlowchartEntityBox::Clone()
/* ============================================================
	Function :		CFlowchartEntityBox::Clone
	Description :	Clone this object to a new object.

	Return :		CDiagramEntity*	-	The new object.
	Parameters :	none

	Usage :			Call to create a clone of the object. The
					caller will have to delete the object.

   ============================================================*/
{

	CFlowchartEntityBox* obj = new CFlowchartEntityBox;
	obj->Copy(this);
	return obj;

}

CDiagramEntity* CFlowchartEntityBox::CreateFromString(const CString& str)
/* ============================================================
	Function :		CFlowchartEntityBox::CreateFromString
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

	CFlowchartEntityBox* obj = new CFlowchartEntityBox;
	if (!obj->FromString(str))
	{
		delete obj;
		obj = NULL;
	}

	return obj;

}
