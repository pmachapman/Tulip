/* ==========================================================================
	Class :			CUMLUndoItem

	Author :		Johan Rosengren, Abstrakt Mekanik AB

	Date :			06/02/04

	Purpose :		"CUMLUndoItem", derived from "CUndoItem", is a single 
					application state for use with the "CDiagramEditor"-undo 
					mechanism.

	Description :	Two members are added, one for the background color and 
					one for the current package name.

	Usage :			Handled by the editor.

   ========================================================================*/

#include "stdafx.h"
#include "UMLUndoItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Construction/destruction
CUMLUndoItem::CUMLUndoItem()
/* ============================================================
	Function :		CUMLUndoItem::CUMLUndoItem
	Description :	Constructor
	Access :		Public

	Return :		void
	Parameters :	none

	Usage :			

   ============================================================*/
{

	col = RGB( 255, 255, 255 );

}

CUMLUndoItem::~CUMLUndoItem()
/* ============================================================
	Function :		CUMLUndoItem::~CUMLUndoItem
	Description :	Destructor
	Access :		Public

	Return :		void
	Parameters :	none

	Usage :			

   ============================================================*/
{
}

