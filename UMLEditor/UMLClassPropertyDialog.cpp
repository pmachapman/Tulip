/* ==========================================================================
	Class :			CUMLClassPropertyDialog

	Author :		Johan Rosengren, Abstrakt Mekanik AB

	Date :			2004-06-21

	Purpose :		"CUMLClassPropertyDialog" derives from "CDiagramPropertyDlg", 
					used by "CUMLEntity"-derived objects to edit the object 
					attributes.

	Description :	A standard Class-Wizard created dialog box class.

	Usage :			In the "CUMLEntity"-derived class, add a member of 
					the "CUMLInterfacePropertyDialog"-derived class, and call 
					"SetPropertyDialog" in the constructor.

					The dialog is displayed as a modeless dialog. The 
					editor will hide the dialog automatically when another
					object is selected, no special Close-button is 
					necessary.

					The dialog template with the resource id 
					"IDD_UML_DIALOG_PROPERTY_CLASS" 
					must be added to the project.

   ========================================================================*/

#include "stdafx.h"
#include "UMLClassPropertyDialog.h"
#include "UMLEntityClass.h"
#include "ClassAttributePropertyDialog.h"
#include "ClassOperationPropertyDialog.h"
#include "PropertyListEditorDialog.h"
#include "ClassDisplayPropertyDialog.h"
#include "GetterSetterDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUMLClassPropertyDialog dialog

CUMLClassPropertyDialog::CUMLClassPropertyDialog(CWnd* pParent /*=NULL*/)
	: CDiagramPropertyDlg(CUMLClassPropertyDialog::IDD, pParent)
/* ============================================================
	Function :		CUMLClassPropertyDialog::CUMLClassPropertyDialog
	Description :	Constructor
	Access :		Public
					
	Return :		void
	Parameters :	CWnd* pParent	-	Dialog parent

	Usage :			

   ============================================================*/
{

	//{{AFX_DATA_INIT(CUMLClassPropertyDialog)
	m_name = _T("");
	m_stereotype = _T("");
	m_propertylist = _T("");
	//}}AFX_DATA_INIT

}

void CUMLClassPropertyDialog::DoDataExchange(CDataExchange* pDX)
/* ============================================================
	Function :		CUMLClassPropertyDialog::DoDataExchange
	Description :	MFC data exchange handler.
	Access :		Protected

	Return :		void
	Parameters :	CDataExchange* pDX	-	Pointer to exchange object
					
	Usage :			Called from MFC to exchange and validate 
					dialog data.

   ============================================================*/
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUMLClassPropertyDialog)
	DDX_Control(pDX, IDC_LIST_OPERATIONS, m_operation);
	DDX_Control(pDX, IDC_LIST_ATTRIBUTES, m_attribute);
	DDX_Control(pDX, IDC_EDIT_NAME, m_nameCtrl);
	DDX_Text(pDX, IDC_EDIT_NAME, m_name);
	DDX_Text(pDX, IDC_COMBO_STEREOTYPE, m_stereotype);
	DDX_Text(pDX, IDC_EDIT_PROPERTY_LIST, m_propertylist);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUMLClassPropertyDialog, CDialog)
	//{{AFX_MSG_MAP(CUMLClassPropertyDialog)
	ON_BN_CLICKED(IDC_BUTTON_ADD_ATTRIBUTE, OnButtonAddAttribute)
	ON_BN_CLICKED(IDC_BUTTON_ADD_OPERATION, OnButtonAddOperation)
	ON_BN_CLICKED(IDC_BUTTON_FONT, OnButtonFont)
	ON_BN_CLICKED(IDC_BUTTON_COLOR, OnButtonColor)
	ON_BN_CLICKED(IDC_BUTTON_PROPERTY_LIST, OnButtonPropertyList)
	ON_BN_CLICKED(IDC_BUTTON_AUTO_GENERATE, OnButtonAutoGenerate)
	ON_BN_CLICKED(IDC_BUTTON_VISIBILITY, OnButtonVisibility)
	ON_BN_CLICKED(IDC_BUTTON_AUTO_GENERATE2, OnButtonAutoGenerate2)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(rwm_EXLISTBOX_DBLCLICK, OnListboxDblClick)
	ON_REGISTERED_MESSAGE(rwm_EXLISTBOX_DELETE, OnListboxDelete)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUMLClassPropertyDialog message handlers

