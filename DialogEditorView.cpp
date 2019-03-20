// DialogEditorView.cpp : implementation of the CDialogEditorView class
//

#include "stdafx.h"
#include "Tulip.h"

#include "MainFrm.h"
#include "DiagramView.h"
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

IMPLEMENT_DYNCREATE(CDialogEditorView, CDiagramView)

BEGIN_MESSAGE_MAP(CDialogEditorView, CDiagramView)
	//{{AFX_MSG_MAP(CDialogEditorView)
	ON_WM_SIZE()
	ON_COMMAND(ID_DIALOG_ADD_BUTTON, OnAddButton)
	ON_COMMAND(ID_DIALOG_ADD_EDIT, OnAddEdit)
	ON_COMMAND(ID_DIALOG_ADD_STATIC, OnAddStatic)
	ON_COMMAND(ID_DIALOG_ADD_CHECKBOX, OnAddCheckbox)
	ON_COMMAND(ID_DIALOG_ADD_COMBOBOX, OnAddCombobox)
	ON_COMMAND(ID_DIALOG_ADD_GROUPBOX, OnAddGroupbox)
	ON_COMMAND(ID_DIALOG_ADD_LISTBOX, OnAddListbox)
	ON_COMMAND(ID_DIALOG_ADD_RADIOBUTTON, OnAddRadiobutton)
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_EXPORT, OnExport)
	ON_UPDATE_COMMAND_UI(ID_EXPORT, OnUpdateExport)
	ON_COMMAND(ID_EXPORT_EMF, OnExportEmf)
	ON_UPDATE_COMMAND_UI(ID_EXPORT_EMF, OnUpdateExport)
	ON_COMMAND(ID_EXPORT_HTML, OnExportHtml)
	ON_UPDATE_COMMAND_UI(ID_EXPORT_HTML, OnUpdateExport)
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

		// Pass the editor to the base class
		SetEditor(&m_editor);

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

// Called the the view is activated
void CDialogEditorView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	CMainFrame* pMainFrame = ((CMainFrame*)GetTopLevelFrame());
	ASSERT_VALID(pMainFrame);
	pMainFrame->SetRibbonContextCategory(ID_DIALOGCONTEXT, bActivate);
}

///////////////////////////////////////////////////////////
// Command and update handlers
//

///////////////////////////////////////////////////////////
// Adding controls. The functions creates an object of the 
// correct type and call StartDrawingObject on the editor.
//

void CDialogEditorView::OnAddButton()
{
	if (m_editor.IsDrawing() && m_drawObject == DRAW_OBJECT_BUTTON)
	{
		m_editor.StartDrawingObject(NULL);
		m_drawObject = DRAW_OBJECT_NONE;
	}
	else
	{
		m_editor.StartDrawingObject(new CDiagramButton);
		m_drawObject = DRAW_OBJECT_BUTTON;
	}
}

void CDialogEditorView::OnAddEdit()
{
	if (m_editor.IsDrawing() && m_drawObject == DRAW_OBJECT_EDIT)
	{
		m_editor.StartDrawingObject(NULL);
		m_drawObject = DRAW_OBJECT_NONE;
	}
	else
	{
		m_editor.StartDrawingObject(new CDiagramEdit);
		m_drawObject = DRAW_OBJECT_EDIT;
	}
}

void CDialogEditorView::OnAddStatic()
{
	if (m_editor.IsDrawing() && m_drawObject == DRAW_OBJECT_STATIC)
	{
		m_editor.StartDrawingObject(NULL);
		m_drawObject = DRAW_OBJECT_NONE;
	}
	else
	{
		m_editor.StartDrawingObject(new CDiagramStatic);
		m_drawObject = DRAW_OBJECT_STATIC;
	}
}

void CDialogEditorView::OnAddCheckbox()
{
	if (m_editor.IsDrawing() && m_drawObject == DRAW_OBJECT_CHECKBOX)
	{
		m_editor.StartDrawingObject(NULL);
		m_drawObject = DRAW_OBJECT_NONE;
	}
	else
	{
		m_editor.StartDrawingObject(new CDiagramCheckbox);
		m_drawObject = DRAW_OBJECT_CHECKBOX;
	}
}

void CDialogEditorView::OnAddCombobox()
{
	if (m_editor.IsDrawing() && m_drawObject == DRAW_OBJECT_COMBOBOX)
	{
		m_editor.StartDrawingObject(NULL);
		m_drawObject = DRAW_OBJECT_NONE;
	}
	else
	{
		m_editor.StartDrawingObject(new CDiagramCombobox);
		m_drawObject = DRAW_OBJECT_COMBOBOX;
	}
}

void CDialogEditorView::OnAddGroupbox()
{
	if (m_editor.IsDrawing() && m_drawObject == DRAW_OBJECT_GROUPBOX)
	{
		m_editor.StartDrawingObject(NULL);
		m_drawObject = DRAW_OBJECT_NONE;
	}
	else
	{
		m_editor.StartDrawingObject(new CDiagramGroupbox);
		m_drawObject = DRAW_OBJECT_GROUPBOX;
	}
}

void CDialogEditorView::OnAddListbox()
{
	if (m_editor.IsDrawing() && m_drawObject == DRAW_OBJECT_LISTBOX)
	{
		m_editor.StartDrawingObject(NULL);
		m_drawObject = DRAW_OBJECT_NONE;
	}
	else
	{
		m_editor.StartDrawingObject(new CDiagramListbox);
		m_drawObject = DRAW_OBJECT_LISTBOX;
	}
}

void CDialogEditorView::OnAddRadiobutton()
{
	if (m_editor.IsDrawing() && m_drawObject == DRAW_OBJECT_RADIOBUTTON)
	{
		m_editor.StartDrawingObject(NULL);
		m_drawObject = DRAW_OBJECT_NONE;
	}
	else
	{
		m_editor.StartDrawingObject(new CDiagramRadiobutton);
		m_drawObject = DRAW_OBJECT_RADIOBUTTON;
	}
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

///////////////////////////////////////////////////////////
// Grid, margin, multidraw and restraints handling
//

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

