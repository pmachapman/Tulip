/* ==========================================================================
	Class :			CUMLEntityClass

	Author :		Johan Rosengren, Abstrakt Mekanik AB

	Date :			2004-04-29

	Purpose :		"CUMLEntityClass", derived from "CUMLEntity", 
					represents an UML class box. 	

	Description :	The class implements the necessary functions for 
					cloning and creation from text ("Clone" and 
					"CreateFromString"). It draws a rectangle using the 
					title attribute of "CDiagramEntity" as the text 
					contents. The box can link from all four sides.
					The class contains separate arrays for the properties, 
					operations and attributes of this class ("CPropertyContainer", 
					"COperationContainer" and "CAttributeContainer" 
					respectively).

	Usage :			When loading, create with "CUMLControlFactory::CreateFromString". 
					Otherwise, create instances in the application view or dialog and 
					add them to the editor with "StartDrawingObject". The 
					editor takes responsibility for the object.
   ========================================================================
	Changes :		8/7 2004	Changed base class handling in c++-export 
								from "CString" to "CStringArray" to support 
								multiple inheritance
					9/7 2004	Setting baseclass(es) to baseClass property 
								when importing class from h-file.
					12/7 2004	Checking for void return type (n0mer)
   ========================================================================
					27/8 2004	Removes prefixing "ClassName::" in function 
								names while importing a class from a h-file.
   ========================================================================*/

#include "stdafx.h"
#include "UMLEntityClass.h"
#include "UMLEntityContainer.h"
#include "StringHelpers.h"

#include "TextFile/TextFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CUMLEntityClass::CUMLEntityClass()
/* ============================================================
	Function :		CUMLEntityClass::CUMLEntityClass
	Description :	Constructor
	Access :		Public
					
	Return :		void
	Parameters :	none

	Usage :			Create either through "CUMLControlFactory" or
					by calling "new" and adding the pointer to the
					editor via "StartDrawingObject"

   ============================================================*/
{

	SetDefaultSize( CSize( 128, 64 ) );
	SetConstraints( GetDefaultSize(), CSize( -1, -1 ) );
	SetBkColor( RGB( 255, 255, 217 ) );
	SetType( _T( "uml_class" ) );

	CString title;
	title.LoadString( IDS_UML_CLASS );
	SetTitle( title );

	SetPropertyDialog( &m_dlg, CUMLClassPropertyDialog::IDD );

	CalcRestraints();

}

CUMLEntityClass::~CUMLEntityClass()
/* ============================================================
	Function :		CUMLEntityClass::~CUMLEntityClass
	Description :	Destructor
	Access :		Public
					
	Return :		void
	Parameters :	none

	Usage :			Note that objects will normally be deleted by
					the container.

   ============================================================*/
{

	if( m_dlg.m_hWnd )
		m_dlg.DestroyWindow();

	ClearAttributes();
	ClearOperations();
	ClearProperties();

}

void CUMLEntityClass::Draw( CDC* dc, CRect rect )
/* ============================================================
	Function :		CUMLEntityClass::Draw
	Description :	Draws the object.
	Access :		Public

	Return :		void
	Parameters :	CDC* dc		-	The "CDC" to draw to. 
					CRect rect	-	The real rectangle of the 
									object.

	Usage :			The function should clean up all selected 
					objects. Note that the "CDC" is a memory "CDC", 
					so creating a memory "CDC" in this function 
					will probably not speed up the function.

   ============================================================*/
{

	int cut = round( static_cast< double >( GetMarkerSize().cx ) * GetZoom() );
	CRect r( rect.left + cut, rect.top + cut / 2, rect.right - cut, rect.bottom - cut / 2 );

	////////////////////////////////////////////////////////////////////
	// The box
	//

	dc->SelectStockObject( BLACK_PEN );
	CBrush bk;
	bk.CreateSolidBrush( GetBkColor() );
	dc->SelectObject( &bk );

	int mode = dc->SetBkMode( TRANSPARENT );
	int height = round( 12.0 * GetZoom() );

	dc->Rectangle( rect );

	////////////////////////////////////////////////////////////////////
	// Set up
	//

	CFont font;
	font.CreateFont( -height, 0,0,0,FW_NORMAL,0,0,0,0,0,0,0,0, GetFont() );
	CFont fontItalic;
	fontItalic.CreateFont( -height, 0,0,0,FW_NORMAL,TRUE,0,0,0,0,0,0,0, GetFont() );
	CFont fontUnderline;
	fontUnderline.CreateFont( -height, 0,0,0,FW_NORMAL,0,TRUE,0,0,0,0,0,0, GetFont() );
	CFont fontBold;
	fontBold.CreateFont( -height, 0,0,0,FW_BOLD,0,0,0,0,0,0,0,0, GetFont() );
	CFont fontBoldItalic;
	fontBoldItalic.CreateFont( -height, 0,0,0,FW_BOLD,TRUE,0,0,0,0,0,0,0, GetFont() );
	CFont fontBoldUnderline;
	fontBoldUnderline.CreateFont( -height, 0,0,0,FW_BOLD,0,TRUE,0,0,0,0,0,0, GetFont() );
	CFont fontUnderlineItalic;
	fontUnderlineItalic.CreateFont( -height, 0,0,0,FW_NORMAL,TRUE,TRUE,0,0,0,0,0,0, GetFont() );

	height = round( 14.0 * GetZoom() );

	CFont* oldfont = NULL;
	oldfont = dc->SelectObject( &fontBold );

	////////////////////////////////////////////////////////////////////
	// Title
	//

	int lines = 1;
	CString stereotype = GetStereotype();
	if( stereotype.GetLength() )
	{
		stereotype = _T( "« " ) + stereotype + _T( " »" );
		dc->SelectObject( &font );
		dc->DrawText( stereotype, r, DT_SINGLELINE | DT_NOPREFIX | DT_CENTER );
		lines++;
		r.top += height;
	}

	BOOL abstract = IsAbstract();

	if( abstract )
		dc->SelectObject( &fontBoldItalic );
	else
		dc->SelectObject( &fontBold );

	CRect textRect( r );
	textRect.bottom = textRect.top + height;
	dc->DrawText( GetTitle(), textRect, DT_SINGLELINE | DT_NOPREFIX | DT_CENTER );

	CString propertylist = m_properties.GetString( STRING_FORMAT_UML );
	if( propertylist.GetLength() )
	{
		dc->SelectObject( &font );
		propertylist.Replace( _T( ", " ) , _T( "\r\n" ) );
		r.top += height;
		textRect = r;
		textRect.bottom = textRect.top + m_properties.GetSize() * height;
		dc->DrawText( propertylist, textRect, DT_NOPREFIX | DT_WORDBREAK | DT_CENTER );
		lines+= m_properties.GetSize();
		r.top += height * m_properties.GetSize();
	}

	////////////////////////////////////////////////////////////////////
	// Attributes
	//

	dc->MoveTo( rect.left, rect.top + height * lines + cut );
	dc->LineTo( rect.right, rect.top + height * lines + cut );

	int max = GetAttributes();
	int top = rect.top + cut + height * lines;
	if( !( GetDisplayOptions() & DISPLAY_NO_ATTRIBUTES ) )
	{
		if( max )
		{
			for( int t = 0 ; t < max ; t++ )
			{
				r.SetRect( rect.left + cut / 2, top, rect.right - cut / 2, top + height );
				CAttribute* obj = GetAttribute( t );
				if( !( GetDisplayOptions() & DISPLAY_ONLY_PUBLIC ) || obj->access == ACCESS_TYPE_PUBLIC )
				{
					CString output = obj->ToString( ( GetDisplayOptions() & DISPLAY_NO_MARKERS ) );
					if( obj->maintype & ENTITY_TYPE_STATIC && obj->maintype & ENTITY_TYPE_ABSTRACT )
						dc->SelectObject( &fontUnderlineItalic );
					else if( obj->maintype & ENTITY_TYPE_STATIC )
						dc->SelectObject( &fontUnderline );
					else if( obj->maintype & ENTITY_TYPE_ABSTRACT )
						dc->SelectObject( &fontItalic );
					else
						dc->SelectObject( &font );

					dc->DrawText( output, r, DT_SINGLELINE | DT_NOPREFIX );
					top += height;
				}
			}

		}
	}

	if( GetAttributes() != GetVisibleAttributes() )
	{
		dc->SelectObject( &font );
		r.SetRect( rect.left + cut / 2, top, rect.right - cut / 2, top + height );
		dc->DrawText( CString( _T( "..." ) ), r, DT_SINGLELINE | DT_NOPREFIX );
		top += height;
	}

	if( GetAttributes() == 0 )
		top += height; // Empty dummy line

	top += cut / 2;
	dc->MoveTo( rect.left, top );
	dc->LineTo( rect.right, top );

	////////////////////////////////////////////////////////////////////
	// Operations
	//

	top += cut / 2;
	max = GetOperations();
	if( !( GetDisplayOptions() & DISPLAY_NO_OPERATIONS ) )
	{
		for( int t = 0 ; t < max ; t++ )
		{
			r.SetRect( rect.left + cut / 2, top, rect.right - cut / 2, top + height );
			COperation* obj = GetOperation( t );
			if( !( GetDisplayOptions() & DISPLAY_ONLY_PUBLIC ) || obj->access == ACCESS_TYPE_PUBLIC )
			{
				CString output = obj->ToString( ( GetDisplayOptions() & DISPLAY_NO_MARKERS ), ( GetDisplayOptions() & DISPLAY_NO_OPERATION_ATTRIBUTE_NAMES ) );
				if( obj->maintype & ENTITY_TYPE_STATIC && obj->maintype & ENTITY_TYPE_ABSTRACT )
					dc->SelectObject( &fontUnderlineItalic );
				else if( obj->maintype & ENTITY_TYPE_STATIC )
					dc->SelectObject( &fontUnderline );
				else if( obj->maintype & ENTITY_TYPE_ABSTRACT )
					dc->SelectObject( &fontItalic );
				else
					dc->SelectObject( &font );

				dc->DrawText( output, r, DT_SINGLELINE | DT_NOPREFIX );
				top += height;
			}

		}
	}

	if( GetOperations() != GetVisibleOperations() )
	{
		dc->SelectObject( &font );
		r.SetRect( rect.left + cut / 2, top, rect.right - cut / 2, top + height );
		dc->DrawText( CString( _T( "..." ) ), r, DT_SINGLELINE | DT_NOPREFIX );
	}

	////////////////////////////////////////////////////////////////////
	// Clean up
	//

	dc->SelectObject( oldfont );
	dc->SelectStockObject( BLACK_PEN );
	dc->SelectStockObject( WHITE_BRUSH );
	dc->SetBkMode( mode );

}