void CUMLClassPropertyDialog::OnOK() 
/* ============================================================
	Function :		CUMLClassPropertyDialog::OnOK
	Description :	Handler for the dialog OK-button.
	Access :		Protected

	Return :		void
	Parameters :	none

	Usage :			Called from MFC.

   ============================================================*/
{

	CUMLEntityClass* uml = static_cast< CUMLEntityClass* >( GetEntity() );

	UpdateData();
	if( m_name.IsEmpty() )
	{
		AfxMessageBox( IDS_UML_CLASS_MUST_HAVE_A_NAME );
		m_nameCtrl.SetFocus();
		return;
	}

	uml->SetTitle( m_name );
	uml->SetBkColor( m_color );
	uml->SetFont( m_font );

	m_stereotype.Remove( _TCHAR( '«' ) );
	m_stereotype.Remove( _TCHAR( '»' ) );
	m_stereotype.TrimLeft();
	m_stereotype.TrimRight();

	uml->SetStereotype( m_stereotype );
	uml->GetProperties()->Copy( m_properties );

	CreateAttributes();
	CreateOperations();

	ClearListboxes();

	uml->CalcRestraints();

	int t;
	int max;
	int style = ENTITY_TYPE_NONE;

	if( m_stereotype == _T( "interface" ) )
		style |= ENTITY_TYPE_ABSTRACT;
	if( m_stereotype == _T( "utility" ) )
		style |= ENTITY_TYPE_STATIC;

	if( style != ENTITY_TYPE_NONE )
	{
		max = uml->GetOperations();
		for( int t = 0 ; t < max ; t++ )
		{
			COperation* op = uml->GetOperation( t );
			op->maintype |= style;
		}

		max = uml->GetAttributes();
		for( int t = 0 ; t < max ; t++ )
		{
			CAttribute* attr = uml->GetAttribute( t );
			attr->maintype |= style;
		}
	}

	Redraw();
	ShowWindow( SW_HIDE );
	GetRedrawWnd()->SetFocus();

}

void CUMLClassPropertyDialog::OnCancel() 
/* ============================================================
	Function :		CUMLClassPropertyDialog::OnCancel
	Description :	Handler for the dialog Cancel-button.
	Access :		Protected

	Return :		void
	Parameters :	none

	Usage :			Called from MFC.

   ============================================================*/
{

	ClearListboxes();
	CDialog::OnCancel();
	GetRedrawWnd()->SetFocus();

}

void CUMLClassPropertyDialog::SetValues() 
/* ============================================================
	Function :		CUMLClassPropertyDialog::SetValues
	Description :	Sets the fields in the dialog box with the
					values from the attached object.
	Access :		Public

	Return :		void
	Parameters :	none

	Usage :			Call to update the dialog box. Note that 
					this function will be called before the 
					dialog is created.

   ============================================================*/
{

	CUMLEntityClass* uml = static_cast< CUMLEntityClass* >( GetEntity() );

	m_name = uml->GetTitle();
	m_color = uml->GetBkColor();
	m_font = uml->GetFont();
	m_stereotype = uml->GetStereotype();

	m_properties.Copy( *( uml->GetProperties() ) );

	if( m_hWnd && ::IsWindowVisible( m_hWnd ) )
	{
		FillListboxes();
		m_propertylist = m_properties.GetString( STRING_FORMAT_UML );
		UpdateData( FALSE );
	}

}

