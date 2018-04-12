/* ==========================================================================
	Class :			CUMLPackagePropertyDialog

	Author :		Johan Rosengren, Abstrakt Mekanik AB

	Date :			2004-04-29

	Purpose :		"CUMLPackagePropertyDialog" is a dialog box wrapper derived from 
					"CDiagramPropertyDlg", used by "CUMLEntity"-derived 
					objects to edit the object title attribute.

	Description :	Class-Wizard created class.

	Usage :			In the "CUMLEntity"-derived class, add a member of 
					the "CUMLPackagePropertyDialog"-derived class, and call 
					"SetPropertyDialog" in the constructor.

					The dialog is displayed as a modeless dialog. The 
					editor will hide the dialog automatically when another
					object is selected, no special Close-button is 
					necessary.

					The dialog template with the res id "IDD_UML_DIALOG_PROPERTY_PACKAGE"
					must be added to the project.
   ========================================================================*/

#include "stdafx.h"
#include "UMLEntityPackage.h"
#include "UMLPackagePropertyDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUMLPackagePropertyDialog dialog

CUMLPackagePropertyDialog::CUMLPackagePropertyDialog(CWnd* pParent /*=NULL*/)
	: CDiagramPropertyDlg(CUMLPackagePropertyDialog::IDD, pParent)
/* ============================================================
	Function :		CUMLPackagePropertyDialog::CUMLPackagePropertyDialog
	Description :	Constructor
	Access :		Public
					
	Return :		void
	Parameters :	CWnd* pParent	-	Dialog parent

	Usage :			

   ============================================================*/
{

	//{{AFX_DATA_INIT(CUMLPackagePropertyDialog)
	m_text = _T("");
	//}}AFX_DATA_INIT

}

CUMLPackagePropertyDialog::~CUMLPackagePropertyDialog()
/* ============================================================
	Function :		CUMLPackagePropertyDialog::~CUMLPackagePropertyDialog
	Description :	Destructor
	Access :		Public
					
	Return :		void
	Parameters :	none

	Usage :			

   ============================================================*/
{
}

void CUMLPackagePropertyDialog::DoDataExchange(CDataExchange* pDX)
/* ============================================================
	Function :		CUMLPackagePropertyDialog::DoDataExchange
	Description :	MFC data exchange handler.
	Access :		Protected

	Return :		void
	Parameters :	CDataExchange* pDX	-	Pointer to exchange object
					
	Usage :			Called from MFC to exchange and validate 
					dialog data.

   ============================================================*/
{

	CDiagramPropertyDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUMLPackagePropertyDialog)
	DDX_Text(pDX, IDC_EDIT_TEXT, m_text);
	//}}AFX_DATA_MAP

}

BEGIN_MESSAGE_MAP(CUMLPackagePropertyDialog, CDiagramPropertyDlg)
	//{{AFX_MSG_MAP(CUMLPackagePropertyDialog)
	ON_BN_CLICKED(IDC_BUTTON_FONT, OnButtonFont)
	ON_BN_CLICKED(IDC_BUTTON_COLOR, OnButtonColor)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUMLPackagePropertyDialog message handlers

void CUMLPackagePropertyDialog::OnOK() 
/* ============================================================
	Function :		CUMLPackagePropertyDialog::OnOK
	Description :	Called when the Apply-key is pressed.
	Access :		Protected

	Return :		void
	Parameters :	none

	Usage :			Called from MFC. Updates the attached object 
					and hides the modeless dialog.

   ============================================================*/
{

	CUMLEntityPackage* uml = static_cast< CUMLEntityPackage* >( GetEntity() );

	UpdateData();
	if( uml->ModifyTitle( m_text ) )
	{
		uml->SetBkColor( m_color );
		uml->SetFont( m_font );
		Redraw();
		ShowWindow( SW_HIDE );
		GetRedrawWnd()->SetFocus();
	}

}

void CUMLPackagePropertyDialog::OnCancel() 
/* ============================================================
	Function :		CUMLPackagePropertyDialog::OnCancel
	Description :	Called when the ESC-key is pressed.
	Access :		Protected
					
	Return :		void
	Parameters :	none

	Usage :			Called from MFC. Overridden to close the 
					dialog.

   ============================================================*/
{

	CDialog::OnCancel();
	GetRedrawWnd()->SetFocus();

}

/////////////////////////////////////////////////////////////////////////////
// CUMLPackagePropertyDialog overrides

void CUMLPackagePropertyDialog::SetValues() 
/* ============================================================
	Function :		CUMLPackagePropertyDialog::SetValues
	Description :	Set the values in the dialog from the 
					attached object.
	Access :		Public
					
	Return :		void
	Parameters :	none

	Usage :			Will be called by the framework and the 
					attached object to initialize the dialog. 
					The editbox is filled with the contents of 
					the object title attribute.

   ============================================================*/
{

	CUMLEntityPackage* uml = static_cast< CUMLEntityPackage* >( GetEntity() );

	m_text = uml->GetTitle();
	m_color = uml->GetBkColor();
	m_font = uml->GetFont();

	if( m_hWnd )
		UpdateData( FALSE );

}

void CUMLPackagePropertyDialog::OnButtonFont() 
/* ============================================================
	Function :		CUMLPackagePropertyDialog::OnButtonFont
	Description :	Handler for the Font button.
	Access :		Protected

	Return :		void
	Parameters :	none

	Usage :			Called from MFC. Displays the Windows font 
					dialog.

   ============================================================*/
{

	CFont font;
	CUMLEntityPackage* uml = static_cast< CUMLEntityPackage* >( GetEntity() );
	font.CreatePointFont( 120, uml->GetFont() );
	LOGFONT lf;
	font.GetLogFont( &lf );
	CFontDialog	dlg( &lf );
	if( dlg.DoModal() == IDOK )
		m_font = dlg.GetFaceName();

}
void CUMLPackagePropertyDialog::OnButtonColor() 
/* ============================================================
	Function :		CUMLPackagePropertyDialog::OnButtonColor
	Description :	Handler for the Color button
	Access :		Protected

	Return :		void
	Parameters :	none

	Usage :			Called from MFC. Displays the Windows color 
					dialog.

   ============================================================*/
{

	CUMLEntityPackage* uml = static_cast< CUMLEntityPackage* >( GetEntity() );
	COLORREF color = uml->GetBkColor();
	CColorDialog	dlg( color );
	if( dlg.DoModal() == IDOK )
		m_color = dlg.GetColor();
	
}
