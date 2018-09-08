#ifndef _FLOCHARTCLIPBOARDHANDLER_H_
#define _FLOCHARTCLIPBOARDHANDLER_H_

#include "../DiagramEditor/DiagramClipboardHandler.h"

class CFlowchartClipboardHandler : public CDiagramClipboardHandler
{
public:
	// Construction/initialization/desturction
	CFlowchartClipboardHandler();
	virtual ~CFlowchartClipboardHandler();

	// Overrides
	virtual void	Copy(CDiagramEntity* obj);
	virtual void	Paste(CDiagramEntityContainer* container);
	virtual void	CopyAllSelected(CDiagramEntityContainer* container);
	virtual void	ClearPaste();

private:
	// Private data
	CObArray		m_pasteLinks;

};

#endif // _FLOCHARTCLIPBOARDHANDLER_H_