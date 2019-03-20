// CDiagramView.cpp : implementation file
//

#include "stdafx.h"
#include "Tulip.h"
#include "DiagramView.h"
#include "DialogEditor/DialogSettings.h"

// CDiagramView

IMPLEMENT_DYNCREATE(CDiagramView, CView)

CDiagramView::CDiagramView()
{
	m_editor = NULL;
}

CDiagramView::~CDiagramView()
{
}

BEGIN_MESSAGE_MAP(CDiagramView, CView)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_COMMAND(ID_EDIT_REDO, OnEditRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, OnUpdateEditRedo)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_COMMAND(ID_EDIT_DELETE, OnEditDelete)
	ON_UPDATE_COMMAND_UI(ID_EDIT_DELETE, OnUpdateEditDelete)
	ON_COMMAND(ID_EDIT_SELECT_ALL, OnEditSelectAll)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SELECT_ALL, OnUpdateEditSelectAll)
	ON_COMMAND(ID_DUPLICATE, OnDuplicate)
	ON_UPDATE_COMMAND_UI(ID_DUPLICATE, OnUpdateDuplicate)
	ON_COMMAND(ID_EDIT_GROUP, OnGroup)
	ON_UPDATE_COMMAND_UI(ID_EDIT_GROUP, OnUpdateGroup)
	ON_COMMAND(ID_EDIT_UNGROUP, OnUngroup)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNGROUP, OnUpdateUngroup)
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
END_MESSAGE_MAP()


// CDiagramView drawing

void CDiagramView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}


// CDiagramView diagnostics

#ifdef _DEBUG
void CDiagramView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CDiagramView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG

void CDiagramView::SetEditor(CDiagramEditor* editor)
{
	m_editor = editor;
}

// CDiagramView message handlers

void CDiagramView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	if (m_editor != NULL && m_editor->m_hWnd)
	{
		m_editor->MoveWindow(0, 0, cx, cy);
	}
}

BOOL CDiagramView::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}

void CDiagramView::OnEditUndo()
{
	m_editor->Undo();
}

void CDiagramView::OnEditRedo()
{
	m_editor->Redo();
}

void CDiagramView::OnEditCopy()
{
	m_editor->Copy();
}

void CDiagramView::OnEditCut()
{
	m_editor->Cut();
}

void CDiagramView::OnEditPaste()
{
	m_editor->Paste();
}

void CDiagramView::OnEditDelete()
{
	m_editor->DeleteAllSelected();
}

void CDiagramView::OnEditSelectAll()
{
	m_editor->SelectAll();
}

// CDiagramView update handlers

void CDiagramView::OnUpdateEditUndo(CCmdUI* pCmdUI)
{
	m_editor->UpdateUndo(pCmdUI);
}

void CDiagramView::OnUpdateEditRedo(CCmdUI* pCmdUI)
{
	m_editor->UpdateRedo(pCmdUI);
}

void CDiagramView::OnUpdateEditCopy(CCmdUI* pCmdUI)
{
	m_editor->UpdateCopy(pCmdUI);
}

void CDiagramView::OnUpdateEditCut(CCmdUI* pCmdUI)
{
	m_editor->UpdateCut(pCmdUI);
}

void CDiagramView::OnUpdateEditPaste(CCmdUI* pCmdUI)
{
	m_editor->UpdatePaste(pCmdUI);
}

void CDiagramView::OnUpdateEditDelete(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_editor->GetSelectCount() > 0);
}

void CDiagramView::OnUpdateEditSelectAll(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_editor->GetObjectCount() > 0);
}

void CDiagramView::OnDuplicate()
{
	m_editor->Duplicate();
}

void CDiagramView::OnUpdateDuplicate(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_editor->GetSelectCount() == 1);
}

void CDiagramView::OnGroup()
{
	m_editor->Group();
}

void CDiagramView::OnUpdateGroup(CCmdUI* pCmdUI)
{
	BOOL enable = FALSE;
	if (m_editor->GetSelectCount() > 1)
	{
		int count = 0;
		int lastGroup = -1;
		CDiagramEntity* obj;
		while (!enable && (obj = m_editor->GetDiagramEntityContainer()->GetAt(count++)))
		{
			if (obj->IsSelected())
			{
				int group = obj->GetGroup();

				// Initialise the last group
				if (lastGroup == -1)
				{
					lastGroup = group;
				}

				// If there is an object with no group or a different group, allow grouping
				if (group == 0 || group != lastGroup)
				{
					enable = TRUE;
				}

				lastGroup = group;
			}
		}
	}

	pCmdUI->Enable(enable);
}

void CDiagramView::OnUngroup()
{
	m_editor->Ungroup();
}