void CUMLClassPropertyDialog::OnButtonAddAttribute() 
/* ============================================================
	Function :		CUMLClassPropertyDialog::OnButtonAddAttribute
	Description :	Handler for the dialog button Add Attribute
	Access :		Protected

	Return :		void
	Parameters :	none

	Usage :			Called from MFC.

   ============================================================*/
{

	CClassAttributePropertyDialog	dlg;
	if( dlg.DoModal() == IDOK )
	{
		CAttribute* obj = dlg.GetAttribute();
		CString output = obj->ToString( FALSE );
		int index = m_attribute.AddString( output );
		m_attribute.SetItemData( index, ( DWORD ) obj );
		m_attribute.SetCurSel( index );
		m_attribute.SetFocus();
	}
	else
		delete dlg.GetAttribute();

}

void CUMLClassPropertyDialog::EditAttribute() 
/* ============================================================
	Function :		CUMLClassPropertyDialog::EditAttribute
	Description :	Edits the selected attribute in the 
					attribute list.
	Access :		Private

	Return :		void
	Parameters :	none

	Usage :			Call to edit the current attribute. The Edit 
					attribute dialog will be displayed.

   ============================================================*/
{

	int index = m_attribute.GetCurSel();
	if( index != LB_ERR )
	{
		CAttribute* obj = reinterpret_cast< CAttribute* >( m_attribute.GetItemData( index ) );
		if( obj )
		{
			CClassAttributePropertyDialog	dlg;
			dlg.SetAttribute( obj );
			if( dlg.DoModal() == IDOK )
				RefreshListboxes();
			m_attribute.SetCurSel( index );
			m_attribute.SetFocus();
		}
	}	

}

void CUMLClassPropertyDialog::DeleteAttribute() 
/* ============================================================
	Function :		CUMLClassPropertyDialog::DeleteAttribute
	Description :	Deletes the current attribute in the attribute list.
	Access :		Private

	Return :		void
	Parameters :	none

	Usage :			Call to delete the current attribute in the 
					attribute list.

   ============================================================*/
{

	int index = m_attribute.GetCurSel();
	if( index != LB_ERR )
	{
		if( AfxMessageBox( IDS_UML_DELETE_ATTRIBUTE, MB_YESNO ) == IDYES )
		{
			CAttribute* obj = reinterpret_cast< CAttribute* >( m_attribute.GetItemData( index ) );
			m_attribute.DeleteString( index );
			delete obj;
			m_attribute.SetCurSel( index );
			m_attribute.SetFocus();
		}
	}

}

void CUMLClassPropertyDialog::OnButtonAddOperation() 
/* ============================================================
	Function :		CUMLClassPropertyDialog::OnButtonAddOperation
	Description :	Handler for the dialog button Add Operation
	Access :		Protected

	Return :		void
	Parameters :	none

	Usage :			Called from MFC.

   ============================================================*/
{

	CClassOperationPropertyDialog	dlg;
	if( dlg.DoModal() == IDOK )
	{
		COperation* obj = dlg.GetOperation();
		CString output = obj->ToString( FALSE, FALSE );
		int index = m_operation.AddString( output );
		m_operation.SetItemData( index, ( DWORD ) obj );
		m_operation.SetCurSel( index );
		m_operation.SetFocus();
	}
	else
		delete dlg.GetOperation();

}

void CUMLClassPropertyDialog::EditOperation() 
/* ============================================================
	Function :		CUMLClassPropertyDialog::EditOperation
	Description :	Edits the selected operation in the 
					operation list.
	Access :		Private

	Return :		void
	Parameters :	none

	Usage :			Call to edit the selected operation in the 
					operation list. The Edit operation dialog 
					will be displayed.

   ============================================================*/
{

	int index = m_operation.GetCurSel();
	if( index != LB_ERR )
	{
		COperation* obj = reinterpret_cast< COperation* >( m_operation.GetItemData( index ) );
		if( obj )
		{
			CClassOperationPropertyDialog	dlg;
			dlg.SetOperation( obj );
			if( dlg.DoModal() == IDOK )
				RefreshListboxes();
			m_operation.SetCurSel( index );
			m_operation.SetFocus();
		}
	}	

}

