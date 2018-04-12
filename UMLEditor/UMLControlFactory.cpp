/* ==========================================================================
	Class :			CUMLControlFactory

	Author :		Johan Rosengren, Abstrakt Mekanik AB

	Date :			2004-04-22

	Purpose :		"CUMLControlFactory" is a factory-class with one 
					single member, allowing the creation of "CDiagramEntity"-
					derived objects from a string.	

	Description :	"CreateFromString" tries to instantiate one of the control 
					objects by testing the control object static function 
					"CreateFromString" until it hits a valid one. The 
					resulting pointer is then returned and can be added to 
					the application data.

	Usage :			Call "CUMLControlFactory::CreateFromString" and if 
					a non-"NULL" pointer is returned, add it to the 
					editor data.

   ========================================================================
	Changes :		12/7 2004	Added creation of CUMLEntityClassTemplate 
								instances.
   ========================================================================*/
#include "stdafx.h"
#include "UMLControlFactory.h"

#include "UMLEntityClass.h"
#include "UMLEntityNote.h"
#include "UMLEntityLabel.h"
#include "UMLEntityPackage.h"
#include "UMLEntityInterface.h"
#include "UMLEntityClassTemplate.h"
#include "UMLLineSegment.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUMLControlFactory

CDiagramEntity* CUMLControlFactory::CreateFromString( const CString& str )
/* ============================================================
	Function :		CUMLControlFactory::CreateFromString
	Description :	The function returns an object from the 
					parameter "str".
	Access :		Public
					
	Return :		CDiagramEntity*		-	The new object, or 
											"NULL" is "str" is not a 
											valid representation.
	Parameters :	const CString& str	-	The string to create 
											a new object from
					
	Usage :			Call this static function while reading 
					string representations of objects from a 
					text file. Note that the caller is 
					responsible for the allocated memory.

   ============================================================*/
{
	CDiagramEntity* obj;

	obj = CUMLEntityClass::CreateFromString( str );
	if( !obj )
		obj = CUMLLineSegment::CreateFromString( str );
	if( !obj )
		obj = CUMLEntityNote::CreateFromString( str );
	if( !obj )
		obj = CUMLEntityPackage::CreateFromString( str );
	if( !obj )
		obj = CUMLEntityLabel::CreateFromString( str );
	if( !obj )
		obj = CUMLEntityInterface::CreateFromString( str );
	if( !obj )
		obj = CUMLEntityClassTemplate::CreateFromString( str );

	return obj;
}