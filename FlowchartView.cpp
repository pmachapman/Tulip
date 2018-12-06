// FlowchartView.cpp : implementation of the CFlowchartView class
//

#include "stdafx.h"
#include "Tulip.h"

#include "MainFrm.h"
#include "FlowchartDoc.h"
#include "FlowchartView.h"

#include "DialogEditor/DialogSettings.h"

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
	ON_COMMAND(ID_EDIT_SELECT_ALL, OnSelectAll)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SELECT_ALL, OnUpdateSelectAll)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_COMMAND(ID_EXPORT, OnExport)
	ON_UPDATE_COMMAND_UI(ID_EXPORT, OnUpdateExport)
	ON_COMMAND(ID_EXPORT_EMF, OnExport)
	ON_UPDATE_COMMAND_UI(ID_EXPORT_EMF, OnUpdateExport)
	ON_COMMAND(ID_FLOWCHART_BUTTON_ARROW, OnButtonArrow)
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
	ON_COMMAND(ID_FLOWCHART_BUTTON_LABEL, OnButtonLabel)
	ON_COMMAND(ID_BUTTON_LINK, OnButtonLink)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_LINK, OnUpdateButtonLink)
	ON_COMMAND(ID_FLOWCHART_LINK_LABEL, OnLinkLabel)
	ON_UPDATE_COMMAND_UI(ID_FLOWCHART_LINK_LABEL, OnUpdateLinkLabel)
	ON_COMMAND(ID_FLOWCHART_FLIP_LINK, OnFlipLink)
	ON_UPDATE_COMMAND_UI(ID_FLOWCHART_FLIP_LINK, OnUpdateFlipLink)
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

void CFlowchartView::OnDrawIconicThumbnailOrLivePreview(CDC& dc, CRect rect, CSize szRequiredThumbnailSize, BOOL bIsThumbnail, BOOL& bAlphaChannelSet)
{
	m_editor.DrawPreview(&dc, rect);
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

CFlowchartDoc* CFlowchartView::GetDocument() const // non-debug version is inline
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

void CFlowchartView::OnMargins()
{
	m_editor.ShowMargin(!m_editor.IsMarginVisible());
}

void CFlowchartView::OnRestraints()
{
	if (m_editor.GetRestraints() == RESTRAINT_MARGIN)
		m_editor.SetRestraints(RESTRAINT_VIRTUAL);
	else
		m_editor.SetRestraints(RESTRAINT_MARGIN);
}

void CFlowchartView::OnSettings()
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

void CFlowchartView::OnUpdateExport(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_editor.GetObjectCount() > 0);
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

void CFlowchartView::OnSelectAll()
{
	m_editor.SelectAll();
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

void CFlowchartView::OnUpdateMargins(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_editor.IsMarginVisible());
}

void CFlowchartView::OnUpdateRestraints(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_editor.GetRestraints() == RESTRAINT_MARGIN);
}

void CFlowchartView::OnUpdateSelectAll(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_editor.GetObjectCount() > 0);
}

///////////////////////////////////////////////////////////
// Align menu
//

void CFlowchartView::OnLeftAlign()
{
	m_editor.LeftAlignSelected();
}
void CFlowchartView::OnTopAlign()
{
	m_editor.TopAlignSelected();
}
void CFlowchartView::OnRightAlign()
{
	m_editor.RightAlignSelected();
}
void CFlowchartView::OnBottomAlign()
{
	m_editor.BottomAlignSelected();
}

void CFlowchartView::OnUpdateLeftAlign(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_editor.GetSelectCount() > 1);
}
void CFlowchartView::OnUpdateTopAlign(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_editor.GetSelectCount() > 1);
}
void CFlowchartView::OnUpdateRightAlign(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_editor.GetSelectCount() > 1);
}
void CFlowchartView::OnUpdateBottomAlign(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_editor.GetSelectCount() > 1);
}

void CFlowchartView::OnMakeSameSize()
{
	m_editor.MakeSameSizeSelected();
}
void CFlowchartView::OnUpdateMakeSameSize(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_editor.GetSelectCount() > 1);
}

void CFlowchartView::OnUp()
{
	m_editor.Up();
}
void CFlowchartView::OnDown()
{
	m_editor.Down();
}
void CFlowchartView::OnFront()
{
	m_editor.Front();
}
void CFlowchartView::OnBottom()
{
	m_editor.Bottom();
}

void CFlowchartView::OnUpdateUp(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_editor.GetSelectCount() == 1);
}
void CFlowchartView::OnUpdateDown(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_editor.GetSelectCount() == 1);
}
void CFlowchartView::OnUpdateFront(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_editor.GetSelectCount() == 1);
}
void CFlowchartView::OnUpdateBottom(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_editor.GetSelectCount() == 1);
}
