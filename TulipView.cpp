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

// TulipView.cpp : implementation of the CTulipView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Tulip.h"
#endif

#include "TulipDoc.h"
#include "TulipView.h"

#include "UMLEditor/UMLEntityClass.h"
#include "UMLEditor/UMLLineSegment.h"
#include "UMLEditor/UMLEntityNote.h"
#include "UMLEditor/UMLEntityPackage.h"
#include "UMLEditor/UMLEntityLabel.h"
#include "UMLEditor/UMLEntityInterface.h"
#include "UMLEditor/UMLEntityClassTemplate.h"
#include "UMLEditor/UMLEditorPropertyDialog.h"
#include "ExportDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTulipView

IMPLEMENT_DYNCREATE(CTulipView, CView)

BEGIN_MESSAGE_MAP(CTulipView, CView)
	ON_COMMAND(ID_BUTTON_CLASS, OnButtonClass)
	ON_COMMAND(ID_BUTTON_LINK, OnButtonLink)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_BUTTON_NOTE, OnButtonNote)
	ON_COMMAND(ID_BUTTON_PACKAGE, OnButtonPackage)
	ON_COMMAND(ID_BUTTON_LABEL, OnButtonLabel)
	ON_COMMAND(ID_BUTTON_INTERFACE, OnButtonInterface)
	ON_COMMAND(ID_BUTTON_DISPLAY_PROPERTIES, OnButtonDisplayProperties)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_COMMAND(ID_EXPORT, OnExport)
	ON_COMMAND(ID_FLIP_LINK, OnFlipLink)
	ON_UPDATE_COMMAND_UI(ID_FLIP_LINK, OnUpdateFlipLink)
	ON_COMMAND(ID_PROPERTY, OnProperty)
	ON_COMMAND(ID_ZOOM, OnZoom)
	ON_COMMAND(ID_ZOOM_100, OnZoom100)
	ON_COMMAND(ID_ZOOM_150, OnZoom150)
	ON_COMMAND(ID_ZOOM_200, OnZoom200)
	ON_COMMAND(ID_ZOOM_25, OnZoom25)
	ON_COMMAND(ID_ZOOM_50, OnZoom50)
	ON_COMMAND(ID_SNAP, OnSnap)
	ON_UPDATE_COMMAND_UI(ID_SNAP, OnUpdateSnap)
	ON_COMMAND(ID_SHOW_GRID, OnShowGrid)
	ON_UPDATE_COMMAND_UI(ID_SHOW_GRID, OnUpdateShowGrid)
	ON_COMMAND(ID_IMPORT, OnImport)
	ON_UPDATE_COMMAND_UI(ID_IMPORT, OnUpdateImport)
	ON_COMMAND(ID_EDIT_DELETE, OnEditDelete)
	ON_UPDATE_COMMAND_UI(ID_EDIT_DELETE, OnUpdateEditDelete)
	ON_COMMAND(ID_BUTTON_TEMPLATE, OnButtonTemplate)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CTulipView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CTulipView construction/destruction

CTulipView::CTulipView()
{
	m_screenResolutionX = 0;
	m_onlyh = FALSE;
}

CTulipView::~CTulipView()
{
}

BOOL CTulipView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}

// CTulipView drawing

void CTulipView::OnDraw(CDC* pDC)
{
	CTulipDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if (pDC->IsPrinting())
	{
		COLORREF col = m_editor.GetBackgroundColor();

		// Print zoom is the difference between screen- 
		// and printer resolution.
		double zoom = pDC->GetDeviceCaps(LOGPIXELSX) / m_screenResolutionX;

		CRect rect(0, 0,
			round(static_cast<double>(m_editor.GetVirtualSize().cx) * zoom),
			round(static_cast<double>(m_editor.GetVirtualSize().cy) * zoom));

		m_editor.SetRedraw(FALSE);
		m_editor.SetBackgroundColor(RGB(255, 255, 255));
		m_editor.Print(pDC, rect, zoom);
		m_editor.SetBackgroundColor(col);
		m_editor.SetRedraw(TRUE);
	}
}


// CTulipView printing


void CTulipView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CTulipView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CTulipView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

void CTulipView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

// CTulipView diagnostics

#ifdef _DEBUG
// Asserts the object validity.
void CTulipView::AssertValid() const
{
	CView::AssertValid();
}

// Dumps the object state
void CTulipView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

//Gets a casted pointer to the attached document.
CTulipDoc* CTulipView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTulipDoc)));
	return (CTulipDoc*)m_pDocument;
}
#endif //_DEBUG

