/* ==========================================================================
	Class :			CUMLEditorPropertyDialog

	Author :		Johan Rosengren, Abstrakt Mekanik AB

	Date :			2004-06-21

	Purpose :		"CUMLEditorPropertyDialog" derives from "CDialog" and 
					is a wrapper around the editor property dialog.

	Description :	The class uses the dialog template with the resource ID 
					"IDD_UML_DIALOG_PROPERTY_EDITOR" from the resources.

	Usage :			

   ========================================================================*/

#include "stdafx.h"
#include "UMLEditorPropertyDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUMLEditorPropertyDialog dialog

CUMLEditorPropertyDialog::CUMLEditorPropertyDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CUMLEditorPropertyDialog::IDD, pParent)
/* ============================================================
	Function :		CUMLEditorPropertyDialog::CUMLEditorPropertyDialog
	Description :	Constructor
	Access :		Public
					
	Return :		void
	Parameters :	CWnd* pParent	-	Dialog parent

	Usage :			

   ============================================================*/
{
	//{{AFX_DATA_INIT(CUMLEditorPropertyDialog)
	m_grid = FALSE;
	m_nomarkers = FALSE;
	m_onlypublic = FALSE;
	m_showgrid = FALSE;
	m_noattributes = FALSE;
	m_nooperations = FALSE;
	m_nooperationattributenames = FALSE;
	m_showmargin = FALSE;
	//}}AFX_DATA_INIT

	m_color = RGB( 0, 0, 0 );

}


void CUMLEditorPropertyDialog::DoDataExchange(CDataExchange* pDX)
/* ============================================================
	Function :		CUMLEditorPropertyDialog::DoDataExchange
	Description :	MFC data exchange handler.
	Access :		Protected

	Return :		void
	Parameters :	CDataExchange* pDX	-	Pointer to exchange object
					
	Usage :			Called from MFC to exchange and validate 
					dialog data.

   ============================================================*/
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUMLEditorPropertyDialog)
	DDX_Check(pDX, IDC_CHECK_GRID, m_grid);
	DDX_Check(pDX, IDC_CHECK_NO_MARKERS, m_nomarkers);
	DDX_Check(pDX, IDC_CHECK_ONLY_PUBLIC, m_onlypublic);
	DDX_Check(pDX, IDC_CHECK_SHOW_GRID, m_showgrid);
	DDX_Check(pDX, IDC_CHECK_NO_ATTRIBUTES, m_noattributes);
	DDX_Check(pDX, IDC_CHECK_NO_OPERATIONS, m_nooperations);
	DDX_Check(pDX, IDC_CHECK_NO_ATTRIBUTE_NAMES, m_nooperationattributenames);
	DDX_Check(pDX, IDC_CHECK_SHOW_MARGINS, m_showmargin);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CUMLEditorPropertyDialog, CDialog)
	//{{AFX_MSG_MAP(CUMLEditorPropertyDialog)
	ON_BN_CLICKED(IDC_BUTTON_COLOR, OnButtonColor)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUMLEditorPropertyDialog message handlers

void CUMLEditorPropertyDialog::OnButtonColor() 
/* ============================================================
	Function :		CUMLEditorPropertyDialog::OnButtonColor
	Description :	Handler for the dialog button Color
	Access :		Protected

	Return :		void
	Parameters :	none

	Usage :			Called from MFC.

   ============================================================*/
{

	CColorDialog dlg( m_color );
	if( dlg.DoModal() == IDOK )
		m_color = dlg.GetColor();
	
}
