// FlowchartView.h : interface of the CFlowchartView class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "FlowchartEditor/FlowchartEditor.h"
#include "DiagramView.h"

#define DRAW_OBJECT_NONE		0
#define DRAW_OBJECT_ARROW		1
#define DRAW_OBJECT_BOX			2
#define DRAW_OBJECT_CONDITION	3
#define DRAW_OBJECT_CONNECTOR	4
#define DRAW_OBJECT_IO			5
#define DRAW_OBJECT_LABEL		6
#define DRAW_OBJECT_LINE		7
#define DRAW_OBJECT_START		8

class CFlowchartView : public CDiagramView
{
protected: // create from serialization only
	CFlowchartView();
	DECLARE_DYNCREATE(CFlowchartView)

	// Attributes
public:
	CFlowchartDoc* GetDocument() const;

	// Operations
public:

	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CFlowchartView)
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	virtual void OnDrawIconicThumbnailOrLivePreview(CDC& dc, CRect rect, CSize szRequiredThumbnailSize, BOOL bIsThumbnail, BOOL& bAlphaChannelSet);
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
	afx_msg void OnButtonArrow();
	afx_msg void OnUpdateButtonArrow(CCmdUI* pCmdUI);
	afx_msg void OnButtonBox();
	afx_msg void OnUpdateButtonBox(CCmdUI* pCmdUI);
	afx_msg void OnButtonCondition();
	afx_msg void OnUpdateButtonCondition(CCmdUI* pCmdUI);
	afx_msg void OnButtonConnector();
	afx_msg void OnUpdateButtonConnector(CCmdUI* pCmdUI);
	afx_msg void OnButtonIo();
	afx_msg void OnUpdateButtonIo(CCmdUI* pCmdUI);
	afx_msg void OnButtonLabel();
	afx_msg void OnUpdateButtonLabel(CCmdUI* pCmdUI);
	afx_msg void OnButtonLine();
	afx_msg void OnUpdateButtonLine(CCmdUI* pCmdUI);
	afx_msg void OnButtonStart();
	afx_msg void OnUpdateButtonStart(CCmdUI* pCmdUI);
	afx_msg void OnButtonLink();
	afx_msg void OnUpdateButtonLink(CCmdUI* pCmdUI);
	afx_msg void OnLinkLabel();
	afx_msg void OnUpdateLinkLabel(CCmdUI* pCmdUI);
	afx_msg void OnFlipLink();
	afx_msg void OnUpdateFlipLink(CCmdUI* pCmdUI);
	afx_msg void OnExport();
	afx_msg void OnUpdateExport(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	// Private data
	CFlowchartEditor	m_editor;
	int					m_drawObject; // What type of object we are drawing
	int					m_screenResolutionX;

};

#ifndef _DEBUG  // debug version in FlowchartView.cpp
inline CFlowchartDoc* CFlowchartView::GetDocument() const
{
	return reinterpret_cast<CFlowchartDoc*>(m_pDocument);
}
#endif
