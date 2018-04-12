#ifndef _ATTRIBUTECONTAINER_H_D3C33FC_673D_434A_A76E563AFDB
#define _ATTRIBUTECONTAINER_H_D3C33FC_673D_434A_A76E563AFDB

///////////////////////////////////////////////////////////
// File :		AttributeContainer.h
// Created :	06/08/04
//

#include "Attribute.h"

class CAttributeContainer
{
public:
	// Construction/destruction
	CAttributeContainer();
	virtual ~CAttributeContainer();

// Operations
	void Copy( CAttributeContainer & attributes );
	int GetSize(  ) const;
	CAttribute* GetAt( int index ) const;
	void RemoveAt( int index );
	void RemoveAll(  );
	void Add( CAttribute * attribute );
	CString GetString( int format = STRING_FORMAT_SAVE ) const;
	void SetSize( int size );
	void SetAt( int index, CAttribute* obj );

// Attributes
private:
	CObArray m_attributes;


};
#endif //_ATTRIBUTECONTAINER_H_D3C33FC_673D_434A_A76E563AFDB
