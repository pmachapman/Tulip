// FlowchartView.cpp : implementation of the CFlowchartView class
//

#include "stdafx.h"
#include "Tulip.h"

#include "MainFrm.h"
#include "DiagramView.h"
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

IMPLEMENT_DYNCREATE(CFlowchartView, CDiagramView)

BEGIN_MESSAGE_MAP(CFlowchartView, CDiagramView)
	//{{AFX_MSG_MAP(CFlowchartView)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_FLOWCHART_BUTTON_ARROW, OnButtonArrow)
	ON_UPDATE_COMMAND_UI(ID_FLOWCHART_BUTTON_ARROW, OnUpdateButtonArrow)
	ON_COMMAND(ID_FLOWCHART_BUTTON_BOX, OnButtonBox)
	ON_UPDATE_COMMAND_UI(ID_FLOWCHART_BUTTON_BOX, OnUpdateButtonBox)
	ON_COMMAND(ID_FLOWCHART_BUTTON_CONDITION, OnButtonCondition)
	ON_UPDATE_COMMAND_UI(ID_FLOWCHART_BUTTON_CONDITION, OnUpdateButtonCondition)
	ON_COMMAND(ID_FLOWCHART_BUTTON_CONNECTOR, OnButtonConnector)
	ON_UPDATE_COMMAND_UI(ID_FLOWCHART_BUTTON_CONNECTOR, OnUpdateButtonConnector)
	ON_COMMAND(ID_FLOWCHART_BUTTON_IO, OnButtonIo)
	ON_UPDATE_COMMAND_UI(ID_FLOWCHART_BUTTON_IO, OnUpdateButtonIo)
	ON_COMMAND(ID_FLOWCHART_BUTTON_LABEL, OnButtonLabel)
	ON_UPDATE_COMMAND_UI(ID_FLOWCHART_BUTTON_LABEL, OnUpdateButtonLabel)
	ON_COMMAND(ID_FLOWCHART_BUTTON_LINE, OnButtonLine)
	ON_UPDATE_COMMAND_UI(ID_FLOWCHART_BUTTON_LINE, OnUpdateButtonLine)
	ON_COMMAND(ID_FLOWCHART_BUTTON_START, OnButtonStart)
	ON_UPDATE_COMMAND_UI(ID_FLOWCHART_BUTTON_START, OnUpdateButtonStart)
	ON_COMMAND(ID_EXPORT, OnExport)
	ON_UPDATE_COMMAND_UI(ID_EXPORT, OnUpdateExport)
	ON_COMMAND(ID_EXPORT_EMF, OnExport)
	ON_UPDATE_COMMAND_UI(ID_EXPORT_EMF, OnUpdateExport)
	ON_COMMAND(ID_BUTTON_LINK, OnButtonLink)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_LINK, OnUpdateButtonLink)
	ON_COMMAND(ID_FLOWCHART_LINK_LABEL, OnLinkLabel)
	ON_UPDATE_COMMAND_UI(ID_FLOWCHART_LINK_LABEL, OnUpdateLinkLabel)
	ON_COMMAND(ID_FLOWCHART_FLIP_LINK, OnFlipLink)
	ON_UPDATE_COMMAND_UI(ID_FLOWCHART_FLIP_LINK, OnUpdateFlipLink)
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
	m_editor.Draw(&dc, rect);
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

		// Pass the editor to the base class
		SetEditor(&m_editor);

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
		m_editor.Clear();
}

// Called the the view is activated
void CFlowchartView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	CMainFrame* pMainFrame = ((CMainFrame*)GetTopLevelFrame());
	ASSERT_VALID(pMainFrame);
	pMainFrame->SetRibbonContextCategory(ID_FLOWCHARTCONTEXT, bActivate);
}

/////////////////////////////////////////////////////////////////////////////
// CFlowchartView command handlers


// Toolbar objects
void CFlowchartView::OnButtonBox()
{
	m_editor.StartDrawingObject(new CFlowchartEntityBox);
	if (m_editor.IsDrawing() && m_drawObject == DRAW_OBJECT_BOX)
	{
		m_editor.StartDrawingObject(NULL);
		m_drawObject = DRAW_OBJECT_NONE;
	}
	else
	{
		m_editor.StartDrawingObject(new CFlowchartEntityBox);
		m_drawObject = DRAW_OBJECT_BOX;
	}
}

