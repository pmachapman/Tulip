/* ==========================================================================
	Class :			CUMLInterfacePropertyDialog

	Author :		Johan Rosengren, Abstrakt Mekanik AB

	Date :			2004-04-29

	Purpose :		"CUMLInterfacePropertyDialog" is a dialog box wrapper derived from 
					"CDiagramPropertyDlg", used by "CUMLEntity"-derived 
					objects to edit the object title attribute.

	Description :	Class-Wizard created class.

	Usage :			In the "CUMLEntity"-derived class, add a member of 
					the "CUMLInterfacePropertyDialog"-derived class, and call 
					"SetPropertyDialog" in the constructor.

					The dialog is displayed as a modeless dialog. The 
					editor will hide the dialog automatically when another
					object is selected, no special Close-button is 
					necessary.

					The dialog template with the res id "IDD_UML_DIALOG_PROPERTY_INTERFACE" 
					must be added to the project.
   ========================================================================*/

#include "stdafx.h"
#include "UMLEntityInterface.h"
#include "UMLInterfacePropertyDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUMLInterfacePropertyDialog dialog

CUMLInterfacePropertyDialog::CUMLInterfacePropertyDialog(CWnd* pParent /*=NULL*/)
	: CDiagramPropertyDlg(CUMLInterfacePropertyDialog::IDD, pParent)
/* ============================================================
	Function :		CUMLInterfacePropertyDialog::CUMLInterfacePropertyDialog
	Description :	Constructor
	Access :		Public
					
	Return :		void
	Parameters :	CWnd* pParent	-	Dialog parent

	Usage :			

   ============================================================*/
{

	//{{AFX_DATA_INIT(CUMLInterfacePropertyDialog)
	m_text = _T("");
	//}}AFX_DATA_INIT

}

CUMLInterfacePropertyDialog::~CUMLInterfacePropertyDialog()
/* ============================================================
	Function :		CUMLInterfacePropertyDialog::~CUMLInterfacePropertyDialog
	Description :	Destructor
	Access :		Public
					
	Return :		void
	Parameters :	none

	Usage :			

   ============================================================*/
{
}

void CUMLInterfacePropertyDialog::DoDataExchange(CDataExchange* pDX)
/* ============================================================
	Function :		CUMLInterfacePropertyDialog::DoDataExchange
	Description :	MFC data exchange handler.
	Access :		Protected

	Return :		void
	Parameters :	CDataExchange* pDX	-	Pointer to exchange object
					
	Usage :			Called from MFC to exchange and validate 
					dialog data.

   ============================================================*/
{

	CDiagramPropertyDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUMLInterfacePropertyDialog)
	DDX_Text(pDX, IDC_EDIT_TEXT, m_text);
	//}}AFX_DATA_MAP

}

BEGIN_MESSAGE_MAP(CUMLInterfacePropertyDialog, CDiagramPropertyDlg)
	//{{AFX_MSG_MAP(CUMLInterfacePropertyDialog)
	ON_BN_CLICKED(IDC_BUTTON_FONT, OnButtonFont)
	ON_BN_CLICKED(IDC_BUTTON_COLOR, OnButtonColor)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUMLInterfacePropertyDialog message handlers

void CUMLInterfacePropertyDialog::OnOK() 
/* ============================================================
	Function :		CUMLInterfacePropertyDialog::OnOK
	Description :	Called when the Apply-key is pressed.
	Access :		Protected
					
	Return :		void
	Parameters :	none

	Usage :			Called from MFC. Updates the attached object 
					and hides the modeless dialog.

   ============================================================*/
{

	CUMLEntityInterface* uml = static_cast< CUMLEntityInterface* >( GetEntity() );

	UpdateData();
	uml->SetTitle( m_text );
	uml->SetBkColor( m_color );
	uml->SetFont( m_font );
	Redraw();
	ShowWindow( SW_HIDE );
	GetRedrawWnd()->SetFocus();

}

void CUMLInterfacePropertyDialog::OnCancel() 
/* ============================================================
	Function :		CUMLInterfacePropertyDialog::OnCancel
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
// CUMLInterfacePropertyDialog overrides

void CUMLInterfacePropertyDialog::SetValues() 
/* ============================================================
	Function :		CUMLInterfacePropertyDialog::SetValues
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

	CUMLEntityInterface* uml = static_cast< CUMLEntityInterface* >( GetEntity() );

	m_text = uml->GetTitle();
	m_color = uml->GetBkColor();
	m_font = uml->GetFont();

	if( m_hWnd )
		UpdateData( FALSE );

}

void CUMLInterfacePropertyDialog::OnButtonFont() 
/* ============================================================
	Function :		CUMLInterfacePropertyDialog::OnButtonFont
	Description :	Handler for the dialog button Font
	Access :		Protected

	Return :		void
	Parameters :	none

	Usage :			Called from MFC.

   ============================================================*/
{

	CFont font;
	CUMLEntityInterface* uml = static_cast< CUMLEntityInterface* >( GetEntity() );
	font.CreatePointFont( 120, uml->GetFont() );
	LOGFONT lf;
	font.GetLogFont( &lf );
	CFontDialog	dlg( &lf );
	if( dlg.DoModal() == IDOK )
		m_font = dlg.GetFaceName();
	
}
void CUMLInterfacePropertyDialog::OnButtonColor() 
/* ============================================================
	Function :		CUMLInterfacePropertyDialog::OnButtonColor
	Description :	Handler for the dialog button Color
	Access :		Protected

	Return :		void
	Parameters :	none

	Usage :			Called from MFC.

   ============================================================*/
{

	CUMLEntityInterface* uml = static_cast< CUMLEntityInterface* >( GetEntity() );
	COLORREF color = uml->GetBkColor();
	CColorDialog	dlg( color );
	if( dlg.DoModal() == IDOK )
		m_color = dlg.GetColor();
	
}
