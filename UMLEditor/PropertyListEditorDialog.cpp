/* ==========================================================================
	Class :			CPropertyListEditorDialog

	Author :		Johan Rosengren, Abstrakt Mekanik AB

	Date :			2004-06-21

	Purpose :		"CPropertyListEditorDialog" derives from "CDialog" and 
					is a wrapper for the property list editor dialog.

	Description :	A standard Wizard-created dialog box class. Uses the 
					dialog template with the ID "IDD_UML_DIALOG_PROPERTY_LIST_EDITOR" 
					from the resources.

	Usage :			Call to edit the UML-properties of an object.

   ========================================================================
	Changes :		12/7 2004	Added default Save and exit-button that 
								automatically saves the current item and 
								closes the dialog.
   ========================================================================*/

#include "stdafx.h"
#include "PropertyListEditorDialog.h"
#include "DiagramEditor/Tokenizer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyListEditorDialog dialog

CPropertyListEditorDialog::CPropertyListEditorDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CPropertyListEditorDialog::IDD, pParent)
/* ============================================================
	Function :		CPropertyListEditorDialog::CPropertyListEditorDialog
	Description :	Constructor
	Access :		Public
			
	Return :		void
	Parameters :	CWnd* pParent	-	Dialog parent

	Usage :			

   ============================================================*/
{
	//{{AFX_DATA_INIT(CPropertyListEditorDialog)
	m_tag = _T("");
	m_value = _T("");
	//}}AFX_DATA_INIT
}


void CPropertyListEditorDialog::DoDataExchange(CDataExchange* pDX)
/* ============================================================
	Function :		CPropertyListEditorDialog::DoDataExchange
	Description :	MFC data exchange handler.
	Access :		Protected

	Return :		void
	Parameters :	CDataExchange* pDX	-	Pointer to exchange object
					
	Usage :			Called from MFC to exchange and validate 
					dialog data.

   ============================================================*/
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropertyListEditorDialog)
	DDX_Control(pDX, IDC_COMBO_TAG, m_tagCtrl);
	DDX_Control(pDX, IDC_LIST_TAGS, m_tags);
	DDX_Text(pDX, IDC_COMBO_TAG, m_tag);
	DDX_Text(pDX, IDC_EDIT_VALUE, m_value);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropertyListEditorDialog, CDialog)
	//{{AFX_MSG_MAP(CPropertyListEditorDialog)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE, OnButtonUpdate)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, OnButtonClose)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(rwm_EXLISTBOX_SELCHANGE, OnListboxSelChange)
	ON_REGISTERED_MESSAGE(rwm_EXLISTBOX_DELETE, OnListboxDelete)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyListEditorDialog message handlers

void CPropertyListEditorDialog::OnOK() 
/* ============================================================
	Function :		CPropertyListEditorDialog::OnOK
	Description :	Handler for the dialog OK-button.
	Access :		Protected

	Return :		void
	Parameters :	none

	Usage :			Called from MFC.

   ============================================================*/
{

	m_container.RemoveAll();
	int max = m_tags.GetCount();
	for( int t = 0 ; t < max ; t++ )
	{
		CString tag;
		CString value;

		m_tags.GetText( t, tag );
		int tab = tag.Find( _TCHAR( '\t' ) );
		if( tab != -1 )
		{
			value = tag.Right( tag.GetLength() - ( tab + 1 ) );
			tag = tag.Left( tab );
		}

		CProperty* prop = new CProperty( tag, value );
		m_container.Add( prop );

	}

	CDialog::OnOK();
}

void CPropertyListEditorDialog::OnButtonAdd() 
/* ============================================================
	Function :		CPropertyListEditorDialog::OnButtonAdd
	Description :	Handler for the dialog button Add. Adds
					a property.
	Access :		Protected

	Return :		void
	Parameters :	none

	Usage :			Called from MFC. 

   ============================================================*/
{

	UpdateData();
	if( m_tag.IsEmpty() )
	{
		AfxMessageBox( IDS_UML_PROPERTY_LIST_TAG_MUST_HAVE_A_VALUE );
	}
	else
	{

		CString tag( m_tag );
		if( m_value.GetLength() )
			tag += _T( "\t" ) + m_value;
		m_tags.AddString( tag );
		m_tag = _T( "" );
		m_value = _T( "" );
		m_tagCtrl.SetFocus();
		UpdateData( FALSE );

	}
	
}

void CPropertyListEditorDialog::OnButtonDelete() 
/* ============================================================
	Function :		CPropertyListEditorDialog::OnButtonDelete
	Description :	Handler for the dialog button Delete. 
					Deletes the currently selected property.
	Access :		Protected

	Return :		void
	Parameters :	none

	Usage :			Called from MFC.

   ============================================================*/
{

	int index = m_tags.GetCurSel();
	if( index != LB_ERR )
	{

		m_tags.DeleteString( index );

		if( index < m_tags.GetCount() )
		{
			m_tags.SetCurSel( index );
			m_tags.GetText( index, m_tag );
			int tab = m_tag.Find( _TCHAR( '\t' ) );
			if( tab != -1 )
			{
				m_value = m_tag.Right( m_tag.GetLength() - ( tab + 1 ) );
				m_tag = m_tag.Left( tab );
			}

			UpdateData( FALSE );
		}
	}
	
}

