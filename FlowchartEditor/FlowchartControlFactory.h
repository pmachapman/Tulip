#ifndef _FLOWCHARTCONTROLFACTORY_H_
#define _FLOWCHARTCONTROLFACTORY_H_

#include "../DiagramEditor/DiagramEntity.h"

class CFlowchartControlFactory {

public:
	// Implementation
	static CDiagramEntity* CreateFromString(const CString& str);

};

#endif // _FLOWCHARTCONTROLFACTORY_H_