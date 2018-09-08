/* ==========================================================================
	CFlowchartMenu

	Author :		Johan Rosengren, Abstrakt Mekanik AB

	Date :			2004-04-29

	Purpose :		CFlowchartMenu is a CDiagramMenu-derived class, and
					allows a specialized popup-menu (loaded from the
					application resources) to be displayed when the editor
					is right-clicked.

	Description :	Loads the popup-menu from resources instead of creating
					it internally, which helps with internationalization and
					allows adding of new menu selections.

	Usage :			Attach with a call to SetPopupMenu( new CFlowchartMenu )
					in the editor.

					The popup menu IDR_MENU_EDITOR_POPUP must be copied to
					the application resources.

   ========================================================================*/
#include "stdafx.h"
#include "../resource.h"

#include "FlowchartMenu.h"
#include "../DiagramEditor/DiagramEditor.h"
#include "FlowchartEditor.h"

CFlowchartMenu::CFlowchartMenu()
/* ============================================================
	Function :		CFlowchartMenu::CFlowchartMenu
	Description :	constructor

	Return :		void
	Parameters :	none

	Usage :

   ============================================================*/
{
}

CFlowchartMenu::~CFlowchartMenu()
/* ============================================================
	Function :		CFlowchartMenu::~CFlowchartMenu
	Description :	destructor

	Return :		void
	Parameters :	none

	Usage :

   ============================================================*/
{
}

CMenu* CFlowchartMenu::GetPopupMenu(CDiagramEditor* editor)
/* ============================================================
	Function :		CFlowchartMenu::GetPopupMenu
	Description :	Returns a popup-menu for the editor when it
					is right-clicked.

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
	if (m_menu.m_hMenu == NULL)
		m_menu.LoadMenu(IDR_FLOWCHART_MENU_EDITOR_POPUP);

	UINT cut = MF_GRAYED;
	UINT copy = MF_GRAYED;
	UINT paste = MF_GRAYED;
	UINT link = MF_GRAYED;
	UINT linked = MF_GRAYED;

	// Copy/paste
	if (editor->IsAnyObjectSelected())
	{
		cut = 0;
		copy = 0;
	}

	if (editor->GetDiagramEntityContainer() && editor->GetDiagramEntityContainer()->ObjectsInPaste())
		paste = 0;

	// Link-related
	CFlowchartEditor* ed = dynamic_cast<CFlowchartEditor*>(editor);
	if (ed)
	{
		if (ed->CanLink())
			link = 0;
		if (ed->IsLinked())
			linked = 0;
	}

	m_menu.EnableMenuItem(ID_EDIT_CUT, MF_BYCOMMAND | cut);
	m_menu.EnableMenuItem(ID_EDIT_COPY, MF_BYCOMMAND | copy);
	m_menu.EnableMenuItem(ID_EDIT_PASTE, MF_BYCOMMAND | paste);
	m_menu.EnableMenuItem(IDM_FLOWCHART_LINK, MF_BYCOMMAND | link);
	m_menu.EnableMenuItem(IDM_FLOWCHART_UNLINK, MF_BYCOMMAND | linked);
	m_menu.EnableMenuItem(IDM_FLOWCHART_LINK_DIRECTION, MF_BYCOMMAND | linked);
	m_menu.EnableMenuItem(IDM_FLOWCHART_LINK_PROPERTIES, MF_BYCOMMAND | linked);

	return m_menu.GetSubMenu(0);

}
