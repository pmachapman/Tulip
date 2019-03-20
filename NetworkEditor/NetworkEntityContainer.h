#ifndef _NETWORKENTITYCONTAINER_H_
#define _NETWORKENTITYCONTAINER_H_

#include "../DiagramEditor/DiagramEntityContainer.h"
#include "NetworkLink.h"
#include "NetworkSymbol.h"

class CNetworkEntityContainer : public CDiagramEntityContainer
{

public:
	CNetworkEntityContainer();
	virtual ~CNetworkEntityContainer();

	INT_PTR			GetLinks() const;
	CObArray*		GetLinkArray();
	BOOL			CreateLink(CNetworkSymbol* from, CNetworkSymbol* to);
	CNetworkLink*	GetLinkAt(INT_PTR index);
	CNetworkSymbol* GetPrimaryLink();
	CNetworkSymbol* GetSecondaryLink();
	CNetworkLink*	FindLink(CDiagramEntity* obj1, CDiagramEntity* obj2);
	void			DeleteLink(CNetworkLink* inlink);

	// Selection
	int				GetSelectCount();
	CNetworkSymbol*	GetPrimarySelected();
	CNetworkSymbol*	GetSecondarySelected();


	void AddLink(CNetworkLink* link);
	BOOL CanLink();
	BOOL IsLinked();

	// Overrides
	virtual void	RemoveAt(int index);
	virtual void	Undo();
	virtual void	Snapshot();
	virtual void	ClearUndo();

	virtual void	Redo();
	void			PushLinks(CObArray* stack);
	virtual void	ClearRedo();

private:
	CObArray	m_links;
	CObArray	m_redoLinks;
	CObArray	m_undoLinks;

	// Private helpers
	void ClearLinks();

};

#endif //_NETWORKENTITYCONTAINER_H_

