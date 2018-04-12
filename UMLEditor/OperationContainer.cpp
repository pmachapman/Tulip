/* ==========================================================================
	Class :			COperationContainer

	Author :		Johan Rosengren, Abstrakt Mekanik AB

	Date :			06/08/04

	Purpose :		A container for "COperation"s

	Description :	The class has a "CObArray" to store the "COperation"s, and 
					manages memory automatically. 

	Usage :			Use to handle arrays of operations. As memory will be 
					freed automatically, use "Copy" to copy contents.

   ========================================================================*/

#include "stdafx.h"
#include "OperationContainer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////////////////////////////////////////////////////////////
// Construction/destruction
//

COperationContainer::COperationContainer()
/* ============================================================
	Function :		COperationContainer::COperationContainer
	Description :	Constructor
	Access :		Public

	Return :		void
	Parameters :	none

	Usage :			

   ============================================================*/
{
}

COperationContainer::~COperationContainer()
/* ============================================================
	Function :		COperationContainer::~COperationContainer
	Description :	Destructor
	Access :		Public
					
	Return :		void
	Parameters :	none

	Usage :			Deletes all associated memory.

   ============================================================*/
{

	RemoveAll();

}

////////////////////////////////////////////////////////////////////
// Public functions
//
void COperationContainer::Copy( COperationContainer & operations )
/* ============================================================
	Function :		COperationContainer::Copy
	Description :	Copy the contents of operations to this 
					container.
	Access :		Public
					
	Return :		void
	Parameters :	COperationContainer & operations	-	Container to 
															copy from

	Usage :			Call to copy data from one container to 
					another.

   ============================================================*/
{

	RemoveAll();
	int max = operations.GetSize();
	m_operations.SetSize( max );
	for( int t = 0 ; t < max ; t++ )
		m_operations.SetAt( t, ( operations.GetAt( t ) )->Clone() );

}

int COperationContainer::GetSize( ) const
/* ============================================================
	Function :		COperationContainer::GetSize
	Description :	Gets the number of operations in the 
					container.
	Access :		Public
					
	Return :		int	-	The number of operations
	Parameters :	none

	Usage :			Call to get the size of data.

   ============================================================*/
{

	return m_operations.GetSize();

}

COperation* COperationContainer::GetAt( int index) const
/* ============================================================
	Function :		COperationContainer::GetAt
	Description :	Get the operation at "index"
	Access :		Public
					
	Return :		COperation*	-	Operation, or "NULL" if out 
									of bounds.
	Parameters :	int index	-	Index of object to get.

	Usage :			Call to get a specific operation.

   ============================================================*/
{

	COperation* result = NULL;

	if( index > -1 && index < GetSize() )
		result = static_cast< COperation* >( m_operations.GetAt( index ) );

	return result;

}

void COperationContainer::RemoveAt( int index)
/* ============================================================
	Function :		COperationContainer::RemoveAt
	Description :	Remove operation at "index"
	Access :		Public
					
	Return :		void
	Parameters :	int index	-	Index to remove operation at.

	Usage :			Will automatically delete data.

   ============================================================*/
{

	if( index > -1 && index < GetSize() )
	{
		delete GetAt( index );
		m_operations.RemoveAt( index );
	}

}

void COperationContainer::RemoveAll( )
/* ============================================================
	Function :		COperationContainer::RemoveAll
	Description :	Remove all objects from container.
	Access :		Public

	Return :		void
	Parameters :	none

	Usage :			Frees memory as well.

   ============================================================*/
{

	while( GetSize() )
		RemoveAt( 0 );

}

void COperationContainer::Add( COperation * operation )
/* ============================================================
	Function :		COperationContainer::Add
	Description :	Add an operation to the container.
	Access :		Public

	Return :		void
	Parameters :	COperation * operation	-	Operation to add

	Usage :			Call to add an operation to the container. 
					The container owns the object.

   ============================================================*/
{

	m_operations.Add( operation );

}

CString COperationContainer::GetString( int format ) const
/* ============================================================
	Function :		COperationContainer::GetString
	Description :	Get a string representation of all 
					operations in format "format".
	Access :		Public
					
	Return :		CString		-	Resulting string
	Parameters :	int format	-	Format to use.

	Usage :			"format" can be one of:
						"STRING_FORMAT_SAVE" For saving to file
						"STRING_FORMAT_CPP" cpp-file format
						"STRING_FORMAT_UML" UML-format
						"STRING_FORMAT_H" h-file format
						"STRING_FORMAT_HTML" HTML-format
						"STRING_FORMAT_H_CTOR" ctor in a header

   ============================================================*/
{

	CString result;
	if( format == STRING_FORMAT_SAVE )
		for( int t = 0 ; t < GetSize() ; t++ )
			result += _T( "," ) + GetAt( t )->GetString();

	return result;

}

void COperationContainer::SetSize( int size )
/* ============================================================
	Function :		COperationContainer::SetSize
	Description :	Set the size of the container
	Access :		Public

	Return :		void
	Parameters :	int size	-	
					
	Usage :			Can be used to speed up container operations 
					if the size is already known.

   ============================================================*/
{

	m_operations.SetSize( size );

}

void COperationContainer::SetAt( int index, COperation* obj )
/* ============================================================
	Function :		COperationContainer::SetAt
	Description :	Sets "obj" at index "index" in the container.
	Access :		Public

	Return :		void
	Parameters :	int index		-	Index where obj should 
										be added
					CAttribute* obj	-	Object to add.
					
	Usage :			Call to directly set an object. Note that the 
					container should be empty, with just "SetSize" 
					called. As the function is included for 
					speedier operations, as little error 
					checking as possible is made. If an object 
					is set into a slot with an exisiting object, 
					a memory leak will result.

   ============================================================*/
{

	if( index > -1 && index < GetSize() )
		m_operations.SetAt( index, obj );

}