CDiagramEntity* CUMLEntityClass::Clone()
/* ============================================================
	Function :		CUMLEntityClass::Clone
	Description :	Clone this object to a new object.
	Access :		Public
					
	Return :		CDiagramEntity*	-	The new object.
	Parameters :	none

	Usage :			Call to create a clone of the object. The 
					caller will have to delete the object.

   ============================================================*/
{

	CUMLEntityClass* obj = new CUMLEntityClass;
	obj->Copy( this );
	return obj;

}

CDiagramEntity* CUMLEntityClass::CreateFromString( const CString& str )
/* ============================================================
	Function :		CUMLEntityClass::CreateFromString
	Description :	Static factory function that creates and 
					returns an instance of this class if "str" 
					is a valid representation.
	Access :		Public
					
	Return :		CDiagramEntity*		-	The object, or "NULL" 
											if "str" is not a 
											representation of 
											this type.
	Parameters :	const CString& str	-	The string to create 
											from.
					
	Usage :			Can be used as a factory for text file loads. 
					Each object type should have its own 
					version - the default one is a model 
					implementation.

   ============================================================*/
{

	CUMLEntityClass* obj = new CUMLEntityClass;
	if(!obj->FromString( str ) )
	{
		delete obj;
		obj = NULL;
	}

	return obj;

}

BOOL CUMLEntityClass::FromString( const CString& str )
/* ============================================================
	Function :		CUMLEntityClass::FromString
	Description :	Sets the data for this object from "str"
	Access :		Public

	Return :		BOOL				-	"TRUE" if "str" was a 
											valid representation of 
											this type.
	Parameters :	const CString& str	-	String representation
					
	Usage :			Use when loading from file.

   ============================================================*/
{

	ClearAttributes();
	ClearOperations();
	ClearProperties();

	BOOL result = FALSE;
	CString data( str );

	if( LoadFromString( data ) )
	{

		CTokenizer tok( data );

		CString package;
		CString fontName;
		int		bkColor;
		CString stereotype;
		CString propertylist;
		int		displayOptions;
		int		attributes;
		int		operations;
		
		int		count = 0;

		tok.GetAt( count++, package );
		tok.GetAt( count++, fontName );
		tok.GetAt( count++, bkColor );
		tok.GetAt( count++, stereotype );
		tok.GetAt( count++, propertylist );
		tok.GetAt( count++, displayOptions );
		tok.GetAt( count++, attributes);
		tok.GetAt( count++, operations);

		UnmakeSaveString( package );
		UnmakeSaveString( stereotype );
		UnmakeSaveString( propertylist );

		SetPackage( package );
		SetStereotype( stereotype );
		SetFont( fontName );
		SetBkColor( static_cast< COLORREF >( bkColor ) );
		SetDisplayOptions( displayOptions );
		m_properties.FromString( propertylist );

		int t;
		for( t = 0 ; t < attributes ; t++ )
		{

			CString str;
			tok.GetAt( count + t, str );

			CAttribute* at = CAttribute::FromString( str );
			if( at )
				AddAttribute( at );

		}

		count = count + t;
		for( t = 0 ; t < operations; t++ )
		{
			CString str;
			tok.GetAt( count + t, str );

			COperation* op = COperation::FromString( str );
			if( op )
				AddOperation( op );

		}

		CalcRestraints();
		result = TRUE;

	}

	return result;

}

CString CUMLEntityClass::GetString() const
/* ============================================================
	Function :		CUMLEntityClass::GetString
	Description :	Gets a string representation of this object.
	Access :		Public

	Return :		CString	-	Resulting string
	Parameters :	none

	Usage :			Call to get a string representation for
					saving.

   ============================================================*/
{

	CString str;
	CString package = GetPackage();
	CString stereotype = GetStereotype();
	CString propertylist = m_properties.GetString();

	MakeSaveString( package );
	MakeSaveString( stereotype );
	MakeSaveString( propertylist );

	int attributes = GetAttributes();
	int operations = GetOperations();

	str.Format( _T( ",%s,%s,%i,%s,%s,%i,%i,%i" ), 
			package,
			GetFont(),
			static_cast< int >( GetBkColor() ),
			stereotype,
			propertylist,
			GetDisplayOptions(),
			attributes,
			operations
		);

	str += m_attributes.GetString();
	str += m_operations.GetString();

	str += _T( ";" );
	str = GetDefaultGetString() + str;
	return str;

}

int	CUMLEntityClass::GetAttributes() const
/* ============================================================
	Function :		int	CUMLEntityClass::GetAttributes
	Description :	Returns the number of attributes in this 
					object.
	Access :		Public

	Return :		int		-	The number of attributes.
	Parameters :	none

	Usage :			Call to get the total number of attributes
					for this object.

   ============================================================*/
{

	return m_attributes.GetSize();

}

int CUMLEntityClass::GetOperations() const
/* ============================================================
	Function :		CUMLEntityClass::GetOperations
	Description :	Gets the total number of operations in 
					this object.
	Access :		Public

	Return :		int		-	Number of operations
	Parameters :	none

	Usage :			Call to get the total number of operations.

   ============================================================*/
{

	return m_operations.GetSize();

}

CAttribute* CUMLEntityClass::GetAttribute( int index ) const
/* ============================================================
	Function :		CUMLEntityClass::GetAttribute
	Description :	Gets the "CAttribute" at index "index".
	Access :		Public

	Return :		CAttribute*	-	Attribute or "NULL" if out of bounds
	Parameters :	int index	-	Index to get attribute from.
					
	Usage :			Call to get a specific attribute.

   ============================================================*/
{

	return m_attributes.GetAt( index );

}

COperation* CUMLEntityClass::GetOperation( int index ) const
/* ============================================================
	Function :		CUMLEntityClass::GetOperation
	Description :	Gets a pointer to the "COperation" at 
					index "index".
	Access :		Public

	Return :		CAttribute*	-	Operation or "NULL" if out of bounds
	Parameters :	int index	-	Index to get operation from.
					
	Usage :			Call to get a specific operation.

   ============================================================*/
{

	return m_operations.GetAt( index );

}

void CUMLEntityClass::ClearAttributes()
/* ============================================================
	Function :		CUMLEntityClass::ClearAttributes
	Description :	Removes all attributes from this object.
	Access :		Public

	Return :		void
	Parameters :	none

	Usage :			Call to remove all attributes from this 
					object. Removes all associated memory.

   ============================================================*/
{

	m_attributes.RemoveAll();

}

