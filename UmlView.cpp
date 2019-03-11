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

// UmlView.cpp : implementation of the CUmlView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Tulip.h"
#endif

#include "MainFrm.h"
#include "UmlDoc.h"
#include "UmlView.h"

#include "DialogEditor/DialogSettings.h"

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


// CUmlView

IMPLEMENT_DYNCREATE(CUmlView, CView)

BEGIN_MESSAGE_MAP(CUmlView, CView)
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
	ON_COMMAND(ID_EDIT_SELECT_ALL, OnSelectAll)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SELECT_ALL, OnUpdateSelectAll)
	ON_COMMAND(ID_EXPORT, OnExport)
	ON_UPDATE_COMMAND_UI(ID_EXPORT, OnUpdateExport)
	ON_COMMAND(ID_EXPORT_CPP, OnExportCpp)
	ON_UPDATE_COMMAND_UI(ID_EXPORT_CPP, OnUpdateExport)
	ON_COMMAND(ID_EXPORT_EMF, OnExportEmf)
	ON_UPDATE_COMMAND_UI(ID_EXPORT_EMF, OnUpdateExport)
	ON_COMMAND(ID_EXPORT_HTML, OnExportHtml)
	ON_UPDATE_COMMAND_UI(ID_EXPORT_HTML, OnUpdateExport)
	ON_COMMAND(ID_FLIP_LINK, OnFlipLink)
	ON_UPDATE_COMMAND_UI(ID_FLIP_LINK, OnUpdateFlipLink)
	ON_COMMAND(ID_PROPERTY, OnProperty)
	ON_UPDATE_COMMAND_UI(ID_PROPERTY, OnUpdateProperty)
	ON_COMMAND(ID_MARGINS, OnMargins)
	ON_UPDATE_COMMAND_UI(ID_MARGINS, OnUpdateMargins)
	ON_COMMAND(ID_RESTRAIN, OnRestraints)
	ON_UPDATE_COMMAND_UI(ID_RESTRAIN, OnUpdateRestraints)
	ON_COMMAND(ID_BOTTOM, OnBottom)
	ON_UPDATE_COMMAND_UI(ID_BOTTOM, OnUpdateBottom)
	ON_COMMAND(ID_BOTTOM_ALIGN, OnBottomAlign)
	ON_UPDATE_COMMAND_UI(ID_BOTTOM_ALIGN, OnUpdateBottomAlign)
	ON_COMMAND(ID_DOWN, OnDown)
	ON_UPDATE_COMMAND_UI(ID_DOWN, OnUpdateDown)
	ON_COMMAND(ID_FRONT, OnFront)
	ON_UPDATE_COMMAND_UI(ID_FRONT, OnUpdateFront)
	ON_COMMAND(ID_LEFT_ALIGN, OnLeftAlign)
	ON_UPDATE_COMMAND_UI(ID_LEFT_ALIGN, OnUpdateLeftAlign)
	ON_COMMAND(ID_MAKE_SAME_SIZE, OnMakeSameSize)
	ON_UPDATE_COMMAND_UI(ID_MAKE_SAME_SIZE, OnUpdateMakeSameSize)
	ON_COMMAND(ID_RIGHT_ALIGN, OnRightAlign)
	ON_UPDATE_COMMAND_UI(ID_RIGHT_ALIGN, OnUpdateRightAlign)
	ON_COMMAND(ID_TOP_ALIGN, OnTopAlign)
	ON_UPDATE_COMMAND_UI(ID_TOP_ALIGN, OnUpdateTopAlign)
	ON_COMMAND(ID_UP, OnUp)
	ON_UPDATE_COMMAND_UI(ID_UP, OnUpdateUp)
	ON_COMMAND(IDC_SETTINGS, OnSettings)
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
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CUmlView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CUmlView construction/destruction

CUmlView::CUmlView()
{
	m_screenResolutionX = 0;
	m_onlyh = FALSE;
}

CUmlView::~CUmlView()
{
}

BOOL CUmlView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}

// CUmlView drawing

