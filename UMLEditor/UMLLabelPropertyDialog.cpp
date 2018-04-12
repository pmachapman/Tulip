/* ==========================================================================
	Class :			CUMLLabelPropertyDialog

	Author :		Johan Rosengren, Abstrakt Mekanik AB

	Date :			2004-04-29

	Purpose :		"CUMLLabelPropertyDialog" is a dialog box wrapper derived 
					from "CDiagramPropertyDlg", used by "CUMLEntity"-
					derived objects to edit the object title attribute.

	Description :	Class-Wizard created class.

	Usage :			In the "CUMLEntity"-derived class, add a member of 
					the "CUMLLabelPropertyDialog"-derived class, and call 
					"SetPropertyDialog" in the constructor.

					The dialog template with the resource id 
					"IDD_UML_DIALOG_PROPERTY_LABEL" must be added to the project.
   ========================================================================*/

#include "stdafx.h"
#include "UMLLabelPropertyDialog.h"
#include "UMLEntityLabel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUMLLabelPropertyDialog dialog

CUMLLabelPropertyDialog::CUMLLabelPropertyDialog(CWnd* pParent /*=NULL*/)
	: CDiagramPropertyDlg(CUMLLabelPropertyDialog::IDD, pParent)
/* ============================================================
	Function :		CUMLLabelPropertyDialog::CUMLLabelPropertyDialog
	Description :	Constructor
	Access :		Public
					
	Return :		void
	Parameters :	CWnd* pParent	-	Dialog parent

	Usage :			

   ============================================================*/
{

	//{{AFX_DATA_INIT(CUMLLabelPropertyDialog)
	m_text = _T("");
	//}}AFX_DATA_INIT

	m_pointsize = 12;
	m_bold = FALSE;
	m_italic = FALSE;
	m_underline = FALSE;

}

CUMLLabelPropertyDialog::~CUMLLabelPropertyDialog()
/* ============================================================
	Function :		CUMLLabelPropertyDialog::~CUMLLabelPropertyDialog
	Description :	Destructor
	Access :		Public
					
	Return :		void
	Parameters :	none

	Usage :			

   ============================================================*/
{
}

void CUMLLabelPropertyDialog::DoDataExchange(CDataExchange* pDX)
/* ============================================================
	Function :		CUMLLabelPropertyDialog::DoDataExchange
	Description :	MFC data exchange handler.
	Access :		Protected

	Return :		void
	Parameters :	CDataExchange* pDX	-	Pointer to exchange object
					
	Usage :			Called from MFC to exchange and validate 
					dialog data.

   ============================================================*/
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUMLLabelPropertyDialog)
	DDX_Text(pDX, IDC_EDIT_TEXT, m_text);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUMLLabelPropertyDialog, CDialog)
	//{{AFX_MSG_MAP(CUMLLabelPropertyDialog)
	ON_BN_CLICKED(IDC_BUTTON_FONT, OnButtonFont)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUMLLabelPropertyDialog message handlers

void CUMLLabelPropertyDialog::OnOK() 
/* ============================================================
	Function :		CUMLLabelPropertyDialog::OnOK
	Description :	Called when the Apply-key is pressed.
	Access :		Protected

	Return :		void
	Parameters :	none

	Usage :			Called from MFC. Updates the attached object 
					and hides the modeless dialog.

   ============================================================*/
{

	CUMLEntityLabel* uml = static_cast< CUMLEntityLabel* >( GetEntity() );

	UpdateData();
	uml->SetTitle( m_text );
	uml->SetFont( m_font );

	uml->SetBold( m_bold );
	uml->SetItalic( m_italic );
	uml->SetUnderline( m_underline );
	uml->SetPointsize( m_pointsize );

	Redraw();
	ShowWindow( SW_HIDE );
	GetRedrawWnd()->SetFocus();

}

void CUMLLabelPropertyDialog::OnCancel() 
/* ============================================================
	Function :		CUMLLabelPropertyDialog::OnCancel
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
// CUMLLabelPropertyDialog overrides

void CUMLLabelPropertyDialog::SetValues() 
/* ============================================================
	Function :		CUMLLabelPropertyDialog::SetValues
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

	CUMLEntityLabel* uml = static_cast< CUMLEntityLabel* >( GetEntity() );

	m_text = uml->GetTitle();
	m_font = uml->GetFont();
	m_pointsize = uml->GetPointsize();
	m_bold = uml->GetBold();
	m_italic = uml->GetItalic();
	m_underline = uml->GetUnderline();

}

void CUMLLabelPropertyDialog::OnButtonFont() 
/* ============================================================
	Function :		CUMLLabelPropertyDialog::OnButtonFont
	Description :	Handler for the dialog button Font
	Access :		Protected

	Return :		void
	Parameters :	none

	Usage :			Called from MFC.

   ============================================================*/
{

	CFont font;
	CUMLEntityLabel* uml = static_cast< CUMLEntityLabel* >( GetEntity() );
	font.CreatePointFont( m_pointsize * 10, uml->GetFont() );
	LOGFONT lf;
	font.GetLogFont( &lf );

	lf.lfItalic = ( BYTE ) m_italic;
	lf.lfUnderline = ( BYTE ) m_underline;
	if( m_bold )
		lf.lfWeight = FW_BOLD;

	CFontDialog	dlg( &lf );
	if( dlg.DoModal() == IDOK )
	{
		m_font = dlg.GetFaceName();
		m_pointsize = dlg.GetSize() / 10;
		m_bold = dlg.IsBold();
		m_italic = dlg.IsItalic();
		m_underline = dlg.IsUnderline();
	}
	
}

