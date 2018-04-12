/* ==========================================================================
	Class :			CAddParameterDialog

	Author :		Johan Rosengren, Abstrakt Mekanik AB

	Date :			2004-06-14

	Purpose :		Dialog box class to edit or create a single parameter	

	Description :	The class is a standard MFC dialog class, using the 
					"IDD_UML_DIALOG_PROPERTY_CLASS_OPERATION_PARAMETER" dialog 
					resource.

	Usage :			Call "SetParameter" to set the parameter being edited 
					(or don't set if creating a new one). "GetParameter" 
					returns the edited/created parameter.

					See "CParameter" for a description of the parameter 
					class.

   ========================================================================*/

#include "stdafx.h"
#include "AddParameterDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAddParameterDialog dialog

CAddParameterDialog::CAddParameterDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CAddParameterDialog::IDD, pParent)
/* ============================================================
	Function :		CAddParameterDialog::CAddParameterDialog
	Description :	Constructor
	Access :		Public

	Return :		void
	Parameters :	CWnd* pParent	-	Dialog parent

	Usage :			Created in the Add/Edit operation dialog.

   ============================================================*/
{
	//{{AFX_DATA_INIT(CAddParameterDialog)
	m_name = _T("");
	m_type = _T("");
	m_defaultvalue = _T("");
	m_in = FALSE;
	m_out = FALSE;
	m_reference = FALSE;
	//}}AFX_DATA_INIT

	m_parameter = NULL;

}


void CAddParameterDialog::DoDataExchange(CDataExchange* pDX)
/* ============================================================
	Function :		CAddParameterDialog::DoDataExchange
	Description :	Data exchange call for the dialog
	Access :		Protected
					
	Return :		void
	Parameters :	CDataExchange* pDX	-	Pointer to exchange object
					
	Usage :			Called from MFC to exchange and validate 
					dialog data.

   ============================================================*/
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddParameterDialog)
	DDX_Control(pDX, IDC_EDIT_TYPE, m_typeCtrl);
	DDX_Control(pDX, IDC_EDIT_NAME, m_nameCtrl);
	DDX_Text(pDX, IDC_EDIT_NAME, m_name);
	DDX_Text(pDX, IDC_EDIT_TYPE, m_type);
	DDX_Text(pDX, IDC_EDIT_INITIAL_VALUE, m_defaultvalue);
	DDX_Check(pDX, IDC_CHECK_IN, m_in);
	DDX_Check(pDX, IDC_CHECK_OUT, m_out);
	DDX_Check(pDX, IDC_CHECK_REFERENCE, m_reference);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAddParameterDialog, CDialog)
	//{{AFX_MSG_MAP(CAddParameterDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAddParameterDialog message handlers

void CAddParameterDialog::OnOK() 
/* ============================================================
	Function :		CAddParameterDialog::OnOK
	Description :	Handler for the OK button.
	Access :		Protected

	Return :		void
	Parameters :	none

	Usage :			Called from MFC. Checks the data and updates
					the parameter.

   ============================================================*/
{

	UpdateData();

	if( m_name.IsEmpty() )
	{
		AfxMessageBox( IDS_UML_PARAMETER_MUST_HAVE_A_NAME );
		m_nameCtrl.SetFocus();
		return;
	}

	if( m_type.IsEmpty() )
	{
		AfxMessageBox( IDS_UML_PARAMETER_MUST_HAVE_A_TYPE );
		m_typeCtrl.SetFocus();
		return;
	}

	m_parameter->name = m_name;
	m_parameter->type = m_type;
	m_parameter->defaultvalue = m_defaultvalue;
	m_parameter->in = m_in;
	m_parameter->out = m_out;
	m_parameter->reference = m_reference;

	CDialog::OnOK();

}

BOOL CAddParameterDialog::OnInitDialog() 
/* ============================================================
	Function :		CAddParameterDialog::OnInitDialog
	Description :	Handler for the "WM_INITDIALOG" message
	Access :		Protected

	Return :		BOOL	-	Always "TRUE".
	Parameters :	none

	Usage :			Assign data to the fields from the submitted 
					parameter, or creates a new parameter as 
					appropriate.

   ============================================================*/
{

	if( m_parameter )
	{
		m_name = m_parameter->name;
		m_type = m_parameter->type;
		m_defaultvalue = m_parameter->defaultvalue;
		m_in = m_parameter->in;
		m_out = m_parameter->out;
		m_reference = m_parameter->reference;

	}
	else
		m_parameter = new CParameter;

	CDialog::OnInitDialog();
	
	
	return TRUE; 
}

void CAddParameterDialog::SetParameter( CParameter* parameter )
/* ============================================================
	Function :		CAddParameterDialog::SetParameter
	Description :	Sets the current parameter being edited
	Access :		Public
					
	Return :		void
	Parameters :	CParameter* parameter	-	Parameter
					
	Usage :			Call to set the parameter to edit. If no 
					parameter is set, the dialog will create 
					one. The caller has the responsibility of 
					deleting it.

   ============================================================*/
{

	m_parameter = parameter;

}

CParameter* CAddParameterDialog::GetParameter()
/* ============================================================
	Function :		CAddParameterDialog::GetParameter
	Description :	Gets the parameter being created/edited
	Access :		Public
					
	Return :		CParameter*	-	Parameter, "NULL" if none.
	Parameters :	none

	Usage :			Call to get the resulting parameter

   ============================================================*/
{

	return m_parameter;

}