/* ==========================================================================
	Class :			CProperty

	Author :		Johan Rosengren, Abstrakt Mekanik AB

	Date :			06/06/04

	Purpose :		"CProperty", derived from "CObject", represents a single 
					property.

	Description :	"CProperty" is a simple class to hold a property. A 
					property consists of a tag and a value. 
					A "CPropertyContainer" holds the properties themselves.

	Usage :			Create and add to a "CPropertyContainer".

   ========================================================================*/

#include "stdafx.h"
#include "Property.h"
#include "DiagramEditor/Tokenizer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Construction/destruction
CProperty::CProperty()
/* ============================================================
	Function :		CProperty::CProperty
	Description :	Constructor
	Access :		Public

	Return :		void
	Parameters :	none

	Usage :			A "CPropertyContainer" will own this object.

   ============================================================*/
{
	key = _T( "" );
	value = _T( "" );
}

CProperty::~CProperty()
/* ============================================================
	Function :		CProperty::~CProperty
	Description :	Destructor
	Access :		Public

	Return :		void
	Parameters :	none

	Usage :			A "CPropertyContainer" will own this object.

   ============================================================*/
{
}

CProperty::CProperty( CProperty* property )
/* ============================================================
	Function :		CProperty::CProperty
	Description :	Constructor
	Access :		Public

	Return :		void
	Parameters :	CProperty* property	-	Property to copy from
					
	Usage :			A "CPropertyContainer" will own this object.

   ============================================================*/
{

	key = property->key;
	value = property->value;

}

CProperty::CProperty( const CString& tag, const CString& val )
/* ============================================================
	Function :		CProperty::CProperty
	Description :	Constructor
	Access :		Public

	Return :		void
	Parameters :	const CString& tag	-	Tag
					const CString& val	-	Value
					
	Usage :			A "CPropertyContainer" will own this object.

   ============================================================*/
{

	key = tag;
	value = val;

}

// Implementation
CString CProperty::GetString( int format ) const
/* ============================================================
	Function :		CParameter::GetString
	Description :	Gets this property as a string in the 
					format "format".
	Access :		Public
					
	Return :		CString		-	Result
	Parameters :	int format	-	Format to return.

	Usage :			Call to get a string representation of this 
					property. "format" can be one of:
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

			result.Format( _T( "%s=%s" ),
					key,
					value
				);

			break;

		case STRING_FORMAT_UML:
			if( value.GetLength() )
			{
				result.Format( _T( "%s = %s" ),
						key,
						value
					);
			}
			else
				result = key;
			break;
	}

	return result;

}

CProperty* CProperty::FromString( const CString& str )
/* ============================================================
	Function :		CProperty::FromString
	Description :	Fills this property from "str".
	Access :		Public

	Return :		CProperty*	-	Resulting property, or "NULL"
									if "str" was not a valid 
									representation.
	Parameters :	const CString& str	-	String to fill from
					
	Usage :			Use when loading data from a file.

   ============================================================*/
{
	CProperty* result = NULL;

	CTokenizer tok( str, _T( "=" ) );
	int max = tok.GetSize();
	if( max == 2 )
	{
		CString key;
		CString value;
		tok.GetAt( 0, key );
		tok.GetAt( 1, value );

		result = new CProperty;
		result->key = key;
		result->value = value;
	}

	return result;

}
