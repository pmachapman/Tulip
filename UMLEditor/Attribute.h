#ifndef _CATTRIBUTE_H_7261F2E3_1899_4934_B253082664F
#define _CATTRIBUTE_H_7261F2E3_1899_4934_B253082664F

///////////////////////////////////////////////////////////
// Attribute.h
// Created: 06/02/04
//

#include "StringHelpers.h"
#include "PropertyContainer.h"

class CAttribute : public CObject
{
public:
	// Construction/destruction
	CAttribute();
	virtual ~CAttribute();

// Operations
	CString ToString( BOOL nomarkers );
	CAttribute* Clone( );
	static CAttribute* FromString( const CString& str );
	CString GetString( int format = STRING_FORMAT_SAVE ) const;


// Attributes
	int maintype;
	int access;
	CString name;
	CString type;
	CString multiplicity;
	CString defaultvalue;
	CString stereotype;

	CPropertyContainer	properties;

};

#endif //_CATTRIBUTE_H_7261F2E3_1899_4934_B253082664F
