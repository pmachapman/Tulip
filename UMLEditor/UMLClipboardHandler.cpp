/* ==========================================================================
	Class :			CUMLClipboardHandler

	Author :		Johan Rosengren, Abstrakt Mekanik AB

	Date :			2004-04-30

	Purpose :		"CUMLClipboardHandler" is a clipboard handler that 
					includes links in the clipboard.	

	Description :	The class containes, in addition to the default 
					implementation, code to fix the links for the object put 
					into the clipboard. As the links are connected using the 
					object names, and creating new objects creates new names, 
					the links of the copied objects will not longer match. 
					For this reason, the original names are stored in a 
					special property on the "CUMLEntity"-objects, and fixing 
					means looping the new objects, changing the links to 
					the "m_name" property by comparing the values to the 
					original id.

	Usage :			Declare an instance of "CUMLClipboardHandler" in 
					your document (for a SDI-app), dialog class (for a 
					"CDialog"-derived app) or centrally for a MDI-application. 
					Use "CUMLEntityContainer::SetClipboardHandler" to 
					attach it to application data. 

   ========================================================================
					4/8	 2004	Setting package when pasting. (ikolev)
   ========================================================================*/
#include "stdafx.h"
#include "UMLClipboardHandler.h"
#include "LinkFactory.h"
#include "UMLLineSegment.h"
#include "UMLEntityContainer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CUMLClipboardHandler::CUMLClipboardHandler()
/* ============================================================
	Function :		CUMLClipboardHandler::CUMLClipboardHandler
	Description :	Constructor
	Access :		Public
					
	Return :		void
	Parameters :	none

	Usage :			Declare an instance of "CUMLClipboardHandler" in 
					your document (for a SDI-app), dialog class (for a 
					"CDialog"-derived app) or centrally for a MDI-application. 
					Use "CUMLEntityContainer::SetClipboardHandler" to 
					attach it to application data. 

   ============================================================*/
{
}

CUMLClipboardHandler::~CUMLClipboardHandler()
/* ============================================================
	Function :		CUMLClipboardHandler::~CUMLClipboardHandler
	Description :	Destructor
	Access :		Public
					
	Return :		void
	Parameters :	none

	Usage :			Deletes associated memory as well.

   ============================================================*/
{

	ClearPaste();

}

void CUMLClipboardHandler::Copy( CDiagramEntity* obj )
/* ============================================================
	Function :		CUMLClipboardHandler::Copy
	Description :	Copies "obj" to the paste array
	Access :		Public
					
	Return :		void
	Parameters :	CDiagramEntity* obj	-	Object to copy.
					
	Usage :			Overridden to fix the link.

   ============================================================*/
{

	if( obj )
	{
		ClearPaste();
		CDiagramEntity* newobj = obj->Clone();
		newobj->Select( TRUE );
		newobj->MoveRect( 10, 10 );
		GetData()->Add( newobj );
		FixLinks();
	}

}

void CUMLClipboardHandler::CopyAllSelected( CDiagramEntityContainer* container )
/* ============================================================
	Function :		CUMLClipboardHandler::CopyAllSelected
	Description :	Copies all the selected items to the paste 
					array.
	Access :		Public
					
	Return :		void
	Parameters :	none

	Usage :			Overridden to update the copied links.

   ============================================================*/
{

	CDiagramClipboardHandler::CopyAllSelected( container );
	FixLinks();

}

void CUMLClipboardHandler::Paste( CDiagramEntityContainer* container )
/* ============================================================
	Function :		CUMLClipboardHandler::Paste
	Description :	Clones the contents of the paste array 
					into the container data array.
	Access :		Public

					
	Return :		void
	Parameters :	none

	Usage :			Call to paste the contents of the clipboard 
					to screen.

   ============================================================*/
{

	CUMLEntityContainer* objs = static_cast< CUMLEntityContainer* >( container );
	int count = GetData()->GetSize();
	CString package = objs->GetPackage();
	for( int t = 0 ; t < count ; t++ )
	{

		CUMLEntity* obj = static_cast< CUMLEntity* >( GetData()->GetAt( t ) );
		obj->SetPackage( package );

	}

	CDiagramClipboardHandler::Paste( container );
	objs->FixLinks( objs->GetData() );
	objs->DeleteDanglingLines();

}


void CUMLClipboardHandler::FixLinks()
/* ============================================================
	Function :		CUMLClipboardHandler::FixLinks
	Description :	Fixes the links of the clipboard data.
	Access :		Private

	Return :		void
	Parameters :	none

	Usage :			As objects are copied, they get assigned a 
					new ID. This means that the links are no 
					longer valid. As the copied objects save 
					the name of the objects they were copied 
					from, this function goes through the data, 
					matching the old link names with the links, 
					updating them to the new names.

   ============================================================*/
{

	int count = GetData()->GetSize();
	for( int t = 0 ; t < count ; t++ )
	{

		CUMLEntity* obj = static_cast< CUMLEntity* >( GetData()->GetAt( t ) );
		if( obj->GetOldId().GetLength() )
		{
			for( int i = 0 ; i < count ; i++ )
			{
				if( t != i )
				{
					CUMLLineSegment* line = dynamic_cast< CUMLLineSegment* >( GetData()->GetAt( i ) );
					if( line )
					{
						if( line->GetLink( LINK_START ) == obj->GetOldId() )
							line->SetLink( LINK_START, obj->GetName() );
						if( line->GetLink( LINK_END ) == obj->GetOldId() )
							line->SetLink( LINK_END, obj->GetName() );
					}
				}
			}

			obj->SetOldId( _T( "" ) );
		}
	}

}