void CUMLClassPropertyDialog::DeleteOperation() 
/* ============================================================
	Function :		CUMLClassPropertyDialog::DeleteOperation
	Description :	Delete the currently selected operation in 
					the operation list.
	Access :		

	Return :		void
	Parameters :	none

	Usage :			Call to delete the currently selected 
					operation in the operation list.

   ============================================================*/
{

	int index = m_operation.GetCurSel();
	if( index != LB_ERR )
	{
		if( AfxMessageBox( IDS_UML_DELETE_OPERATION, MB_YESNO ) == IDYES )
		{
			COperation* obj = reinterpret_cast< COperation* >( m_operation.GetItemData( index ) );
			m_operation.DeleteString( index );
			delete obj;
			m_operation.SetCurSel( index );
			m_operation.SetFocus();
		}
	}

}

void CUMLClassPropertyDialog::OnButtonFont() 
/* ============================================================
	Function :		CUMLClassPropertyDialog::OnButtonFont
	Description :	Handler for the dialog button Font
	Access :		Protected

	Return :		void
	Parameters :	none

	Usage :			Called from MFC.

   ============================================================*/
{

	CFont font;
	CUMLEntityClass* uml = static_cast< CUMLEntityClass* >( GetEntity() );
	font.CreatePointFont( 120, uml->GetFont() );
	LOGFONT lf;
	font.GetLogFont( &lf );
	CFontDialog	dlg( &lf );
	if( dlg.DoModal() == IDOK )
	{
		m_font = dlg.GetFaceName();
		Redraw();
	}
	
}

void CUMLClassPropertyDialog::OnButtonColor() 
/* ============================================================
	Function :		CUMLClassPropertyDialog::OnButtonColor
	Description :	Handler for the dialog button Color
	Access :		Protected

	Return :		void
	Parameters :	none

	Usage :			Called from MFC.

   ============================================================*/
{

	CUMLEntityClass* uml = static_cast< CUMLEntityClass* >( GetEntity() );
	COLORREF color = uml->GetBkColor();
	CColorDialog	dlg( color );
	if( dlg.DoModal() == IDOK )
	{
		m_color = dlg.GetColor();
		Redraw();
	}
	
}

void CUMLClassPropertyDialog::FillListboxes()
/* ============================================================
	Function :		CUMLClassPropertyDialog::FillListboxes
	Description :	Fills the attribute and operation listboxes 
					from the attached "CUMLEntityClass" object.
	Access :		Private

	Return :		void
	Parameters :	none

	Usage :			Call to (re)fill the listboxes.

   ============================================================*/
{

	ClearListboxes();
	CUMLEntityClass* uml = static_cast< CUMLEntityClass* >( GetEntity() );
	int max = uml->GetAttributes();
	for( int t = 0 ; t < max ; t++ )
	{
		CAttribute* obj = uml->GetAttribute( t );
		CString output = obj->ToString( FALSE );
		int index = m_attribute.AddString( output );
		m_attribute.SetItemData( index, ( DWORD ) obj->Clone() );
	}

	max = uml->GetOperations();
	for( int t = 0 ; t < max ; t++ )
	{
		COperation* obj = uml->GetOperation( t );
		CString output = obj->ToString( FALSE, FALSE );
		int index = m_operation.AddString( output );
		m_operation.SetItemData( index, ( DWORD ) obj->Clone() );
	}

}

void CUMLClassPropertyDialog::ClearListboxes()
/* ============================================================
	Function :		CUMLClassPropertyDialog::ClearListboxes
	Description :	Clears the attribute and operation 
					listboxes.
	Access :		Private

	Return :		void
	Parameters :	none

	Usage :			Call to clear the listboxes.

   ============================================================*/
{

	int max = m_attribute.GetCount();
	for( int t = 0 ; t < max ; t++ )
	{
		CAttribute* obj = reinterpret_cast< CAttribute* >( m_attribute.GetItemData( t ) );
		delete obj;
	}

	max = m_operation.GetCount();
	for( int t = 0 ; t < max ; t++ )
	{
		COperation* obj = reinterpret_cast< COperation* >( m_operation.GetItemData( t ) );
		delete obj;
	}

	m_attribute.ResetContent();
	m_operation.ResetContent();

}