void CUMLEntityClass::ClearOperations()
/* ============================================================
	Function :		CUMLEntityClass::ClearOperations
	Description :	Removes all operations from this object.
	Access :		Public

	Return :		void
	Parameters :	none

	Usage :			Call to remove all operations from this 
					object. Removes all associated memory.

   ============================================================*/
{

	m_operations.RemoveAll();

}

void CUMLEntityClass::ClearProperties()
/* ============================================================
	Function :		CUMLEntityClass::ClearProperties
	Description :	Removes all properties from this object.
	Access :		Public

	Return :		void
	Parameters :	none

	Usage :			Call to remove all properties from this 
					object. Removes all associated memory.

   ============================================================*/
{

	m_properties.RemoveAll();

}

void CUMLEntityClass::AddAttribute( CAttribute* obj )
/* ============================================================
	Function :		CUMLEntityClass::AddAttribute
	Description :	Adds "obj" to the attributes of this object.
	Access :		Public

	Return :		void
	Parameters :	CAttribute* obj	-	Attribute to add.
					
	Usage :			Call to add an attribute to this object. 
					This object takes ownership of "obj".

   ============================================================*/
{

	m_attributes.Add( obj );
	CalcRestraints();

}

void CUMLEntityClass::AddOperation( COperation* obj )
/* ============================================================
	Function :		CUMLEntityClass::AddOperation
	Description :	Adds "obj" to the operations of this object.
	Access :		Public

	Return :		void
	Parameters :	CAttribute* obj	-	Operation to add.
					
	Usage :			Call to add an operation to this object. 
					This object takes ownership of "obj".

   ============================================================*/
{

	m_operations.Add( obj );
	CalcRestraints();

}

void CUMLEntityClass::CalcRestraints()
/* ============================================================
	Function :		CUMLEntityClass::CalcRestraints
	Description :	Calculates, depending on the contents, the 
					minimum size for this object.
	Access :		Public

	Return :		void
	Parameters :	none

	Usage :			Call when the contents change. The title- 
					or default width is the normal minimum width. 
					Default height or the number of visible 
					lines normally the minimum height. The width 
					of the elements in the list compartments is 
					not used, as the can ve very wide.

   ============================================================*/
{

	int titlewidth = 0;
	if( GetTitle().GetLength() && AfxGetMainWnd() )
	{

		CClientDC dc( AfxGetMainWnd() );
		CFont fontBold;
		fontBold.CreateFont( -12, 0,0,0,FW_BOLD,0,0,0,0,0,0,0,0, GetFont() );
		CFont* oldfont = NULL;
		oldfont = dc.SelectObject( &fontBold );

		titlewidth = dc.GetTextExtent( GetTitle() ).cx + GetMarkerSize().cx * 2;

		dc.SelectObject( oldfont );

	}

	int attrs = GetVisibleAttributes();
	if( GetVisibleAttributes() != GetAttributes() )
		attrs++;

	int ops = GetVisibleOperations();
	if( GetVisibleOperations() != GetOperations() )
		ops++;

	int header = 1;
	if( GetStereotype().GetLength() )
		header++;

	header += m_properties.GetSize();

	int lines = max( 1, attrs ) + max( 1, ops ) + header;
	int height = round( 14.0 * GetZoom() );
	int totalheight = lines * height;
	totalheight += GetMarkerSize().cx * 3;

	int totalwidth = max( titlewidth, GetDefaultSize().cx );
	CUMLEntityContainer* container = GetUMLContainer();
	if( container )
	{
		CSize sz = container->CalcMinimumRestraints( GetName() );
		if( sz.cx > totalheight )
			totalheight = sz.cx;
		if( sz.cy > totalwidth )
			totalwidth = sz.cy;
	}

	totalheight = round( static_cast< double >( totalheight ) / 8 ) * 8;
	totalwidth = round( static_cast< double >( totalwidth ) / 8 ) * 8;

	if( GetMinimumSize().cy != totalheight || GetMinimumSize().cx != totalwidth )
	{

		SetMinimumSize( CSize( totalwidth, totalheight ) );
		CRect rc = GetRect();
		if( rc.Height() < totalheight || rc.Width() < totalwidth )
		{
			totalheight = max( totalheight, rc.Height() );
			totalwidth = max( totalwidth, rc.Width() );
			SetRect( GetLeft(), GetTop(), GetLeft() + totalwidth, GetTop() + totalheight );
			if( container )
				container->AdjustLinkedObjects( this );
		}

	}

}

void CUMLEntityClass::Copy( CDiagramEntity* obj )
/* ============================================================
	Function :		CUMLEntityClass::Copy
	Description :	Copies the data from "obj" to this object.
	Access :		Public

	Return :		void
	Parameters :	CDiagramEntity* obj	-	Object to copy from
					
	Usage :			Call to copy data from one object to 
					another.

   ============================================================*/
{

	ClearAttributes();
	ClearOperations();
	ClearProperties();

	CUMLEntity::Copy( obj );

	CUMLEntityClass* uml = dynamic_cast< CUMLEntityClass* >( obj );

	if( uml )
	{
		m_attributes.Copy( uml->m_attributes );
		m_operations.Copy( uml->m_operations );

		m_properties.Copy( uml->m_properties );
		CalcRestraints();

	}

}

CString CUMLEntityClass::Export( UINT format ) const
/* ============================================================
	Function :		CUMLEntityClass::Export
	Description :	Exports this object to the desired format.
	Access :		Public
					
	Return :		CString		-	Result
	Parameters :	UINT format	-	Format to export to
					
	Usage :			"format" can be one of the following:
						"EXPORT_CPP" Export to cpp-files
						"EXPORT_H" Export to header files
						"EXPORT_HTML" Export to HTML-files

   ============================================================*/
{

	CString result;

	switch( format )
	{
		case EXPORT_HTML:
			result = ExportHTML();
			break;
		case EXPORT_CPP:
			result = ExportCPP();
			break;
		case EXPORT_H:
			result = ExportH();
			break;
	}

	return result;

}

int CUMLEntityClass::GetVisibleAttributes() const
/* ============================================================
	Function :		CUMLEntityClass::GetVisibleAttributes
	Description :	Gets the number of visible attributes in 
					this object.
	Access :		Private

	Return :		int		-	Visible attributes.
	Parameters :	none

	Usage :			Call to get the number of visible attributes. 
					This might not be all attributes, as some 
					might be hidded by display settings.

   ============================================================*/
{

	int max = GetAttributes();
	int count = max;

	if( GetDisplayOptions() & DISPLAY_ONLY_PUBLIC )
	{
		count = 0;
		for( int t = 0 ; t < max ; t++ )
		{
			CAttribute* attr = GetAttribute( t );
			if( attr->access == ACCESS_TYPE_PUBLIC )
				count++;
		}
	}

	if( GetDisplayOptions() & DISPLAY_NO_ATTRIBUTES )
		count = 0;

	return count;

}

int CUMLEntityClass::GetVisibleOperations() const
/* ============================================================
	Function :		CUMLEntityClass::GetVisibleOperations
	Description :	Gets the number of visible operations in 
					this object.
	Access :		Private

	Return :		int		-	Visible operations.
	Parameters :	none

	Usage :			Call to get the number of visible operations. 
					This might not be all operations, as some 
					might be hidded by display settings.

   ============================================================*/
{

	int max = GetOperations();
	int count = max;

	if( GetDisplayOptions() & DISPLAY_ONLY_PUBLIC )
	{
		count = 0;
		for( int t = 0 ; t < max ; t++ )
		{
			COperation* op = GetOperation( t );
			if( op->access == ACCESS_TYPE_PUBLIC )
				count++;
		}
	}

	if( GetDisplayOptions() & DISPLAY_NO_OPERATIONS )
		count = 0;

	return count;

}

