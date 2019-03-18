/* ==========================================================================
	Class :			CDiagramMenu

	Author :		Johan Rosengren, Abstrakt Mekanik AB

	Date :			2004-03-30

	Purpose :		"CDiagramMenu" is a simple class to allow easy
					customization of the popup menu of the "CDiagramEditor"
					without deriving a new class.

	Description :	Contains a "CMenu" that is exposed through "GetPopupMenu".

	Usage :			Derive a class from "CDiagramMenu", and implement
					"GetPopupMenu". This function is expected to return a
					pointer to a "CMenu". The menu alternatives on the menu
					should have menu ids between "CMD_START" and "CMD_END"
					inclusively (some are predefined). The constants are
					defined in DiagramEntity.h. The "CDiagramMenu" should
					be added to the "CDiagramEditor" class with a call to
					"SetPopupMenu". Note that the editor then owns the menu
					and will delete it as soon as a new menu is set and
					in the "dtor", so either heap allocate it or allocate and
					add it so:

						"m_editor.SetPopupMenu( new CMyDiagramMenuDerivedClass );"

					Whenever a menu alternative is selected from the popup
					menu, the command is sent to all selected objects. The
					objects will then have to handle the messages they are
					interested in in their "DoCommand" functions.

					The class is a friend class to "CDiagramEditor", so all
					members are accessible.
   ========================================================================
					26/6 2004	Added group handling (Unruled Boy).
   ========================================================================
					18/3 2019	Removed group handling, added undo/redo, etc
   ========================================================================*/
#include "stdafx.h"
#include "DiagramMenu.h"
#include "DiagramEditor.h"

CDiagramMenu::CDiagramMenu()
/* ============================================================
	Function :		CDiagramMenu::CDiagramMenu
	Description :	Constructor
	Access :		Public

	Return :		void
	Parameters :	none

	Usage :			Always heap allocate

   ============================================================*/
{
	selectedMenu = 0;
}

CDiagramMenu::~CDiagramMenu()
/* ============================================================
	Function :		CDiagramMenu::~CDiagramMenu
	Description :	Destructor
	Access :		Public

	Return :		void
	Parameters :	none

	Usage :			Note that the editor will delete attached
					instances automatically

   ============================================================*/
{
	if (m_menu.m_hMenu != NULL)
		m_menu.DestroyMenu();
}

CMenu* CDiagramMenu::GetPopupMenu(CDiagramEditor* editor)
/* ============================================================
	Function :		CDiagramMenu::GetPopupMenu
	Description :	Gets a menu pointer to the desired popup
					menu.
	Access :		Public

	Return :		CMenu*					-	A pointer to
												the popup menu
	Parameters :	CDiagramEditor* editor	-	The editor
												calling for a
												menu.

	Usage :			Call to get the popup menu for the editor.

   ============================================================*/
{
	// Set the default menu
	int resourceId = IDR_MENU_EDITOR_POPUP;

	// See if an object is selected. If it is, get its menu
	if (editor->GetSelectCount() == 1)
	{
		resourceId = editor->GetSelectedObject()->GetMenuResourceId();
	}

	// Free then old menu if it is changing
	if (m_menu.m_hMenu != NULL && selectedMenu != resourceId)
	{
		m_menu.DestroyMenu();
		m_menu.m_hMenu = NULL;
	}

	// Load the menu if it is changing (or first run)
	if (selectedMenu != resourceId)
	{
		m_menu.LoadMenu(resourceId);
		selectedMenu = resourceId;
	}

	UINT undo = MF_GRAYED;
	UINT redo = MF_GRAYED;
	UINT cut = MF_GRAYED;
	UINT copy = MF_GRAYED;
	UINT paste = MF_GRAYED;
	UINT deleteSelected = MF_GRAYED;
	UINT selectall = MF_GRAYED;
	if (editor->IsAnyObjectSelected())
	{
		cut = 0;
		copy = 0;
		deleteSelected = 0;
	}

	if (editor->GetObjectCount() > 0)
	{
		selectall = 0;
	}

	if (editor->GetDiagramEntityContainer() && editor->GetDiagramEntityContainer()->ObjectsInPaste())
	{
		paste = 0;
	}

	if (editor->GetDiagramEntityContainer() && editor->GetDiagramEntityContainer()->IsUndoPossible())
	{
		undo = 0;
	}

	if (editor->GetDiagramEntityContainer() && editor->GetDiagramEntityContainer()->IsRedoPossible())
	{
		redo = 0;
	}

	// TODO: Custom menu item support for GetSelectedItem
	// Maybe load a custom menu from a resource?
	// TODO: Remove other custom menu classes
	m_menu.EnableMenuItem(ID_EDIT_UNDO, MF_BYCOMMAND | undo);
	m_menu.EnableMenuItem(ID_EDIT_REDO, MF_BYCOMMAND | redo);
	m_menu.EnableMenuItem(ID_EDIT_CUT, MF_BYCOMMAND | cut);
	m_menu.EnableMenuItem(ID_EDIT_COPY, MF_BYCOMMAND | copy);
	m_menu.EnableMenuItem(ID_EDIT_PASTE, MF_BYCOMMAND | paste);
	m_menu.EnableMenuItem(ID_EDIT_DELETE, MF_BYCOMMAND | deleteSelected);
	m_menu.EnableMenuItem(ID_EDIT_SELECT_ALL, MF_BYCOMMAND | selectall);

	return m_menu.GetSubMenu(0);
}