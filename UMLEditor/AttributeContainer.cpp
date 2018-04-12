/* ==========================================================================
	Class :			CAttributeContainer

	Author :		Johan Rosengren, Abstrakt Mekanik AB

	Date :			06/08/04

	Purpose :		Holds an array of attributes for an instance of "CUMLEntityClass".

	Description :	The class has a "CObArray" containing the attribute, as 
					well as access functions and automatic memory management.

	Usage :			Used internally by "CUMLEntityClass".

   ========================================================================*/

#include "stdafx.h"
#include "AttributeContainer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////////////////////////////////////////////////////////////
// Construction/destruction
//
CAttributeContainer::CAttributeContainer()
/* ============================================================
	Function :		CAttributeContainer::CAttributeContainer
	Description :	Constructor
	Access :		Public
					
	Return :		void
	Parameters :	none

	Usage :			

   ============================================================*/
{
}

CAttributeContainer::~CAttributeContainer()
/* ============================================================
	Function :		CAttributeContainer::~CAttributeContainer
	Description :	Destructor
	Access :		Public
					
	Return :		void
	Parameters :	none

	Usage :			Will delete associated memory.

   ============================================================*/
{

	RemoveAll();

}

void CAttributeContainer::Copy( CAttributeContainer & attributes )
/* ============================================================
	Function :		CAttributeContainer::Copy
	Description :	Copies all elements from attributes to this
					instance.
	Access :		Public
					
	Return :		void
	Parameters :	CAttributeContainer & attributes -	Container to 
														copy from.

	Usage :			Call - for example - when copying one 
					"CUMLEntityClass" instance to another.

   ============================================================*/
{

	RemoveAll();
	int max = attributes.GetSize();
	m_attributes.SetSize( max );
	for( int t = 0 ; t < max ; t++ )
		m_attributes.SetAt( t, ( attributes.GetAt( t ) )->Clone() );

}

int CAttributeContainer::GetSize( ) const
/* ============================================================
	Function :		CAttributeContainer::GetSize
	Description :	Returns the number of attributes in the 
					container.
	Access :		Public
					
	Return :		int	-	The number of attributes.
	Parameters :	none

	Usage :			Call to get the number of attributes

   ============================================================*/
{

	return m_attributes.GetSize();

}

CAttribute* CAttributeContainer::GetAt( int index ) const
/* ============================================================
	Function :		CAttributeContainer::GetAt
	Description :	Get the attribute at "index".
	Access :		Public

	Return :		CAttribute*	-	The attribute, or "NULL" if 
									out of bounds.
	Parameters :	int index	-	The index of the attribute 
									to get

	Usage :			Call to get a specific attribute.

   ============================================================*/
{

	CAttribute* result = NULL;

	if( index > -1 && index < GetSize() )
		result = static_cast< CAttribute* >( m_attributes.GetAt( index ) );

	return result;

}

void CAttributeContainer::RemoveAt( int index )
/* ============================================================
	Function :		CAttributeContainer::RemoveAt
	Description :	Remove attribute at "index".
	Access :		Public

	Return :		void
	Parameters :	int index	-	The index of the attribute 
									to remove

	Usage :			Deletes memory as well.

   ============================================================*/
{

	if( index > -1 && index < GetSize() )
	{
		delete GetAt( index );
		m_attributes.RemoveAt( index );
	}

}

void CAttributeContainer::RemoveAll( )
/* ============================================================
	Function :		CAttributeContainer::RemoveAll
	Description :	Removes all attributes in the container.
	Access :		Public

	Return :		void
	Parameters :	none

	Usage :			Deletes all memory as well.

   ============================================================*/
{

	while( GetSize() )
		RemoveAt( 0 );

}

void CAttributeContainer::Add( CAttribute * attribute )
/* ============================================================
	Function :		CAttributeContainer::Add
	Description :	Adds an attribute to the container.
	Access :		Public
 
	Return :		void
	Parameters :	CAttribute * attribute	-	Attribute to add

	Usage :			The container now owns the object.

   ============================================================*/
{

	m_attributes.Add( attribute );

}

CString CAttributeContainer::GetString( int format ) const
/* ============================================================
	Function :		CAttributeContainer::GetString
	Description :	Gets a string representation of all 
					attributes in format "format".
	Access :		Public
					
	Return :		CString		-	Resulting string
	Parameters :	int format	-	Format to use.

	Usage :			"format" can be one of:
						"STRING_FORMAT_SAVE" For saving to file
						"STRING_FORMAT_CPP" cpp-file format
						"STRING_FORMAT_UML" UML-format
						"STRING_FORMAT_H" h-file format
						"STRING_FORMAT_HTML" HTML-format
						"STRING_FORMAT_H_CTOR" "ctor" in a header

   ============================================================*/
{

	CString result;
	if( format == STRING_FORMAT_SAVE )
		for( int t = 0 ; t < GetSize() ; t++ )
			result += _T( "," ) + GetAt( t )->GetString();

	return result;

}

void CAttributeContainer::SetSize( int size )
/* ============================================================
	Function :		CAttributeContainer::SetSize
	Description :	Sets the size of the container
	Access :		Public
					
	Return :		void
	Parameters :	int size	-	Size to set
					
	Usage :			Can be used to speed up container operations 
					if the size is already known.

   ============================================================*/
{

	m_attributes.SetSize( size );

}

void CAttributeContainer::SetAt( int index, CAttribute* obj )
/* ============================================================
	Function :		CAttributeContainer::SetAt
	Description :	Sets "obj" at index "index" in the container.
	Access :		Public
					
	Return :		void
	Parameters :	int index		-	Index where "obj" should 
										be added
					CAttribute* obj	-	Object to add.
					
	Usage :			Call to directly set an object. Note that the 
					container should be empty, with just "SetSize" 
					called. As the function is included for 
					speedier operations, as little error 
					checking as possible is done. If an object 
					is set into a slot with an exisiting object, 
					a memory leak will result.

   ============================================================*/
{

	if( index > -1 && index < GetSize() )
		m_attributes.SetAt( index, obj );

}