CString CUMLEntityClass::ExportH() const
/* ============================================================
	Function :		CUMLEntityClass::ExportH
	Description :	Export the data in this object to a 
					c++ header-file
	Access :		Protected

	Return :		CString	-	Resulting contents for a c++ 
								header-file
	Parameters :	none

	Usage :			Call to export the current object to a c++
					header file.

   ============================================================*/
{

	CString result;

	// Create class header
	CString className = GetTitle();
	className.TrimLeft();
	className.TrimRight();

	CString fileName = GetFilename() + _T( ".h" );
	CString currentDate;
	CTime time = CTime::GetCurrentTime();
	currentDate = time.Format( _T( "%x" ) );

	CString includeList = GetIncludeList();

	// Create include guard
	CString includeGuardHead;
	CString includeGuardFoot;
	CString cn( className );

	GUID	guid;
	CString guidstring;
	HRESULT res = CoCreateGuid(&guid);	
	if( res == S_OK )
	{
		guidstring.Format( _T( "%X_%X_%X_%X%X%X%X%X%X" ),
			static_cast< int >( guid.Data1 ),
			static_cast< int >( guid.Data2 ),
			static_cast< int >( guid.Data3 ),
			static_cast< int >( guid.Data4[ 0 ] ),
			static_cast< int >( guid.Data4[ 1 ] ),
			static_cast< int >( guid.Data4[ 2 ] ),
			static_cast< int >( guid.Data4[ 3 ] ),
			static_cast< int >( guid.Data4[ 4 ] ),
			static_cast< int >( guid.Data4[ 5 ] )
			);
	}

	cn.MakeUpper();
	CString definename = _T( "_" ) + cn;
	definename += _T( "_H_" ) + guidstring;

	// Create operation list
	CString operationList = GetOperationList( EXPORT_H );

	// Create attribute list
	CString attributeList = GetAttributeList( EXPORT_H );

	// Load template into result
	CTextFile file( _T( "" ), _T( "\n" ) );
	CString templatename = GetApplicationDirectory();

	// Create base class string
	CStringArray baseClassArray;
	CStringArray baseClassAccessArray;
	GetBaseClassArray( baseClassArray, baseClassAccessArray );

	templatename += GetHeaderTemplateFilename();
	if( file.ReadTextFile( templatename, result ) )
	{

		// Add the prepared info
		result.Replace( _T( "[%filename%]" ), fileName );
		result.Replace( _T( "[%classname%]" ), className );
		result.Replace( _T( "[%currentdate%]" ), currentDate );
		result.Replace( _T( "[%operationlist%]" ), operationList );
		result.Replace( _T( "[%attributelist%]" ), attributeList );
		result.Replace( _T( "[%definename%]" ), definename );

		if( baseClassArray.GetSize() )
		{
			CString baseClass;
			int size = baseClassArray.GetSize();
			for( int t = 0 ; t < size ; t++ )
			{
				if( t == 0 )
					baseClass += _T( " : " );
				else
					baseClass += _T( ", " );

				baseClass += baseClassAccessArray[ t ] + _T( " " ) + baseClassArray[ t ];
			}

			result.Replace( _T( "[%baseclass%]" ), baseClass );
		}
		else
			result.Replace( _T( "[%baseclass%]" ), _T( "" ) );

		result.Replace( _T( "[%includelist%]" ), includeList );

	}
	else
		GetUMLContainer()->SetErrorMessage( file.GetErrorMessage() );

	return result;

}

CString CUMLEntityClass::GetOperationList( int format ) const
/* ============================================================
	Function :		CUMLEntityClass::GetOperationList
	Description :	Returns a string with the operations of 
					this object in the format "format".
	Access :		Protected

	Return :		CString		-	Result
	Parameters :	int format	-	Format to use.
					
	Usage :			Call as a part of an export, for example.
					"format" can be one of the following:
						"EXPORT_CPP" Export to cpp-files
						"EXPORT_H" Export to header files
						"EXPORT_HTML" Export to HTML-files

   ============================================================*/
{

	CString result;
	CString title = GetTitle();

	switch( format )
	{
		case EXPORT_H:
		{
			int oldaccess = ACCESS_TYPE_PUBLIC;
			int max = GetOperations();
			for( int pass = 0; pass < 2 ; pass++ )
			{
				// First pass, we will do ctors and dtors,
				// second, the rest
				if( pass == 0 )
					result += _T( "// Construction/destruction\r\n" );
				else
					result += _T( "\r\n// Operations\r\n" );

				for( int t = 0; t < max ; t++ )
				{
					COperation* op = GetOperation( t );
					if( ( pass == 0 && ( op->name == title || op->name == _T( "~" ) + title ) ) || ( pass != 0 && ( op->name != title && op->name != _T( "~" ) + title ) ) )
					{
						if( op->access != oldaccess )
						{
							switch( op->access )
							{
								case ACCESS_TYPE_PRIVATE:
									result += _T( "\r\nprivate:\r\n" );
								break;
								case ACCESS_TYPE_PROTECTED:
									result += _T( "\r\nprotected:\r\n" );
								break;
								case ACCESS_TYPE_PUBLIC:
									result += _T( "\r\npublic:\r\n" );
								break;
							}
						}

						if( pass == 0 )
							result += op->GetString( STRING_FORMAT_H_CTOR ) + _T( "\r\n" );
						else
							result += op->GetString( STRING_FORMAT_H ) + _T( "\r\n" );

						oldaccess = op->access;
					}

				}
			}
		}
		break;

		case EXPORT_HTML:
			if( !( GetDisplayOptions() & DISPLAY_NO_OPERATIONS ) )
			{
				int max = GetOperations();
				for( int t = 0 ; t < max ; t++ )
				{
					COperation* obj = GetOperation( t );
					if( !( GetDisplayOptions() & DISPLAY_ONLY_PUBLIC ) || obj->access == ACCESS_TYPE_PUBLIC )
					{
						CString output = obj->ToString( ( GetDisplayOptions() & DISPLAY_NO_MARKERS ), ( GetDisplayOptions() & DISPLAY_NO_OPERATION_ATTRIBUTE_NAMES ) );

						if( obj->maintype & ENTITY_TYPE_STATIC && obj->maintype & ENTITY_TYPE_ABSTRACT )
							output = _T( "<u><i>" ) + output + _T( "</i></u>" );
						else if( obj->maintype & ENTITY_TYPE_STATIC )
							output = _T( "<u>" ) + output + _T( "</u>" );
						else if( obj->maintype & ENTITY_TYPE_ABSTRACT )
							output = _T( "<i>" ) + output + _T( "</i>" );

						output += _T( "<br>" );
						result += output;
					}

				}
			}

			if( GetOperations() != GetVisibleOperations() )
				result += _T( "..." );

		break;
	}

	return result;

}

CString CUMLEntityClass::GetAttributeList( int format ) const
/* ============================================================
	Function :		CUMLEntityClass::GetAttributeList
	Description :	Returns a string with the attributes of 
					this object formatted in "format".
	Access :		Protected

	Return :		CString		-	Result
	Parameters :	int format	-	Format to get.
					
	Usage :			Call as a part of an export, for example.
					"format" can be one of the following:
						"EXPORT_CPP" Export to cpp-files
						"EXPORT_H" Export to header files
						"EXPORT_HTML" Export to HTML-files
   ============================================================*/
{

	CString result;
	switch( format )
	{
		case EXPORT_H:
		{
			int oldaccess = ACCESS_TYPE_PUBLIC;
			int max = GetAttributes();
			for( int t = 0; t < max ; t++ )
			{
				CAttribute* attr = GetAttribute( t );
				if( attr->access != oldaccess )
				{
					switch( attr->access )
					{
						case ACCESS_TYPE_PRIVATE:
						result += _T( "\r\nprivate:\r\n" );
						break;
						case ACCESS_TYPE_PROTECTED:
						result += _T( "\r\nprotected:\r\n" );
						break;
						case ACCESS_TYPE_PUBLIC:
						result += _T( "\r\npublic:\r\n" );
						break;
					}
				}

				result += _T( "\t" );
				result += attr->GetString( STRING_FORMAT_H );
				result += _T( "\r\n" );

				oldaccess = attr->access;

			}
		}

		break;

		case EXPORT_HTML:
			if( !( GetDisplayOptions() & DISPLAY_NO_ATTRIBUTES ) )
			{
				int max = GetAttributes();
				if( max )
				{
					for( int t = 0 ; t < max ; t++ )
					{
						CAttribute* obj = GetAttribute( t );
						if( !( GetDisplayOptions() & DISPLAY_ONLY_PUBLIC ) || obj->access == ACCESS_TYPE_PUBLIC )
						{
							CString output = obj->ToString( ( GetDisplayOptions() & DISPLAY_NO_MARKERS ) );
							if( obj->maintype & ENTITY_TYPE_STATIC && obj->maintype & ENTITY_TYPE_ABSTRACT )
								output = _T( "<u><i>" ) + output + _T( "</i></u>" );
							else if( obj->maintype & ENTITY_TYPE_STATIC )
								output = _T( "<u>" ) + output + _T( "</u>" );
							else if( obj->maintype & ENTITY_TYPE_ABSTRACT )
								output = _T( "<i>" ) + output + _T( "</i>" );
							output += _T( "<br>" );
							result += output;
						}
					}

				}
			}

			if( GetAttributes() != GetVisibleAttributes() )
				result += _T( "..." );

		break;
	}

	return result;

}

