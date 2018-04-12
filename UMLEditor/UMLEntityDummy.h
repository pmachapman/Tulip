#ifndef _CUMLENTITYDUMMY_H_60335CF6_7501_4498_854828EEA6D9
#define _CUMLENTITYDUMMY_H_60335CF6_7501_4498_854828EEA6D9

///////////////////////////////////////////////////////////
// File :		UMLEntityDummy.h
// Created :	06/03/04
//
#include "UMLEntity.h"

class CUMLEntityDummy : public CUMLEntity
{
public:
	// Construction/destruction
	CUMLEntityDummy();
	virtual ~CUMLEntityDummy();

// Operations
	virtual int GetHitCode( CPoint point ) const;
	virtual int GetLinkCode( CPoint point ) const;
	virtual BOOL IsSelected( ) const;
	virtual void Select( BOOL select );
	virtual BOOL BodyInRect( CRect rect ) const;

};

#endif //_CUMLENTITYDUMMY_H_60335CF6_7501_4498_854828EEA6D9
