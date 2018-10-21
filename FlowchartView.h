// FlowchartView.h : interface of the CFlowchartView class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "FlowchartEditor/FlowchartEditor.h"

class CFlowchartView : public CView
{
protected: // create from serialization only
	CFlowchartView();
	DECLARE_DYNCREATE(CFlowchartView)

	// Attributes
public:
	CFlowchartDoc* GetDocument();

	// Operations
public:

	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CFlowchartView)
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
protected:
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFlowchartView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// Generated message map functions
protected:
	//{{AFX_MSG(CFlowchartView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnButtonBox();
	afx_msg void OnButtonCondition();
	afx_msg void OnButtonConnector();
	afx_msg void OnButtonIo();
	afx_msg void OnButtonLine();
	afx_msg void OnButtonStart();
	afx_msg void OnEditCopy();
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnEditCut();
	afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnSelectAll();
	afx_msg void OnUpdateSelectAll(CCmdUI* pCmdUI);
	afx_msg void OnEditUndo();
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnSettings();
	afx_msg void OnExport();
	afx_msg void OnButtonArrow();
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
	afx_msg void OnButtonLabel();
	afx_msg void OnButtonLink();
	afx_msg void OnUpdateButtonLink(CCmdUI* pCmdUI);
	afx_msg void OnLinkLabel();
	afx_msg void OnUpdateLinkLabel(CCmdUI* pCmdUI);
	afx_msg void OnFlipLink();
	afx_msg void OnUpdateFlipLink(CCmdUI* pCmdUI);
	afx_msg void OnProperty();
	afx_msg void OnUpdateProperty(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	// Private data
	CFlowchartEditor	m_editor;
	int					m_screenResolutionX;

};

#ifndef _DEBUG  // debug version in FlowchartView.cpp
inline CFlowchartDoc* CFlowchartView::GetDocument() const
{
	return reinterpret_cast<CFlowchartDoc*>(m_pDocument);
}
#endif