CString CUMLEntityClass::ExportCPP() const
/* ============================================================
	Function :		CUMLEntityClass::ExportCPP
	Description :	Exports the data of this object to a c++ 
					cpp-file.
	Access :		Protected

	Return :		CString	-	Resulting cpp-file contents.
	Parameters :	none

	Usage :			Call to export the current object in c++ 
					format to a cpp-file

   ============================================================*/
{
	CString result;
	CStringArray functionaccess;
	functionaccess.Add( _T("[%privatefunctions%]" ) );
	functionaccess.Add( _T("[%protectedfunctions%]" ) );
	functionaccess.Add( _T("[%publicfunctions%]" ) );

	CString className = GetTitle();
	className.TrimLeft();
	className.TrimRight();
	CString fileName = GetFilename();
	CString currentDate;
	CTime time = CTime::GetCurrentTime();
	currentDate = time.Format( _T( "%x" ) );
	CString dependencyList = GetDependencyList();

	CTextFile file( _T( "" ), _T( "\n" ) );;
	CString templatename = GetApplicationDirectory();

	CString functiontemplatename( templatename );
	templatename += _T( "cpp_template.txt" );
	functiontemplatename +=  _T( "cpp_function_template.txt" );
	CString function;
	CString functions;
	CString initializationList = GetAttributeInitializationList();

	if( file.ReadTextFile( templatename, result ) )
	{
		if( file.ReadTextFile( functiontemplatename, function ) )
		{

			result.Replace( _T( "[%filename%]" ), fileName );
			result.Replace( _T( "[%classname%]" ), className );
			result.Replace( _T( "[%currentdate%]" ), currentDate );
			result.Replace( _T( "[%includes%]" ), dependencyList );

			function.Replace( _T( "[%filename%]" ), fileName );
			function.Replace( _T( "[%classname%]" ), className );
			function.Replace( _T( "[%currentdate%]" ), currentDate );

			// Create function bodies
			for( int access = ACCESS_TYPE_PRIVATE ; access <= ACCESS_TYPE_PUBLIC ; access++ )
			{
				int max = GetOperations();
				for( int t = 0 ; t < max ; t++ )
				{
					CString functiontemplate( function );
					COperation* op = GetOperation( t );
					if( op->access == access )
					{
						if( !( op->maintype & ENTITY_TYPE_ABSTRACT ) )
						{
							////////////////////////////////////////////////
							// Function signature
							//
							CString signature;
							if( op->name != className && op->name != CString( _T( "~" ) ) + className  )
							{
								if( op->type.GetLength() )
									signature = op->type + _T( " " );
								else
									signature = _T( "void " );
							}

							signature += className + _T( "::" ) + op->name +_T( "( " );
							signature += op->parameters.GetString( STRING_FORMAT_CPP );
							signature += _T( " )" );
							if( op->properties.GetPropertyValue( _T( "query" ) ) == _T( "true" ) )
								signature += _T( " const" );

							////////////////////////////////////////////////
							// Function parameters for comments
							//
							CString parameters = op->parameters.GetString( STRING_FORMAT_CPP );
							parameters.Replace( _T( ", " ), _T( "\t-\t\r\n\t\t\t\t\t" ) );
							if( parameters.IsEmpty() )
								parameters = _T( "none\r\n" );

							////////////////////////////////////////////////
							// Function body
							//
							CString comment;
							CString usage;
							CString body;
							if( op->getter )
							{
								comment = _T( "Accessor. Getter for \"" ) + op->getsetvariable + _T( "\"." );
								body = _T("\r\n\r\n\treturn " ) + op->getsetvariable + _T( ";\r\n" );
								usage = _T( "Call to get the value of \"" ) + op->getsetvariable + _T( "\"." );
							}

							if( op->setter && op->parameters.GetSize() == 1 )
							{
								comment = _T( "Accessor. Setter for \"" ) + op->getsetvariable + _T( "\"." );
								body = _T("\r\n\r\n\t" ) + op->getsetvariable + _T( " = " ) + op->parameters.GetAt( 0 )->name + _T( ";\r\n" );
								usage = _T( "Call to set the value of \"" ) + op->getsetvariable + _T( "\"." );
							}

							if( body.IsEmpty() && op->type.GetLength() && op->type != _T( "void" ) )
								body = _T( "\r\n\r\n\t" ) + op->type + _T( "\tresult;\r\n\t// TODO: Implement\r\n\treturn result;\r\n" );

							if( body.IsEmpty() )
								body = _T( "\r\n\r\n\t// TODO: Implement\r\n" );

							if( op->name == className )
							{
								signature += initializationList;
								comment = _T( "Constructor." );
							}

							if( op->name == CString( _T( "~" ) ) + className  )
								comment = _T( "Destructor." );

							////////////////////////////////////////////////
							// Function return
							//
							CString returnvalue = op->type;
							if( returnvalue.IsEmpty() )
								returnvalue = _T( "void" );

							////////////////////////////////////////////////
							// Function access
							//
							CString access;
							if( op->access == ACCESS_TYPE_PRIVATE )
								access = _T( "Private" );
							if( op->access == ACCESS_TYPE_PROTECTED )
								access = _T( "Protected" );
							if( op->access == ACCESS_TYPE_PUBLIC )
								access = _T( "Public" );

							functiontemplate.Replace( _T( "[%signature%]" ), signature );
							functiontemplate.Replace( _T( "[%functionname%]" ), op->name );
							functiontemplate.Replace( _T( "[%returnvalue%]" ), returnvalue );
							functiontemplate.Replace( _T( "[%parameters%]" ), parameters );
							functiontemplate.Replace( _T( "[%body%]" ), body );
							functiontemplate.Replace( _T( "[%description%]" ), comment );
							functiontemplate.Replace( _T( "[%access%]" ), access );
							functiontemplate.Replace( _T( "[%usage%]" ), usage );

							functions += functiontemplate;
						}
					}
				}

				result.Replace( functionaccess[ access ], functions );
				functions = _T( "" );

			}
		}
	}
	else
		GetUMLContainer()->SetErrorMessage( file.GetErrorMessage() );

	return result;

}

void CUMLEntityClass::SetBaseClassArray( const CStringArray& baseClass, const CStringArray& baseClassAccess )
/* ============================================================
	Function :		CUMLEntityClass::SetBaseClass
	Description :	Sets the name of the base class for this object.
	Access :		Public

	Return :		void
	Parameters :	const CString& baseClass	-	New base class
					const CString& baseClass	-	New base class 
													access array
					
	Usage :			Call to set the name of the base class and 
					the base class access for this object. 
					Used by the export mechanism.

   ============================================================*/
{

	m_baseClassArray.RemoveAll();
	m_baseClassArray.Append( baseClass );

	m_baseClassAccessArray.RemoveAll();
	m_baseClassAccessArray.Append( baseClassAccess );

}

void CUMLEntityClass::GetBaseClassArray( CStringArray& array, CStringArray& arrayAccess ) const
/* ============================================================
	Function :		CUMLEntityClass::GetBaseClassArray
	Description :	Gets the base class for this object.
	Access :		Public

	Return :		void
	Parameters :	none

	Usage :			Call to get the base class of the current 
					object. Used by the export-mechanism.

   ============================================================*/
{

	array.RemoveAll();
	array.Append( m_baseClassArray );
	arrayAccess.RemoveAll();
	arrayAccess.Append( m_baseClassAccessArray );

}

void CUMLEntityClass::SetBaseClassFilenameArray( const CStringArray& value )
/* ============================================================
	Function :		CUMLEntityClass::SetBaseClassFilename
	Description :	Sets the file name of the base class for 
					this object.
	Access :		Public

	Return :		void
	Parameters :	const CString& value	-	The new name
					
	Usage :			Call to set the filename of the base class 
					for this object. Used as a part of the 
					export mechanism.

   ============================================================*/
{

	m_baseClassFilenameArray.RemoveAll();
	m_baseClassFilenameArray.Append( value );

}

void CUMLEntityClass::GetBaseClassFilenameArray( CStringArray& array ) const
/* ============================================================
	Function :		CUMLEntityClass::GetBaseClassFilename
	Description :	Gets the file name of the base class for 
					this object.
	Access :		Public

	Return :		CString	-	Base class file name
	Parameters :	none

	Usage :			Call to get the filename of the base class 
					for this object. Used as a part of the 
					export mechanism.

   ============================================================*/
{

	array.RemoveAll();
	array.Append( m_baseClassFilenameArray );

}

void CUMLEntityClass::SetFilename( const CString& value )
/* ============================================================
	Function :		CUMLEntityClass::SetFilename
	Description :	Sets the file name for this object when 
					exported to a cpp/h-file.
	Access :		Public

	Return :		void
	Parameters :	const CString& value	-	New filename
					
	Usage :			Call to set the filename of the object. 
					Used by the export-mechanism.

   ============================================================*/
{

	m_filename = value;

}

