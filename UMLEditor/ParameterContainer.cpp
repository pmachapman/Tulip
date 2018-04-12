/* ==========================================================================
	Class :			CParameterContainer

	Author :		Johan Rosengren, Abstrakt Mekanik AB

	Date :			06/06/04

	Purpose :		A container for "CParameter"s

	Description :	The class has a "CObArray" to store the "CParameter"s, and 
					manages memory automatically. 

	Usage :			Use to handle arrays of parameters. As memory will be 
					freed automatically, use "Copy" to copy contents.

   ========================================================================*/

#include "stdafx.h"
#include "ParameterContainer.h"
#include "DiagramEditor/Tokenizer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// Construction/destruction
CParameterContainer::CParameterContainer()
/* ============================================================
	Function :		CParameterContainer::CParameterContainer
	Description :	Constructor
	Access :		Public

	Return :		void
	Parameters :	none

	Usage :			

   ============================================================*/
{
}

CParameterContainer::~CParameterContainer()
/* ============================================================
	Function :		CParameterContainer::~CParameterContainer
	Description :	Destructor
	Access :		Public

	Return :		void
	Parameters :	none

	Usage :			

   ============================================================*/
{

	RemoveAll();

}

void CParameterContainer::Copy( CParameterContainer& parameters )
/* ============================================================
	Function :		CParameterContainer::Copy
	Description :	Copies the contents of "parameters" to this 
					array.
	Access :		Public

	Return :		void
	Parameters :	CParameterContainer& parameters	-	Container to 
														copy from
					
	Usage :			Call to copy the contents of another array 
					to this one.

   ============================================================*/
{

	RemoveAll();
	int max = parameters.GetSize();
	for( int t = 0 ; t < max ; t++ )
		Add( new CParameter( parameters.GetAt( t ) ) );

}

// Implementation
int CParameterContainer::GetSize( ) const
/* ============================================================
	Function :		CParameterContainer::GetSize
	Description :	Gets the number of parameters in this 
					container.
	Access :		Public

	Return :		int	-	The number of parameters.
	Parameters :	none

	Usage :			Call to get the number of parameters in 
					this container.

   ============================================================*/
{

	return m_parameters.GetSize();

}

CParameter* CParameterContainer::GetAt( int index ) const
/* ============================================================
	Function :		CParameterContainer::GetAt
	Description :	Gets the parameter at "index".
	Access :		Public

	Return :		CParameter*	-	Parameter at "index", of "NULL"
									if out of bounds.
	Parameters :	int index	-	Index to get parameter from.
					
	Usage :			Call to get a pointer to a specific 
					parameter in the container.

   ============================================================*/
{

	CParameter* result = NULL;
	if( index < GetSize() )
		result = static_cast< CParameter* >( m_parameters.GetAt( index ) );

	return result;

}

void CParameterContainer::RemoveAt( int index )
/* ============================================================
	Function :		CParameterContainer::RemoveAt
	Description :	Removes the "CParameter" at "index."
	Access :		Public

	Return :		void
	Parameters :	int index	-	Index to remove parameter 
									from.
					
	Usage :			Call to remove a parameter from the 
					container. Will also delete memory.

   ============================================================*/
{

	if( index > -1 && index < GetSize() )
	{

		delete GetAt( index );
		m_parameters.RemoveAt( index );

	}

}

void CParameterContainer::RemoveAll( )
/* ============================================================
	Function :		CParameterContainer::RemoveAll
	Description :	Removes all parameters from the container.
	Access :		Public

	Return :		void
	Parameters :	none

	Usage :			Will also delete the associated memory.

   ============================================================*/
{

	while( GetSize() )
		RemoveAt( 0 );

}

void CParameterContainer::Add( CParameter* parameter )
/* ============================================================
	Function :		CParameterContainer::Add
	Description :	Add a "CParameter" to the container.
	Access :		Public

	Return :		void
	Parameters :	CParameter* parameter	-	"CParameter" to add.
					
	Usage :			Call to add a parameter to the container. 
					The container now owns the "CParameter".

   ============================================================*/
{

	m_parameters.Add( parameter );

}

