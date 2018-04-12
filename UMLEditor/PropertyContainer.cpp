/* ==========================================================================
	Class :			CPropertyContainer

	Author :		Johan Rosengren, Abstrakt Mekanik AB

	Date :			06/06/04

	Purpose :		"CPropertyContainer" is a container for property objects.

	Description :	The class contains a "CObArray" holding "CProperty" 
					instances, as well as access functions for them.

	Usage :			The class manages memory.

   ========================================================================
	Changes :		8/7 2004	Added SetPropertyValue
   ========================================================================*/

#include "stdafx.h"
#include "PropertyContainer.h"
#include "DiagramEditor/Tokenizer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Construction/destruction
CPropertyContainer::CPropertyContainer()
/* ============================================================
	Function :		CPropertyContainer::CPropertyContainer
	Description :	Constructor
	Access :		Public

	Return :		void
	Parameters :	none

	Usage :			

   ============================================================*/
{
}

CPropertyContainer::~CPropertyContainer()
/* ============================================================
	Function :		CPropertyContainer::~CPropertyContainer
	Description :	Destructor
	Access :		Public

	Return :		void
	Parameters :	none

	Usage :			Deletes assocuated memory as well.

   ============================================================*/
{

	RemoveAll();

}

void CPropertyContainer::Copy( CPropertyContainer& properties )
/* ============================================================
	Function :		CPropertyContainer::Copy
	Description :	Copies the contents from "properties" to 
					this container.
	Access :		Public

	Return :		void
	Parameters :	CPropertyContainer& properties	-	Container to copy from.
					
	Usage :			Call to copy the properties from another 
					container into thsi one.

   ============================================================*/
{

	RemoveAll();

	int max = properties.GetSize();
	for( int t = 0 ; t < max ; t++ )
		Add( new CProperty( properties.GetAt( t ) ) );

}

// Implementation
int CPropertyContainer::GetSize( ) const
/* ============================================================
	Function :		CPropertyContainer::GetSize
	Description :	Gets the number of properties in the container.
	Access :		Public
					
	Return :		int	-	Number of properties.
	Parameters :	none

	Usage :			Call to get the number of properties in the 
					container.

   ============================================================*/
{

	return m_properties.GetSize();

}

CProperty* CPropertyContainer::GetAt( int index ) const
/* ============================================================
	Function :		CPropertyContainer::GetAt
	Description :	Gets the object at "index".
	Access :		Public
					
	Return :		CProperty*	-	The object, or "NULL" if out of bounds.
	Parameters :	int index	-	Index to get object from.
					
	Usage :			Call to get a specific property from the 
					container.

   ============================================================*/
{

	CProperty* result = NULL;

	if( index < GetSize() )
		result = static_cast< CProperty* >( m_properties[ index ] );

	return result;

}

void CPropertyContainer::RemoveAt( int index )
/* ============================================================
	Function :		CPropertyContainer::RemoveAt
	Description :	Removes the property at "index"
	Access :		Public
					
	Return :		void
	Parameters :	int index	-	Index to remove property from.
					
	Usage :			Call to remove a property at a specific 
					index. Memory will also be deleted.

   ============================================================*/
{

	if( index > -1 && index < GetSize() )
	{

		delete GetAt( index );
		m_properties.RemoveAt( index );

	}

}

void CPropertyContainer::RemoveAll( )
/* ============================================================
	Function :		CPropertyContainer::RemoveAll
	Description :	Removes all objects from the container.
	Access :		Public
					
	Return :		void
	Parameters :	none

	Usage :			Call to clear the container. Memory will 
					also be deleted.

   ============================================================*/
{

	while( GetSize() )
		RemoveAt( 0 );

}

void CPropertyContainer::Add( CProperty* property )
/* ============================================================
	Function :		CPropertyContainer::Add
	Description :	Adds "property" to the container.
	Access :		Public
					
	Return :		void
	Parameters :	CProperty* property	-	Property to add.
					
	Usage :			Call to add a property to a container. The 
					container takes ownership of the object.

   ============================================================*/
{

	CProperty* oldproperty = FindProperty( property->key );
	if( oldproperty )
	{
		if( property->value != oldproperty->value )
			oldproperty->value = property->value;
		delete property;
	}
	else
		m_properties.Add( property );

}

void CPropertyContainer::Add( const CString& tag, const CString& val )
/* ============================================================
	Function :		CPropertyContainer::Add
	Description :	Create and add a property to the container.
	Access :		Public
					
	Return :		void
	Parameters :	const CString& tag	-	Tag of the property
					const CString& val	-	Value of the tag
					
	Usage :			Call to a add a property to the container.

   ============================================================*/
{

	CProperty* property = new CProperty( tag, val );
	Add( property );

}