void CFlowchartView::OnButtonCondition()
{
	m_editor.StartDrawingObject(new CFlowchartEntityCondition);
	if (m_editor.IsDrawing() && m_drawObject == DRAW_OBJECT_CONDITION)
	{
		m_editor.StartDrawingObject(NULL);
		m_drawObject = DRAW_OBJECT_NONE;
	}
	else
	{
		m_editor.StartDrawingObject(new CFlowchartEntityCondition);
		m_drawObject = DRAW_OBJECT_CONDITION;
	}
}

void CFlowchartView::OnButtonConnector()
{
	if (m_editor.IsDrawing() && m_drawObject == DRAW_OBJECT_CONNECTOR)
	{
		m_editor.StartDrawingObject(NULL);
		m_drawObject = DRAW_OBJECT_NONE;
	}
	else
	{
		m_editor.StartDrawingObject(new CFlowchartEntityConnector);
		m_drawObject = DRAW_OBJECT_CONNECTOR;
	}
}

void CFlowchartView::OnButtonIo()
{
	if (m_editor.IsDrawing() && m_drawObject == DRAW_OBJECT_IO)
	{
		m_editor.StartDrawingObject(NULL);
		m_drawObject = DRAW_OBJECT_NONE;
	}
	else
	{
		m_editor.StartDrawingObject(new CFlowchartEntityIO);
		m_drawObject = DRAW_OBJECT_IO;
	}
}

void CFlowchartView::OnButtonLine()
{
	if (m_editor.IsDrawing() && m_drawObject == DRAW_OBJECT_LINE)
	{
		m_editor.StartDrawingObject(NULL);
		m_drawObject = DRAW_OBJECT_NONE;
	}
	else
	{
		m_editor.StartDrawingObject(new CFlowchartLineSegment);
		m_drawObject = DRAW_OBJECT_LINE;
	}
}

void CFlowchartView::OnButtonStart()
{
	if (m_editor.IsDrawing() && m_drawObject == DRAW_OBJECT_START)
	{
		m_editor.StartDrawingObject(NULL);
		m_drawObject = DRAW_OBJECT_NONE;
	}
	else
	{
		m_editor.StartDrawingObject(new CFlowchartEntityTerminator);
		m_drawObject = DRAW_OBJECT_START;
	}
}

void CFlowchartView::OnButtonArrow()
{
	if (m_editor.IsDrawing() && m_drawObject == DRAW_OBJECT_ARROW)
	{
		m_editor.StartDrawingObject(NULL);
		m_drawObject = DRAW_OBJECT_NONE;
	}
	else
	{
		m_editor.StartDrawingObject(new CFlowchartLinkableLineSegment);
		m_drawObject = DRAW_OBJECT_ARROW;
	}
}

void CFlowchartView::OnButtonLabel()
{
	if (m_editor.IsDrawing() && m_drawObject == DRAW_OBJECT_LABEL)
	{
		m_editor.StartDrawingObject(NULL);
		m_drawObject = DRAW_OBJECT_NONE;
	}
	else
	{
		m_editor.StartDrawingObject(new CFlowchartLabel);
		m_drawObject = DRAW_OBJECT_LABEL;
	}
}

void CFlowchartView::OnUpdateButtonBox(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_editor.IsDrawing() && m_drawObject == DRAW_OBJECT_BOX);
}

void CFlowchartView::OnUpdateButtonCondition(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_editor.IsDrawing() && m_drawObject == DRAW_OBJECT_CONDITION);
}

void CFlowchartView::OnUpdateButtonConnector(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_editor.IsDrawing() && m_drawObject == DRAW_OBJECT_CONNECTOR);
}

void CFlowchartView::OnUpdateButtonIo(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_editor.IsDrawing() && m_drawObject == DRAW_OBJECT_IO);
}

void CFlowchartView::OnUpdateButtonLine(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_editor.IsDrawing() && m_drawObject == DRAW_OBJECT_LINE);
}

void CFlowchartView::OnUpdateButtonStart(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_editor.IsDrawing() && m_drawObject == DRAW_OBJECT_START);
}

void CFlowchartView::OnUpdateButtonArrow(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_editor.IsDrawing() && m_drawObject == DRAW_OBJECT_ARROW);
}

void CFlowchartView::OnUpdateButtonLabel(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_editor.IsDrawing() && m_drawObject == DRAW_OBJECT_LABEL);
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

/////////////////////////////////////////////////////////////////////////////
// CFlowchartView command enablers

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