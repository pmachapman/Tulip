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

// NetView.cpp : implementation of the CNetView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Tulip.h"
#endif

#include "MainFrm.h"
#include "NetDoc.h"
#include "NetView.h"

#include "DialogEditor/DialogSettings.h"

#include "NetworkEditor/NetworkSymbol.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CNetView

IMPLEMENT_DYNCREATE(CNetView, CView)

BEGIN_MESSAGE_MAP(CNetView, CView)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
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
	ON_COMMAND(ID_EXPORT_EMF, OnExport)
	ON_UPDATE_COMMAND_UI(ID_EXPORT_EMF, OnUpdateExport)
	ON_COMMAND(ID_BUTTON_LINK, OnButtonLink)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_LINK, OnUpdateLink)
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
	ON_COMMAND(ID_EDIT_DELETE, OnEditDelete)
	ON_UPDATE_COMMAND_UI(ID_EDIT_DELETE, OnUpdateEditDelete)
    // Custom to the document type
    ON_COMMAND(ID_NET_BUTTON_ADD_CLIENT, OnButtonAddClient)
    ON_COMMAND(ID_NET_BUTTON_ADD_HUB, OnButtonAddHub)
    ON_COMMAND(ID_NET_BUTTON_ADD_INTERNET, OnButtonAddInternet)
    ON_COMMAND(ID_NET_BUTTON_ADD_MODEM, OnButtonAddModem)
    ON_COMMAND(ID_NET_BUTTON_ADD_PRINTER, OnButtonAddPrinter)
    ON_COMMAND(ID_NET_BUTTON_ADD_SERVER, OnButtonAddServer)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CNetView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CNetView construction/destruction

CNetView::CNetView()
{
	m_screenResolutionX = 0;
	m_onlyh = FALSE;
}

CNetView::~CNetView()
{
}

BOOL CNetView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}

// CNetView drawing

void CNetView::OnDraw(CDC* pDC)
{
	CNetDoc* pDoc = GetDocument();
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

void CNetView::OnDrawIconicThumbnailOrLivePreview(CDC& dc, CRect rect, CSize szRequiredThumbnailSize, BOOL bIsThumbnail, BOOL& bAlphaChannelSet)
{
	m_editor.DrawPreview(&dc, rect);
}


// CNetView printing


void CNetView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CNetView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CNetView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

void CNetView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

// CNetView diagnostics

#ifdef _DEBUG
// Asserts the object validity.
void CNetView::AssertValid() const
{
	CView::AssertValid();
}

// Dumps the object state
void CNetView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

//Gets a casted pointer to the attached document.
CNetDoc* CNetView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CNetDoc)));
	return (CNetDoc*)m_pDocument;
}
#endif //_DEBUG

// Called when a view is created.
void CNetView::OnInitialUpdate()
{

	CView::OnInitialUpdate();

	if (!m_editor.m_hWnd)
	{
		// Creating the editor window
		CNetDoc* pDoc = GetDocument();

		CRect rect;
		GetClientRect(rect);
		pDoc->GetData()->SetClipboardHandler(&theApp.m_netClip);
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

			// Only set the size if one wasn't pre-specified
			if (pDoc->GetData()->GetVirtualSize() == CSize(0, 0))
			{
				m_editor.SetVirtualSize(CSize(round(static_cast<double>(horzSize) / zoom), round(static_cast<double>(vertSize) / zoom)));
			}
			else
			{
				m_editor.SetVirtualSize(pDoc->GetData()->GetVirtualSize());
			}

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
			if (pDoc->GetData()->GetVirtualSize() == CSize(0, 0))
			{
				m_editor.SetVirtualSize(CSize(8 * m_screenResolutionX, 11 * m_screenResolutionX));
			}
			else
			{
				m_editor.SetVirtualSize(pDoc->GetData()->GetVirtualSize());
			}
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
void CNetView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	CMainFrame* pMainFrame = ((CMainFrame*)GetTopLevelFrame());
	ASSERT_VALID(pMainFrame);
	pMainFrame->SetRibbonContextCategory(ID_NETCONTEXT, bActivate);
}

// CNetView message handlers

void CNetView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	if (m_editor.m_hWnd)
	{
		m_editor.MoveWindow(0, 0, cx, cy);
	}
}

