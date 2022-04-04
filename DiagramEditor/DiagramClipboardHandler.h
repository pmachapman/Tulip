#ifndef _DIAGRAMCLIPBOARDHANDLER_H_
#define _DIAGRAMCLIPBOARDHANDLER_H_

#include "DiagramEntity.h"

class CDiagramClipboardHandler {

public:
	// Construction/initialization/destruction
	CDiagramClipboardHandler();
	virtual ~CDiagramClipboardHandler();

	// Implementation
	virtual void	Copy(CDiagramEntity* obj);
	virtual void	Paste(CDiagramEntityContainer* container);

	virtual void	CopyAllSelected(CDiagramEntityContainer* container);
	virtual INT_PTR	ObjectsInPaste();
	virtual void	ClearPaste();

	virtual CObArray* GetData();

protected:

	virtual CObArray* GetPastedData();

private:
	// Private data
	CObArray		m_paste;
	CObArray		m_lastPaste;
};

#endif // _DIAGRAMCLIPBOARDHANDLER_H_