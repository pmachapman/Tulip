// DialogEditorView.cpp : implementation of the CDialogEditorView class
//

#include "stdafx.h"
#include "Tulip.h"

#include "MainFrm.h"
#include "DialogEditorDoc.h"
#include "DialogEditorView.h"

// --- DiagramEditor ---
#include "DialogEditor/StdGrfx.h"
#include "DialogEditor/DialogSettings.h"

#include "DialogEditor/DiagramButton.h"
#include "DialogEditor/DiagramEdit.h"
#include "DialogEditor/DiagramStatic.h"
#include "DialogEditor/DiagramCheckbox.h"
#include "DialogEditor/DiagramRadiobutton.h"
#include "DialogEditor/DiagramCombobox.h"
#include "DialogEditor/DiagramListbox.h"
#include "DialogEditor/DiagramGroupbox.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogEditorView

IMPLEMENT_DYNCREATE(CDialogEditorView, CView)

BEGIN_MESSAGE_MAP(CDialogEditorView, CView)
	//{{AFX_MSG_MAP(CDialogEditorView)
	ON_WM_SIZE()
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
	ON_COMMAND(ID_PROPERTY, OnProperty)
	ON_UPDATE_COMMAND_UI(ID_PROPERTY, OnUpdateProperty)
	ON_COMMAND(ID_ZOOM, OnZoom)
	ON_COMMAND(ID_ZOOM_100, OnZoom100)
	ON_COMMAND(ID_ZOOM_150, OnZoom150)
	ON_COMMAND(ID_ZOOM_200, OnZoom200)
	ON_COMMAND(ID_ZOOM_25, OnZoom25)
	ON_COMMAND(ID_ZOOM_50, OnZoom50)
	ON_COMMAND(IDC_SETTINGS, OnSettings)
	ON_COMMAND(ID_DIALOG_ADD_BUTTON, OnAddButton)
	ON_COMMAND(ID_DIALOG_ADD_EDIT, OnAddEdit)
	ON_COMMAND(ID_DIALOG_ADD_STATIC, OnAddStatic)
	ON_COMMAND(ID_DIALOG_ADD_CHECKBOX, OnAddCheckbox)
	ON_COMMAND(ID_DIALOG_ADD_COMBOBOX, OnAddCombobox)
	ON_COMMAND(ID_DIALOG_ADD_GROUPBOX, OnAddGroupbox)
	ON_COMMAND(ID_DIALOG_ADD_LISTBOX, OnAddListbox)
	ON_COMMAND(ID_DIALOG_ADD_RADIOBUTTON, OnAddRadiobutton)
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_SHOW_GRID, OnShowGrid)
	ON_UPDATE_COMMAND_UI(ID_SHOW_GRID, OnUpdateShowGrid)
	ON_COMMAND(ID_SNAP, OnSnapToGrid)
	ON_UPDATE_COMMAND_UI(ID_SNAP, OnUpdateSnapToGrid)
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
	ON_COMMAND(ID_EXPORT, OnExport)
	ON_UPDATE_COMMAND_UI(ID_EXPORT, OnUpdateExport)
	ON_COMMAND(ID_EXPORT_EMF, OnExportEmf)
	ON_UPDATE_COMMAND_UI(ID_EXPORT_EMF, OnUpdateExport)
	ON_COMMAND(ID_EXPORT_HTML, OnExportHtml)
	ON_UPDATE_COMMAND_UI(ID_EXPORT_HTML, OnUpdateExport)
	ON_COMMAND(ID_DIALOG_ADD_NONE, OnAddNone)
	ON_UPDATE_COMMAND_UI(ID_DIALOG_ADD_NONE, OnUpdateAddNone)
	ON_UPDATE_COMMAND_UI(ID_DIALOG_ADD_BUTTON, OnUpdateAddButton)
	ON_UPDATE_COMMAND_UI(ID_DIALOG_ADD_CHECKBOX, OnUpdateAddCheckbox)
	ON_UPDATE_COMMAND_UI(ID_DIALOG_ADD_COMBOBOX, OnUpdateAddCombobox)
	ON_UPDATE_COMMAND_UI(ID_DIALOG_ADD_EDIT, OnUpdateAddEdit)
	ON_UPDATE_COMMAND_UI(ID_DIALOG_ADD_GROUPBOX, OnUpdateAddGroupbox)
	ON_UPDATE_COMMAND_UI(ID_DIALOG_ADD_LISTBOX, OnUpdateAddListbox)
	ON_UPDATE_COMMAND_UI(ID_DIALOG_ADD_RADIOBUTTON, OnUpdateAddRadiobutton)
	ON_UPDATE_COMMAND_UI(ID_DIALOG_ADD_STATIC, OnUpdateAddStatic)
	ON_COMMAND(ID_BUTTON_MULTIDRAW, OnButtonMultidraw)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_MULTIDRAW, OnUpdateButtonMultidraw)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogEditorView construction/destruction