CString CUMLEntityClass::GetFilename() const
/* ============================================================
	Function :		CUMLEntityClass::GetFilename
	Description :	Gets the file name for this object when 
					exported to a cpp/h-file.
	Access :		Public

	Return :		CString	-	Current filename
	Parameters :	none

	Usage :			Call to get the filename of the object. 
					Used by the export-mechanism.

   ============================================================*/
{

	return m_filename;

}

CString CUMLEntityClass::GetAttributeInitializationList() const
/* ============================================================
	Function :		CUMLEntityClass::GetAttributeInitializationList
	Description :	Returns a string where class members are 
					initialized as for a c++ "ctor".
	Access :		Private
					
	Return :		CString	-	The initialization list
	Parameters :	none

	Usage :			To create an initialization list for the 
					default "ctor" for c++ exports.

   ============================================================*/
{

	CString result;
	int max = GetAttributes();
	for( int t = 0 ; t < max ; t++ )
	{
		CAttribute* attr = GetAttribute( t );
		if( attr->defaultvalue.GetLength() )
			result += attr->name + _T( "( " ) + attr->defaultvalue + _T( " ),\r\n" );
	}

	if( result.GetLength() )
		result = _T( " : " ) + result.Left( result.GetLength() - 3 );

	return result;

}

BOOL CUMLEntityClass::IsAbstract() const
/* ============================================================
	Function :		CUMLEntityClass::IsAbstract
	Description :	Check if this is an abstract class.
	Access :		Public

	Return :		BOOL	-	"TRUE" if the class is abstract.
	Parameters :	none

	Usage :			Call to check if this is an abstract class.
					An abstract class is having at least one 
					pure virtual class member.

   ============================================================*/
{

	BOOL result = FALSE;
	CString stereotype = GetStereotype();
	if( stereotype == _T( "abstract" ) )
		result = TRUE;
	else
	{
		int max = GetOperations();
		for( int t = 0 ; t < max ; t++ )
		{
			COperation* op = GetOperation( t );
			if( op->maintype & ENTITY_TYPE_ABSTRACT )
				result = TRUE;
		}
	}

	return result;

}

void CUMLEntityClass::ShowPopup( CPoint point, CWnd* parent )
/* ============================================================
	Function :		CUMLEntityClass::ShowPopup
	Description :	Shows the popup menu for the object.
	Access :		Public

	Return :		void
	Parameters :	CPoint point	-	The point to track.
					CWnd* parent	-	The parent "CWnd" of the 
										menu (should be the 
										"CDiagramEditor")

	Usage :			
   ============================================================*/
{

	CMenu menu;
	menu.LoadMenu( IDR_UML_MENU_CLASS_POPUP );
	menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, parent );

}

BOOL CUMLEntityClass::DoMessage( UINT msg, CDiagramEntity* sender, CWnd* from )
/* ============================================================
	Function :		CUMLEntityClass::DoMessage
	Description :	Called when an alternative is selected 
					from the object popup-menu.
	Access :		Public

	Return :		BOOL					-	"TRUE" to stop further 
												processing.
	Parameters :	UINT msg				-	The message
					CDiagramEntity* sender	-	The original sender of this 
												message
					CWnd* from				-	Parent window, normally the editor.
					
	Usage :			Message handling mechanism for 
					"CDiagramEntity"-derived objects. We handle 
					imports from h-files here

   ============================================================*/
{

	BOOL result = FALSE;
	if( msg == CMD_IMPORT )
	{
		CFileDialog	dlg( TRUE, _T( "h" ), _T( "*.h" ) );
		if( dlg.DoModal() == IDOK )
			ImportH( dlg.GetPathName() );

		result = TRUE;
	}
	else
		result = CUMLEntity::DoMessage( msg, sender, from );

	return result;

}

#define STATE_STARTING	0
#define STATE_READING	1

