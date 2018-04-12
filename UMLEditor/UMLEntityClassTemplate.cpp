/* ==========================================================================
	Class :			CUMLEntityClassTemplate

	Author :		Johan Rosengren, Abstrakt Mekanik AB

	Date :			2004-07-12

	Purpose :		"CUMLEntityClassTemplate" is a specialization of 
					"CUMLEntityClass" and represents a template class.

	Description :	The class derives from "CUMLEntityClass" and adds 
					functionality to handle the template parameter as 
					well as specialized export-functionality.

	Usage :			Use as a "CUMLEntityClass".

   ========================================================================*/

#include "stdafx.h"
#include "UMLEntityClassTemplate.h"
#include "UMLEntityContainer.h"
#include "TextFile/TextFile.h"

CUMLEntityClassTemplate::CUMLEntityClassTemplate()
/* ============================================================
	Function :		CUMLEntityClassTemplate::CUMLEntityClassTemplate
	Description :	Constructor
	Access :		Public

	Return :		void
	Parameters :	none

	Usage :			

   ============================================================*/
{
	SetType( _T( "uml_template" ) );

	CString title;
	title.LoadString( IDS_UML_TEMPLATE );
	SetTitle( title );

	SetPropertyDialog( &m_dlg, CUMLTemplatePropertyDialog::IDD );

}

CUMLEntityClassTemplate::~CUMLEntityClassTemplate()
/* ============================================================
	Function :		CUMLEntityClassTemplate::~CUMLEntityClassTemplate
	Description :	Destructor
	Access :		Public

	Return :		void
	Parameters :	none

	Usage :			

   ============================================================*/
{

	if( m_dlg.m_hWnd )
		m_dlg.DestroyWindow();

	ClearAttributes();
	ClearOperations();
	ClearProperties();

}

CDiagramEntity* CUMLEntityClassTemplate::Clone()
/* ============================================================
	Function :		CUMLEntityClassTemplate::Clone
	Description :	Clone this object to a new object.
	Access :		Public
					
	Return :		CDiagramEntity*	-	The new object.
	Parameters :	none

	Usage :			Call to create a clone of the object. The 
					caller will have to delete the object.

   ============================================================*/
{

	CUMLEntityClassTemplate* obj = new CUMLEntityClassTemplate;
	obj->Copy( this );
	return obj;

}

BOOL CUMLEntityClassTemplate::FromString( const CString& str )
/* ============================================================
	Function :		CUMLEntityClassTemplate::FromString
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
		CString parameterType;
		int		attributes;
		int		operations;
		
		int		count = 0;

		tok.GetAt( count++, package );
		tok.GetAt( count++, fontName );
		tok.GetAt( count++, bkColor );
		tok.GetAt( count++, stereotype );
		tok.GetAt( count++, propertylist );
		tok.GetAt( count++, displayOptions );
		tok.GetAt( count++, parameterType );
		tok.GetAt( count++, attributes);
		tok.GetAt( count++, operations);

		UnmakeSaveString( package );
		UnmakeSaveString( stereotype );
		UnmakeSaveString( propertylist );
		UnmakeSaveString( parameterType );

		SetPackage( package );
		SetStereotype( stereotype );
		SetFont( fontName );
		SetBkColor( static_cast< COLORREF >( bkColor ) );
		SetDisplayOptions( displayOptions );
		SetParameterType( parameterType );
		GetProperties()->FromString( propertylist );

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

CDiagramEntity* CUMLEntityClassTemplate::CreateFromString( const CString& str )
/* ============================================================
	Function :		CUMLEntityClassTemplate::CreateFromString
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

   ============================================================*/
{

	CUMLEntityClassTemplate* obj = new CUMLEntityClassTemplate;
	if(!obj->FromString( str ) )
	{
		delete obj;
		obj = NULL;
	}

	return obj;

}