CDialogEditorView::CDialogEditorView()
{

	// --- DiagramEditor ---
	m_screenResolutionX = 0;
	m_drawObject = DRAW_OBJECT_NONE;

	m_bitmap = ::LoadBitmap(AfxGetResourceHandle(), MAKEINTRESOURCE(IDB_BITMAP1));
}

CDialogEditorView::~CDialogEditorView()
{
	::DeleteObject(m_bitmap);
}

BOOL CDialogEditorView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CDialogEditorView drawing

void CDialogEditorView::OnDraw(CDC* pDC)
{
	CDialogEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// --- DiagramEditor ---
	// An example of how to handle print 
	// and print preview
	if (pDC->IsPrinting())
	{
		// Print zoom is the difference between screen- 
		// and printer resolution.
		double zoom = pDC->GetDeviceCaps(LOGPIXELSX) / m_screenResolutionX;

		CRect rect(0, 0,
			(int)((double)m_editor.GetVirtualSize().cx * zoom),
			(int)((double)m_editor.GetVirtualSize().cy * zoom));

		m_editor.Print(pDC, rect, zoom);
	}

	// If we are not printing, we don't have do do 
	// anything special, the editor is a child window 
	// of the view and will be redrawn automatically.
}

void CDialogEditorView::OnDrawIconicThumbnailOrLivePreview(CDC& dc, CRect rect, CSize szRequiredThumbnailSize, BOOL bIsThumbnail, BOOL& bAlphaChannelSet)
{
	m_editor.Draw(&dc, rect);
}

/////////////////////////////////////////////////////////////////////////////
// CDialogEditorView printing

BOOL CDialogEditorView::OnPreparePrinting(CPrintInfo* pInfo)
{
	return DoPreparePrinting(pInfo);
}

void CDialogEditorView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

void CDialogEditorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

/////////////////////////////////////////////////////////////////////////////
// CDialogEditorView diagnostics

#ifdef _DEBUG
void CDialogEditorView::AssertValid() const
{
	CView::AssertValid();
}

void CDialogEditorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDialogEditorDoc* CDialogEditorView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDialogEditorDoc)));
	return (CDialogEditorDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDialogEditorView message handlers

void CDialogEditorView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// --- DiagramEditor ---
	if (!m_editor.m_hWnd)
	{
		// Creating the editor window
		CDialogEditorDoc* pDoc = GetDocument();

		CRect rect;
		GetClientRect(rect);
		m_editor.Create(WS_CHILD | WS_VISIBLE, rect, this, pDoc->GetData());

		// Only set the size if one wasn't pre-specified
		if (!pDoc->GetData()->IsModified())
		{
			m_editor.SetVirtualSize(CSize(300, 200));
			m_editor.SetBackgroundColor(::GetSysColor(COLOR_3DFACE));
			m_editor.SetGridColor(::GetSysColor(COLOR_3DDKSHADOW));
		}
		else
		{
			m_editor.SetVirtualSize(pDoc->GetData()->GetVirtualSize());
			m_editor.SetBackgroundColor(pDoc->GetData()->GetColor());
		}

		m_editor.SetResize(TRUE);
		m_editor.SetModified(FALSE);

		// We get the screen resolution, which we will use 
		// for scaling to printer. See also OnDraw.
		CClientDC dc(this);
		m_screenResolutionX = dc.GetDeviceCaps(LOGPIXELSX);
	}
	else
	{
		m_editor.Clear();
	}
}

void CDialogEditorView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// --- DiagramEditor ---
	// We resize the editor as well
	if (m_editor.m_hWnd)
		m_editor.MoveWindow(0, 0, cx, cy);
}

