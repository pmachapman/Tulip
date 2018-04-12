/* ==========================================================================
	Class :			COperation

	Author :		Johan Rosengren, Abstrakt Mekanik AB

	Date :			06/02/04

	Purpose :		"COperation", derived from "CObject", represents a single 
					operation for a "CUMLEntityClass" object. 

	Description :	The operations are stored in a "COperationContainer" 
					member of "CUMLEntityClass".

	Usage :			Create from "CUMLEntityClass" and add to the 
					"COperationContainer".

   ========================================================================
	Changes :		12/7 2004	Added support for template C++-generation.
   ========================================================================*/

#include "stdafx.h"
#include "Operation.h"
#include "DiagramEditor/Tokenizer.h"
#include "StringHelpers.h"
#include "UMLEntity.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Construction/destruction
COperation::COperation()
/* ============================================================
	Function :		COperation::COperation
	Description :	Constructor
	Access :		Public

	Return :		void
	Parameters :	none

	Usage :			Always add to a container.

   ============================================================*/
{

	maintype = 0;
	access = 0;

	setter = FALSE;
	getter = FALSE;

}

COperation::~COperation()
/* ============================================================
	Function :		COperation::~COperation
	Description :	Destructor
	Access :		Public
					
	Return :		void
	Parameters :	none

	Usage :			Will be destroyed by the owning container

   ============================================================*/
{
}

// Implementation
CString COperation::ToString( BOOL nomarkers, BOOL nooperationattributenames )
/* ============================================================
	Function :		COperation::ToString
	Description :	Gets a display string representation in 
					UML-format of the operation.
	Access :		Public

	Return :		CString	-	display string
	Parameters :	COperation* obj					-	Operation to display
					BOOL nomarkers					-	"TRUE" if access markers
														should be supressed.
					BOOL nooperationattributenames	-	"TRUE" if parameter name
														should be supressed.
	
	Usage :			Call to get a display string for the operation.

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
	output += _T( "( " );
	output += parameters.ToString( nooperationattributenames );

	output += _T( " )" );
	if( type.GetLength() )
	{
		output += _T( ":" );
		output += type;
	}

	output += properties.GetString( STRING_FORMAT_UML );

	return output;

}

COperation* COperation::Clone( )
/* ============================================================
	Function :		COperation::Clone
	Description :	Returns a copy of this object.
	Access :		Public

	Return :		COperation*	-	New object
	Parameters :	none

	Usage :			Call to copy the operation

   ============================================================*/
{

	COperation* obj = new COperation;
	obj->maintype = maintype;
	obj->name = name;
	obj->access = access;
	obj->type = type;

	obj->parameters.Copy( parameters );
	obj->properties.Copy( properties );

	obj->stereotype = stereotype;

	obj->setter = setter;
	obj->getter = getter;
	obj->getsetvariable = getsetvariable;

	obj->parameters.Copy( parameters );

	return obj;

}

COperation* COperation::FromString( const CString& str )
/* ============================================================
	Function :		COperation::FromString
	Description :	Returns a new object from a string 
					representation.
	Access :		Public
					
	Return :		COperation*	-			New operation, or "NULL" if "str"
											is not a valid "COperation"
	Parameters :	const CString& str	-	String to use

	Usage :			Call from load-functions

   ============================================================*/
{

	COperation* op = NULL;
	CTokenizer tok( str, _T( "|" ) );
	if( tok.GetSize() == 10 )
	{
		op = new COperation;
		CString params;
		CString propertylist;
		CString stereotype;
		BOOL	getter;
		BOOL	setter;
		CString getsetvariable;

		int count = 0;
		tok.GetAt( count++, op->maintype );
		tok.GetAt( count++, op->access );
		tok.GetAt( count++, op->name );
		tok.GetAt( count++, op->type );
		tok.GetAt( count++, propertylist );
		tok.GetAt( count++, stereotype );
		tok.GetAt( count++, getter );
		tok.GetAt( count++, setter );
		tok.GetAt( count++, getsetvariable );
		tok.GetAt( count++, params );

		UnmakeSaveString( propertylist );
		UnmakeSaveString( params );
		UnmakeSaveString( stereotype );
		UnmakeSaveString( getsetvariable );

		op->properties.FromString( propertylist );
		op->parameters.FromString( params );

		op->stereotype = stereotype;
		op->getter = getter;
		op->setter = setter;
		op->getsetvariable = getsetvariable;

	}

	return op;

}

