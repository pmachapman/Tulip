#ifndef _CPROPERTY_H_16554D14_3C55_4CD4_BB287BFB5822
#define _CPROPERTY_H_16554D14_3C55_4CD4_BB287BFB5822

///////////////////////////////////////////////////////////
// File :		Property.h
// Created :	06/06/04
//

#include "StringHelpers.h"

class CProperty : public CObject
{
public:
	// Construction/destruction
	CProperty();
	virtual ~CProperty();
	CProperty( CProperty* property );
	CProperty( const CString& tag, const CString& val );
	CString GetString( int format = STRING_FORMAT_SAVE ) const;

	static CProperty* FromString( const CString& str );

// Attributes
	CString key;
	CString value;

};

#endif //_CPROPERTY_H_16554D14_3C55_4CD4_BB287BFB5822