BOOL CDialogEditorView::OnEraseBkgnd(CDC* /*pDC*/)
{
	// --- DiagramEditor ---
	// To avoid flicker. The editor
	// covers all of the client area
	return TRUE;
}

// Called the the view is activated
void CDialogEditorView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	CMainFrame* pMainFrame = ((CMainFrame*)GetTopLevelFrame());
	ASSERT_VALID(pMainFrame);
	pMainFrame->SetRibbonContextCategory(ID_DIALOGCONTEXT, bActivate);
}

///////////////////////////////////////////////////////////
// Copy/paste/undo commands and update handlers
//

void CDialogEditorView::OnEditCopy()
{
	m_editor.Copy();
}

void CDialogEditorView::OnUpdateEditCopy(CCmdUI* pCmdUI)
{
	m_editor.UpdateCopy(pCmdUI);
}

void CDialogEditorView::OnEditCut()
{
	m_editor.Cut();
}

void CDialogEditorView::OnUpdateEditCut(CCmdUI* pCmdUI)
{
	m_editor.UpdateCut(pCmdUI);
}

void CDialogEditorView::OnEditPaste()
{
	m_editor.Paste();
}

void CDialogEditorView::OnUpdateEditPaste(CCmdUI* pCmdUI)
{
	m_editor.UpdatePaste(pCmdUI);
}

void CDialogEditorView::OnEditUndo()
{
	m_editor.Undo();

}

void CDialogEditorView::OnUpdateEditUndo(CCmdUI* pCmdUI)
{
	m_editor.UpdateUndo(pCmdUI);
}

void CDialogEditorView::OnSelectAll()
{
	m_editor.SelectAll();
}

void CDialogEditorView::OnUpdateSelectAll(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_editor.GetObjectCount() > 0);
}

///////////////////////////////////////////////////////////
// Misc dialogs
//

void CDialogEditorView::OnProperty()
{
	m_editor.ShowProperties();
}

void CDialogEditorView::OnUpdateProperty(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_editor.GetSelectCount() == 1);
}

void CDialogEditorView::OnSettings()
{
	CDialogSettings	dlg;
	dlg.m_disableColor = TRUE;
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

///////////////////////////////////////////////////////////
// Adding controls. The functions creates an object of the 
// correct type and call StartDrawingObject on the editor.
//

void CDialogEditorView::OnAddButton()
{
	CDiagramButton* obj = new CDiagramButton;
	m_editor.StartDrawingObject(obj);
	m_drawObject = DRAW_OBJECT_BUTTON;
}

void CDialogEditorView::OnAddEdit()
{
	CDiagramEdit* obj = new CDiagramEdit;
	m_editor.StartDrawingObject(obj);
	m_drawObject = DRAW_OBJECT_EDIT;
}

void CDialogEditorView::OnAddStatic()
{
	CDiagramStatic* obj = new CDiagramStatic;
	m_editor.StartDrawingObject(obj);
	m_drawObject = DRAW_OBJECT_STATIC;
}

void CDialogEditorView::OnAddCheckbox()
{
	CDiagramCheckbox* obj = new CDiagramCheckbox;
	m_editor.StartDrawingObject(obj);
	m_drawObject = DRAW_OBJECT_CHECKBOX;
}

void CDialogEditorView::OnAddCombobox()
{
	CDiagramCombobox* obj = new CDiagramCombobox;
	m_editor.StartDrawingObject(obj);
	m_drawObject = DRAW_OBJECT_COMBOBOX;
}

void CDialogEditorView::OnAddGroupbox()
{
	CDiagramGroupbox* obj = new CDiagramGroupbox;
	m_editor.StartDrawingObject(obj);
	m_drawObject = DRAW_OBJECT_GROUPBOX;
}

void CDialogEditorView::OnAddListbox()
{
	CDiagramListbox* obj = new CDiagramListbox;
	m_editor.StartDrawingObject(obj);
	m_drawObject = DRAW_OBJECT_LISTBOX;
}

void CDialogEditorView::OnAddRadiobutton()
{
	CDiagramRadiobutton* obj = new CDiagramRadiobutton;
	m_editor.StartDrawingObject(obj);
	m_drawObject = DRAW_OBJECT_RADIOBUTTON;
}

void CDialogEditorView::OnAddNone()
{
	m_editor.StartDrawingObject(NULL);
	m_drawObject = DRAW_OBJECT_NONE;
}

void CDialogEditorView::OnUpdateAddButton(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_editor.IsDrawing() && m_drawObject == DRAW_OBJECT_BUTTON);
}