void CUMLClassPropertyDialog::RefreshListboxes()
/* ============================================================
	Function :		CUMLClassPropertyDialog::RefreshListboxes
	Description :	Refills the attribute and operation 
					listboxes from the internal attribute- 
					and operation containers.
	Access :		Private

	Return :		void
	Parameters :	none

	Usage :			Call to refill the listboxes with internal 
					data.

   ============================================================*/
{

	int max = m_attribute.GetCount();
	for( int t = 0 ; t < max ; t++ )
	{
		CAttribute* obj = reinterpret_cast< CAttribute* >( m_attribute.GetItemData( t ) );
		CString output = obj->ToString( FALSE );
		m_attribute.DeleteString( t );
		int index = m_attribute.InsertString( t, output );
		m_attribute.SetItemData( index, ( DWORD ) obj );
	}

	max = m_operation.GetCount();
	for( int t = 0 ; t < max ; t++ )
	{
		COperation* obj = reinterpret_cast< COperation* >( m_operation.GetItemData( t ) );
		CString output = obj->ToString( FALSE, FALSE );
		m_operation.DeleteString( t );
		int index = m_operation.InsertString( t, output );
		m_operation.SetItemData( index, ( DWORD ) obj );
	}

}

LRESULT CUMLClassPropertyDialog::OnListboxDblClick( WPARAM id, LPARAM )
/* ============================================================
	Function :		CUMLClassPropertyDialog::OnListboxDblClick
	Description :	Mapped to the registered message 
					"rwm_EXLISTBOX_DBLCLICK".
	Access :		Protected

	Return :		LRESULT		-	Always 0
	Parameters :	WPARAM id	-	ID of "CExListBox" sending this.
					LPARAM		-	Not used
					
	Usage :			Sent from "CExListBox" when the user 
					double-clicks a line. Can come from either 
					the attribute- or operation listbox.

   ============================================================*/
{

	if( static_cast< int >( id ) == m_attribute.GetDlgCtrlID() )
		EditAttribute();
	if( static_cast< int >( id ) == m_operation.GetDlgCtrlID() )
		EditOperation();

	return 0;

}

LRESULT CUMLClassPropertyDialog::OnListboxDelete( WPARAM id, LPARAM )
/* ============================================================
	Function :		CUMLClassPropertyDialog::OnListboxDelete
	Description :	Mapped to the registered message 
					"rwm_EXLISTBOX_DELETE"
	Access :		Protected

	Return :		LRESULT		-	Always 0
	Parameters :	WPARAM id	-	ID of "CExListBox" sending this.
					LPARAM		-	Not used
					
	Usage :			Sent from "CExListBox" when the user 
					presses the DEL-key. Can come from either 
					the attribute- or operation listbox.

   ============================================================*/
{

	if( static_cast< int >( id ) == m_attribute.GetDlgCtrlID() )
		DeleteAttribute();
	if( static_cast< int >( id ) == m_operation.GetDlgCtrlID() )
		DeleteOperation();

	return 0;

}

void CUMLClassPropertyDialog::OnButtonPropertyList() 
/* ============================================================
	Function :		CUMLClassPropertyDialog::OnButtonPropertyList
	Description :	Handler for the dialog button Property List
	Access :		Protected

	Return :		void
	Parameters :	none

	Usage :			Called from MFC.

   ============================================================*/
{

	UpdateData();
	CPropertyListEditorDialog	dlg;

	dlg.SetProperties( m_properties );
	if( dlg.DoModal() == IDOK )
	{
		m_properties.Copy( *( dlg.GetProperties() ) );
		m_propertylist = m_properties.GetString( STRING_FORMAT_UML );
		UpdateData( FALSE );
	}
	
}

