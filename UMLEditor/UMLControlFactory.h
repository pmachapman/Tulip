#ifndef _UMLCONTROLFACTORY_H_
#define _UMLCONTROLFACTORY_H_

#include "DiagramEditor/DiagramEntity.h"

class CUMLControlFactory {

public:
// Implementation
	static CDiagramEntity* CreateFromString( const CString& str );

};

#endif // _UMLCONTROLFACTORY_H_