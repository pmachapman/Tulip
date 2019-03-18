// This MFC Samples source code demonstrates using MFC Microsoft Office Fluent User Interface 
// (the "Fluent UI") and is provided only as referential material to supplement the 
// Microsoft Foundation Classes Reference and related electronic documentation 
// included with the MFC C++ library software.  
// License terms to copy, use or distribute the Fluent UI are available separately.  
// To learn more about our Fluent UI licensing program, please visit 
// https://go.microsoft.com/fwlink/?LinkId=238214.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// NetView.h : interface of the CNetView class
//

#pragma once

#include "NetworkEditor/NetworkEditor.h"

#define DRAW_OBJECT_NONE		0
#define DRAW_OBJECT_CLIENT		1
#define DRAW_OBJECT_HUB			2
#define DRAW_OBJECT_INTERNET	3
#define DRAW_OBJECT_MODEM		4
#define DRAW_OBJECT_PRINTER		5
#define DRAW_OBJECT_SERVER		6

class CNetView : public CView
{
protected: // create from serialization only
	CNetView();
	DECLARE_DYNCREATE(CNetView)

	// Attributes
public:
	CNetDoc * GetDocument() const;

	// Operations
public:

	// Overrides
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

	// Implementation
public:
	virtual ~CNetView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnButtonAddClient();
	afx_msg void OnUpdateButtonAddClient(CCmdUI* pCmdUI);
	afx_msg void OnButtonAddHub();
	afx_msg void OnUpdateButtonAddHub(CCmdUI* pCmdUI);
	afx_msg void OnButtonAddInternet();
	afx_msg void OnUpdateButtonAddInternet(CCmdUI* pCmdUI);
	afx_msg void OnButtonAddModem();
	afx_msg void OnUpdateButtonAddModem(CCmdUI* pCmdUI);
	afx_msg void OnButtonAddPrinter();
	afx_msg void OnUpdateButtonAddPrinter(CCmdUI* pCmdUI);
	afx_msg void OnButtonAddServer();
	afx_msg void OnUpdateButtonAddServer(CCmdUI* pCmdUI);
	afx_msg void OnButtonLink();
	afx_msg void OnUpdateLink(CCmdUI* pCmdUI);
	afx_msg void OnExport();
	afx_msg void OnUpdateExport(CCmdUI* pCmdUI);
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
	afx_msg void OnSelectAll();
	afx_msg void OnUpdateSelectAll(CCmdUI* pCmdUI);
	afx_msg void OnProperty();
	afx_msg void OnUpdateProperty(CCmdUI* pCmdUI);
	afx_msg void OnDuplicate();
	afx_msg void OnUpdateDuplicate(CCmdUI* pCmdUI);
	afx_msg void OnGroup();
	afx_msg void OnUpdateGroup(CCmdUI* pCmdUI);
	afx_msg void OnUngroup();
	afx_msg void OnUpdateUngroup(CCmdUI* pCmdUI);
	afx_msg void OnSettings();
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
	afx_msg void OnEditDelete();
	afx_msg void OnUpdateEditDelete(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()

private:
	CNetworkEditor	m_editor;
	int			m_screenResolutionX;
	int			m_drawObject; // What type of object we are drawing
	BOOL		m_onlyh;
};

#ifndef _DEBUG  // debug version in NetView.cpp
inline CNetDoc* CNetView::GetDocument() const
{
	return reinterpret_cast<CNetDoc*>(m_pDocument);
}
#endif