void CUMLClassPropertyDialog::OnButtonAutoGenerate() 
/* ============================================================
	Function :		CUMLClassPropertyDialog::OnButtonAutoGenerate
	Description :	Handler for the dialog button Generate 
					getters/setters
	Access :		Protected

	Return :		void
	Parameters :	none

	Usage :			Called from MFC.

   ============================================================*/
{

	int index = m_attribute.GetCurSel();
	if( index != LB_ERR )
	{
		CAttribute* attr = reinterpret_cast< CAttribute* >( m_attribute.GetItemData( index ) );
		if( attr )
		{
			if( AfxMessageBox( IDS_UML_CREATEGETTERS_SETTERS, MB_YESNO ) == IDYES )
			{

				CString name = attr->name;
				if( name.GetLength() > 2 && name.Left( 2 ) == _T( "m_" ) )
					name = name.Right( name.GetLength() - 2 );
				name = _TCHAR( _totupper( name[ 0 ] ) ) + name.Right( name.GetLength() - 1 );
				CString getName( _T( "Get" ) + name );
				CString setName( _T( "Set" ) + name );

				BOOL getFound = FALSE;
				BOOL setFound = FALSE;

				CreateAttributes();
				CreateOperations();

				CUMLEntityClass* uml = static_cast< CUMLEntityClass* >( GetEntity() );
				int size = uml->GetOperations();
				for( int i = 0 ; i < size ; i++ )
				{
					COperation* op = uml->GetOperation( i );
					if( op->getter && op->getsetvariable == attr->name )
						getFound = TRUE;
					if( op->setter && op->getsetvariable == attr->name )
						setFound = TRUE;
				}

				if( !getFound || !setFound )
				{
					CGetterSetterDialog	dlg;
					if( !getFound )
						dlg.m_getter = getName;
					else
						dlg.m_noget = TRUE;
					if( !setFound )
						dlg.m_setter = setName;
					else
						dlg.m_noset = TRUE;

					if( dlg.DoModal() == IDOK )
					{
						BOOL added = FALSE;
						if( !dlg.m_noget && dlg.m_getter.GetLength() )
						{
							COperation* op = new COperation;
							op->name = dlg.m_getter;
							op->type = attr->type;
							op->access = ACCESS_TYPE_PUBLIC;
							op->properties.Add( _T( "query" ) );
							op->getter = TRUE;
							op->getsetvariable = attr->name;
							uml->AddOperation( op );
							added = TRUE;
						}

						if( !dlg.m_noset && dlg.m_setter.GetLength() )
						{
							COperation* op = new COperation;
							op->name = dlg.m_setter;
							op->access = ACCESS_TYPE_PUBLIC;
							CParameter* parameter = new CParameter;
							parameter->name = _T( "value" );
							parameter->type = attr->type;
							op->parameters.Add( parameter );

							op->setter = TRUE;
							op->getsetvariable = attr->name;
							uml->AddOperation( op );
							added = TRUE;
						}

						if( added )
							FillListboxes();
					}
				}
				else
					AfxMessageBox( IDS_UML_GETSET_DEFINED );
			}
		}
	}
	else
		AfxMessageBox( IDS_UML_MUST_SELECT_AN_ATTRIBUTE );

	m_attribute.SetCurSel( index );
	m_attribute.SetFocus();

}

void CUMLClassPropertyDialog::CreateAttributes()
/* ============================================================
	Function :		CUMLClassPropertyDialog::CreateAttributes
	Description :	Creates the edited attributes in the 
					attached "CUMLEntityClass" object.
	Access :		Private

	Return :		void
	Parameters :	none

	Usage :			Call to update the attached object.

   ============================================================*/
{

	CUMLEntityClass* uml = static_cast< CUMLEntityClass* >( GetEntity() );

	// Wipe current attributes from the class.
	uml->ClearAttributes();

	// Re-add from listbox.
	int max = m_attribute.GetCount();
	for( int t = 0 ; t < max ; t++ )
	{
		CAttribute* obj = reinterpret_cast< CAttribute* >( m_attribute.GetItemData( t ) );
		if( obj )
			uml->AddAttribute( obj->Clone() );
	}

}

