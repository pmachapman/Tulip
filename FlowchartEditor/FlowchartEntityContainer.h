#ifndef _FLOWCHARTENTITYCONTAINER_H_
#define _FLOWCHARTENTITYCONTAINER_H_

#include "../DiagramEditor/DiagramEntityContainer.h"
#include "../DiagramEditor/DiagramEntity.h"
#include "../DiagramEditor/Tokenizer.h"
#include "FlowchartEntity.h"
#include "FlowchartLink.h"

class CFlowchartEntityContainer : public CDiagramEntityContainer {

public:
	// Construction/initialization/destruction
	CFlowchartEntityContainer();
	virtual ~CFlowchartEntityContainer();

	// Implementation
		// Links
	BOOL			CreateLink(CFlowchartEntity* from, CFlowchartEntity* to, const CString& title);
	BOOL			HasLinks(CFlowchartEntity* from, CFlowchartEntity* to);

	CFlowchartLink* GetLinkAt(INT_PTR index);
	INT_PTR			GetLinks();
	void			AddLink(CFlowchartLink* link);
	CFlowchartLink* FindLink(CDiagramEntity* obj1, CDiagramEntity* obj2);
	void			DeleteLink(CFlowchartLink* link);

	BOOL			IsLinked();
	BOOL			CanLink();

	CFlowchartEntity* GetPrimaryLink();
	CFlowchartEntity* GetSecondaryLink();

	// Selection
	int					GetSelectCount();
	CFlowchartEntity*	GetPrimarySelected();
	CFlowchartEntity*	GetSecondarySelected();

	// Overrides
	virtual void	RemoveAt(int index);

	virtual void	Undo();
	virtual void	Snapshot();
	virtual void	ClearUndo();

	virtual void	Redo();
	void			PushLinks(CObArray* stack);
	virtual void	ClearRedo();

	CObArray*		GetLinkArray() { return &m_links; }

private:
	// Private data
	CObArray		m_links;
	CObArray		m_redoLinks;
	CObArray		m_undoLinks;

	// Private helpers
	void ClearLinks();
	BOOL FindClosestLink(CFlowchartEntity* obj1, CFlowchartEntity* obj2, int& fromtype, int& totype);
	double	Dist(CPoint point1, CPoint point2);

};

#endif // _FLOWCHARTENTITYCONTAINER_H_