void CDiagramView::OnUpdateUngroup(CCmdUI* pCmdUI)
{
	BOOL enable = FALSE;
	if (m_editor->GetSelectCount() > 1)
	{
		int count = 0;
		CDiagramEntity* obj;
		while (!enable && (obj = m_editor->GetDiagramEntityContainer()->GetAt(count++)))
		{
			// If there is a selected object with a group, allow ungrouping
			if (obj->IsSelected() && obj->GetGroup() != 0)
			{
				enable = TRUE;
			}
		}
	}

	pCmdUI->Enable(enable);
}

void CDiagramView::OnZoom()
{
}

void CDiagramView::OnZoom100()
{
	m_editor->SetZoom(1);
}

void CDiagramView::OnZoom150()
{
	m_editor->SetZoom(1.5);
}

void CDiagramView::OnZoom200()
{
	m_editor->SetZoom(2);
}

void CDiagramView::OnZoom25()
{
	m_editor->SetZoom(.25);
}

void CDiagramView::OnZoom50()
{
	m_editor->SetZoom(.5);
}

void CDiagramView::OnSnap()
{
	m_editor->SetSnapToGrid(!m_editor->GetSnapToGrid());
}

void CDiagramView::OnShowGrid()
{
	m_editor->ShowGrid(!m_editor->IsGridVisible());
}

void CDiagramView::OnRestraints()
{
	if (m_editor->GetRestraints() == RESTRAINT_MARGIN)
		m_editor->SetRestraints(RESTRAINT_VIRTUAL);
	else
		m_editor->SetRestraints(RESTRAINT_MARGIN);
}

void CDiagramView::OnUpdateRestraints(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_editor->GetRestraints() == RESTRAINT_MARGIN);
}

void CDiagramView::OnUpdateSnap(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_editor->GetSnapToGrid());
}

void CDiagramView::OnUpdateShowGrid(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_editor->IsGridVisible());
}

void CDiagramView::OnUpdateMargins(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_editor->IsMarginVisible());
}

void CDiagramView::OnMargins()
{
	m_editor->ShowMargin(!m_editor->IsMarginVisible());
}

void CDiagramView::OnSettings()
{
	CDialogSettings	dlg;

	dlg.m_color = m_editor->GetBackgroundColor();
	dlg.m_width = m_editor->GetVirtualSize().cx;
	dlg.m_height = m_editor->GetVirtualSize().cy;
	dlg.m_gridWidth = m_editor->GetGridSize().cx;
	dlg.m_gridHeight = m_editor->GetGridSize().cy;
	m_editor->GetMargins(dlg.m_marginLeft, dlg.m_marginTop, dlg.m_marginRight, dlg.m_marginBottom);

	if (dlg.DoModal() == IDOK)
	{
		// Check if we need to snapshot for undo
		if (m_editor->GetBackgroundColor() != dlg.m_color)
		{
			m_editor->GetDiagramEntityContainer()->Snapshot();
		}

		m_editor->SetBackgroundColor(dlg.m_color);
		m_editor->SetGridSize(CSize(dlg.m_gridWidth, dlg.m_gridHeight));
		m_editor->SetVirtualSize(CSize(dlg.m_width, dlg.m_height));
		m_editor->SetMargins(dlg.m_marginLeft, dlg.m_marginTop, dlg.m_marginRight, dlg.m_marginBottom);
		m_editor->RedrawWindow();
	}
}

///////////////////////////////////////////////////////////
// Align menu
//

void CDiagramView::OnLeftAlign()
{
	m_editor->LeftAlignSelected();
}
void CDiagramView::OnTopAlign()
{
	m_editor->TopAlignSelected();
}
void CDiagramView::OnRightAlign()
{
	m_editor->RightAlignSelected();
}
void CDiagramView::OnBottomAlign()
{
	m_editor->BottomAlignSelected();
}

void CDiagramView::OnUpdateLeftAlign(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_editor->GetSelectCount() > 1);
}
void CDiagramView::OnUpdateTopAlign(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_editor->GetSelectCount() > 1);
}
void CDiagramView::OnUpdateRightAlign(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_editor->GetSelectCount() > 1);
}
void CDiagramView::OnUpdateBottomAlign(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_editor->GetSelectCount() > 1);
}

void CDiagramView::OnMakeSameSize()
{
	m_editor->MakeSameSizeSelected();
}
void CDiagramView::OnUpdateMakeSameSize(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_editor->GetSelectCount() > 1);
}

void CDiagramView::OnUp()
{
	m_editor->Up();
}
void CDiagramView::OnDown()
{
	m_editor->Down();
}
void CDiagramView::OnFront()
{
	m_editor->Front();
}
void CDiagramView::OnBottom()
{
	m_editor->Bottom();
}

void CDiagramView::OnUpdateUp(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_editor->GetSelectCount() == 1);
}
void CDiagramView::OnUpdateDown(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_editor->GetSelectCount() == 1);
}
void CDiagramView::OnUpdateFront(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_editor->GetSelectCount() > 0);
}
void CDiagramView::OnUpdateBottom(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_editor->GetSelectCount() > 0);
}