void CUmlView::OnDraw(CDC* pDC)
{
	CUmlDoc* pDoc = GetDocument();
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

void CUmlView::OnDrawIconicThumbnailOrLivePreview(CDC& dc, CRect rect, CSize szRequiredThumbnailSize, BOOL bIsThumbnail, BOOL& bAlphaChannelSet)
{
	m_editor.Draw(&dc, rect);
}


// CUmlView printing


void CUmlView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CUmlView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CUmlView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

void CUmlView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

// CUmlView diagnostics

#ifdef _DEBUG
// Asserts the object validity.
void CUmlView::AssertValid() const
{
	CView::AssertValid();
}

// Dumps the object state
void CUmlView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

//Gets a casted pointer to the attached document.
CUmlDoc* CUmlView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CUmlDoc)));
	return (CUmlDoc*)m_pDocument;
}
#endif //_DEBUG

// Called when a view is created.
void CUmlView::OnInitialUpdate()
{

	CView::OnInitialUpdate();

	if (!m_editor.m_hWnd)
	{
		// Creating the editor window
		CUmlDoc* pDoc = GetDocument();

		CRect rect;
		GetClientRect(rect);
		pDoc->GetData()->SetClipboardHandler(&theApp.m_umlClip);
		m_editor.Create(WS_CHILD | WS_VISIBLE, rect, this, pDoc->GetData());

		// Only set the size if one wasn't pre-specified
		if (!pDoc->GetData()->IsModified())
		{
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
				m_editor.SetBackgroundColor(RGB(250, 250, 230));

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
				m_editor.SetBackgroundColor(RGB(250, 250, 230));
			}
		}
		else
		{
			m_editor.SetVirtualSize(pDoc->GetData()->GetVirtualSize());
			m_editor.SetBackgroundColor(pDoc->GetData()->GetColor());
		}

		m_editor.SetScrollWheelMode(WHEEL_SCROLL);
		m_editor.SetResize(TRUE);
		m_editor.SetModified(FALSE);

	}
	else
	{
		m_editor.Clear();
	}
}

// Called the the view is activated
void CUmlView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	CMainFrame* pMainFrame = ((CMainFrame*)GetTopLevelFrame());
	ASSERT_VALID(pMainFrame);
	pMainFrame->SetRibbonContextCategory(ID_UMLCONTEXT, bActivate);
}

// CUmlView message handlers

void CUmlView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	if (m_editor.m_hWnd)
	{
		m_editor.MoveWindow(0, 0, cx, cy);
	}
}

BOOL CUmlView::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}

// CUmlView button and menu handlers

void CUmlView::OnButtonLink()
{
	m_editor.StartDrawingObject(new CUMLLineSegment);
}

void CUmlView::OnButtonNote()
{
	CUMLEntityNote* obj = new CUMLEntityNote;
	m_editor.StartDrawingObject(obj);
}

void CUmlView::OnButtonPackage()
{
	CUMLEntityPackage* obj = new CUMLEntityPackage;
	m_editor.StartDrawingObject(obj);
}

void CUmlView::OnButtonClass()
{
	CUMLEntityClass* obj = new CUMLEntityClass;
	m_editor.StartDrawingObject(obj);
}

void CUmlView::OnButtonLabel()
{
	CUMLEntityLabel* obj = new CUMLEntityLabel;
	m_editor.StartDrawingObject(obj);
}

void CUmlView::OnButtonInterface()
{
	CUMLEntityInterface* obj = new CUMLEntityInterface;
	m_editor.StartDrawingObject(obj);
}

void CUmlView::OnButtonDisplayProperties()
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
		m_editor.SetBackgroundColor(dlg.m_color);

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

void CUmlView::OnEditUndo()
{
	m_editor.Undo();
}

void CUmlView::OnEditCopy()
{
	m_editor.Copy();
}

void CUmlView::OnEditCut()
{
	m_editor.Cut();
}

void CUmlView::OnEditPaste()
{
	m_editor.Paste();
}

void CUmlView::OnEditDelete()
{
	m_editor.DeleteAllSelected();
}

void CUmlView::OnFlipLink()
{
	m_editor.FlipLink();
	m_editor.RedrawWindow();
}

void CUmlView::OnProperty()
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

