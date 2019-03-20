#pragma once

#include "DiagramEditor/DiagramEditor.h"

// CDiagramView view

class CDiagramView : public CView
{
	DECLARE_DYNCREATE(CDiagramView)

protected:
	CDiagramView();           // protected constructor used by dynamic creation
	virtual ~CDiagramView();

public:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	void SetEditor(CDiagramEditor* editor);

	// Generated message map functions
protected:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnEditUndo();
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnEditRedo();
	afx_msg void OnUpdateEditRedo(CCmdUI* pCmdUI);
	afx_msg void OnEditCopy();
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnEditCut();
	afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnEditDelete();
	afx_msg void OnUpdateEditDelete(CCmdUI* pCmdUI);
	afx_msg void OnEditSelectAll();
	afx_msg void OnUpdateEditSelectAll(CCmdUI* pCmdUI);
	afx_msg void OnSettings();
	afx_msg void OnProperty();
	afx_msg void OnUpdateProperty(CCmdUI* pCmdUI);
	afx_msg void OnDuplicate();
	afx_msg void OnUpdateDuplicate(CCmdUI* pCmdUI);
	afx_msg void OnGroup();
	afx_msg void OnUpdateGroup(CCmdUI* pCmdUI);
	afx_msg void OnUngroup();
	afx_msg void OnUpdateUngroup(CCmdUI* pCmdUI);
	afx_msg void OnZoom();
	afx_msg void OnZoom100();
	afx_msg void OnZoom150();
	afx_msg void OnZoom200();
	afx_msg void OnZoom25();
	afx_msg void OnZoom50();
	afx_msg void OnSnap();
	afx_msg void OnUpdateSnap(CCmdUI* pCmdUI);
	afx_msg void OnShowGrid();
	afx_msg void OnUpdateShowGrid(CCmdUI* pCmdUI);
	afx_msg void OnMargins();
	afx_msg void OnUpdateMargins(CCmdUI* pCmdUI);
	afx_msg void OnRestraints();
	afx_msg void OnUpdateRestraints(CCmdUI* pCmdUI);
	afx_msg void OnBottom();
	afx_msg void OnUpdateBottom(CCmdUI* pCmdUI);
	afx_msg void OnBottomAlign();
	afx_msg void OnUpdateBottomAlign(CCmdUI* pCmdUI);
	afx_msg void OnDown();
	afx_msg void OnUpdateDown(CCmdUI* pCmdUI);
	afx_msg void OnFront();
	afx_msg void OnUpdateFront(CCmdUI* pCmdUI);
	afx_msg void OnLeftAlign();
	afx_msg void OnUpdateLeftAlign(CCmdUI* pCmdUI);
	afx_msg void OnMakeSameSize();
	afx_msg void OnUpdateMakeSameSize(CCmdUI* pCmdUI);
	afx_msg void OnRightAlign();
	afx_msg void OnUpdateRightAlign(CCmdUI* pCmdUI);
	afx_msg void OnTopAlign();
	afx_msg void OnUpdateTopAlign(CCmdUI* pCmdUI);
	afx_msg void OnUp();
	afx_msg void OnUpdateUp(CCmdUI* pCmdUI);
protected:
	DECLARE_MESSAGE_MAP()

private:
	CDiagramEditor* m_editor;
};


