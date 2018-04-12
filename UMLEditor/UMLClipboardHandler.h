#ifndef _UMLCLIPBOARDHANDLER_H_
#define _UMLCLIPBOARDHANDLER_H_

#include "DiagramEditor/DiagramClipboardHandler.h"

class CUMLClipboardHandler : public CDiagramClipboardHandler
{
public:
// Construction/initialization/desturction
	CUMLClipboardHandler();
	virtual ~CUMLClipboardHandler();

// Overrides
	virtual void	Copy( CDiagramEntity* obj );
	virtual void	CopyAllSelected( CDiagramEntityContainer* container );
	virtual void	Paste( CDiagramEntityContainer* container );

private:
	void			FixLinks();

};

#endif // _UMLCLIPBOARDHANDLER_H_