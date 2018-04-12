/* ==========================================================================
	Class :			CAttribute

	Author :		Johan Rosengren, Abstrakt Mekanik AB

	Date :			06/02/04

	Purpose :		The "CAttribute" class - derived from "CObject" - represents 
					a single attribute in an UML-class. The attributes are 
					stored in a "CObArray" in the "CAttributeContainer" class.

	Description :	The class is mainly a holder for attribute attributes (!) 
					and functions to create them and get string 
					representations in different formats.

	Usage :			Create and store in a "CAttributeContainer" instance.

   ========================================================================*/

#include "stdafx.h"
#include "Attribute.h"
#include "DiagramEditor/Tokenizer.h"
#include "StringHelpers.h"
#include "UMLEntity.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Construction/destruction
CAttribute::CAttribute()
/* ============================================================
	Function :		CAttribute::CAttribute
	Description :	Constructor
	Access :		Public

	Return :		void
	Parameters :	none

	Usage :			

   ============================================================*/
{

	maintype = 0;
	access = 0;

}

CAttribute::~CAttribute()
/* ============================================================
	Function :		CAttribute::~CAttribute
	Description :	Destructor					
	Access :		Public

	Return :		void
	Parameters :	none

	Usage :			

   ============================================================*/
{
}

// Implementation
CString CAttribute::ToString( BOOL nomarkers )
/* ============================================================
	Function :		CAttribute::ToString
	Description :	Returns a string representation in UML-
					format for presenting the attribute 
					on screen.
	Access :		Public

	Return :		
	Parameters :	CAttribute* obj	-	Attribute to display
					BOOL nomarkers	-	If access markers should 
										be displayed or hidden.

	Usage :			Call to get a string to display on screen. 
					The function takes display settings into 
					consideration.

   ============================================================*/
{

	CString output;
	if( stereotype.GetLength() )
		output = _T( "« " ) + stereotype +  _T( " » " );

	if( !nomarkers )
	{
		if( access == ACCESS_TYPE_PRIVATE )
			output = _T( "-" );
		if( access == ACCESS_TYPE_PROTECTED )
			output = _T( "#" );
		if( access == ACCESS_TYPE_PUBLIC )
			output = _T( "+" );
	}
	output += name;
	output += _T( ":" );
	output += type;

	if( multiplicity.GetLength() )
		output += _T( "[ " ) + multiplicity + _T( " ]" );

	if( defaultvalue.GetLength() )
		output += _T( " = " ) + defaultvalue;

	output += properties.GetString( STRING_FORMAT_UML );

	return output;

}

CAttribute* CAttribute::Clone( )
/* ============================================================
	Function :		CAttribute::Clone
	Description :	Creates and return a clone of this 
					attribute.
	Access :		Public

	Return :		CAttribute*	-	New attribute
	Parameters :	none

	Usage :			Call to clone the attribute, for example 
					when copying the parent UML-class object.

   ============================================================*/
{

	CAttribute* obj = new CAttribute;

	obj->maintype = maintype;
	obj->name = name;
	obj->access = access;
	obj->type = type;

	obj->multiplicity = multiplicity;
	obj->defaultvalue = defaultvalue;
	obj->properties.Copy( properties );

	obj->stereotype = stereotype;

	return obj;

}

CAttribute* CAttribute::FromString( const CString& str )
/* ============================================================
	Function :		CAttribute::FromString
	Description :	Creates a new "CAttribute" from the string "str"			
	Access :		Public

	Return :		CAttribute*			-	New attribute or "NULL" 
											if "str" doesn't 
											represent an attribute.
	Parameters :	const CString& str	-	String to create 
											attribute from

	Usage :			Call - for example - when loading diagrams 
					from a file.

   ============================================================*/
{
	CAttribute* at = NULL;

	CTokenizer tok( str, _T( "|" ) );
	if( tok.GetSize() >= 8 )
	{
		at = new CAttribute;
		CString	multi;
		CString	defaultvalue;
		CString	propertylist;
		CString stereotype;

		tok.GetAt(0, at->maintype );
		tok.GetAt(1, at->access );
		tok.GetAt(2, at->name );
		tok.GetAt(3, at->type );
		tok.GetAt(4, multi );
		tok.GetAt(5, defaultvalue );
		tok.GetAt(6, propertylist );
		tok.GetAt(7, stereotype );

		UnmakeSaveString( multi );
		UnmakeSaveString( defaultvalue );
		UnmakeSaveString( propertylist );
		UnmakeSaveString( stereotype );

		at->multiplicity = multi;
		at->defaultvalue = defaultvalue;
		at->properties.FromString( propertylist );
		at->stereotype = stereotype;

	}

	return at;

}

CString CAttribute::GetString( int format ) const
/* ============================================================
	Function :		CAttribute::GetString
	Description :	Returns a string representation of the 
					object in the format specified.
	Access :		Public

	Return :		CString		-	The resulting string
	Parameters :	int format	-	The format to use.

	Usage :			"format" can be one of:
						"STRING_FORMAT_SAVE" For saving to file
						"STRING_FORMAT_CPP" cpp-file format.
						"STRING_FORMAT_UML" UML-format. Use "ToString" instead.
						"STRING_FORMAT_H" h-file format
						"STRING_FORMAT_HTML" HTML-format
						"STRING_FORMAT_H_CTOR" "ctor" in a header

   ============================================================*/
{

	CString result;
	if( format == STRING_FORMAT_SAVE )
	{
		CString multi( multiplicity );
		CString defval( defaultvalue );
		CString propertylist( properties.GetString( STRING_FORMAT_SAVE ) );
		CString stereo( stereotype );

		MakeSaveString( multi );
		MakeSaveString( defval );
		MakeSaveString( propertylist );
		MakeSaveString( stereo );

		result.Format( _T( "%i|%i|%s|%s|%s|%s|%s|%s" ), 
			maintype,
			access,
			name,
			type,
			multi,
			defval,
			propertylist,
			stereo
		);
	}

	if( format == STRING_FORMAT_H )
	{
		if( maintype & ENTITY_TYPE_STATIC )
			result += _T( "static " );

		result += type + _T( " " );
		result += name;
		if( multiplicity.GetLength() )
		{
			CString multiplicity( multiplicity );
			int found = multiplicity.Find( _T( "." ) );
			while( found != -1 )
			{
				multiplicity = multiplicity.Right( multiplicity.GetLength() - ( found + 1 ) );
				found = multiplicity.Find( _T( "." ) );
			}

			result += _T( "[ " ) + multiplicity +  _T( " ]" );
		}
		result += _T( ";" );
	}

	return result;

}
