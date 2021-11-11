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
#include "DiagramView.h"
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

IMPLEMENT_DYNCREATE(CUmlView, CDiagramView)

BEGIN_MESSAGE_MAP(CUmlView, CDiagramView)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_BUTTON_CLASS, OnButtonClass)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_CLASS, OnUpdateButtonClass)
	ON_COMMAND(ID_BUTTON_INTERFACE, OnButtonInterface)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_INTERFACE, OnUpdateButtonInterface)
	ON_COMMAND(ID_BUTTON_LABEL, OnButtonLabel)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_LABEL, OnUpdateButtonLabel)
	ON_COMMAND(ID_BUTTON_LINK, OnButtonLink)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_LINK, OnUpdateButtonLink)
	ON_COMMAND(ID_BUTTON_NOTE, OnButtonNote)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_NOTE, OnUpdateButtonNote)
	ON_COMMAND(ID_BUTTON_PACKAGE, OnButtonPackage)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_PACKAGE, OnUpdateButtonPackage)
	ON_COMMAND(ID_BUTTON_TEMPLATE, OnButtonTemplate)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_TEMPLATE, OnUpdateButtonTemplate)
	ON_COMMAND(ID_BUTTON_DISPLAY_PROPERTIES, OnButtonDisplayProperties)
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
	ON_COMMAND(ID_IMPORT, OnImport)
	ON_UPDATE_COMMAND_UI(ID_IMPORT, OnUpdateImport)
	ON_COMMAND(ID_UML_OPEN_PACKAGE, OnOpenPackage)
	ON_UPDATE_COMMAND_UI(ID_UML_OPEN_PACKAGE, OnUpdateOpenPackage)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
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

// CUmlView button and menu handlers

void CUmlView::OnButtonClass()
{
	if (m_editor.IsDrawing() && m_drawObject == DRAW_OBJECT_CLASS)
	{
		m_editor.StartDrawingObject(NULL);
		m_drawObject = DRAW_OBJECT_NONE;
	}
	else
	{
		m_editor.StartDrawingObject(new CUMLEntityClass);
		m_drawObject = DRAW_OBJECT_CLASS;
	}
}

void CUmlView::OnUpdateButtonClass(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_editor.IsDrawing() && m_drawObject == DRAW_OBJECT_CLASS ? 1 : 0);
}

void CUmlView::OnButtonInterface()
{
	if (m_editor.IsDrawing() && m_drawObject == DRAW_OBJECT_INTERFACE)
	{
		m_editor.StartDrawingObject(NULL);
		m_drawObject = DRAW_OBJECT_NONE;
	}
	else
	{
		m_editor.StartDrawingObject(new CUMLEntityInterface);
		m_drawObject = DRAW_OBJECT_INTERFACE;
	}
}

void CUmlView::OnUpdateButtonInterface(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_editor.IsDrawing() && m_drawObject == DRAW_OBJECT_INTERFACE ? 1 : 0);
}

void CUmlView::OnButtonLabel()
{
	if (m_editor.IsDrawing() && m_drawObject == DRAW_OBJECT_UML_LABEL)
	{
		m_editor.StartDrawingObject(NULL);
		m_drawObject = DRAW_OBJECT_NONE;
	}
	else
	{
		m_editor.StartDrawingObject(new CUMLEntityLabel);
		m_drawObject = DRAW_OBJECT_UML_LABEL;
	}
}

void CUmlView::OnUpdateButtonLabel(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_editor.IsDrawing() && m_drawObject == DRAW_OBJECT_UML_LABEL ? 1 : 0);
}

void CUmlView::OnButtonLink()
{
	if (m_editor.IsDrawing() && m_drawObject == DRAW_OBJECT_LINK)
	{
		m_editor.StartDrawingObject(NULL);
		m_drawObject = DRAW_OBJECT_NONE;
	}
	else
	{
		m_editor.StartDrawingObject(new CUMLLineSegment);
		m_drawObject = DRAW_OBJECT_LINK;
	}
}

void CUmlView::OnUpdateButtonLink(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_editor.GetObjectCount() > 1);
	pCmdUI->SetCheck(m_editor.IsDrawing() && m_drawObject == DRAW_OBJECT_LINK ? 1 : 0);
}

void CUmlView::OnButtonNote()
{
	if (m_editor.IsDrawing() && m_drawObject == DRAW_OBJECT_NOTE)
	{
		m_editor.StartDrawingObject(NULL);
		m_drawObject = DRAW_OBJECT_NONE;
	}
	else
	{
		m_editor.StartDrawingObject(new CUMLEntityNote);
		m_drawObject = DRAW_OBJECT_NOTE;
	}
}

void CUmlView::OnUpdateButtonNote(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_editor.IsDrawing() && m_drawObject == DRAW_OBJECT_NOTE ? 1 : 0);
}

void CUmlView::OnButtonPackage()
{
	if (m_editor.IsDrawing() && m_drawObject == DRAW_OBJECT_PACKAGE)
	{
		m_editor.StartDrawingObject(NULL);
		m_drawObject = DRAW_OBJECT_NONE;
	}
	else
	{
		m_editor.StartDrawingObject(new CUMLEntityPackage);
		m_drawObject = DRAW_OBJECT_PACKAGE;
	}
}

void CUmlView::OnUpdateButtonPackage(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_editor.IsDrawing() && m_drawObject == DRAW_OBJECT_PACKAGE ? 1 : 0);
}

void CUmlView::OnButtonTemplate()
{
	if (m_editor.IsDrawing() && m_drawObject == DRAW_OBJECT_TEMPLATE)
	{
		m_editor.StartDrawingObject(NULL);
		m_drawObject = DRAW_OBJECT_NONE;
	}
	else
	{
		m_editor.StartDrawingObject(new CUMLEntityClassTemplate);
		m_drawObject = DRAW_OBJECT_TEMPLATE;
	}
}

void CUmlView::OnUpdateButtonTemplate(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_editor.IsDrawing() && m_drawObject == DRAW_OBJECT_TEMPLATE ? 1 : 0);
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
		// Check if we need to snapshot for undo
		if (m_editor.GetBackgroundColor() != dlg.m_color)
		{
			m_editor.GetDiagramEntityContainer()->Snapshot();
		}

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
	pCmdUI->Enable(TRUE);
}

void CUmlView::OnImport()
{
	m_editor.Import();
}

void CUmlView::OnOpenPackage()
{
	CUMLEntityPackage* package = dynamic_cast<CUMLEntityPackage*>(m_editor.GetSelectedObject());
	if (package)
	{
		m_editor.UnselectAll();
		m_editor.SetPackage(package->GetName());
		m_editor.RedrawWindow();
	}
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

// CUmlView update handlers

void CUmlView::OnUpdateFlipLink(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_editor.IsLinkSelected());
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

void CUmlView::OnUpdateOpenPackage(CCmdUI* pCmdUI)
{
	BOOL result = FALSE;

	if (m_editor.GetSelectCount() == 1)
	{
		CUMLEntityPackage* obj = dynamic_cast<CUMLEntityPackage*>(m_editor.GetSelectedObject());
		if (obj)
		{
			result = TRUE;
		}
	}

	pCmdUI->Enable(result);
}