CString CPropertyContainer::GetString( int format ) const
/* ============================================================
	Function :		CPropertyContainer::GetString
	Description :	Gets a string representation of this 
					object in the format "format".
	Access :		Public

	Return :		CString		-	Result
	Parameters :	int format	-	Format

	Usage :			"format" can be one of:
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
						CProperty* property = GetAt( t );
						output += property->GetString( format );
						if( t < max - 1 )
							output += _T( "#" );
					}
				}
				break;
			case STRING_FORMAT_CPP:
				{
					for( int t = 0 ; t < max ; t++ )
					{
						CProperty* property = GetAt( t );
						output += property->GetString( format );
						if( t < max - 1 )
							output += _T( ", " );
					}
				}
				break;
			case STRING_FORMAT_UML:
				{
					output = _T( "{ " );
					for( int t = 0 ; t < max ; t++ )
					{
						CProperty* property = GetAt( t );
						output += property->GetString( format );
						if( t < max - 1 )
							output += _T( ", " );
					}
					output += _T( " }" );
				}
				break;
			case STRING_FORMAT_HTML:
				{
					output = _T( "{ " );
					for( int t = 0 ; t < max ; t++ )
					{
						CProperty* property = GetAt( t );
						output += property->GetString( STRING_FORMAT_UML );
						if( t < max - 1 )
							output += _T( "<br>" );
					}
					output += _T( " }" );
				}
				break;
		}
	}

	return output;

}

void CPropertyContainer::FromString( const CString& str )
/* ============================================================
	Function :		CPropertyContainer::FromString
	Description :	Creates properties from a string 
					representation and adds them to the 
					container.
	Access :		Public

	Return :		void
	Parameters :	const CString& str	-	New operation, or "NULL" if "str"
											is not a valid representation of
											properties
					
	Usage :			Call from load-functions

   ============================================================*/
{

	CTokenizer tok( str, _T( "#" ) );
	int max = tok.GetSize();
	for( int t = 0 ; t < max ; t++ )
	{

		CString propval;
		tok.GetAt( t, propval );
		CProperty* prop = CProperty::FromString( propval );
		if( prop )
			Add( prop );

	}

}

CString	CPropertyContainer::GetPropertyValue( const CString& tag ) const
/* ============================================================
	Function :		CString	CPropertyContainer::GetPropertyValue
	Description :	Gets the value for the property "tag"
	Access :		Public

	Return :		CString				-	Result
	Parameters :	const CString& tag	-	Tag to get the value for.
					
	Usage :			Returns the value of the tag, or "true" if 
					the tag has no value. Empty if the tag doesn't
					exist.

   ============================================================*/
{

	CString result;
	int max = GetSize();
	for( int t = 0 ; t < max ; t++ )
	{
		CProperty* property = GetAt( t );
		if( property->key == tag )
		{
			result = property->value;
			if( result.IsEmpty() )
				result = _T( "true" );
		}
	}

	return result;

}

CProperty* CPropertyContainer::FindProperty( const CString& tag )
/* ============================================================
	Function :		CPropertyContainer::FindProperty
	Description :	Returns a pointer to the property having 
					"tag".
	Access :		Public

	Return :		CProperty*			-	Resulting property, or 
											"NULL" if it doesn't exist.
	Parameters :	const CString& tag	-	Tag to find.
					
	Usage :			Call to get a pointer to the property with 
					the tag "tag", if it exists.

   ============================================================*/
{

	CProperty* result = NULL;

	int max = GetSize();
	for( int t = 0 ; t < max ; t++ )
	{
		CProperty* property = GetAt( t );
		if( property->key == tag )
			result = property;
	}

	return result;

}

void CPropertyContainer::RemoveProperty( const CString& tag )
/* ============================================================
	Function :		CPropertyContainer::RemoveProperty
	Description :	Removes the property with the tag "tag".
	Access :		Public

	Return :		void
	Parameters :	const CString& tag	-	Tag of property to 
											remove.
					
	Usage :			Call to remove a specific tag.

   ============================================================*/
{

	int max = GetSize();
	for( int t = 0 ; t < max ; t++ )
	{
		CProperty* property = GetAt( t );
		if( property->key == tag )
		{
			RemoveAt( t );
			t = max;
		}
	}

}

void CPropertyContainer::SetPropertyValue( const CString& tag, const CString& value )
/* ============================================================
	Function :		CPropertyContainer::SetPropertyValue
	Description :	Sets the value of "tag" to "value"
	Access :		

	Return :		void
	Parameters :	const CString& tag		-	Tag of property to set.
					const CString& value	-	New value
					
	Usage :			Call to set the value of the property with 
					the tag "tag". If the property does not 
					exist, it will be created.

   ============================================================*/
{

	CProperty* property = FindProperty( tag );
	if( property == NULL )
	{
		property = new CProperty( tag, value );
		Add( property );
	}
	else
	{
		property->value = value;
	}

}
