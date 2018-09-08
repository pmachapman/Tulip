/* ==========================================================================
	CLabelPropertyDialog

	Author :		Johan Rosengren, Abstrakt Mekanik AB

	Date :			2004-04-29

	Purpose :		CLabelPropertyDialog is a dialog box wrapper derived
					from CDiagramPropertyDlg, used by CFlowchartEntity-
					derived objects to edit the object title attribute.

	Description :	Class-Wizard created class.

	Usage :			In the CFlowchartEntity-derived class, add a member of
					the CLabelPropertyDialog-derived class, and call
					SetPropertyDialog in the constructor.

					The dialog is displayed as a modeless dialog. The
					editor will hide the dialog automatically when another
					object is selected, no special Close-button is
					necessary.

					The dialog template with the resource id
					IDD_DIALOG_PROPERTY_LABEL must be added to the project.
   ========================================================================*/

#include "stdafx.h"
#include "LabelPropertyDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLabelPropertyDialog dialog

CLabelPropertyDialog::CLabelPropertyDialog(CWnd* pParent /*=NULL*/)
	: CDiagramPropertyDlg(CLabelPropertyDialog::IDD, pParent)
	/* ============================================================
		Function :		CLabelPropertyDialog::CLabelPropertyDialog
		Description :	constructor

		Return :		void
		Parameters :	none

		Usage :

	   ============================================================*/
{

	//{{AFX_DATA_INIT(CPropertyDialog)
	m_text = _T("");
	//}}AFX_DATA_INIT

}

CLabelPropertyDialog::~CLabelPropertyDialog()
/* ============================================================
	Function :		CLabelPropertyDialog::~CLabelPropertyDialog
	Description :	destructor

	Return :		void
	Parameters :	none

	Usage :

   ============================================================*/
{
}

void CLabelPropertyDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLabelPropertyDialog)
	DDX_Text(pDX, IDC_EDIT_TEXT, m_text);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLabelPropertyDialog, CDialog)
	//{{AFX_MSG_MAP(CLabelPropertyDialog)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLabelPropertyDialog message handlers

void CLabelPropertyDialog::OnOK()
/* ============================================================
	Function :		CLabelPropertyDialog::OnOK
	Description :	Called when the Apply-key is pressed.

	Return :		void
	Parameters :	none

	Usage :			Called from MFC. Updates the attached object
					and hides the modeless dialog.

   ============================================================*/
{

	UpdateData();
	GetEntity()->SetTitle(m_text);
	Redraw();
	ShowWindow(SW_HIDE);
	GetRedrawWnd()->SetFocus();

}

void CLabelPropertyDialog::OnCancel()
/* ============================================================
	Function :		CLabelPropertyDialog::OnCancel
	Description :	Called when the ESC-key is pressed.

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
// CLabelPropertyDialog overrides

void CLabelPropertyDialog::SetValues()
/* ============================================================
	Function :		CLabelPropertyDialog::SetValues
	Description :	Set the values in the dialog from the
					attached object.

	Return :		void
	Parameters :	none

	Usage :			Will be called by the framework and the
					attached object to initialize the dialog.
					The editbox is filled with the contents of
					the object title attribute.

   ============================================================*/
{

	m_text = GetEntity()->GetTitle();

}
