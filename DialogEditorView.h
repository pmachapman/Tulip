// DialogEditorView.h : interface of the CDialogEditorView class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

// --- DiagramEditor ---
#include "DialogEditor/DialogEditor.h"

#define DRAW_OBJECT_NONE		0
#define DRAW_OBJECT_BUTTON		1
#define DRAW_OBJECT_EDIT		2
#define DRAW_OBJECT_STATIC		3
#define DRAW_OBJECT_CHECKBOX	4
#define DRAW_OBJECT_COMBOBOX	5
#define DRAW_OBJECT_GROUPBOX	6
#define DRAW_OBJECT_LISTBOX		7
#define DRAW_OBJECT_RADIOBUTTON	8

class CDialogEditorView : public CView
{
protected: // create from serialization only
	CDialogEditorView();
	DECLARE_DYNCREATE(CDialogEditorView)

	// Attributes
public:
	CDialogEditorDoc* GetDocument();

	// Operations
public:

	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CDialogEditorView)
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
	virtual ~CDialogEditorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// Generated message map functions
protected:
	//{{AFX_MSG(CDialogEditorView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
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
	afx_msg void OnProperty();
	afx_msg void OnUpdateProperty(CCmdUI* pCmdUI);
	afx_msg void OnZoom();
	afx_msg void OnZoom100();
	afx_msg void OnZoom150();
	afx_msg void OnZoom200();
	afx_msg void OnZoom25();
	afx_msg void OnZoom50();
	afx_msg void OnAddButton();
	afx_msg void OnAddEdit();
	afx_msg void OnAddStatic();
	afx_msg void OnAddCheckbox();
	afx_msg void OnAddCombobox();
	afx_msg void OnAddGroupbox();
	afx_msg void OnAddListbox();
	afx_msg void OnAddRadiobutton();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnShowGrid();
	afx_msg void OnUpdateShowGrid(CCmdUI* pCmdUI);
	afx_msg void OnSnapToGrid();
	afx_msg void OnUpdateSnapToGrid(CCmdUI* pCmdUI);
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
	afx_msg void OnExport();
	afx_msg void OnUpdateExport(CCmdUI* pCmdUI);
	afx_msg void OnAddNone();
	afx_msg void OnUpdateAddNone(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAddButton(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAddCheckbox(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAddCombobox(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAddEdit(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAddGroupbox(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAddListbox(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAddRadiobutton(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAddStatic(CCmdUI* pCmdUI);
	afx_msg void OnButtonMultidraw();
	afx_msg void OnUpdateButtonMultidraw(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	// --- DiagramEditor ---
private:

	CDialogEditor	m_editor;
	int			m_screenResolutionX;	// Used for scaling printouts
	int			m_drawObject;			// What type of object we are drawing


	HBITMAP		m_bitmap;
};

#ifndef _DEBUG  // debug version in DialogEditorView.cpp
inline CDialogEditorDoc* CDialogEditorView::GetDocument()
{
	return (CDialogEditorDoc*)m_pDocument;
}
#endif
