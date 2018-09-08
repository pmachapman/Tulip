#if !defined(AFX_FLOWCHARTEDITOR_H__609E7C31_5443_4059_8ED9_DF9A743CFD2F__INCLUDED_)
#define AFX_FLOWCHARTEDITOR_H__609E7C31_5443_4059_8ED9_DF9A743CFD2F__INCLUDED_

#include "../DiagramEditor/DiagramEditor.h"
#include "FlowchartEntity.h"

class CFlowchartEditor : public CDiagramEditor
{

public:
	// Construction/initialization/destruction
	CFlowchartEditor();
	virtual ~CFlowchartEditor();

	// Operations
	BOOL CanLink();
	BOOL IsLinked();
	void ExportEMF(const CString& filename);

protected:
	// Overrides:
	virtual void DrawGrid(CDC* dc, CRect rect, double zoom) const;
	virtual void DrawObjects(CDC* dc, double zoom) const;
	virtual void SaveObjects(CStringArray& stra);

	// Message handlers
	virtual afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	virtual afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

public:
	// Command handlers
	afx_msg void OnLink();
	afx_msg void OnUnlink();
	afx_msg void OnLinkDirection();
	afx_msg void OnLinkProperties();
	DECLARE_MESSAGE_MAP()

private:
	// Private helpers
	void	ModifyLinkedPositions();
	void	AdjustLinkedObjects(CFlowchartEntity* parent, CFlowchartEntity* filter = NULL);

	CFlowchartEntity*	GetNamedObject(const CString& name) const;

};

#endif // !defined(AFX_FLOWCHARTEDITOR_H__609E7C31_5443_4059_8ED9_DF9A743CFD2F__INCLUDED_)