CString CUMLEntityClassTemplate::GetString() const
/* ============================================================
	Function :		CUMLEntityClassTemplate::GetString
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
	CString propertylist = ( const_cast< CUMLEntityClassTemplate* >( this )->GetProperties() )->GetString();
	CString param = GetParameterType();

	MakeSaveString( package );
	MakeSaveString( stereotype );
	MakeSaveString( propertylist );
	MakeSaveString( param );

	int attributes = GetAttributes();
	int operations = GetOperations();

	str.Format( _T( ",%s,%s,%i,%s,%s,%i,%s,%i,%i" ), 
			package,
			GetFont(),
			static_cast< int >( GetBkColor() ),
			stereotype,
			propertylist,
			GetDisplayOptions(),
			param,
			attributes,
			operations
		);

	str += ( const_cast< CUMLEntityClassTemplate* >( this )->GetAttributesContainer() )->GetString();
	str += ( const_cast< CUMLEntityClassTemplate* >( this )->GetOperationsContainer() )->GetString();

	str += _T( ";" );
	str = GetDefaultGetString() + str;
	return str;

}

void CUMLEntityClassTemplate::Draw( CDC* dc, CRect rect )
/* ============================================================
	Function :		CUMLEntityClassTemplate::Draw
	Description :	Draws the template.
	Access :		

	Return :		void
	Parameters :	CDC* dc		-	"CDC" to draw to
					CRect rect	-	Rectangle to draw to.
					
	Usage :			Called by the editor. Draws the underlying 
					class object and adds the template parameter 
					type box if the parameter type is not empty.

   ============================================================*/
{
	CUMLEntityClass::Draw( dc, rect );

	CString param = GetParameterType();
	if( param.GetLength() )
	{
		dc->SelectStockObject( WHITE_BRUSH );
		CPen pen;
		pen.CreatePen( PS_DOT, 0, RGB( 0, 0, 0 ) );
		dc->SelectObject( &pen );

		int cut = round( GetZoom() * static_cast< double >( GetMarkerSize().cx ) );

		CFont font;
		int height = round( 10.0 * GetZoom() );
		font.CreateFont( -height, 0,0,0,FW_NORMAL,0,0,0,0,0,0,0,0, GetFont() );
		CFont* oldfont = NULL;
		oldfont = dc->SelectObject( &font );

		int width = max( rect.Width() / 2, dc->GetTextExtent( param ).cx );

		CRect tpl( rect );
		tpl.right += cut;
		tpl.left = tpl.right - ( width + cut * 2 );
		tpl.bottom = rect.top + cut / 2;
		tpl.top = tpl.bottom  - ( round( GetZoom() * 12 ) + cut );

		dc->Rectangle( tpl );
		tpl.InflateRect( -1, -1 );
		int mode = dc->SetBkMode( TRANSPARENT );
		dc->DrawText( param, tpl, DT_NOPREFIX | DT_SINGLELINE | DT_CENTER | DT_VCENTER );

		dc->SelectObject( oldfont );
		dc->SetBkMode( mode );
		dc->SelectStockObject( WHITE_BRUSH );
		dc->SelectStockObject( BLACK_PEN );
	}
}

void CUMLEntityClassTemplate::Copy( CDiagramEntity* obj )
/* ============================================================
	Function :		CUMLEntityClassTemplate::Copy
	Description :	Copies the information in "obj" to this 
					object.
	Access :		

	Return :		void
	Parameters :	CDiagramEntity* obj	-	Object to copy from.
					
	Usage :			Call to copy the information from "obj" to 
					this object.

   ============================================================*/
{
	CUMLEntityClass::Copy( obj );

	CUMLEntityClassTemplate* uml = dynamic_cast< CUMLEntityClassTemplate* >( obj );
	SetParameterType( uml->GetParameterType() );

}

void CUMLEntityClassTemplate::SetParameterType( const CString& parameterType )
/* ============================================================
	Function :		CUMLEntityClassTemplate::SetParameterType
	Description :	Sets the template class parameter type.
	Access :		

	Return :		void
	Parameters :	const CString& parameterType	-	New parameter 
														type
					
	Usage :			Call to set the parameter type of the 
					template.

   ============================================================*/
{
	m_parameterType = parameterType;
}

CString CUMLEntityClassTemplate::GetParameterType() const
/* ============================================================
	Function :		CUMLEntityClassTemplate::GetParameterType
	Description :	Gets the template class parameter type.
	Access :		

	Return :		CString	-	Parameter type, empty if none.
	Parameters :	none

	Usage :			Call to get the parameter type of the 
					template.

   ============================================================*/
{
	return m_parameterType;
}