void CUmlView::OnUpdateProperty(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_editor.GetSelectCount() <= 1);
}

void CUmlView::OnZoom()
{
}

void CUmlView::OnZoom100()
{
	m_editor.SetZoom(1);
}

void CUmlView::OnZoom150()
{
	m_editor.SetZoom(1.5);
}

void CUmlView::OnZoom200()
{
	m_editor.SetZoom(2);
}

void CUmlView::OnZoom25()
{
	m_editor.SetZoom(.25);
}

void CUmlView::OnZoom50()
{
	m_editor.SetZoom(.5);
}

void CUmlView::OnSnap()
{
	m_editor.SetSnapToGrid(!m_editor.GetSnapToGrid());
}

void CUmlView::OnShowGrid()
{
	m_editor.ShowGrid(!m_editor.IsGridVisible());
}

void CUmlView::OnRestraints()
{
	if (m_editor.GetRestraints() == RESTRAINT_MARGIN)
		m_editor.SetRestraints(RESTRAINT_VIRTUAL);
	else
		m_editor.SetRestraints(RESTRAINT_MARGIN);
}

void CUmlView::OnUpdateRestraints(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_editor.GetRestraints() == RESTRAINT_MARGIN);
}

void CUmlView::OnImport()
{
	m_editor.Import();
}

// Displays the Export dialog and exports the editor contents to the desired format.
void CUmlView::OnExport()
{
	// We save the current modified-state of 
	// the editor. The export mechanism 
	// modifies the data (although in a harmless 
	// way) and we want to reset the state 
	// after the operation - to avoid save 
	// questions when no conscious changes have 
	// been made.

	BOOL modified = m_editor.IsModified();

	CUmlDoc* doc = GetDocument();
	CExportDialog dlg;
	dlg.m_onlyh = m_onlyh;
	if (dlg.DoModal() == IDOK)
	{
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
			m_editor.ExportHTML(title);
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

void CUmlView::OnExportCpp()
{
	// We save the current modified-state of 
	// the editor. The export mechanism 
	// modifies the data (although in a harmless 
	// way) and we want to reset the state 
	// after the operation - to avoid save 
	// questions when no conscious changes have 
	// been made.

	BOOL modified = m_editor.IsModified();

	CUmlDoc* doc = GetDocument();
	CExportDialog dlg;
	dlg.m_format = 2;
	dlg.m_onlyh = m_onlyh;
	if (dlg.DoModal() == IDOK)
	{
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
			m_editor.ExportHTML(title);
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

void CUmlView::OnExportEmf()
{
	// We save the current modified-state of 
	// the editor. The export mechanism 
	// modifies the data (although in a harmless 
	// way) and we want to reset the state 
	// after the operation - to avoid save 
	// questions when no conscious changes have 
	// been made.

	BOOL modified = m_editor.IsModified();

	CUmlDoc* doc = GetDocument();
	CString title = doc->GetTitle();
	int found = title.ReverseFind(_TCHAR('.'));
	if (found != -1)
	{
		title = title.Left(found);
	}

	m_editor.ExportEMF(title);

	m_editor.SetModified(modified);
}

void CUmlView::OnExportHtml()
{
	// We save the current modified-state of 
	// the editor. The export mechanism 
	// modifies the data (although in a harmless 
	// way) and we want to reset the state 
	// after the operation - to avoid save 
	// questions when no conscious changes have 
	// been made.

	BOOL modified = m_editor.IsModified();

	CUmlDoc* doc = GetDocument();
	CString title = doc->GetTitle();
	int found = title.ReverseFind(_TCHAR('.'));
	if (found != -1)
	{
		title = title.Left(found);
	}

	m_editor.ExportHTML(title);

	m_editor.SetModified(modified);
}

void CUmlView::OnUpdateExport(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_editor.GetObjectCount() > 0);
}

void CUmlView::OnButtonTemplate()
{
	CUMLEntityClassTemplate* obj = new CUMLEntityClassTemplate;
	m_editor.StartDrawingObject(obj);
}

void CUmlView::OnSelectAll()
{
	m_editor.SelectAll();
}

// CUMLEditorDemoView update handlers

void CUmlView::OnUpdateEditUndo(CCmdUI* pCmdUI)
{
	m_editor.UpdateUndo(pCmdUI);
}

void CUmlView::OnUpdateEditCopy(CCmdUI* pCmdUI)
{
	m_editor.UpdateCopy(pCmdUI);
}

void CUmlView::OnUpdateEditCut(CCmdUI* pCmdUI)
{
	m_editor.UpdateCut(pCmdUI);
}

void CUmlView::OnUpdateEditPaste(CCmdUI* pCmdUI)
{
	m_editor.UpdatePaste(pCmdUI);
}

void CUmlView::OnUpdateFlipLink(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_editor.IsLinkSelected());
}

void CUmlView::OnUpdateSnap(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_editor.GetSnapToGrid());
}

void CUmlView::OnUpdateShowGrid(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_editor.IsGridVisible());
}

