#if !defined(AFX_UMLMENU_H__8BBDE424_2729_426C_89EC_B3E3E1021F92__INCLUDED_)
#define AFX_UMLMENU_H__8BBDE424_2729_426C_89EC_B3E3E1021F92__INCLUDED_

#include "DiagramEditor/DiagramMenu.h"

class CUMLMenu : public CDiagramMenu
{

public:
// Construction/initialization/destruction
	CUMLMenu();
	virtual ~CUMLMenu();

// Overrides
	virtual CMenu* GetPopupMenu( CDiagramEditor* editor );
};

#endif // !defined(AFX_UMLMENU_H__8BBDE424_2729_426C_89EC_B3E3E1021F92__INCLUDED_)