// Called when a view is created.
void CTulipView::OnInitialUpdate()
{

	CView::OnInitialUpdate();

	if (!m_editor.m_hWnd)
	{
		// Creating the editor window
		CTulipDoc* pDoc = GetDocument();

		CRect rect;
		GetClientRect(rect);
		pDoc->GetData()->SetClipboardHandler(&theApp.m_clip);
		m_editor.Create(WS_CHILD | WS_VISIBLE, rect, this, pDoc->GetData());

		// We get the screen resolution, which we will use 
		// for scaling to printer. See also OnDraw.
		CClientDC dc(this);
		m_screenResolutionX = dc.GetDeviceCaps(LOGPIXELSX);

		// Setting up the virtual screen size
		// We want this to represent a single paper 
		// from the default printer.

		CPrintDialog	printer(TRUE, PD_RETURNDC);
		printer.GetDefaults();
		HDC	hdc = printer.GetPrinterDC();
		if (hdc)
		{

			double zoom = GetDeviceCaps(hdc, LOGPIXELSX) / m_screenResolutionX;
			int horzSize = ::GetDeviceCaps(hdc, PHYSICALWIDTH);
			int vertSize = ::GetDeviceCaps(hdc, PHYSICALHEIGHT);

			m_editor.SetVirtualSize(CSize(round(static_cast<double>(horzSize) / zoom), round(static_cast<double>(vertSize) / zoom)));

			int leftMarg = ::GetDeviceCaps(hdc, PHYSICALOFFSETX);
			int topMarg = ::GetDeviceCaps(hdc, PHYSICALOFFSETY);

			int horzPrintable = ::GetDeviceCaps(hdc, HORZRES);
			int vertPrintable = ::GetDeviceCaps(hdc, VERTRES);

			int rightMarg = horzSize - (horzPrintable + leftMarg);
			int bottomMarg = vertSize - (vertPrintable + topMarg);

			m_editor.SetMargins(round(static_cast<double>(leftMarg) / zoom), round(static_cast<double>(topMarg) / zoom), round(static_cast<double>(rightMarg) / zoom), round(static_cast<double>(bottomMarg) / zoom));

			::DeleteDC(hdc);

		}
		else
		{
			// No default printer installed
			m_editor.SetVirtualSize(CSize(8 * m_screenResolutionX, 11 * m_screenResolutionX));
		}

		m_editor.SetScrollWheelMode(WHEEL_ZOOM);
		m_editor.SetModified(FALSE);

	}
	else
	{
		m_editor.Clear();
	}
}

// CTulipView message handlers

void CTulipView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	if (m_editor.m_hWnd)
	{
		m_editor.MoveWindow(0, 0, cx, cy);
	}
}

BOOL CTulipView::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}

// CTulipView button and menu handlers

void CTulipView::OnButtonLink()
{
	m_editor.StartDrawingObject(new CUMLLineSegment);
}

void CTulipView::OnButtonNote()
{
	CUMLEntityNote* obj = new CUMLEntityNote;
	m_editor.StartDrawingObject(obj);
}

void CTulipView::OnButtonPackage()
{
	CUMLEntityPackage* obj = new CUMLEntityPackage;
	m_editor.StartDrawingObject(obj);
}

void CTulipView::OnButtonClass()
{
	CUMLEntityClass* obj = new CUMLEntityClass;
	m_editor.StartDrawingObject(obj);
}

void CTulipView::OnButtonLabel()
{
	CUMLEntityLabel* obj = new CUMLEntityLabel;
	m_editor.StartDrawingObject(obj);
}

void CTulipView::OnButtonInterface()
{
	CUMLEntityInterface* obj = new CUMLEntityInterface;
	m_editor.StartDrawingObject(obj);
}

void CTulipView::OnButtonDisplayProperties()
{
	CUMLEditorPropertyDialog dlg;

	dlg.m_color = m_editor.GetBackgroundColor();
	dlg.m_showgrid = m_editor.IsGridVisible();
	dlg.m_grid = m_editor.GetSnapToGrid();
	dlg.m_showmargin = m_editor.IsMarginVisible();

	int displayOptions = m_editor.GetDisplayOptions();
	if (displayOptions & DISPLAY_ONLY_PUBLIC)
	{
		dlg.m_onlypublic = TRUE;
	}

	if (displayOptions & DISPLAY_NO_MARKERS)
	{
		dlg.m_nomarkers = TRUE;
	}

	if (displayOptions & DISPLAY_NO_ATTRIBUTES)
	{
		dlg.m_noattributes = TRUE;
	}

	if (displayOptions & DISPLAY_NO_OPERATIONS)
	{
		dlg.m_nooperations = TRUE;
	}

	if (displayOptions & DISPLAY_NO_OPERATION_ATTRIBUTE_NAMES)
	{
		dlg.m_nooperationattributenames = TRUE;
	}

	displayOptions = DISPLAY_ALL;
	if (dlg.DoModal() == IDOK)
	{
		m_editor.ShowGrid(dlg.m_showgrid);
		m_editor.ShowMargin(dlg.m_showmargin);
		m_editor.SetSnapToGrid(dlg.m_grid);
		m_editor.SetColor(dlg.m_color);

		if (dlg.m_onlypublic)
		{
			displayOptions |= DISPLAY_ONLY_PUBLIC;
		}

		if (dlg.m_nomarkers)
		{
			displayOptions |= DISPLAY_NO_MARKERS;
		}

		if (dlg.m_noattributes)
		{
			displayOptions |= DISPLAY_NO_ATTRIBUTES;
		}

		if (dlg.m_nooperations)
		{
			displayOptions |= DISPLAY_NO_OPERATIONS;
		}

		if (dlg.m_nooperationattributenames)
		{
			displayOptions |= DISPLAY_NO_OPERATION_ATTRIBUTE_NAMES;
		}

		m_editor.SetDisplayOptions(displayOptions);
		m_editor.RedrawWindow();
	}
}

