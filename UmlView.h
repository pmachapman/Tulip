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

// UmlView.h : interface of the CUmlView class
//

#pragma once

#include "UMLEditor/UMLEditor.h"
#include "DiagramView.h"

#define DRAW_OBJECT_NONE		0
#define DRAW_OBJECT_CLASS		1
#define DRAW_OBJECT_INTERFACE	2
#define DRAW_OBJECT_UML_LABEL	3
#define DRAW_OBJECT_LINK		4
#define DRAW_OBJECT_NOTE		5
#define DRAW_OBJECT_PACKAGE		6
#define DRAW_OBJECT_TEMPLATE	7

class CUmlView : public CDiagramView
{
protected: // create from serialization only
	CUmlView();
	DECLARE_DYNCREATE(CUmlView)

	// Attributes
public:
	CUmlDoc * GetDocument() const;

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
	virtual ~CUmlView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	afx_msg void OnButtonClass();
	afx_msg void OnUpdateButtonClass(CCmdUI* pCmdUI);
	afx_msg void OnButtonLabel();
	afx_msg void OnUpdateButtonLabel(CCmdUI* pCmdUI);
	afx_msg void OnButtonLink();
	afx_msg void OnUpdateButtonLink(CCmdUI* pCmdUI);
	afx_msg void OnButtonInterface();
	afx_msg void OnUpdateButtonInterface(CCmdUI* pCmdUI);
	afx_msg void OnButtonNote();
	afx_msg void OnUpdateButtonNote(CCmdUI* pCmdUI);
	afx_msg void OnButtonPackage();
	afx_msg void OnUpdateButtonPackage(CCmdUI* pCmdUI);
	afx_msg void OnButtonTemplate();
	afx_msg void OnUpdateButtonTemplate(CCmdUI* pCmdUI);
	afx_msg void OnExport();
	afx_msg void OnExportCpp();
	afx_msg void OnExportEmf();
	afx_msg void OnExportHtml();
	afx_msg void OnUpdateExport(CCmdUI* pCmdUI);
	afx_msg void OnFlipLink();
	afx_msg void OnUpdateFlipLink(CCmdUI* pCmdUI);
	afx_msg void OnImport();
	afx_msg void OnUpdateImport(CCmdUI* pCmdUI);
	afx_msg void OnOpenPackage();
	afx_msg void OnUpdateOpenPackage(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()

private:
	CUMLEditor	m_editor;
	int			m_screenResolutionX;
	int			m_drawObject;			// What type of object we are drawing
	BOOL		m_onlyh;
};

#ifndef _DEBUG  // debug version in UmlView.cpp
inline CUmlDoc* CUmlView::GetDocument() const
{
	return reinterpret_cast<CUmlDoc*>(m_pDocument);
}
#endif

