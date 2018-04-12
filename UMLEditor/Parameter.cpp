/* ==========================================================================
	Class :			CParameter

	Author :		Johan Rosengren, Abstrakt Mekanik AB

	Date :			06/06/04

	Purpose :		"CParameter", derived from "CObject", represents a single 
					parameter for an operation.

	Description :	Parameters will be kept in a "CParameterContainer" 
					member of the operation. 

	Usage :			Create from the owning "COperation".

   ========================================================================*/

#include "stdafx.h"
#include "Parameter.h"
#include "DiagramEditor/Tokenizer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// Construction/destruction
CParameter::CParameter()
/* ============================================================
	Function :		CParameter::CParameter
	Description :	Constructor
	Access :		Public
		
	Return :		void
	Parameters :	none

	Usage :			

   ============================================================*/
{

	reference = FALSE;
	out = FALSE;
	in = TRUE;

}

CParameter::~CParameter()
/* ============================================================
	Function :		CParameter::~CParameter
	Description :	Destructor
	Access :		Public

	Return :		void
	Parameters :	none

	Usage :			

   ============================================================*/
{
}

CParameter::CParameter( CParameter* parameter )
/* ============================================================
	Function :		CParameter::CParameter
	Description :	Constructor
	Access :		Public

	Return :		void
	Parameters :	CParameter* parameter	-	copy values 
												from this 
												parameter.
					
	Usage :			

   ============================================================*/
{

	name = parameter->name;
	type = parameter->type;
	defaultvalue = parameter->defaultvalue;
	reference = parameter->reference;
	in = parameter->in;
	out = parameter->out;

}

// Implementation
CParameter* CParameter::FromString( const CString& str )
/* ============================================================
	Function :		CParameter::FromString
	Description :	Create a "CParameter" from "str"
	Access :		Public
					
	Return :		CParameter*			-	Resulting parameter, 
											or "NULL" if "str" is not 
											a valid representation.
	Parameters :	const CString& str	-	"CString" to create from
					
	Usage :			Called from the load functionality.

   ============================================================*/
{

	CParameter* result = NULL;

	CTokenizer tok( str, _T( "#" ) );
	int max = tok.GetSize();
	if( max == 6 )
	{
		CString name;
		CString type;
		CString defaultvalue;
		int		reference;
		int		in;
		int		out;
		int count = 0;
		tok.GetAt( count++, name );
		tok.GetAt( count++, type );
		tok.GetAt( count++, defaultvalue );
		tok.GetAt( count++, reference );
		tok.GetAt( count++, in );
		tok.GetAt( count++, out );

		result = new CParameter;
		result->name = name;
		result->type = type;
		result->defaultvalue = defaultvalue;
		result->reference = reference;
		result->in = in;
		result->out = out;

	}

	return result;

}

CString CParameter::GetString( int format ) const
/* ============================================================
	Function :		CParameter::GetString
	Description :	Returns a string representation in "format" 
					of this object.
	Access :		Public
					
	Return :		CString		-	Resulting string
	Parameters :	int format	-	Format to use

	Usage :			Call to get a string representation of this 
					parameter. "format" can be one of:
						"STRING_FORMAT_SAVE" For saving to file
						"STRING_FORMAT_CPP" cpp-file format
						"STRING_FORMAT_UML" UML-format
						"STRING_FORMAT_H" h-file format
						"STRING_FORMAT_HTML" HTML-format
						"STRING_FORMAT_H_CTOR" ctor in a header

  ============================================================*/
{

	CString result;

	switch( format )
	{
		case STRING_FORMAT_SAVE:

			result.Format( _T( "%s#%s#%s#%i#%i#%i" ),
					name,
					type,
					defaultvalue,
					static_cast< int >( reference ),
					static_cast< int >( in ),
					static_cast< int >( out )
				);

			break;
		case STRING_FORMAT_CPP:
			{

				result = type + _T( " " );
				if( reference )
				{
					if( out ) // alias
						result += _T( "& " );
					else // const ref
						result = _T( "const " ) + result + _T( "& " );
				}
				else
				{
					if( out ) // pointer
						result += _T( "* " );
				}

				result += name;

			}
			break;

		case STRING_FORMAT_H:
			{

				result = type + _T( " " );
				if( reference )
				{
					if( out ) // alias
						result += _T( "& " );
					else // const ref
						result = _T( "const " ) + result + _T( "& " );
				}
				else
				{
					if( out ) // pointer
						result += _T( "* " );
				}

				result += name;

				if( defaultvalue.GetLength() )
					result += _T( " = " ) + defaultvalue;

			}
			break;

		case STRING_FORMAT_UML:
			{

				if( in && out )
					result = _T( "inout " );
				else if( out )
					result = _T( "out " );

				result += name + _T( ":" ) + type;

				if( defaultvalue.GetLength() )
					result += _T( " = " ) + defaultvalue;

			}
			break;
	}

	return result;

}

CString CParameter::ToString( BOOL nooperationattributenames ) const
/* ============================================================
	Function :		CParameter::ToString
	Description :	Returns a string representation in "format" 
					of this object.
	Access :		Public
					
	Return :		CString		-	Resulting string
	Parameters :	BOOL nooperationattributenames	-	"TRUE" if parameter name
														should be supressed.

	Usage :			Call to get a string representation of this 
					parameter for the screen.

  ============================================================*/
{

	CString result;

	if( in && out )
		result = _T( "inout " );
	else if( out )
		result = _T( "out " );

	if( !nooperationattributenames )
		result += name + _T( ":" );

	result += type;

	if( defaultvalue.GetLength() )
		result += _T( " = " ) + defaultvalue;

	return result;

}

BOOL CParameter::operator==( const CParameter& parameter )
/* ============================================================
	Function :		CParameter::operator==
	Description :	Returns "TRUE" if this parameter and 
					"parameter" contains the same information.
	Access :		Public

	Return :		BOOL						-	"TRUE" if the same
	Parameters :	const CParameter& parameter	-	Parameter to test
					
	Usage :			Call to see if two parameters contains the 
					same information.

   ============================================================*/
{

	BOOL result = FALSE;

	if( type == parameter.type &&
		reference == parameter.reference &&
		in == parameter.in &&
		out == parameter.out )
		result = TRUE;

	return result;

}

BOOL CParameter::operator!=( const CParameter& parameter )
/* ============================================================
	Function :		CParameter::operator!=
	Description :	Returns "TRUE" if the information in this 
					parameter and "parameter" are not identical.
	Access :		Public
					
	Return :		BOOL						-	"TRUE" if not the same
	Parameters :	const CParameter& parameter	-	Parameter to test
					
	Usage :			Call to see if two parameters are unequal.

   ============================================================*/
{

	return !( *this == parameter );

}