void CTulipView::OnEditUndo()
{
	m_editor.Undo();
}

void CTulipView::OnEditCopy()
{
	m_editor.Copy();
}

void CTulipView::OnEditCut()
{
	m_editor.Cut();
}

void CTulipView::OnEditPaste()
{
	m_editor.Paste();
}

void CTulipView::OnEditDelete()
{
	m_editor.DeleteAllSelected();
}

void CTulipView::OnFlipLink()
{
	m_editor.FlipLink();
	m_editor.RedrawWindow();
}

void CTulipView::OnProperty()
{
	if (m_editor.GetSelectCount() == 1)
	{
		m_editor.ShowProperties();
	}
	else
	{
		OnButtonDisplayProperties();
	}
}

void CTulipView::OnZoom()
{
}

void CTulipView::OnZoom100()
{
	m_editor.SetZoom(1);
}

void CTulipView::OnZoom150()
{
	m_editor.SetZoom(1.5);
}

void CTulipView::OnZoom200()
{
	m_editor.SetZoom(2);
}

void CTulipView::OnZoom25()
{
	m_editor.SetZoom(.25);
}

void CTulipView::OnZoom50()
{
	m_editor.SetZoom(.5);
}

void CTulipView::OnSnap()
{
	m_editor.SetSnapToGrid(!m_editor.GetSnapToGrid());
}

void CTulipView::OnShowGrid()
{
	m_editor.ShowGrid(!m_editor.IsGridVisible());
}

void CTulipView::OnImport()
{
	m_editor.Import();
}

// Displays the Export dialog and exports the editor contents to the desired format.
void CTulipView::OnExport()
{
	// We save the current modified-state of 
	// the editor. The export mechanism 
	// modifies the data (although in a harmless 
	// way) and we want to reset the state 
	// after the operation - to avoid save 
	// questions when no conscious changes have 
	// been made.

	BOOL modified = m_editor.IsModified();

	CTulipDoc* doc = GetDocument();
	CExportDialog dlg;
	dlg.m_onlyh = m_onlyh;
	if (dlg.DoModal() == IDOK)
	{
		CWaitCursor wait;
		CString title = doc->GetTitle();
		int found = title.ReverseFind(_TCHAR('.'));
		if (found != -1)
		{
			title = title.Left(found);
		}

		switch (dlg.m_format)
		{
		case 0:
			m_editor.ExportEMF(title);
			break;
		case 1:
			m_editor.ExportHTML();
			break;
		case 2:
			m_editor.SetStripLeadingClassCharacter(dlg.m_strip);
			m_onlyh = dlg.m_onlyh;
			m_editor.ExportCPP(title, m_onlyh);
			break;
		}
	}

	m_editor.SetModified(modified);

}

// CUMLEditorDemoView update handlers

void CTulipView::OnUpdateEditUndo(CCmdUI* pCmdUI)
{
	m_editor.UpdateUndo(pCmdUI);
}

void CTulipView::OnUpdateEditCopy(CCmdUI* pCmdUI)
{
	m_editor.UpdateCopy(pCmdUI);
}

void CTulipView::OnUpdateEditCut(CCmdUI* pCmdUI)
{
	m_editor.UpdateCut(pCmdUI);
}

void CTulipView::OnUpdateEditPaste(CCmdUI* pCmdUI)
{

	m_editor.UpdatePaste(pCmdUI);

}

void CTulipView::OnUpdateFlipLink(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_editor.IsLinkSelected());
}

void CTulipView::OnUpdateSnap(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_editor.GetSnapToGrid());
}

void CTulipView::OnUpdateShowGrid(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_editor.IsGridVisible());
}

void CTulipView::OnUpdateImport(CCmdUI* pCmdUI)
{
	BOOL result = FALSE;

	if (m_editor.GetSelectCount() == 1)
	{
		CUMLEntityClass* obj = dynamic_cast<CUMLEntityClass*>(m_editor.GetSelectedObject());
		if (obj)
		{
			result = TRUE;
		}
	}

	pCmdUI->Enable(result);
}

void CTulipView::OnUpdateEditDelete(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_editor.GetSelectCount() > 0);
}

void CTulipView::OnButtonTemplate()
{
	CUMLEntityClassTemplate* obj = new CUMLEntityClassTemplate;
	m_editor.StartDrawingObject(obj);
}