BOOL CUMLEntityClass::ImportH( const CString& filename )
/* ============================================================
	Function :		CUMLEntityClass::ImportH
	Description :	Imports information from a c++ h-file to 
					this object.
	Access :		Protected

	Return :		BOOL					-	"TRUE" if the header 
												could be parsed
	Parameters :	const CString& filename	-	Import from this
												file.
					
	Usage :			Call to import data for this object from a 
					c++ header-file.

   ============================================================*/
{

	ClearAttributes();
	ClearOperations();
	ClearProperties();

	CTextFile		file( _T( "" ), _T( "\n" ) );;
	CString			localfilename( filename );
	CString			str;
	if( file.ReadTextFile( localfilename, str ) )
	{
		int comment = str.Find( _T( "/*" ) );
		while( comment != -1 )
		{
			int endcomment = str.Find( _T( "*/" ) );
			if( endcomment != -1 )
				str = str.Left( comment ) + str.Right( str.GetLength() - ( endcomment + 2 ) );
			else
			{
				CString err;
				err.LoadString( IDS_UML_MALFORMED_COMMENT );
				GetUMLContainer()->SetErrorMessage( err );
				return FALSE;
			}

			comment = str.Find( _T( "/*" ) );
		}

		CTokenizer tok( str, _T( "\n" ) );
		CStringArray classContents;
		int max = tok.GetSize();
		int state = STATE_STARTING;
		int rb = 0;
		for( int t = 0; t < max ; t++ )
		{
			CString data;
			tok.GetAt( t, data );
			data.TrimLeft();
			int found = data.Find( _T( "//" ) );
			if( found != -1 )
				data = data.Left( found );
			data.TrimRight();
			if( data.GetLength() )
			{
				switch( state )
				{
					case STATE_STARTING:
						if( data.Left( 6 ) == _T( "class " ) && data[ data.GetLength() - 1 ] != _TCHAR( ';' ) )
						{
							if( data.Find( _TCHAR( '{' ) ) != -1 )
								rb++;
							state = STATE_READING;
							classContents.Add( data );
						}
						break;
					case STATE_READING:
						if( data.Find( _TCHAR( '{' ) ) != -1 )
							rb++;
						if( data.Find( _TCHAR( '}' ) ) != -1 )
							rb--;

						if( rb && data.GetLength() && data[0] != _TCHAR( '#' ) )
							classContents.Add( data );

						break;
				}
			}
		}

		// Now, we assemble the cleaned contents into one single string,
		// to be able to clear out inline functions.
		max = classContents.GetSize();
		str = _T( "" );
		for( int t = 0 ; t < max ; t++ )
			str += classContents[ t ] + _T("\n" );
		int leftbracket = str.Find( _TCHAR( '{' ) );
		// We must remove the first bracket found
		if( leftbracket != -1 )
			str = str.Left( leftbracket ) + str.Right( str.GetLength() - ( leftbracket + 1 ) );

		leftbracket = str.Find( _TCHAR( '{' ) );
		while( leftbracket!= -1 )
		{
			int rightbracket = str.Find( _T( "};" ) );
			if( rightbracket != -1 )
			{
				CString str1 = str.Left( leftbracket );
				CString str2 = str.Right( str.GetLength() - ( rightbracket + 1 ) );
				str = str1 + str2;
			}
			else
			{
				CString err;
				err.LoadString( IDS_UML_MISSING_BRACKET );
				GetUMLContainer()->SetErrorMessage( err );
				return FALSE;
			}

			leftbracket = str.Find( _TCHAR( '{' ) );
		}

		// To make sure declarations are ended with a ";"
		str.Replace( _T( "\n;" ), _T( ";" ) );

		// Now, we tokenize again
		CString className;
		CString baseClass;

		tok.Init( str, _T( "\n" ) );
		max = tok.GetSize();
		classContents.RemoveAll();
		for( int t = 0 ; t < max ; t++ )
		{
			CString data;
			tok.GetAt( t, data );
			data.TrimRight();

			if( data.GetLength() )
			{
				data.Replace( _TCHAR( '\t' ), _TCHAR( ' ' ) );
				data.Replace( _T( " *" ), _T( "*" ) );
				data.Replace( _T( " &" ), _T( "&" ) );
				data.Replace( _T( " (" ), _T( "(" ) );
				data.Replace( _T( "( " ), _T( "(" ) );
				data.Replace( _T( " )" ), _T( ")" ) );
				data.Replace( _T( " ;" ), _T( ";" ) );
				data.Replace( _T( " [" ), _T( "[" ) );
				data.Replace( _T( " ]" ), _T( "]" ) );
				data.Replace( _T( " =" ), _T( "=" ) );
				data.Replace( _T( "= " ), _T( "=" ) );

				data.Replace( _T( "*" ), _T( "* " ) );
				data.Replace( _T( "&" ), _T( "& " ) );

				data.Replace( _T( "  " ), _T( " " ) );
				data.Replace( _T( "  " ), _T( " " ) );

				if( t == 0 )
				{
					// The first line should contain the class name
					int colon = data.Find( _TCHAR( ':' ) );
					if( colon != -1 )
					{
						baseClass = data.Right( data.GetLength() - ( colon + 1 ) );
						className = data.Left( colon );
						className = className.Right( className.GetLength() - 5 ); // remove 'class'
						className.TrimLeft();
						className.TrimRight();
						baseClass.TrimLeft();
						baseClass.TrimRight();
					}
					else
						className = data.Right( data.GetLength() - 5 ); // remove 'class'
				}
				else
				{
					if( data[ data.GetLength() - 1 ] == _TCHAR( ';' ) || data[ data.GetLength() - 1 ] == _TCHAR( ':' ) )
						// We want to keep only access markers and declarations
						classContents.Add( data );
		
				}
			}

		}

		if( className.IsEmpty() )
			return FALSE;

		// Now, we are left with a list of the declarations and access markers from the header file. Now, we will start to create attributes and operations from those. We will recognize an operation as it will have a paranthesis somewhere in the string.
		max = classContents.GetSize();
		int access = ACCESS_TYPE_PRIVATE;
		for( int t = 0 ; t < max ; t++ )
		{
			CString data = classContents[ t ];
			if( data[ data.GetLength() - 1 ] == _TCHAR( ':' ) )
			{
				// An access marker
				if( data == _T( "private:" ) )
					access = ACCESS_TYPE_PRIVATE;
				else if( data == _T( "protected:" ) )
					access = ACCESS_TYPE_PROTECTED;
				else if( data == _T( "public:" ) )
					access = ACCESS_TYPE_PUBLIC;
			}
			else if( data.Find( _TCHAR( '(' ) ) != -1 )
			{
				// A member function
				COperation* op = new COperation;
				op->access = access;

				// Get the main type of the function
				CString afxmsgmarker( _T( "afx_msg " ) );
				int afxmsgmarkerfound = data.Find( afxmsgmarker ) ;
				if( afxmsgmarkerfound != -1 )
					data = data.Left( afxmsgmarkerfound ) + data.Right( data.GetLength() - ( afxmsgmarkerfound + afxmsgmarker.GetLength() ) );

				CString virtualmarker( _T( "virtual " ) );
				int virtualmarkerfound = data.Find( virtualmarker ) ;
				if( virtualmarkerfound != -1 )
				{
					data = data.Left( virtualmarkerfound ) + data.Right( data.GetLength() - ( virtualmarkerfound + virtualmarker.GetLength() ) );
					op->properties.Add( _T( "virtual" ) );
				}

				CString staticmarker( _T( "static " ) );
				int staticmarkerfound = data.Find( staticmarker ) ;
				if( staticmarkerfound != -1 )
				{
					data = data.Left( staticmarkerfound ) + data.Right( data.GetLength() - ( staticmarkerfound + staticmarker.GetLength() ) );
					op->maintype |= ENTITY_TYPE_STATIC;
				}

				// Get the return value of the function
				int space = data.Find( _TCHAR( ' ' ) );
				int paramstart = data.Find( _TCHAR( '(' ) );
				if( space != -1 && paramstart > space )
				{
					CString type = data.Left( space );
					data = data.Right( data.GetLength() - ( space + 1 ) );
					if( type == _T( "void" ) )
						type = _T( "" );

					op->type = type;
				}

				// Strip away class names prefixing the function name
				if( data.GetLength() > className.GetLength() + 2 )
				{
					if( data.Left( className.GetLength() + 2 ) == CString( className + _T( "::" ) ) )
						data = data.Right( data.GetLength() - ( className.GetLength() + 2 ) );
				}

				paramstart = data.Find( _TCHAR( '(' ) );
				if( paramstart != -1 )
				{
					CString name = data.Left( paramstart );
					data = data.Right( data.GetLength() - ( paramstart + 1 ) );
					op->name = name;
				}

				//////////////////////////////////////////////////////
				// Function parameters
				// 
				int paramend = data.ReverseFind( _TCHAR( ')' ) );
				if( paramend != -1 )
				{
					CString params = data.Left( paramend );
					data = data.Right( data.GetLength() - ( paramend + 1 ) );
					int paramlen = params.GetLength();
					if( paramlen )
					{
						tok.Init( params );
						int size = tok.GetSize();
						for( int i = 0 ; i < size ; i++ )
						{
							CString param;
							tok.GetAt( i, param );
							param.TrimLeft();
							param.TrimRight();
							int space = param.ReverseFind( _TCHAR( ' ' ) );
							if( space != -1 )
							{
								CString type = param.Left( space );
								CString name = param.Right( param.GetLength() - ( space + 1 ) );
								CString defval;
								int eq = name.Find( _TCHAR( '=' ) );
								if( eq != -1 )
								{
									defval = name.Right( name.GetLength() - ( eq + 1 ) );
									name = name.Left( eq );
								}

								CParameter* param = new CParameter;
								param->name = name;
								param->defaultvalue = defval;
								param->in = TRUE;

								param->out = FALSE;
								if( type.Remove( _TCHAR( '&' ) ) )
								{
									param->out = TRUE;
									param->reference = TRUE;
								}

								if( type.Remove( _TCHAR( '*' ) ) )
									param->out = TRUE;

								CString constmarker( _T( "const " ) );
								int constmarkerfound = type.Find( constmarker ) ;
								if( constmarkerfound != -1 )
								{

									type = type.Left( constmarkerfound ) + type.Right( type.GetLength() - ( constmarkerfound + constmarker.GetLength() ) );
									param->out = FALSE;

								}

								param->type = type;
								op->parameters.Add( param );

							}
						}
					}
				}

				CString constmarker( _T( "const" ) );
				int constmarkerfound = data.Find( constmarker );
				if( constmarkerfound != -1 )
				{
					data = data.Left( constmarkerfound ) + data.Right( data.GetLength() - ( constmarkerfound + constmarker.GetLength() ) );
					op->properties.Add( _T( "query" ) );
				}

				CString virtmarker( _T( "=0" ) );
				int virtmarkerfound = data.Find( virtmarker );
				if( virtmarkerfound != -1 )
				{
					data = data.Left( virtmarkerfound ) + data.Right( data.GetLength() - ( virtmarkerfound + virtmarker.GetLength() ) );
					op->maintype |= ENTITY_TYPE_ABSTRACT;
				}

				AddOperation( op );

			}
			else
			{

				// A member variable
				// Parse into type and name
				data = data.Left( data.GetLength() - 1 ); // remove semiconol
				int space = data.Find( _TCHAR( ' ' ) );
				if( space != -1 )
				{
					CAttribute* attr = new CAttribute;
					attr->maintype = ENTITY_TYPE_NONE;

				CString staticmarker( _T( "static " ) );
				int staticmarkerfound = data.Find( staticmarker ) ;
				if( staticmarkerfound != -1 )
				{
					data = data.Left( staticmarkerfound ) + data.Right( data.GetLength() - ( staticmarkerfound + staticmarker.GetLength() ) );
					attr->maintype |= ENTITY_TYPE_STATIC;
				}

					int space = data.Find( _TCHAR( ' ' ) );
					CString type = data.Left( space );
					CString name = data.Right( data.GetLength() - ( space + 1 ) );
					int bracket = name.Find( _TCHAR( '[' ) );
					if( bracket != -1 )
					{
						CString multiplicity = name.Right( name.GetLength() - ( bracket + 1 ) );
						name = name.Left( bracket );
						if( multiplicity.GetLength() )
							multiplicity = multiplicity.Left( multiplicity.GetLength() - 1 ); // Removing closing bracket
						attr->multiplicity = multiplicity;
					}

					attr->access = access;
					attr->name = name;
					attr->type = type;

					AddAttribute( attr );
				}
			}
		}

		SetTitle( className );

		if( baseClass.GetLength() )
		{
			CString base;
			CTokenizer tok( baseClass );
			int max = tok.GetSize();
			for( int t = 0 ; t < max ; t++ )
			{
				CString cls;
				tok.GetAt( t, cls );
				cls.TrimLeft();
				int find = cls.Find( _TCHAR( ' ' ) );
				if( find != -1 )
					cls = cls.Right( cls.GetLength() - ( find + 1 ) );
				cls.TrimLeft();
				cls.TrimRight();
				base += cls + _T( " " );
			}

			base.TrimRight();
			GetProperties()->Add( _T( "baseClass" ), base );

		}

		CalcRestraints();
		return TRUE;
	}

	return FALSE;
}

