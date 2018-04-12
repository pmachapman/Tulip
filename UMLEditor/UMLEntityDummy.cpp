/* ==========================================================================
	Class :			CUMLEntityDummy

	Author :		Johan Rosengren, Abstrakt Mekanik AB

	Date :			06/03/04

	Purpose :		"CUMLEntityDummy" is derived from "CUMLEntity".

	Description :	"CUMLEntityDummy" is a dummy object that is returned 
					from the editor container instead of objects in other 
					packages. 

	Usage :			Managed internally.

   ========================================================================*/

#include "stdafx.h"
#include "UMLEntityDummy.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Construction/destruction
CUMLEntityDummy::CUMLEntityDummy()
/* ============================================================
	Function :		CUMLEntityDummy::CUMLEntityDummy
	Description :	Constructor
	Access :		Public

	Return :		void
	Parameters :	none

	Usage :			Normally, don't create instances of this 
					class - it's for use in the container as 
					a dummy for objects located in other 
					packages than the current.

   ============================================================*/
{

	SetRect( 0, 0, 0, 0 );
	SetType( _T( "uml_dummy" ) );

}

CUMLEntityDummy::~CUMLEntityDummy()
/* ============================================================
	Function :		CUMLEntityDummy::~CUMLEntityDummy
	Description :	Destructor
	Access :		Public

	Return :		void
	Parameters :	none

	Usage :			

   ============================================================*/
{
}

// Implementation
int CUMLEntityDummy::GetHitCode( CPoint /*point*/ ) const
/* ============================================================
	Function :		CUMLEntityDummy::GetHitCode
	Description :	Returns the hit-code of the object at the 
					position "point".
	Access :		Public
					
	Return :		int				-	Hit code, always "DEHT_NONE"
	Parameters :	CPoint point	-	Position to test.
					
	Usage :			Dummy objects can't be interacted with.

   ============================================================*/
{

	return DEHT_NONE;

}

int CUMLEntityDummy::GetLinkCode( CPoint /*point*/ ) const
/* ============================================================
	Function :		CUMLEntityDummy::GetLinkCode
	Description :	Gets the link code at position "point".
	Access :		Public
					
	Return :		int				-	Link code, always "LINK_NONE"
	Parameters :	CPoint point	-	Position to test
					
	Usage :			Dummy objects can't be interacted with.

   ============================================================*/
{

	return LINK_NONE;

}

BOOL CUMLEntityDummy::IsSelected( ) const
/* ============================================================
	Function :		CUMLEntityDummy::IsSelected
	Description :	Checks if the object is selected.
	Access :		Public
					
	Return :		BOOL	-	Always "FALSE"
	Parameters :	none

	Usage :			Dummy objects can't be interacted with.

   ============================================================*/
{

	return FALSE;

}

void CUMLEntityDummy::Select( BOOL /*select*/ )
/* ============================================================
	Function :		CUMLEntityDummy::Select
	Description :	Sets the select-flag
	Access :		Public
					
	Return :		void
	Parameters :	BOOL select	-	Not used
					
	Usage :			Dummy objects can't be interacted with.

   ============================================================*/
{
	// Do nothing
}

BOOL CUMLEntityDummy::BodyInRect( CRect /*rect*/ ) const
/* ============================================================
	Function :		CUMLEntityDummy::BodyInRect
	Description :	Checks if any part of the body of the 
					object is inside "rect".
	Access :		Public
					
	Return :		BOOL		-	Always "FALSE".
	Parameters :	CRect rect	-	Rectangle to test.
					
	Usage :			Dummy objects can't be interacted with.

   ============================================================*/
{

	return FALSE;

}



