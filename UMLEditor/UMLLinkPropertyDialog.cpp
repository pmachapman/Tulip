/* ==========================================================================
	Class :			CUMLLinkPropertyDialog

	Author :		Johan Rosengren, Abstrakt Mekanik AB

	Date :			2004-06-21

	Purpose :		"CUMLLinkPropertyDialog" derives from "CDiagramPropertyDlg".
					It is a wrapper for the link property dialog.

	Description :	A standard Class-Wizard created dialog box class.

	Usage :			In the "CUMLEntity"-derived class, add a member of 
					the "CUMLInterfacePropertyDialog"-derived class, and call 
					"SetPropertyDialog" in the constructor.

					The dialog is displayed as a modeless dialog. The 
					editor will hide the dialog automatically when another
					object is selected, no special Close-button is 
					necessary.

					The dialog template with the resource id 
					"IDD_UML_DIALOG_PROPERTY_LINK" 
					must be added to the project.

   ========================================================================
	Changes :
		8/7 2004	Support for the Interface line type added.
   ========================================================================*/

#include "stdafx.h"
#include "UMLLinkPropertyDialog.h"
#include "UMLLineSegment.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUMLLinkPropertyDialog dialog

CUMLLinkPropertyDialog::CUMLLinkPropertyDialog(CWnd* pParent /*=NULL*/)
	: CDiagramPropertyDlg(CUMLLinkPropertyDialog::IDD, pParent)
/* ============================================================
	Function :		CUMLLinkPropertyDialog::CUMLLinkPropertyDialog
	Description :	Constructor
	Access :		Public
					
	Return :		void
	Parameters :	CWnd* pParent	-	Dialog parent

	Usage :			

   ============================================================*/
{
	//{{AFX_DATA_INIT(CUMLLinkPropertyDialog)
	m_unidirectional = FALSE;
	m_endlabel = _T("");
	m_startlabel = _T("");
	m_secondaryendlabel = _T("");
	m_secondarystartlabel = _T("");
	m_linktype = 0;
	m_title = _T("");
	//}}AFX_DATA_INIT
}


void CUMLLinkPropertyDialog::DoDataExchange(CDataExchange* pDX)
/* ============================================================
	Function :		CUMLLinkPropertyDialog::DoDataExchange
	Description :	MFC data exchange handler.
	Access :		Protected

	Return :		void
	Parameters :	CDataExchange* pDX	-	Pointer to exchange object
					
	Usage :			Called from MFC to exchange and validate 
					dialog data.

   ============================================================*/
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUMLLinkPropertyDialog)
	DDX_Check(pDX, IDC_CHECK_UNIDIRECTIONAL, m_unidirectional);
	DDX_Text(pDX, IDC_EDIT_END_LABEL, m_endlabel);
	DDX_Text(pDX, IDC_EDIT_START_LABEL, m_startlabel);
	DDX_Text(pDX, IDC_EDIT_SECONDARY_END_LABEL, m_secondaryendlabel);
	DDX_Text(pDX, IDC_EDIT_SECONDARY_START_LABEL, m_secondarystartlabel);
	DDX_Radio(pDX, IDC_RADIO_ASSOCIATION, m_linktype);
	DDX_Text(pDX, IDC_UML_EDIT_LINK_TITLE, m_title);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUMLLinkPropertyDialog, CDialog)
	//{{AFX_MSG_MAP(CUMLLinkPropertyDialog)
	ON_BN_CLICKED(IDC_BUTTON_FONT, OnButtonFont)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUMLLinkPropertyDialog message handlers

void CUMLLinkPropertyDialog::OnOK() 
/* ============================================================
	Function :		CUMLLinkPropertyDialog::OnOK
	Description :	Handler for the dialog OK-button.
	Access :		Protected

	Return :		void
	Parameters :	none

	Usage :			Called from MFC.

   ============================================================*/
{

	UpdateData();

	CUMLLineSegment* uml = static_cast< CUMLLineSegment* >( GetEntity() );

	uml->SetEndLabel( m_endlabel );
	uml->SetStartLabel( m_startlabel );
	uml->SetSecondaryEndLabel( m_secondaryendlabel );
	uml->SetSecondaryStartLabel( m_secondarystartlabel );
	uml->SetTitle( m_title );
	uml->SetFont( m_font );

	switch( m_linktype )
	{
		case TYPE_ASSOCIATION:
			uml->SetLineStyle( STYLE_NONE );
			break;
		case TYPE_DEPENDANCY:
			uml->SetLineStyle( STYLE_DASHED );
			break;
		case TYPE_INHERITANCE:
			uml->SetLineStyle( STYLE_ARROWHEAD );
			break;
		case TYPE_COMPOSITION:
			uml->SetLineStyle( STYLE_FILLED_DIAMOND );
			break;
		case TYPE_PARTOF:
			uml->SetLineStyle( STYLE_CIRCLECROSS );
			break;
		case TYPE_INVISIBLE:
			uml->SetLineStyle( STYLE_INVISIBLE );
		case TYPE_INTERFACE:
			uml->SetLineStyle( STYLE_ARROWHEAD );
			uml->AddLineStyle( STYLE_DASHED );
			break;
	}

	if( m_unidirectional )
		uml->AddLineStyle( STYLE_FILLED_ARROWHEAD );

	Redraw();
	ShowWindow( SW_HIDE );
	GetRedrawWnd()->SetFocus();

}

void CUMLLinkPropertyDialog::OnCancel() 
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

void CUMLLinkPropertyDialog::SetValues() 
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

	CUMLLineSegment* uml = static_cast< CUMLLineSegment* >( GetEntity() );

	m_endlabel = uml->GetEndLabel();
	m_startlabel = uml->GetStartLabel();
	m_secondaryendlabel = uml->GetSecondaryEndLabel();
	m_secondarystartlabel = uml->GetSecondaryStartLabel();
	m_title = uml->GetTitle();

	int style = uml->GetLineStyle();
	m_linktype = TYPE_ASSOCIATION;
	if( style & STYLE_DASHED )
		m_linktype = TYPE_DEPENDANCY;
	if( style & STYLE_ARROWHEAD )
		m_linktype = TYPE_INHERITANCE;
	if( style & STYLE_FILLED_DIAMOND )
		m_linktype = TYPE_COMPOSITION;
	if( style & STYLE_CIRCLECROSS )
		m_linktype = TYPE_PARTOF;
	if( style & STYLE_INVISIBLE )
		m_linktype = TYPE_INVISIBLE;
	if( style & STYLE_DASHED && style & STYLE_ARROWHEAD )
		m_linktype = TYPE_INTERFACE;

	m_unidirectional = FALSE;
	if( style & STYLE_FILLED_ARROWHEAD )
		m_unidirectional = TRUE;

	m_font = uml->GetFont();

	if( m_hWnd )
		UpdateData( FALSE );

}

void CUMLLinkPropertyDialog::OnButtonFont() 
/* ============================================================
	Function :		CUMLLinkPropertyDialog::OnButtonFont
	Description :	Handler for the dialog button Font
	Access :		Protected

	Return :		void
	Parameters :	none

	Usage :			Called from MFC.

   ============================================================*/
{

	CFont font;
	CUMLLineSegment* uml = static_cast< CUMLLineSegment* >( GetEntity() );
	font.CreatePointFont( 120, uml->GetFont() );
	LOGFONT lf;
	font.GetLogFont( &lf );
	CFontDialog	dlg( &lf );
	if( dlg.DoModal() == IDOK )
		m_font = dlg.GetFaceName();
	
}