CString CParameterContainer::ToString( BOOL nooperationattributenames ) const
/* ============================================================
	Function :		CParameterContainer::ToString
	Description :	Gets a string representation of the 
					"CParameter"s in this container, in
					screen format
	Access :		Public

	Return :		CString							-	Result
	Parameters :	BOOL nooperationattributenames	-	If parameter names
														should be added or not
					
	Usage :			Call to get a screen string representation
					of all the parameters.

   ============================================================*/
{

	CString output;
	int max = GetSize();

	for( int t = 0 ; t < max ; t++ )
	{
		CParameter* param = GetAt( t );
		output += param->ToString( nooperationattributenames );
		if( t < max - 1 )
			output += _T(", " );
	}

	return output;

}

CString CParameterContainer::GetString( int format ) const
/* ============================================================
	Function :		CParameterContainer::GetString
	Description :	Gets a string representation of the 
					"CParameter"s in this container, in the 
					format "format".
	Access :		Public

	Return :		CString		-	Resulting string
	Parameters :	int format	-	Format to get.
					
	Usage :			Call to, for example, export or save the 
					parameters to a file. "format" can be one of:
						"STRING_FORMAT_SAVE" For saving to file
						"STRING_FORMAT_CPP" cpp-file format
						"STRING_FORMAT_UML" UML-format
						"STRING_FORMAT_H" h-file format
						"STRING_FORMAT_HTML" HTML-format
						"STRING_FORMAT_H_CTOR" ctor in a header

   ============================================================*/
{

	CString output;
	int max = GetSize();
	if( max )
	{
		switch( format )
		{
			case STRING_FORMAT_SAVE:
				{
					for( int t = 0 ; t < max ; t++ )
					{
						CParameter* param = GetAt( t );
						output+= param->GetString();
						if( t < max - 1 )
							output += _T("@" );
					}
				}
				break;
			case STRING_FORMAT_CPP:
			case STRING_FORMAT_H:
			case STRING_FORMAT_H_CTOR:
			case STRING_FORMAT_UML:
				{
					for( int t = 0 ; t < max ; t++ )
					{
						CParameter* param = GetAt( t );
						output += param->GetString( format );
						if( t < max - 1 )
							output += _T(", " );
					}
				}
				break;
		}
	}

	return output;

}

void CParameterContainer::FromString( const CString& str )
/* ============================================================
	Function :		CParameterContainer::FromString
	Description :	Fills this container with parameters in 
					text format from "str".
	Access :		Public

	Return :		void
	Parameters :	const CString& str	-	Input string
					
	Usage :			Called when loading the container from file.

   ============================================================*/
{

	CTokenizer tok( str, _T( "@" ) );
	int max = tok.GetSize();
	for( int t = 0 ; t < max ; t++ )
	{

		CString paramval;
		tok.GetAt( t, paramval );
		CParameter* param = CParameter::FromString( paramval );
		if( param )
			Add( param );

	}
}

void CParameterContainer::Remove( CParameter* parameter )
/* ============================================================
	Function :		CParameterContainer::Remove
	Description :	Removes a specific "CParameter" from the 
					container.
	Access :		Public

	Return :		void
	Parameters :	CParameter* parameter	-	"CParameter" to remove
					
	Usage :			Finds and removes "parameter" from the 
					container, will also delete memory.

   ============================================================*/
{

	int max = GetSize();
	for( int t = 0 ; t < max ; t++ )
	{
		CParameter* param = GetAt( t );
		if( param == parameter )
		{
			RemoveAt( t );
			parameter = NULL;
			t = max;
		}
	}

}

BOOL CParameterContainer::operator==( const CParameterContainer& parameters )
/* ============================================================
	Function :		CParameterContainer::operator==
	Description :	Checks if two parameter containers are identical, 
					datawise.
	Access :		Public

	Return :		BOOL	-	"TRUE" if the two parameters have the same data.
	Parameters :	const CParameterContainer& parameters	-	Container to check against.
					
	Usage :			Call as an equality-test between two 
					parameter containers.

   ============================================================*/
{

	BOOL result = TRUE;
	if( GetSize() == parameters.GetSize() )
	{
		int max = GetSize();
		for( int t = 0 ; t < max ; t++ )
			if( *( GetAt( t ) ) != *( parameters.GetAt( t ) ) )
				result = FALSE;
	}
	else
		result = FALSE;

	return result;

}