void CUMLEntityClass::SetTitle( CString title )
/* ============================================================
	Function :		CUMLEntityClass::SetTitle
	Description :	Sets the title of this object.
	Access :		Public

	Return :		void
	Parameters :	CString title	-	New title
					
	Usage :			Call to set the title of this object. 
					Overridden to remove linefeeds and 
					recalculate the object restraint rectangle.

   ============================================================*/
{

	title.TrimLeft();
	title.TrimRight();
	title.Remove( TCHAR( '\r' ) );
	title.Remove( TCHAR( '\n' ) );
	title.Remove( TCHAR( ' ' ) );

	CUMLEntity::SetTitle( title );
	CalcRestraints();

}

CString CUMLEntityClass::GetIncludeList() const
/* ============================================================
	Function :		CUMLEntityClass::GetIncludeList
	Description :	Gets a list of include files for a h-file
					export of this object.
	Access :		Protected

	Return :		CString	-	Result
	Parameters :	none

	Usage :			Call to get a list of the necessary include 
					files (and forward declarations) for this 
					object when exporting to a c++ header file.

   ============================================================*/
{

	CUMLEntityClass* const local = const_cast<CUMLEntityClass* const>( this );

	CString result;
	CUMLEntityContainer* container = GetUMLContainer();
	if( container )
	{
		CStringArray stra;
		container->GetIncludeList( local, stra );
		int max = stra.GetSize();
		for( int t = 0 ; t < max ; t++ )
		{
			if( stra[ t ][ 0 ] == _TCHAR( '#' ) )
				result += _T( "class " ) + stra[ t ].Right( stra[ t ].GetLength() - 1 ) + _T( ";\r\n" );
			else
				result += _T( "#include \"" ) + stra[ t ] + _T( ".h\"\r\n" );
		}
	}

	return result;

}

CString CUMLEntityClass::GetDependencyList() const
/* ============================================================
	Function :		CUMLEntityClass::GetDependencyList
	Description :	Gets a list of include files for a cpp-file
					export of this object.
	Access :		Protected

	Return :		CString	-	Result
	Parameters :	none

	Usage :			Call to get a list of the necessary include 
					files (and forward declarations) for this 
					object when exporting to a c++ cpp file.

   ============================================================*/
{

	CUMLEntityClass* const local = const_cast<CUMLEntityClass* const>( this );

	CString result;
	CUMLEntityContainer* container = GetUMLContainer();
	if( container )
	{
		CStringArray stra;
		container->GetDependencyList( local, stra );
		int max = stra.GetSize();
		for( int t = 0 ; t < max ; t++ )
			result += _T( "#include \"" ) + stra[ t ] + _T( ".h\"\r\n" );
	}

	return result;

}

CPropertyContainer* CUMLEntityClass::GetProperties()
/* ============================================================
	Function :		CUMLEntityClass::GetProperties
	Description :	Gets a pointer to the property container 
					for this object.
	Access :		Public

	Return :		CPropertyContainer*	-	A pointer to the 
											property container.
	Parameters :	none

	Usage :			Call to get a pointer to the property 
					container.

   ============================================================*/
{

	return &m_properties;

}

CString CUMLEntityClass::ExportHTML() const
/* ============================================================
	Function :		CUMLEntityClass::ExportHTML
	Description :	Exports the contents of this object to HTML.
	Access :		Protected

	Return :		CString	-	Resulting HTML.
	Parameters :	none

	Usage :			Call to get the contents of this object as 
					HTML.

   ============================================================*/
{

	CString result;

	CRect rect = GetRect();
	int lineheight = 14;
	int height = 12;
	int cut = GetMarkerSize().cx;
	int lines = 1;
	if( GetStereotype().GetLength() )
		lines++;
	lines += m_properties.GetSize();

	int left = rect.left;
	int top = rect.top;
	int width = rect.Width();
	int height_of_name_compartment = lines * lineheight + cut;
	lines = max( 1, GetVisibleAttributes() );
	int height_of_attribute_compartment = lines * lineheight + cut;
	lines = max( 1, GetVisibleOperations() );
	int height_of_operation_compartment = lines * lineheight + cut;
	CString background_color_string = ColorrefToString( GetBkColor() );
	int font_size_in_pixels = height;
	CString font_name = GetFont();

	int totalheight = height_of_name_compartment + height_of_attribute_compartment + height_of_operation_compartment;
	if( totalheight < rect.Height() )
	{
		int diff = rect.Height() - totalheight;
		height_of_operation_compartment += diff;
	}

	CString contents_of_name_compartment;
	if( GetStereotype().GetLength() )
		contents_of_name_compartment = _T( "« " ) + GetStereotype() + _T( " »<br>" );

	BOOL abstract = IsAbstract();
	if( abstract )
		contents_of_name_compartment += _T( "<i>" );
	contents_of_name_compartment += _T( "<b>" ) + GetTitle() +  _T( "</b>" );
	if( abstract )
		contents_of_name_compartment += _T( "</i>" );

	CString propertylist = m_properties.GetString( STRING_FORMAT_HTML );
	if( propertylist.GetLength() )
		contents_of_name_compartment += _T( "<br>" ) + propertylist;

	CString contents_of_attribute_compartment = GetAttributeList( EXPORT_HTML );
	CString contents_of_operation_compartment = GetOperationList( EXPORT_HTML );

	CString nameCompartment;
	nameCompartment.Format( _T( "<div style=\'position:absolute;overflow:hidden;left:%i;top:%i;width:%i;height:%i;border:1px solid black;background-color:#%s;font-size:%i;font-family:%s;text-align:center;\'>%s</div>" ),
								left, top, width, height_of_name_compartment, background_color_string, font_size_in_pixels, font_name,
								contents_of_name_compartment );

	CString attributeCompartment;
	attributeCompartment.Format( _T( "<div style=\'position:absolute;overflow:hidden;left:%i;top:%i;width:%i;height:%i;border:1px solid black;background-color:#%s;font-size:%i;font-family:%s;text-align:left;\'>%s</div>" ),
	left, top + height_of_name_compartment - 1, width, height_of_attribute_compartment, background_color_string, font_size_in_pixels, font_name,
	contents_of_attribute_compartment );

	CString operationCompartment;
	operationCompartment.Format( _T( "<div style=\'position:absolute;overflow:hidden;left:%i;top:%i;width:%i;height:%i;border:1px solid black;background-color:#%s;font-size:%i;font-family:%s;text-align:left;\'>%s</div>" ),
	left, top + ( height_of_name_compartment - 1 ) + ( height_of_attribute_compartment - 1 ), width, height_of_operation_compartment, background_color_string, font_size_in_pixels, font_name,
	contents_of_operation_compartment );

	result = nameCompartment + attributeCompartment + operationCompartment;

	return result;

}

BOOL CUMLEntityClass::InBaseClassArray( const CString& title ) const
/* ============================================================
	Function :		CUMLEntityClass::InBaseClassArray
	Description :	Checks if "title" is a class that is 
					already registered as a base class for 
					this object.
	Access :		Private

	Return :		BOOL					-	"TRUE" if "title" 
												is already registered 
												as a base class.
	Parameters :	const CString& title	-	Class to test
					
	Usage :			Call to see if "title" has already been 
					registered as a base class

   ============================================================*/
{

	BOOL result = FALSE;

	int max = m_baseClassArray.GetSize();
	for( int t = 0 ; t < max ; t++ )
	{
		if( title == m_baseClassArray[ t ] )
			result = TRUE;
	}

	return result;

}

COperationContainer* CUMLEntityClass::GetOperationsContainer()
/* ============================================================
	Function :		CUMLEntityClass::GetOperationsContainer
	Description :	Returns a pointer to the operation container.
	Access :		Public

	Return :		COperationContainer*	-	Pointer to the 
												operation container.
	Parameters :	none

	Usage :			Call to get a pointer to the operation 
					container.

   ============================================================*/
{
	return &m_operations;
}

CAttributeContainer* CUMLEntityClass::GetAttributesContainer()
/* ============================================================
	Function :		CUMLEntityClass::GetAttributesContainer
	Description :	Returns a pointer to the attribute container.
	Access :		Public

	Return :		CAttributeContainer*	-	Pointer to the 
												attribute container.
	Parameters :	none

	Usage :			Call to get a pointer to the attribute 
					container.


   ============================================================*/
{
	return &m_attributes;
}

CString CUMLEntityClass::GetHeaderTemplateFilename() const
/* ============================================================
	Function :		CUMLEntityClass::GetHeaderTemplateFilename
	Description :	Returns the name of the prefered header 
					file template.
	Access :		Private

	Return :		CString	-	Name of header file template
	Parameters :	none

	Usage :			Call to get the name of the prefered header 
					file template during generation to c++.

   ============================================================*/
{
	return _T( "h_template.txt" );
}
