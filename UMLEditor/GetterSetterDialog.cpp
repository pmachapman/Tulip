/* ==========================================================================
	Class :			CGetterSetterdialog

	Author :		Johan Rosengren, Abstrakt Mekanik AB

	Date :			2004-06-21

	Purpose :		"CGetterSetterdialog" is derived from "CDialog" and 
					wraps the getter/setter-name dialog.

	Description :	This is a Standard Wizard-created dialog box class.

	Usage :			Instantiate to edit the names of the getter/setter.

   ========================================================================*/

#include "stdafx.h"
#include "GetterSetterDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGetterSetterDialog dialog

CGetterSetterDialog::CGetterSetterDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CGetterSetterDialog::IDD, pParent)
/* ============================================================
	Function :		CGetterSetterDialog::CGetterSetterDialog
	Description :	Constructor
	Access :		Public

	Return :		void
	Parameters :	CWnd* pParent	-	Dialog parent

	Usage :			

   ============================================================*/
{

	//{{AFX_DATA_INIT(CGetterSetterDialog)
	m_getter = _T("");
	m_setter = _T("");
	//}}AFX_DATA_INIT

	m_noset = FALSE;
	m_noget = FALSE;

}


void CGetterSetterDialog::DoDataExchange(CDataExchange* pDX)
/* ============================================================
	Function :		CGetterSetterDialog::DoDataExchange
	Description :	Data exchange handler
	Access :		Protected

	Return :		void
	Parameters :	CDataExchange* pDX	-	Pointer to exchange object
					
	Usage :			Called from MFC to exchange and validate 
					dialog data.

   ============================================================*/
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGetterSetterDialog)
	DDX_Control(pDX, IDC_EDIT_SETTER, m_setterCtrl);
	DDX_Control(pDX, IDC_EDIT_GETTER, m_getterCtrl);
	DDX_Text(pDX, IDC_EDIT_GETTER, m_getter);
	DDX_Text(pDX, IDC_EDIT_SETTER, m_setter);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CGetterSetterDialog, CDialog)
	//{{AFX_MSG_MAP(CGetterSetterDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGetterSetterDialog message handlers

BOOL CGetterSetterDialog::OnInitDialog() 
/* ============================================================
	Function :		CGetterSetterDialog::OnInitDialog
	Description :	Handler for the "WM_INITDIALOG"-message.
	Access :		Protected

	Return :		BOOL	-	Always "TRUE"
	Parameters :	none

	Usage :			Called from MFC.

   ============================================================*/
{

	CDialog::OnInitDialog();
	
	if( m_noset )
		m_setterCtrl.SetReadOnly( TRUE );
	
	if( m_noget )
		m_getterCtrl.SetReadOnly( TRUE );

	return TRUE;  

}

void CGetterSetterDialog::OnOK() 
/* ============================================================
	Function :		CGetterSetterDialog::OnOK
	Description :	Handler for the OK-button.
	Access :		Protected

	Return :		void
	Parameters :	none

	Usage :			Called from MFC. Validates data and closes 
					the dialog.

   ============================================================*/
{

	if( !m_noset && m_setter.IsEmpty() )
	{
		AfxMessageBox( IDS_UML_SETTER_MUST_HAVE_A_NAME );
		m_setterCtrl.SetFocus();
	}
	
	if( !m_noget && m_getter.IsEmpty() )
	{
		AfxMessageBox( IDS_UML_GETTER_MUST_HAVE_A_NAME );
		m_getterCtrl.SetFocus();
	}
	
	CDialog::OnOK();

}
