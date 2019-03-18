#ifndef _DIAGRAMMENU_H_
#define _DIAGRAMMENU_H_

#include "resource.h"

class CDiagramEditor;

class CDiagramMenu
{

public:
	CDiagramMenu();
	virtual ~CDiagramMenu();

	virtual CMenu* GetPopupMenu(CDiagramEditor* editor);

protected:
	CMenu	m_menu;

};

#endif // _DIAGRAMMENU_H_