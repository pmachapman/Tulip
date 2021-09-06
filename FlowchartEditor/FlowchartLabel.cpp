/* ==========================================================================
	CFlowchartLabel

	Author :		Johan Rosengren, Abstrakt Mekanik AB

	Date :			2004-04-29

	Purpose :		CFlowchartLabel is a CDiagramEntity-derived class.
					It is a non-linkable transparent box with no border and
					the title attribute displayed as text.

	Description :	The class implements a minimum amount of functionality -
					drawing, cloning and creating from string.

	Usage :			Create with CFlowchartControlFactory::CreateFromString

   ========================================================================*/

#include "stdafx.h"
#include "FlowchartLabel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CFlowchartLabel::CFlowchartLabel()
/* ============================================================
	Function :		CFlowchartLabel::CFlowchartLabel
	Description :	constructor

	Return :		void
	Parameters :	none

	Usage :

   ============================================================*/
{

	SetConstraints(CSize(40, 12), CSize(-1, -1));
	SetType(_T("flowchart_label"));
	SetPropertyDialog(&m_dlg, CLabelPropertyDialog::IDD);

	CString title;
	if (title.LoadString(IDS_FLOWCHART_LABEL) > 0)
	{
		SetTitle(title);
	}
}

CFlowchartLabel::~CFlowchartLabel()
/* ============================================================
	Function :		CFlowchartLabel::~CFlowchartLabel
	Description :	destructor

	Return :		void
	Parameters :	none

	Usage :

   ============================================================*/
{

	if (m_dlg.m_hWnd)
		m_dlg.DestroyWindow();

}

void CFlowchartLabel::Draw(CDC* dc, CRect rect)
/* ============================================================
	Function :		CFlowchartLabel::Draw
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

	CFont font;
	font.CreateFont(-round(12.0 * GetZoom()), 0, 0, 0, FW_NORMAL, 0, 0, 0, 0, 0, 0, 0, 0, _T("Courier New"));
	dc->SelectObject(&font);
	int mode = dc->SetBkMode(TRANSPARENT);
	dc->DrawText(GetTitle(), rect, DT_NOPREFIX | DT_WORDBREAK);
	dc->SelectStockObject(DEFAULT_GUI_FONT);
	dc->SetBkMode(mode);

}

CDiagramEntity* CFlowchartLabel::Clone()
/* ============================================================
	Function :		CFlowchartLabel::Clone
	Description :	Clone this object to a new object.

	Return :		CDiagramEntity*	-	The new object.
	Parameters :	none

	Usage :			Call to create a clone of the object. The
					caller will have to delete the object.

   ============================================================*/
{

	CFlowchartLabel* obj = new CFlowchartLabel;
	obj->Copy(this);
	return obj;

}

CDiagramEntity* CFlowchartLabel::CreateFromString(const CString& str)
/* ============================================================
	Function :		CFlowchartLabel::CreateFromString
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

	CFlowchartLabel* obj = new CFlowchartLabel;
	if (!obj->FromString(str))
	{
		delete obj;
		obj = NULL;
	}

	return obj;

}