void CDialogEditorView::OnUpdateAddCheckbox(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_editor.IsDrawing() && m_drawObject == DRAW_OBJECT_CHECKBOX);
}

void CDialogEditorView::OnUpdateAddCombobox(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_editor.IsDrawing() && m_drawObject == DRAW_OBJECT_COMBOBOX);
}

void CDialogEditorView::OnUpdateAddEdit(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_editor.IsDrawing() && m_drawObject == DRAW_OBJECT_EDIT);
}

void CDialogEditorView::OnUpdateAddGroupbox(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_editor.IsDrawing() && m_drawObject == DRAW_OBJECT_GROUPBOX);
}

void CDialogEditorView::OnUpdateAddListbox(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_editor.IsDrawing() && m_drawObject == DRAW_OBJECT_LISTBOX);
}

void CDialogEditorView::OnUpdateAddRadiobutton(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_editor.IsDrawing() && m_drawObject == DRAW_OBJECT_RADIOBUTTON);
}

void CDialogEditorView::OnUpdateAddStatic(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_editor.IsDrawing() && m_drawObject == DRAW_OBJECT_STATIC);
}

void CDialogEditorView::OnUpdateAddNone(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(!m_editor.IsDrawing());
}

///////////////////////////////////////////////////////////
// Zoom handling
//

void CDialogEditorView::OnZoom()
{
}

void CDialogEditorView::OnZoom100()
{
	m_editor.SetZoom(1);
}

void CDialogEditorView::OnZoom150()
{
	m_editor.SetZoom(1.5);
}

void CDialogEditorView::OnZoom200()
{
	m_editor.SetZoom(2);
}

void CDialogEditorView::OnZoom25()
{
	m_editor.SetZoom(.25);
}

void CDialogEditorView::OnZoom50()
{
	m_editor.SetZoom(.5);
}

///////////////////////////////////////////////////////////
// Grid, margin, multidraw and restraints handling
//

void CDialogEditorView::OnShowGrid()
{
	m_editor.ShowGrid(!m_editor.IsGridVisible());
}

void CDialogEditorView::OnUpdateShowGrid(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_editor.IsGridVisible());
}

void CDialogEditorView::OnSnapToGrid()
{
	m_editor.SetSnapToGrid(!m_editor.GetSnapToGrid());

}

void CDialogEditorView::OnUpdateSnapToGrid(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_editor.GetSnapToGrid());
}

void CDialogEditorView::OnMargins()
{
	m_editor.ShowMargin(!m_editor.IsMarginVisible());
}

void CDialogEditorView::OnUpdateMargins(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_editor.IsMarginVisible());
}

void CDialogEditorView::OnRestraints()
{
	if (m_editor.GetRestraints() == RESTRAINT_MARGIN)
		m_editor.SetRestraints(RESTRAINT_VIRTUAL);
	else
		m_editor.SetRestraints(RESTRAINT_MARGIN);
}

void CDialogEditorView::OnUpdateRestraints(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_editor.GetRestraints() == RESTRAINT_MARGIN);
}

void CDialogEditorView::OnButtonMultidraw()
{
	m_editor.SetMultidraw(!m_editor.GetMultidraw());
	m_drawObject = DRAW_OBJECT_NONE;
}

void CDialogEditorView::OnUpdateButtonMultidraw(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_editor.GetMultidraw());
}

///////////////////////////////////////////////////////////
// Align menu
//

void CDialogEditorView::OnLeftAlign()
{
	m_editor.LeftAlignSelected();
}
void CDialogEditorView::OnTopAlign()
{
	m_editor.TopAlignSelected();
}
void CDialogEditorView::OnRightAlign()
{
	m_editor.RightAlignSelected();
}
void CDialogEditorView::OnBottomAlign()
{
	m_editor.BottomAlignSelected();
}

