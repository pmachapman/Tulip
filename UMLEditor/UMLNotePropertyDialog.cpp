/* ==========================================================================
	Class :			CUMLNotePropertyDialog

	Author :		Johan Rosengren, Abstrakt Mekanik AB

	Date :			2004-04-29

	Purpose :		"CUMLNotePropertyDialog" is a dialog box wrapper derived 
					from "CDiagramPropertyDlg", used by "CUMLEntity"-
					derived objects to edit the object title attribute.

	Description :	Class-Wizard created class.

	Usage :			In the "CUMLEntity"-derived class, add a member of 
					the "CUMLNotePropertyDialog"-derived class, and call 
					"SetPropertyDialog" in the constructor.

					The dialog template with the resource id 
					"IDD_UML_DIALOG_PROPERTY_NOTE" must be added to the project.
   ========================================================================*/

#include "stdafx.h"
#include "UMLNotePropertyDialog.h"
#include "UMLEntityNote.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUMLNotePropertyDialog dialog

CUMLNotePropertyDialog::CUMLNotePropertyDialog(CWnd* pParent /*=NULL*/)
	: CDiagramPropertyDlg(CUMLNotePropertyDialog::IDD, pParent)
/* ============================================================
	Function :		CUMLNotePropertyDialog::CUMLNotePropertyDialog
	Description :	Constructor
	Access :		Public
					
	Return :		void
	Parameters :	CWnd* pParent	-	Dialog parent

	Usage :			

   ============================================================*/
{

	//{{AFX_DATA_INIT(CUMLNotePropertyDialog)
	m_text = _T("");
	//}}AFX_DATA_INIT

}

CUMLNotePropertyDialog::~CUMLNotePropertyDialog()
/* ============================================================
	Function :		CUMLNotePropertyDialog::~CUMLNotePropertyDialog
	Description :	Destructor
	Access :		Public
					
	Return :		void
	Parameters :	none

	Usage :			

   ============================================================*/
{
}

void CUMLNotePropertyDialog::DoDataExchange(CDataExchange* pDX)
/* ============================================================
	Function :		CUMLNotePropertyDialog::DoDataExchange
	Description :	MFC data exchange handler.
	Access :		Protected

	Return :		void
	Parameters :	CDataExchange* pDX	-	Pointer to exchange object
					
	Usage :			Called from MFC to exchange and validate 
					dialog data.

   ============================================================*/
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUMLNotePropertyDialog)
	DDX_Text(pDX, IDC_EDIT_TEXT, m_text);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUMLNotePropertyDialog, CDialog)
	//{{AFX_MSG_MAP(CUMLNotePropertyDialog)
	ON_BN_CLICKED(IDC_BUTTON_FONT, OnButtonFont)
	ON_BN_CLICKED(IDC_BUTTON_COLOR, OnButtonColor)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUMLNotePropertyDialog message handlers

void CUMLNotePropertyDialog::OnOK() 
/* ============================================================
	Function :		CUMLNotePropertyDialog::OnOK
	Description :	Called when the Apply-key is pressed.
	Access :		Protected
			
	Return :		void
	Parameters :	none

	Usage :			Called from MFC. Updates the attached object 
					and hides the modeless dialog.

   ============================================================*/
{

	CUMLEntityNote* uml = static_cast< CUMLEntityNote* >( GetEntity() );

	UpdateData();
	uml->SetTitle( m_text );
	uml->SetBkColor( m_color );
	uml->SetFont( m_font );
	Redraw();
	ShowWindow( SW_HIDE );
	GetRedrawWnd()->SetFocus();

}

void CUMLNotePropertyDialog::OnCancel() 
/* ============================================================
	Function :		CUMLNotePropertyDialog::OnCancel
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
// CUMLNotePropertyDialog overrides

void CUMLNotePropertyDialog::SetValues() 
/* ============================================================
	Function :		CUMLNotePropertyDialog::SetValues
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

	CUMLEntityNote* uml = static_cast< CUMLEntityNote* >( GetEntity() );

	m_text = uml->GetTitle();
	m_color = uml->GetBkColor();
	m_font = uml->GetFont();

}

void CUMLNotePropertyDialog::OnButtonFont() 
/* ============================================================
	Function :		CUMLNotePropertyDialog::OnButtonFont
	Description :	Handler for the dialog button Font
	Access :		Protected

	Return :		void
	Parameters :	none

	Usage :			Called from MFC.

   ============================================================*/
{

	CFont font;
	CUMLEntityNote* uml = static_cast< CUMLEntityNote* >( GetEntity() );
	font.CreatePointFont( 120, uml->GetFont() );
	LOGFONT lf;
	font.GetLogFont( &lf );
	CFontDialog	dlg( &lf );
	if( dlg.DoModal() == IDOK )
		m_font = dlg.GetFaceName();
	
}

void CUMLNotePropertyDialog::OnButtonColor() 
/* ============================================================
	Function :		CUMLNotePropertyDialog::OnButtonColor
	Description :	Handler for the dialog button Color
	Access :		Protected

	Return :		void
	Parameters :	none

	Usage :			Called from MFC.

   ============================================================*/
{

	CUMLEntityNote* uml = static_cast< CUMLEntityNote* >( GetEntity() );
	COLORREF color = uml->GetBkColor();
	CColorDialog	dlg( color );
	if( dlg.DoModal() == IDOK )
		m_color = dlg.GetColor();
	
}