BOOL CNetView::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}

// CNetView button and menu handlers

void CNetView::OnEditUndo()
{
	m_editor.Undo();
}

void CNetView::OnEditCopy()
{
	m_editor.Copy();
}

void CNetView::OnEditCut()
{
	m_editor.Cut();
}

void CNetView::OnEditPaste()
{
	m_editor.Paste();
}

void CNetView::OnEditDelete()
{
	m_editor.DeleteAllSelected();
}

void CNetView::OnProperty()
{
	if (m_editor.GetSelectCount() == 1)
	{
		m_editor.ShowProperties();
	}
}

void CNetView::OnZoom()
{
}

void CNetView::OnZoom100()
{
	m_editor.SetZoom(1);
}

void CNetView::OnZoom150()
{
	m_editor.SetZoom(1.5);
}

void CNetView::OnZoom200()
{
	m_editor.SetZoom(2);
}

void CNetView::OnZoom25()
{
	m_editor.SetZoom(.25);
}

void CNetView::OnZoom50()
{
	m_editor.SetZoom(.5);
}

void CNetView::OnSnap()
{
	m_editor.SetSnapToGrid(!m_editor.GetSnapToGrid());
}

void CNetView::OnShowGrid()
{
	m_editor.ShowGrid(!m_editor.IsGridVisible());
}

void CNetView::OnMargins()
{
	m_editor.ShowMargin(!m_editor.IsMarginVisible());
}

void CNetView::OnRestraints()
{
	if (m_editor.GetRestraints() == RESTRAINT_MARGIN)
		m_editor.SetRestraints(RESTRAINT_VIRTUAL);
	else
		m_editor.SetRestraints(RESTRAINT_MARGIN);
}

void CNetView::OnButtonLink()
{
	if (m_editor.IsLinked())
	{
		m_editor.OnUnlink();
	}
	else
	{
		m_editor.OnLink();
	}
}

void CNetView::OnButtonAddClient()
{
	CNetworkSymbol*	obj = new CNetworkSymbol(IDB_NET_BITMAP_SYMBOL_CLIENT);
	m_editor.StartDrawingObject(obj);
}

void CNetView::OnButtonAddHub()
{
	CNetworkSymbol*	obj = new CNetworkSymbol(IDB_NET_BITMAP_SYMBOL_HUB);
	m_editor.StartDrawingObject(obj);
}

void CNetView::OnButtonAddInternet()
{
	CNetworkSymbol*	obj = new CNetworkSymbol(IDB_NET_BITMAP_SYMBOL_INTERNET);
	m_editor.StartDrawingObject(obj);
}

void CNetView::OnButtonAddModem()
{
	CNetworkSymbol*	obj = new CNetworkSymbol(IDB_NET_BITMAP_SYMBOL_ADSL);
	m_editor.StartDrawingObject(obj);
}

void CNetView::OnButtonAddPrinter()
{
	CNetworkSymbol*	obj = new CNetworkSymbol(IDB_NET_BITMAP_SYMBOL_PRINTER);
	m_editor.StartDrawingObject(obj);
}

void CNetView::OnButtonAddServer()
{
	CNetworkSymbol*	obj = new CNetworkSymbol(IDB_NET_BITMAP_SYMBOL_SERVER);
	m_editor.StartDrawingObject(obj);
}

// Displays the Export dialog and exports the editor contents to the desired format.
void CNetView::OnExport()
{
	// We save the current modified-state of 
	// the editor. The export mechanism 
	// modifies the data (although in a harmless 
	// way) and we want to reset the state 
	// after the operation - to avoid save 
	// questions when no conscious changes have 
	// been made.

	BOOL modified = m_editor.IsModified();
	CNetDoc* doc = GetDocument();
	CWaitCursor wait;
	CString title = doc->GetTitle();
	int found = title.ReverseFind(_TCHAR('.'));
	if (found != -1)
	{
		title = title.Left(found);
	}
	m_editor.ExportEMF(title);
	m_editor.SetModified(modified);
}

void CNetView::OnUpdateExport(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_editor.GetObjectCount() > 0);
}