CString COperation::GetString( int format ) const
/* ============================================================
	Function :		COperation::GetString
	Description :	Returns a string representation in format 
					"format" of this object.
	Access :		Public
					
	Return :		CString		-	Result
	Parameters :	int format	-	Format to use

	Usage :			"format" can be one of:
						"STRING_FORMAT_SAVE" For saving to file
						"STRING_FORMAT_CPP" cpp-file format
						"STRING_FORMAT_UML" UML-format. Use "ToString" instead.
						"STRING_FORMAT_H" h-file format
						"STRING_FORMAT_HTML" HTML-format
						"STRING_FORMAT_H_CTOR" ctor in a header
						"STRING_FORMAT_H_TEMPLATE" h-file format for templates
						"STRING_FORMAT_H_CTOR_TEMPLATE" ctor in a header for a template class

   ============================================================*/
{

	CString result;
	switch( format )
	{
		case STRING_FORMAT_SAVE:
			{
				CString propertylist( properties.GetString() );
				CString stereo( stereotype );
				CString getset( getsetvariable );
				CString params( parameters.GetString() );

				MakeSaveString( propertylist );
				MakeSaveString( stereo );
				MakeSaveString( getset );
				MakeSaveString( params );

				result.Format( _T( "%i|%i|%s|%s|%s|%s|%i|%i|%s|%s" ),
					maintype,
					access,
					name,
					type,
					propertylist,
					stereo,
					getter,
					setter,
					getset,
					params );
			}
			break;

		case STRING_FORMAT_H:
			{

				// main type
				if( maintype & ENTITY_TYPE_ABSTRACT || properties.GetPropertyValue( _T( "virtual" ) ) == _T( "true" ) )
					result += _T( "\tvirtual " );

				else if( maintype & ENTITY_TYPE_STATIC )
					result += _T( "\tstatic " );
				else
					result += _T( "\t" );

				// return value
				if( type.GetLength() )
					result += type + _T( " " );
				else
					result += _T( "void " );

				// function name and parameters
				result += name + _T( "( " );
				result += parameters.GetString( STRING_FORMAT_H );

				// const
				result += _T( " )" );
				if( properties.GetPropertyValue( _T( "query" ) ) == _T( "true" ) )
					result += _T( " const" );

				// pure virtual
				if( maintype & ENTITY_TYPE_ABSTRACT )
					result += _T( " = 0" );

				result += _T( ";" );
			}
			break;

		case STRING_FORMAT_H_TEMPLATE:
			{

				// main type
				if( maintype & ENTITY_TYPE_ABSTRACT || properties.GetPropertyValue( _T( "virtual" ) ) == _T( "true" ) )
					result += _T( "\tvirtual " );

				else if( maintype & ENTITY_TYPE_STATIC )
					result += _T( "\tstatic " );
				else
					result += _T( "\t" );

				// return value
				if( type.GetLength() )
					result += type + _T( " " );
				else
					result += _T( "void " );

				// function name and parameters
				result += name + _T( "( " );
				result += parameters.GetString( STRING_FORMAT_H );

				// const
				result += _T( " )" );
				if( properties.GetPropertyValue( _T( "query" ) ) == _T( "true" ) )
					result += _T( " const" );

				// pure virtual
				if( maintype & ENTITY_TYPE_ABSTRACT )
					result += _T( " = 0" );

				result += _T( " {\r\n\t\t// TODO: Implement\r\n\t};" );

			}
			break;

		case STRING_FORMAT_H_CTOR:
			{
				// This is a constructor/destructor, and
				// we don't want any return value

				result += _T( "\t" );

				if( maintype & ENTITY_TYPE_ABSTRACT || properties.GetPropertyValue( _T( "virtual" ) ) == _T( "true" ) )
					result += _T( "virtual " );

				// function name and parameters
				result += name + _T( "( " );
				result += parameters.GetString( STRING_FORMAT_H );
				result += _T( " );" );

			}
			break;

		case STRING_FORMAT_H_CTOR_TEMPLATE:
			{
				// This is a constructor/destructor, and
				// we don't want any return value

				result += _T( "\t" );

				if( maintype & ENTITY_TYPE_ABSTRACT || properties.GetPropertyValue( _T( "virtual" ) ) == _T( "true" ) )
					result += _T( "virtual " );

				// function name and parameters
				result += name + _T( "( " );
				result += parameters.GetString( STRING_FORMAT_H );
				result += _T( " ) {\r\n\t\t//TODO: Implement\r\n\t};" );

			}
			break;
	}

	return result;

}

BOOL COperation::operator==( const COperation& operation )
/* ============================================================
	Function :		COperation::operator==
	Description :	Returns "TRUE" if this object containes the 
					same information as "operation".
	Access :		Public
					
	Return :		BOOL						-	"TRUE" if the same
	Parameters :	const COperation& operation	-	Operation to test
					
	Usage :			Call to see if two objects are equal 
					information-wise.

   ============================================================*/
{

	BOOL result = FALSE;

	if( name == operation.name &&
		type == operation.type &&
		parameters == operation.parameters )
		result = TRUE;

	return result;

}
