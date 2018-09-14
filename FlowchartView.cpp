// FlowchartView.cpp : implementation of the CFlowchartView class
//

#include "stdafx.h"
#include "Tulip.h"

#include "MainFrm.h"
#include "FlowchartDoc.h"
#include "FlowchartView.h"

#include "FlowchartEditor/FlowchartEntityTerminator.h"
#include "FlowchartEditor/FlowchartEntityBox.h"
#include "FlowchartEditor/FlowchartEntityCondition.h"
#include "FlowchartEditor/FlowchartEntityConnector.h"
#include "FlowchartEditor/FlowchartEntityIO.h"
#include "FlowchartEditor/FlowchartLineSegment.h"
#include "FlowchartEditor/FlowchartLinkableLineSegment.h"
#include "FlowchartEditor/FlowchartLabel.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFlowchartView

IMPLEMENT_DYNCREATE(CFlowchartView, CView)

BEGIN_MESSAGE_MAP(CFlowchartView, CView)
	//{{AFX_MSG_MAP(CFlowchartView)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_FLOWCHART_BUTTON_BOX, OnButtonBox)
	ON_COMMAND(ID_FLOWCHART_BUTTON_CONDITION, OnButtonCondition)
	ON_COMMAND(ID_FLOWCHART_BUTTON_CONNECTOR, OnButtonConnector)
	ON_COMMAND(ID_FLOWCHART_BUTTON_IO, OnButtonIo)
	ON_COMMAND(ID_FLOWCHART_BUTTON_LINE, OnButtonLine)
	ON_COMMAND(ID_FLOWCHART_BUTTON_START, OnButtonStart)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_COMMAND(ID_EXPORT, OnExport)
	ON_COMMAND(ID_FLOWCHART_BUTTON_ARROW, OnButtonArrow)
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
	ON_COMMAND(ID_FLOWCHART_BUTTON_LABEL, OnButtonLabel)
	ON_COMMAND(ID_BUTTON_LINK, OnButtonLink)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_LINK, OnUpdateButtonLink)
	ON_COMMAND(ID_FLOWCHART_LINK_LABEL, OnLinkLabel)
	ON_UPDATE_COMMAND_UI(ID_FLOWCHART_LINK_LABEL, OnUpdateLinkLabel)
	ON_COMMAND(ID_FLOWCHART_FLIP_LINK, OnFlipLink)
	ON_UPDATE_COMMAND_UI(ID_FLOWCHART_FLIP_LINK, OnUpdateFlipLink)
	ON_COMMAND(ID_PROPERTY, OnProperty)
	ON_UPDATE_COMMAND_UI(ID_PROPERTY, OnUpdateProperty)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFlowchartView construction/destruction

CFlowchartView::CFlowchartView()
{

	m_screenResolutionX = 0;

}

CFlowchartView::~CFlowchartView()
{
}

BOOL CFlowchartView::PreCreateWindow(CREATESTRUCT& cs)
{

	return CView::PreCreateWindow(cs);

}

/////////////////////////////////////////////////////////////////////////////
// CFlowchartView drawing

void CFlowchartView::OnDraw(CDC* pDC)
{

	CFlowchartDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (pDC->IsPrinting())
	{

		COLORREF col = m_editor.GetBackgroundColor();
		// Print zoom is the difference between screen- 
		// and printer resolution.
		double zoom = pDC->GetDeviceCaps(LOGPIXELSX) / m_screenResolutionX;

		CRect rect(0, 0,
			(int)((double)m_editor.GetVirtualSize().cx * zoom),
			(int)((double)m_editor.GetVirtualSize().cy * zoom));
		m_editor.SetRedraw(FALSE);
		m_editor.SetBackgroundColor(RGB(255, 255, 255));
		m_editor.Print(pDC, rect, zoom);
		m_editor.SetBackgroundColor(col);
		m_editor.SetRedraw(TRUE);

	}

}

/////////////////////////////////////////////////////////////////////////////
// CFlowchartView printing

BOOL CFlowchartView::OnPreparePrinting(CPrintInfo* pInfo)
{
	return DoPreparePrinting(pInfo);
}

void CFlowchartView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

void CFlowchartView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

/////////////////////////////////////////////////////////////////////////////
// CFlowchartView diagnostics

#ifdef _DEBUG
void CFlowchartView::AssertValid() const
{
	CView::AssertValid();
}

void CFlowchartView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CFlowchartDoc* CFlowchartView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFlowchartDoc)));
	return (CFlowchartDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFlowchartView message handlers

void CFlowchartView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	if (!m_editor.m_hWnd)
	{
		// Creating the editor window
		CFlowchartDoc* pDoc = GetDocument();

		CRect rect;
		GetClientRect(rect);
		pDoc->GetData()->SetClipboardHandler(&theApp.m_flowchartClip);
		m_editor.Create(WS_CHILD | WS_VISIBLE, rect, this, pDoc->GetData());

		// We get the screen resolution, which we will use 
		// for scaling to printer. See also OnDraw.
		CClientDC dc(this);
		m_screenResolutionX = dc.GetDeviceCaps(LOGPIXELSX);
		int horzSize = round((double)dc.GetDeviceCaps(HORZSIZE) / (double)m_screenResolutionX);
		int vertSize = round((double)dc.GetDeviceCaps(VERTSIZE) / (double)m_screenResolutionX);

		m_editor.SetVirtualSize(CSize(8 * m_screenResolutionX, 11 * m_screenResolutionX));

		m_editor.SetModified(FALSE);

	}
	else
		m_editor.Clear();
}