void CNetView::OnSettings()
{
	CDialogSettings	dlg;

	dlg.m_width = m_editor.GetVirtualSize().cx;
	dlg.m_height = m_editor.GetVirtualSize().cy;
	dlg.m_gridWidth = m_editor.GetGridSize().cx;
	dlg.m_gridHeight = m_editor.GetGridSize().cy;
	m_editor.GetMargins(dlg.m_marginLeft, dlg.m_marginTop, dlg.m_marginRight, dlg.m_marginBottom);

	if (dlg.DoModal() == IDOK)
	{
		m_editor.SetGridSize(CSize(dlg.m_gridWidth, dlg.m_gridHeight));
		m_editor.SetVirtualSize(CSize(dlg.m_width, dlg.m_height));
		m_editor.SetMargins(dlg.m_marginLeft, dlg.m_marginTop, dlg.m_marginRight, dlg.m_marginBottom);
		m_editor.RedrawWindow();
	}
}

void CNetView::OnSelectAll()
{
	m_editor.SelectAll();
}

// CNetEditorDemoView update handlers

void CNetView::OnUpdateEditUndo(CCmdUI* pCmdUI)
{
	m_editor.UpdateUndo(pCmdUI);
}

void CNetView::OnUpdateEditCopy(CCmdUI* pCmdUI)
{
	m_editor.UpdateCopy(pCmdUI);
}

void CNetView::OnUpdateEditCut(CCmdUI* pCmdUI)
{
	m_editor.UpdateCut(pCmdUI);
}

void CNetView::OnUpdateEditPaste(CCmdUI* pCmdUI)
{
	m_editor.UpdatePaste(pCmdUI);
}

void CNetView::OnUpdateSnap(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_editor.GetSnapToGrid());
}

void CNetView::OnUpdateShowGrid(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_editor.IsGridVisible());
}

void CNetView::OnUpdateMargins(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_editor.IsMarginVisible());
}

void CNetView::OnUpdateRestraints(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_editor.GetRestraints() == RESTRAINT_MARGIN);
}

void CNetView::OnUpdateEditDelete(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_editor.GetSelectCount() > 0);
}

void CNetView::OnUpdateLink(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_editor.GetSelectCount() == 2);
	if (m_editor.IsLinked())
	{
		pCmdUI->SetText(_T("Unlink"));
	}
	else
	{
		pCmdUI->SetText(_T("Link"));
	}
}

void CNetView::OnUpdateProperty(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_editor.GetSelectCount() == 1);
}

void CNetView::OnUpdateSelectAll(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_editor.GetObjectCount() > 0);
}

///////////////////////////////////////////////////////////
// Align menu
//

void CNetView::OnLeftAlign()
{
	m_editor.LeftAlignSelected();
}
void CNetView::OnTopAlign()
{
	m_editor.TopAlignSelected();
}
void CNetView::OnRightAlign()
{
	m_editor.RightAlignSelected();
}
void CNetView::OnBottomAlign()
{
	m_editor.BottomAlignSelected();
}

void CNetView::OnUpdateLeftAlign(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_editor.GetSelectCount() > 1);
}
void CNetView::OnUpdateTopAlign(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_editor.GetSelectCount() > 1);
}
void CNetView::OnUpdateRightAlign(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_editor.GetSelectCount() > 1);
}
void CNetView::OnUpdateBottomAlign(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_editor.GetSelectCount() > 1);
}

void CNetView::OnMakeSameSize()
{
	m_editor.MakeSameSizeSelected();
}
void CNetView::OnUpdateMakeSameSize(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_editor.GetSelectCount() > 1);
}

void CNetView::OnUp()
{
	m_editor.Up();
}
void CNetView::OnDown()
{
	m_editor.Down();
}
void CNetView::OnFront()
{
	m_editor.Front();
}
void CNetView::OnBottom()
{
	m_editor.Bottom();
}

void CNetView::OnUpdateUp(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_editor.GetSelectCount() == 1);
}
void CNetView::OnUpdateDown(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_editor.GetSelectCount() == 1);
}
void CNetView::OnUpdateFront(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_editor.GetSelectCount() == 1);
}
void CNetView::OnUpdateBottom(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_editor.GetSelectCount() == 1);
}