void CDialogEditorView::OnUpdateLeftAlign(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_editor.GetSelectCount() > 1);
}
void CDialogEditorView::OnUpdateTopAlign(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_editor.GetSelectCount() > 1);
}
void CDialogEditorView::OnUpdateRightAlign(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_editor.GetSelectCount() > 1);
}
void CDialogEditorView::OnUpdateBottomAlign(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_editor.GetSelectCount() > 1);
}

void CDialogEditorView::OnMakeSameSize()
{
	m_editor.MakeSameSizeSelected();
}
void CDialogEditorView::OnUpdateMakeSameSize(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_editor.GetSelectCount() > 1);
}

void CDialogEditorView::OnUp()
{
	m_editor.Up();
}
void CDialogEditorView::OnDown()
{
	m_editor.Down();
}
void CDialogEditorView::OnFront()
{
	m_editor.Front();
}
void CDialogEditorView::OnBottom()
{
	m_editor.Bottom();
}

void CDialogEditorView::OnUpdateUp(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_editor.GetSelectCount() == 1);
}
void CDialogEditorView::OnUpdateDown(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_editor.GetSelectCount() == 1);
}
void CDialogEditorView::OnUpdateFront(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_editor.GetSelectCount() > 0);
}
void CDialogEditorView::OnUpdateBottom(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_editor.GetSelectCount() > 0);
}

///////////////////////////////////////////////////////////
// Exporting
//

void CDialogEditorView::OnExport()
{
	// We save the current modified-state of 
	// the editor. The export mechanism 
	// modifies the data (although in a harmless 
	// way) and we want to reset the state 
	// after the operation - to avoid save 
	// questions when no conscious changes have 
	// been made.

	BOOL modified = m_editor.IsModified();
	CDialogEditorDoc* doc = GetDocument();
	CWaitCursor wait;
	CString filename = doc->GetTitle();
	int found = filename.ReverseFind(_TCHAR('.'));
	if (found != -1)
	{
		filename = filename.Left(found);
	}

	CFileDialog dlg(FALSE, _T("emf"), filename, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("HTML File (*.html)|*.html;*.htm|Enhanced MetaFile (*.emf)|*.emf|All Files (*.*)|*.*||"));
	if (dlg.DoModal() == IDOK)
	{
		if (dlg.GetFileExt().CompareNoCase(_T("EMF")) == 0)
		{
			m_editor.ExportEMF(dlg.GetPathName(), true);
		}
		else
		{
			doc->ExportHTML(dlg.GetPathName());
		}
	}
}

void CDialogEditorView::OnExportEmf()
{
	// We save the current modified-state of 
	// the editor. The export mechanism 
	// modifies the data (although in a harmless 
	// way) and we want to reset the state 
	// after the operation - to avoid save 
	// questions when no conscious changes have 
	// been made.

	BOOL modified = m_editor.IsModified();
	CDialogEditorDoc* doc = GetDocument();
	CWaitCursor wait;
	CString filename = doc->GetTitle();
	int found = filename.ReverseFind(_TCHAR('.'));
	if (found != -1)
	{
		filename = filename.Left(found);
	}

	CFileDialog dlg(FALSE, _T("emf"), filename, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("Enhanced MetaFile (*.emf)|*.emf|All Files (*.*)|*.*||"));
	if (dlg.DoModal() == IDOK)
	{
		m_editor.ExportEMF(dlg.GetPathName(), true);
	}
}

void CDialogEditorView::OnExportHtml()
{
	// We save the current modified-state of 
	// the editor. The export mechanism 
	// modifies the data (although in a harmless 
	// way) and we want to reset the state 
	// after the operation - to avoid save 
	// questions when no conscious changes have 
	// been made.

	BOOL modified = m_editor.IsModified();
	CDialogEditorDoc* doc = GetDocument();
	CWaitCursor wait;
	CString filename = doc->GetTitle();
	int found = filename.ReverseFind(_TCHAR('.'));
	if (found != -1)
	{
		filename = filename.Left(found);
	}

	CFileDialog dlg(FALSE, _T("html"), filename, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("HTML File (*.html)|*.html;*.htm|All Files (*.*)|*.*||"));
	if (dlg.DoModal() == IDOK)
	{
		doc->ExportHTML(dlg.GetPathName());
	}
}

void CDialogEditorView::OnUpdateExport(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_editor.GetObjectCount() > 0);
}