void CPropertyListEditorDialog::OnButtonUpdate() 
/* ============================================================
	Function :		CPropertyListEditorDialog::OnButtonUpdate
	Description :	Handler for the dialog button Update. 
					Updates the currently selected property 
					from the edit fields.
	Access :		Protected

	Return :		void
	Parameters :	none

	Usage :			Called from MFC.

   ============================================================*/
{

	int index = m_tags.GetCurSel();
	if( index != LB_ERR )
	{
		UpdateData();

		m_tags.DeleteString( index );

		CString tag( m_tag );
		if( m_value.GetLength() )
			tag += _T( "\t" ) + m_value;
		m_tags.InsertString( index, tag );

		m_tags.SetCurSel( index );

	}	

}

BOOL CPropertyListEditorDialog::OnInitDialog() 
/* ============================================================
	Function :		CPropertyListEditorDialog::OnInitDialog
	Description :	Handler for the "WM_INITDIALOG" message
	Access :		Protected

	Return :		BOOL	-	
	Parameters :	none

	Usage :			Called from MFC

   ============================================================*/
{

	CDialog::OnInitDialog();

	int max = m_container.GetSize();
	for( int t = 0 ; t < max ; t++ )
	{

		CProperty* prop = m_container.GetAt( t );
		CString tag( prop->key );
		if( prop->value.GetLength() )
			tag += _T( "\t" ) + prop->value;

		m_tags.AddString( tag );
		
	}

	return TRUE;

}

void CPropertyListEditorDialog::SetProperties( CPropertyContainer& container )
/* ============================================================
	Function :		CPropertyListEditorDialog::SetProperties
	Description :	Copies "container" to the internal 
					property container.
	Access :		Public

	Return :		void
	Parameters :	CPropertyContainer& container	-	Container to copy from.
					
	Usage :			Call to set data to edit in this dialog.

   ============================================================*/
{

	m_container.Copy( container );

}

CPropertyContainer* CPropertyListEditorDialog::GetProperties()
/* ============================================================
	Function :		CPropertyListEditorDialog::GetProperties
	Description :	Gets a pointer to the internal property
					container.
	Access :		Public

	Return :		CPropertyContainer*	-	Internal property 
											container.
	Parameters :	none

	Usage :			Call to get a pointer to the internal 
					property container.

   ============================================================*/
{

	return &m_container;

}

LRESULT CPropertyListEditorDialog::OnListboxDelete( WPARAM, LPARAM )
/* ============================================================
	Function :		CPropertyListEditorDialog::OnListboxDelete
	Description :	Handler for the registered message 
					"rwm_EXLISTBOX_SELCHANGE".
	Access :		Protected

	Return :		LRESULT		-	Always 0
	Parameters :	WPARAM id	-	Not used
					LPARAM		-	Not used
					
	Usage :			Sent from the "CExListBox" when the user 
					presses DEL and the listbox has the focus.
					Deletes the currently selected property.

   ============================================================*/
{

	OnButtonDelete();
	return 0;

}

LRESULT CPropertyListEditorDialog::OnListboxSelChange( WPARAM, LPARAM )
/* ============================================================
	Function :		CPropertyListEditorDialog::OnListboxSelChange
	Description :	Handler for the registered message 
					"rwm_EXLISTBOX_SELCHANGE"
	Access :		Protected

	Return :		LRESULT		-	Always 0
	Parameters :	WPARAM id	-	Not used
					LPARAM		-	Not used
					
	Usage :			Sent from the "CExListBox" when the 
					selection has changed. We update the edit 
					boxes.

   ============================================================*/
{

	int index = m_tags.GetCurSel();
	if( index != LB_ERR )
	{
		m_tags.GetText( index, m_tag );
		int tab = m_tag.Find( _TCHAR( '\t' ) );
		if( tab != -1 )
		{
			m_value = m_tag.Right( m_tag.GetLength() - ( tab + 1 ) );
			m_tag = m_tag.Left( tab );
		}
		else
			m_value = _T( "" );

		UpdateData( FALSE );

	}

	return 0;

}

void CPropertyListEditorDialog::OnButtonClose() 
/* ============================================================
	Function :		CPropertyListEditorDialog::OnButtonClose
	Description :	Handler for the dialog button Close
	Access :		Protected

	Return :		void
	Parameters :	none

	Usage :			Called from MFC.

   ============================================================*/
{

	UpdateData();
	if( !m_tag.IsEmpty() )
		OnButtonAdd();

	OnOK();

}