#ifndef _COPERATION_H_BC44C7A7_DC64_452A_877F13EC3D7E
#define _COPERATION_H_BC44C7A7_DC64_452A_877F13EC3D7E

///////////////////////////////////////////////////////////
// Operation.h
// Created: 06/02/04
//

#include "StringHelpers.h"
#include "PropertyContainer.h"
#include "ParameterContainer.h"

class COperation : public CObject
{
public:
	// Construction/destruction
	COperation();
	virtual ~COperation();

	BOOL operator==( const COperation& operation );

// Operations
	CString ToString( BOOL nomarkers, BOOL nooperationattributenames );
	COperation* Clone( );
	static COperation* FromString( const CString& str );
	CString GetString( int format = STRING_FORMAT_SAVE ) const;

// Attributes
	int maintype;					// Static or virtual
	int access;						// public, private, protected
	CString name;					// variable name
	CString type;					// variable type
	CString stereotype;				// stereotype, if any.

	BOOL	setter;					// TRUE if a setter
	BOOL	getter;					// TRUE if a getter
	CString getsetvariable;			// getter/setter variable

	CParameterContainer	parameters;	// Parameters
	CPropertyContainer	properties;	// Properties

};

#endif //_COPERATION_H_BC44C7A7_DC64_452A_877F13EC3D7E
