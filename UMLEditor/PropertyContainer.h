#ifndef _CPROPERTYCONTAINER_H_6B8221AA_8977_4557_AF6D36C93978
#define _CPROPERTYCONTAINER_H_6B8221AA_8977_4557_AF6D36C93978

///////////////////////////////////////////////////////////
// File :		PropertyContainer.h
// Created :	06/06/04
//

#include "Property.h"

class CPropertyContainer 
{
public:
	// Construction/destruction
	CPropertyContainer();
	virtual ~CPropertyContainer();

	void Copy( CPropertyContainer& properties );

// Operations
	int			GetSize( ) const;
	CProperty*	GetAt( int index ) const;
	void		RemoveAt( int index );
	void		RemoveAll( );
	void		Add( CProperty* property );
	void		Add( const CString& tag, const CString& val = _T( "" ) );
	CString		GetString( int format = STRING_FORMAT_SAVE ) const;
	void		FromString( const CString& str );

	CString		GetPropertyValue( const CString& tag ) const;
	void		SetPropertyValue( const CString& tag, const CString& value );
	void		RemoveProperty( const CString& tag );

// Attributes
private:
	CObArray m_properties;

	// Private helpers
	CProperty* FindProperty( const CString& tag );

};

#endif //_CPROPERTYCONTAINER_H_6B8221AA_8977_4557_AF6D36C93978