CString CUMLEntityClassTemplate::Export( UINT format ) const
/* ============================================================
	Function :		CUMLEntityClassTemplate::Export
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

	if( format == EXPORT_H )
		result = ExportH();

	if( format == EXPORT_HTML )
		result = ExportHTML();

	return result;
}

CString CUMLEntityClassTemplate::ExportHTML() const
/* ============================================================
	Function :		CUMLEntityClassTemplate::ExportHTML
	Description :	Exports the class to DHTML
	Access :		Private

	Return :		CString	-	Generated HTML
	Parameters :	none

	Usage :			Called by the export-mechanism

   ============================================================*/
{

	CString result;
	result = CUMLEntityClass::ExportHTML();
	int left;
	int top;
	int width;
	int height;

	CString param = GetParameterType();
	CRect rect = GetRect();
	CClientDC	dc( AfxGetMainWnd() );
	CFont font;
	font.CreateFont( -10, 0,0,0,FW_NORMAL,0,0,0,0,0,0,0,0, GetFont() );
	CFont* oldfont = NULL;
	oldfont = dc.SelectObject( &font );
	width = max( rect.Width() / 2, dc.GetTextExtent( param ).cx );
	dc.SelectObject( oldfont );

	int cut = GetMarkerSize().cx;
	CRect tpl( rect );
	tpl.right += cut;
	tpl.left = tpl.right - ( width + cut * 2 );
	tpl.bottom = rect.top + cut / 2;
	tpl.top = tpl.bottom  - ( 12 + cut );

	left = tpl.left;
	top = tpl.top;
	width = tpl.Width();
	height = tpl.Height();

	CString prefix;
	prefix.Format( _T( "<div style='position:absolute;overflow:hidden;left:%i;top:%i;width:%i;height:%i;border:1px dashed black;background-color:#ffffff;font-size:10;font-family:Arial;text-align:center;'>%s</div>\n" ),
		left,
		top,
		width,
		height,
		param );

	result += prefix;
	return result;

}

CString CUMLEntityClassTemplate::ExportH() const
/* ============================================================
	Function :		CUMLEntityClassTemplate::ExportH
	Description :	Exports the class to a c++-header file.
	Access :		Private

	Return :		CString	-	Generated contents of header 
								file.
	Parameters :	none

	Usage :			Called by the export-mechanism

   ============================================================*/
{

	CString result;
	result = CUMLEntityClass::ExportH();
	result.Replace( _T( "[%parametertype%]" ), GetParameterType() );

	return result;

}

CString CUMLEntityClassTemplate::GetHeaderTemplateFilename() const
/* ============================================================
	Function :		CUMLEntityClassTemplate::GetHeaderTemplateFilename
	Description :	Returns the name of the prefered header 
					file template.
	Access :		

	Return :		CString	-	Name of header file template
	Parameters :	none

	Usage :			Call to get the name of the prefered header 
					file template during generation to c++.

   ============================================================*/
{

	return _T( "h_template_template.txt" );

}

CString CUMLEntityClassTemplate::GetOperationList( int format ) const
/* ============================================================
	Function :		CUMLEntityClassTemplae::GetOperationList
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

	if( format == EXPORT_H )
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
						result += op->GetString( STRING_FORMAT_H_CTOR_TEMPLATE ) + _T( "\r\n" );
					else
						result += op->GetString( STRING_FORMAT_H_TEMPLATE ) + _T( "\r\n" );

					oldaccess = op->access;
				}

			}
		}
	}
	else
		result = CUMLEntityClass::GetOperationList( format );

	return result;
}

#define STATE_STARTING	0
#define STATE_READING	1

BOOL CUMLEntityClassTemplate::ImportH( const CString& filename )
/* ============================================================
	Function :		CUMLEntityClassTemplate::ImportH
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
						if( data.Left( 8 ) == _T( "template" ) )
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
		CString parameterType;

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
				data.Replace( _T( "< " ), _T( "<" ) );
				data.Replace( _T( " >" ), _T( ">" ) );

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
					{
						int space = data.ReverseFind( _TCHAR( ' ' ) );
						if( space != -1 )
						{
							className = data.Right( data.GetLength() - ( space + 1 ) );
							parameterType = data.Left( space );
							parameterType = parameterType.Right( parameterType.GetLength() - 9 );
							space = parameterType.Find( _TCHAR( '>' ) );
							if( space != -1 )
								parameterType = parameterType.Left( space );
						}
					}
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
		SetParameterType( parameterType );

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