void CUMLClassPropertyDialog::CreateOperations()
/* ============================================================
	Function :		CUMLClassPropertyDialog::CreateOperations
	Description :	Creates the edited operations in the 
					attached "CUMLEntityClass" object.
	Access :		Private

	Return :		void
	Parameters :	none

	Usage :			Call to update the attached object.

   ============================================================*/
{

	CUMLEntityClass* uml = static_cast< CUMLEntityClass* >( GetEntity() );

	// Wipe current operations from the class.
	uml->ClearOperations();

	int max = m_operation.GetCount();
	for( int t = 0 ; t < max ; t++ )
	{
		COperation* obj = reinterpret_cast< COperation* >( m_operation.GetItemData( t ) );
		if( obj )
			uml->AddOperation( obj->Clone() );
	}

}

void CUMLClassPropertyDialog::OnButtonVisibility() 
/* ============================================================
	Function :		CUMLClassPropertyDialog::OnButtonVisibility
	Description :	Handler for the dialog button Visibility
	Access :		Protected

	Return :		void
	Parameters :	none

	Usage :			Called from MFC.

   ============================================================*/
{

	CUMLEntityClass* uml = static_cast< CUMLEntityClass* >( GetEntity() );

	CClassDisplayPropertyDialog	dlg;
	int displayOptions = uml->GetDisplayOptions();

	dlg.m_noattributenames = ( displayOptions & DISPLAY_NO_OPERATION_ATTRIBUTE_NAMES ) != 0;
	dlg.m_noattributes = ( displayOptions & DISPLAY_NO_ATTRIBUTES ) != 0;
	dlg.m_nomarkers = ( displayOptions & DISPLAY_NO_MARKERS ) != 0;
	dlg.m_nooperations = ( displayOptions & DISPLAY_NO_OPERATIONS ) != 0;
	dlg.m_onlypublic = ( displayOptions & DISPLAY_ONLY_PUBLIC ) != 0;
	if( dlg.DoModal() )
	{

		displayOptions = 0;
		if( dlg.m_noattributenames )
			displayOptions |= DISPLAY_NO_OPERATION_ATTRIBUTE_NAMES;
		if( dlg.m_noattributes )
			displayOptions |= DISPLAY_NO_ATTRIBUTES;
		if( dlg.m_nomarkers )
			displayOptions |= DISPLAY_NO_MARKERS;
		if( dlg.m_nooperations )
			displayOptions |= DISPLAY_NO_OPERATIONS;
		if( dlg.m_onlypublic )
			displayOptions |= DISPLAY_ONLY_PUBLIC;

		uml->SetDisplayOptions( displayOptions );

	}

}

void CUMLClassPropertyDialog::OnButtonAutoGenerate2() 
/* ============================================================
	Function :		CUMLClassPropertyDialog::OnButtonAutoGenerate2
	Description :	Handler for the dialog button Generate 
					ctor/dtor
	Access :		Protected

	Return :		void
	Parameters :	none

	Usage :			Called from MFC.

   ============================================================*/
{

	if( AfxMessageBox( IDS_UML_CREATECTOR_DTOR, MB_YESNO ) == IDYES )
	{
		UpdateData();
		CreateAttributes();
		CreateOperations();

		CUMLEntityClass* uml = static_cast< CUMLEntityClass* >( GetEntity() );
		COperation* op = new COperation;
		op->name = m_name;
		op->access = ACCESS_TYPE_PUBLIC;
		uml->AddOperation( op );

		op = new COperation;
		op->name = _T( "~" ) + m_name;
		op->access = ACCESS_TYPE_PUBLIC;
		op->maintype = ENTITY_TYPE_VIRTUAL;
		op->properties.Add( _T( "virtual" ) );
		uml->AddOperation( op );

		FillListboxes();

	}

}