// Called the the view is activated
void CFlowchartView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	CMainFrame* pMainFrame = ((CMainFrame*)GetTopLevelFrame());
	ASSERT_VALID(pMainFrame);
	pMainFrame->SetRibbonContextCategory(ID_FLOWCHARTCONTEXT, bActivate);
}

void CFlowchartView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	if (m_editor.m_hWnd)
		m_editor.MoveWindow(0, 0, cx, cy);
}

BOOL CFlowchartView::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CFlowchartView command handlers


// Toolbar objects
void CFlowchartView::OnButtonBox()
{
	m_editor.StartDrawingObject(new CFlowchartEntityBox);
}

void CFlowchartView::OnButtonCondition()
{
	m_editor.StartDrawingObject(new CFlowchartEntityCondition);
}

void CFlowchartView::OnButtonConnector()
{
	m_editor.StartDrawingObject(new CFlowchartEntityConnector);
}

void CFlowchartView::OnButtonIo()
{
	m_editor.StartDrawingObject(new CFlowchartEntityIO);
}

void CFlowchartView::OnButtonLine()
{
	m_editor.StartDrawingObject(new CFlowchartLineSegment);
}

void CFlowchartView::OnButtonStart()
{
	m_editor.StartDrawingObject(new CFlowchartEntityTerminator);
}

void CFlowchartView::OnButtonArrow()
{
	m_editor.StartDrawingObject(new CFlowchartLinkableLineSegment);
}

void CFlowchartView::OnButtonLabel()
{
	m_editor.StartDrawingObject(new CFlowchartLabel);
}

void CFlowchartView::OnZoom()
{
}

void CFlowchartView::OnZoom100()
{
	m_editor.SetZoom(1);
}

void CFlowchartView::OnZoom150()
{
	m_editor.SetZoom(1.5);
}

void CFlowchartView::OnZoom200()
{
	m_editor.SetZoom(2);
}

void CFlowchartView::OnZoom25()
{
	m_editor.SetZoom(.25);
}

void CFlowchartView::OnZoom50()
{
	m_editor.SetZoom(.5);
}

void CFlowchartView::OnSnap()
{
	m_editor.SetSnapToGrid(!m_editor.GetSnapToGrid());
}

void CFlowchartView::OnShowGrid()
{
	m_editor.ShowGrid(!m_editor.IsGridVisible());
}

// Edit menu
void CFlowchartView::OnEditCopy()
{
	m_editor.Copy();
}

void CFlowchartView::OnEditCut()
{
	m_editor.Cut();
}

void CFlowchartView::OnEditPaste()
{
	m_editor.Paste();
}

void CFlowchartView::OnEditUndo()
{
	m_editor.Undo();
}

void CFlowchartView::OnProperty()
{
	if (m_editor.IsLinked())
	{
		m_editor.OnLinkProperties();
	}
	else
	{
		m_editor.ShowProperties();
	}
}

// File menu
void CFlowchartView::OnExport()
{
	// We save the current modified-state of 
	// the editor. The export mechanism 
	// modifies the data (although in a harmless 
	// way) and we want to reset the state 
	// after the operation - to avoid save 
	// questions when no conscious changes have 
	// been made.

	BOOL modified = m_editor.IsModified();
	CFlowchartDoc* doc = GetDocument();
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

// Links menu
void CFlowchartView::OnButtonLink()
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

void CFlowchartView::OnLinkLabel()
{
	m_editor.OnLinkProperties();
}

void CFlowchartView::OnFlipLink()
{
	m_editor.OnLinkDirection();
}

/////////////////////////////////////////////////////////////////////////////
// CFlowchartView cammand enablers

// Edit menu
void CFlowchartView::OnUpdateEditUndo(CCmdUI* pCmdUI)
{
	m_editor.UpdateUndo(pCmdUI);
}

void CFlowchartView::OnUpdateEditCopy(CCmdUI* pCmdUI)
{
	m_editor.UpdateCopy(pCmdUI);
}

void CFlowchartView::OnUpdateEditCut(CCmdUI* pCmdUI)
{
	m_editor.UpdateCut(pCmdUI);
}

void CFlowchartView::OnUpdateEditPaste(CCmdUI* pCmdUI)
{
	m_editor.UpdatePaste(pCmdUI);
}

void CFlowchartView::OnUpdateProperty(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_editor.GetSelectCount() == 1 || m_editor.IsLinked());
}

// Links menu
void CFlowchartView::OnUpdateButtonLink(CCmdUI* pCmdUI)
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

void CFlowchartView::OnUpdateLinkLabel(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_editor.IsLinked());
}

void CFlowchartView::OnUpdateFlipLink(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_editor.IsLinked());
}

void CFlowchartView::OnUpdateSnap(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_editor.GetSnapToGrid());
}

void CFlowchartView::OnUpdateShowGrid(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_editor.IsGridVisible());
}
