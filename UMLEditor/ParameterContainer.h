#ifndef _CPARAMETERCONTAINER_H_2B9B2375_842D_4DD1_92551EE6776
#define _CPARAMETERCONTAINER_H_2B9B2375_842D_4DD1_92551EE6776

///////////////////////////////////////////////////////////
// File :		ParameterContainer.h
// Created :	06/06/04
//
#include "Parameter.h"

class CParameterContainer
{
public:
	// Construction/destruction
	CParameterContainer();
	virtual ~CParameterContainer();

	BOOL operator==( const CParameterContainer& parameters );
	void Copy( CParameterContainer& parameters );

// Operations
	int GetSize( ) const;
	CParameter* GetAt( int index ) const;
	void RemoveAt( int index );
	void RemoveAll( );
	void Remove( CParameter* parameter );
	void Add( CParameter* parameter );

	CString GetString( int format = STRING_FORMAT_SAVE ) const;
	CString ToString( BOOL nooperationattributenames ) const;
	void	FromString( const CString& str );

// Attributes
private:
	CObArray m_parameters;


};

#endif //_CPARAMETERCONTAINER_H_2B9B2375_842D_4DD1_92551EE6776
