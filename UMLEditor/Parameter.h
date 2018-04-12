#ifndef _CPARAMETER_H_91173656_87E4_4EAC_83E628967C9A
#define _CPARAMETER_H_91173656_87E4_4EAC_83E628967C9A

///////////////////////////////////////////////////////////
// File :		Parameter.h
// Created :	06/06/04
//

#include "StringHelpers.h"

class CParameter : public CObject
{
public:
	// Construction/destruction
	CParameter();
	CParameter( CParameter* parameter );
	virtual ~CParameter();

	BOOL operator==( const CParameter& parameter );
	BOOL operator!=( const CParameter& parameter );

// Operations
	static	CParameter* FromString( const CString& str );
	CString	GetString( int format = STRING_FORMAT_SAVE ) const;
	CString	ToString( BOOL nooperationattributenames ) const;

// Attributes
	CString name;
	CString defaultvalue;
	CString type;
	BOOL	reference;
	BOOL	in;
	BOOL	out;

};

#endif //_CPARAMETER_H_91173656_87E4_4EAC_83E628967C9A