void CUmlView::OnUpdateMargins(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_editor.IsMarginVisible());
}

void CUmlView::OnMargins()
{
	m_editor.ShowMargin(!m_editor.IsMarginVisible());
}

void CUmlView::OnSettings()
{
	CDialogSettings	dlg;

	dlg.m_color = m_editor.GetBackgroundColor();
	dlg.m_width = m_editor.GetVirtualSize().cx;
	dlg.m_height = m_editor.GetVirtualSize().cy;
	dlg.m_gridWidth = m_editor.GetGridSize().cx;
	dlg.m_gridHeight = m_editor.GetGridSize().cy;
	m_editor.GetMargins(dlg.m_marginLeft, dlg.m_marginTop, dlg.m_marginRight, dlg.m_marginBottom);

	if (dlg.DoModal() == IDOK)
	{
		m_editor.SetBackgroundColor(dlg.m_color);
		m_editor.SetGridSize(CSize(dlg.m_gridWidth, dlg.m_gridHeight));
		m_editor.SetVirtualSize(CSize(dlg.m_width, dlg.m_height));
		m_editor.SetMargins(dlg.m_marginLeft, dlg.m_marginTop, dlg.m_marginRight, dlg.m_marginBottom);
		m_editor.RedrawWindow();
	}
}

void CUmlView::OnUpdateImport(CCmdUI* pCmdUI)
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

void CUmlView::OnUpdateEditDelete(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_editor.GetSelectCount() > 0);
}

void CUmlView::OnUpdateSelectAll(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_editor.GetObjectCount() > 0);
}

///////////////////////////////////////////////////////////
// Align menu
//

void CUmlView::OnLeftAlign()
{
	m_editor.LeftAlignSelected();
}
void CUmlView::OnTopAlign()
{
	m_editor.TopAlignSelected();
}
void CUmlView::OnRightAlign()
{
	m_editor.RightAlignSelected();
}
void CUmlView::OnBottomAlign()
{
	m_editor.BottomAlignSelected();
}

void CUmlView::OnUpdateLeftAlign(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_editor.GetSelectCount() > 1);
}
void CUmlView::OnUpdateTopAlign(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_editor.GetSelectCount() > 1);
}
void CUmlView::OnUpdateRightAlign(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_editor.GetSelectCount() > 1);
}
void CUmlView::OnUpdateBottomAlign(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_editor.GetSelectCount() > 1);
}

void CUmlView::OnMakeSameSize()
{
	m_editor.MakeSameSizeSelected();
}
void CUmlView::OnUpdateMakeSameSize(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_editor.GetSelectCount() > 1);
}

void CUmlView::OnUp()
{
	m_editor.Up();
}
void CUmlView::OnDown()
{
	m_editor.Down();
}
void CUmlView::OnFront()
{
	m_editor.Front();
}
void CUmlView::OnBottom()
{
	m_editor.Bottom();
}

void CUmlView::OnUpdateUp(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_editor.GetSelectCount() == 1);
}
void CUmlView::OnUpdateDown(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_editor.GetSelectCount() == 1);
}
void CUmlView::OnUpdateFront(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_editor.GetSelectCount() == 1);
}
void CUmlView::OnUpdateBottom(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_editor.GetSelectCount() == 1);
}
