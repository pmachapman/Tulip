/* ==========================================================================
	Class :			CUMLMenu

	Author :		Johan Rosengren, Abstrakt Mekanik AB

	Date :			2004-04-29

	Purpose :		"CUMLMenu" is a "CDiagramMenu"-derived class, and 
					allows a specialized popup-menu (loaded from the 
					application resources) to be displayed when the editor 
					is right-clicked.

	Description :	Loads the popup-menu from resources instead of creating 
					it internally, which helps with internationalization and 
					allows adding of new menu selections.

	Usage :			Attach with a call to "SetPopupMenu( new CUMLMenu )"
					in the editor.

					The popup menu "IDR_UML_MENU_EDITOR_POPUP" must be copied to 
					the application resources.

   ========================================================================*/
#include "stdafx.h"
#include "../resource.h"

#include "UMLMenu.h"
#include "DiagramEditor/DiagramEditor.h"
#include "UMLEditor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CUMLMenu::CUMLMenu()
/* ============================================================
	Function :		CUMLMenu::CUMLMenu
	Description :	Constructor
	Access :		Public
					
	Return :		void
	Parameters :	none

	Usage :			

   ============================================================*/
{
}

CUMLMenu::~CUMLMenu()
/* ============================================================
	Function :		CUMLMenu::~CUMLMenu
	Description :	Destructor
	Access :		Public
					
	Return :		void
	Parameters :	none

	Usage :			

   ============================================================*/
{
}

CMenu* CUMLMenu::GetPopupMenu( CDiagramEditor* editor )
/* ============================================================
	Function :		CUMLMenu::GetPopupMenu
	Description :	Returns a popup-menu for the editor when it 
					is right-clicked.
	Access :		Public
					
	Return :		CMenu*					-	The popup-menu 
												to display
	Parameters :	CDiagramEditor* editor	-	The editor requesting 
												the menu. This pointer 
												can be used to enable/
												disable entries, for 
												example.
					
	Usage :			Called by the editor.

   ============================================================*/
{
	if( m_menu.m_hMenu == NULL )
		m_menu.LoadMenu( IDR_UML_MENU_EDITOR_POPUP );

	UINT cut = MF_GRAYED;
	UINT copy = MF_GRAYED;
	UINT paste = MF_GRAYED;
	UINT group = MF_GRAYED;

	// Copy/paste
	if( editor->IsAnyObjectSelected() )
	{
		cut = 0;
		copy = 0;
	}

	if( editor->GetSelectCount() > 1 )
		group = 0;

	if( editor->GetDiagramEntityContainer() && editor->GetDiagramEntityContainer()->ObjectsInPaste() )
		paste = 0;

	m_menu.EnableMenuItem( ID_EDIT_CUT, MF_BYCOMMAND | cut );
	m_menu.EnableMenuItem( ID_EDIT_COPY, MF_BYCOMMAND | copy );
	m_menu.EnableMenuItem( ID_EDIT_PASTE, MF_BYCOMMAND | paste );
	m_menu.EnableMenuItem( ID_EDIT_GROUP, MF_BYCOMMAND | group );
	m_menu.EnableMenuItem( ID_EDIT_UNGROUP, MF_BYCOMMAND | group );

	return m_menu.GetSubMenu( 0 );

}
