/* ==========================================================================
	Class :			CUMLEditor

	Author :		Johan Rosengren, Abstrakt Mekanik AB

	Date :			2004-06-28

	Purpose :		"CUMLEditor" is the main editor class of the UML-editor,
					derived from "CDiagramEditor".

	Description :	"CUMLEditor" manages the drawing and contains the
					interface to the application (together with
					"CUMLEntityContainer"). In addition to the features of
					"CDiagramEditor", it manages line-drawing with links
					(the "CUMLLineSegment" class). It also handles the
					display and navigation of packages.

	Usage :			Use as a "CDiagramEditor".
   ========================================================================
	Changes :		4/8 2004	Added "Export only header files" support
								(ikolev)
					4/8 2004	Setting package to current when pasting
								items (ikolev)
					4/8 2004	Saving the destination between exports
								(ikolev)
					4/8 2004	Added initialization list from class
								attributes to ctors.
					5/8 2004	Corrected jog-drawing when zoomed.
   ========================================================================
					27/8 2004	Corrected jog-drawing when scrolling.
   ========================================================================*/

#include "stdafx.h"
#include "UMLEditor.h"
#include "UMLEntityContainer.h"
#include "UMLMenu.h"
#include "UMLLineSegment.h"
#include "UMLEntityPackage.h"
#include "../TextFile/TextFile.h"
#include "DiskObject/DiskObject.h"

#include <math.h>
#include <shlobj.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma warning( disable : 4706 )

int CALLBACK BFFCallbackProc(HWND hwnd, UINT uMsg, LPARAM /*lParam*/, LPARAM lpData)
{
	if (uMsg == BFFM_INITIALIZED)
		::SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CUMLEditor

CUMLEditor::CUMLEditor()
/* ============================================================
	Function :		CUMLEditor::CUMLEditor
	Description :	Constructor
	Access :		Public

	Return :		void
	Parameters :	none

	Usage :

   ============================================================*/
{
	m_data = NULL;
	SetSnapToGrid(TRUE);
	ShowGrid(FALSE);
	SetRestraints(RESTRAINT_VIRTUAL);

	SetPopupMenu(new CUMLMenu);

	m_drawingLine = FALSE;

	m_curDrawLine = ::LoadCursor(AfxGetResourceHandle(), MAKEINTRESOURCE(IDC_UML_CURSOR_DRAW));
	m_curAttach = ::LoadCursor(AfxGetResourceHandle(), MAKEINTRESOURCE(IDC_UML_CURSOR_ATTACH));
	m_curUp = ::LoadCursor(AfxGetResourceHandle(), MAKEINTRESOURCE(IDC_UML_CURSOR_UP));
	m_currentCursor = NULL;

}

CUMLEditor::~CUMLEditor()
/* ============================================================
	Function :		CUMLEditor::~CUMLEditor
	Description :	Destructor
	Access :		Public

	Return :		void
	Parameters :	none

	Usage :

   ============================================================*/
{
	delete m_data;
}

/////////////////////////////////////////////////////////////////////////////
// CUMLEditor overrides

BOOL CUMLEditor::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, CDiagramEntityContainer* data)
/* ============================================================
	Function :		CUMLEditor::Create
	Description :	Creates the editor.
	Access :		Public

	Return :		BOOL							-	"TRUE" if created ok.
	Parameters :	DWORD dwStyle					-	Style of editor
					const RECT& rect				-	Position of editor
					CWnd* pParentWnd				-	Parent of editor
					CDiagramEntityContainer* data	-	Data container

	Usage :			Call to create an editor.

   ============================================================*/
{

	m_data = NULL;
	if (data == NULL)
	{
		m_data = new CUMLEntityContainer;
		data = m_data;
	}

	return CDiagramEditor::Create(dwStyle, rect, pParentWnd, data);

}

void CUMLEditor::DrawObjects(CDC* dc, double zoom) const
/* ============================================================
	Function :		CUMLEditor::DrawObjects
	Description :	Draws the objects in the editor
	Access :		Protected

	Return :		void
	Parameters :	CDC* dc		-	"CDC" to draw to
					double zoom	-	Zoom factor. Might not be
									the same as the editor zoom
									(for print previews, for
									example)

	Usage :			Call to draw the objects of the editor.
					Also draws package information and jogs.

   ============================================================*/
{

	CString package = GetPackage();
	if (package.GetLength())
	{
		int place = round(4 * zoom);
		int size = round(32 * zoom);
		if (!dc->IsPrinting())
		{
			HICON icon = (HICON) ::LoadImage(AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_UML_ICON_UP), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
			if (icon)
				::DrawIconEx(dc->m_hDC, place, place, icon, size, size, 0, NULL, DI_NORMAL);
		}

		CString path;
		CString level(package);
		while (level.GetLength())
		{
			CUMLEntity* obj = GetAllNamedObject(level);
			if (obj)
			{
				path = obj->GetTitle() + _T(":") + path;
				level = obj->GetPackage();
			}
			else
				level = _T("");
		}

		if (path.GetLength())
			path = path.Left(path.GetLength() - 1);

		CFont font;
		font.CreateFont(-round(12 * zoom), 0, 0, 0, FW_NORMAL, 0, 0, 0, 0, 0, 0, 0, 0, _T("Arial"));
		dc->SelectObject(&font);
		dc->TextOut(7 * place, place, path);
		dc->SelectStockObject(ANSI_VAR_FONT);
	}

	INT_PTR count = GetObjectCount();
	for (INT_PTR i = 0; i < count; i++)
	{
		CUMLEntity* obj = GetObjectAt(i);
		if (obj && package == obj->GetPackage())
			obj->DrawObject(dc, zoom);
	}

	CPen gridPen;
	gridPen.CreatePen(PS_SOLID, 0, GetGridColor());
	CPen bgPen;
	bgPen.CreatePen(PS_SOLID, 0, GetBackgroundColor());

	double x = static_cast<double>(GetMarkerSize().cx) * GetZoom();
	double y = static_cast<double>(GetMarkerSize().cy) * GetZoom();

	int topscroll = GetScrollPos(SB_VERT);
	int leftscroll = GetScrollPos(SB_LEFT);

	for (int i = 0; i < count; i++)
	{

		CUMLLineSegment* obj = dynamic_cast<CUMLLineSegment*>(GetObjectAt(i));
		if (obj && package == obj->GetPackage())
		{
			// Drawing tunnels
			BOOL horz = obj->IsHorizontal();
			for (int t = i + 1; t < count; t++)
			{
				CUMLLineSegment* line = dynamic_cast<CUMLLineSegment*>(GetObjectAt(t));
				if (line)
				{
					dc->SelectStockObject(NULL_BRUSH);
					if (horz && !line->IsHorizontal())
					{
						if (min(obj->GetLeft(), obj->GetRight()) < line->GetLeft() &&
							max(obj->GetLeft(), obj->GetRight()) > line->GetLeft() &&
							min(line->GetTop(), line->GetBottom()) < obj->GetTop() &&
							max(line->GetTop(), line->GetBottom()) > obj->GetTop())
						{

							CRect rect(round(line->GetLeft() - x),
								round(obj->GetTop() - y),
								round(line->GetLeft() + x),
								round(obj->GetTop() + y));

							rect.InflateRect(-1, -1);

							if (IsGridVisible())
								dc->SelectObject(&gridPen);
							else
								dc->SelectObject(&bgPen);

							CRect seg(rect);
							seg.top = round((double)seg.top * GetZoom());
							seg.bottom = round((double)seg.bottom * GetZoom());
							seg.left = round((double)seg.left * GetZoom());
							seg.right = round((double)seg.right * GetZoom());
							int left = round(line->GetLeft() * GetZoom());

							dc->MoveTo(left, seg.top);
							dc->LineTo(left, seg.top + seg.Height() / 2);

							dc->MoveTo(left, seg.top + seg.Height() / 2 + 1);
							dc->LineTo(left, seg.bottom);

							CRgn rgn;
							CRect rgnrect(rect);
							VirtualToScreen(rgnrect);
							rgn.CreateRectRgn(rgnrect.left, rgnrect.top, rgnrect.left + rgnrect.Width() / 2, rgnrect.bottom);
							dc->SelectClipRgn(&rgn, RGN_AND);

							rgnrect.top += topscroll;
							rgnrect.bottom += topscroll;
							rgnrect.left += leftscroll;
							rgnrect.right += leftscroll;

							dc->SelectStockObject(BLACK_PEN);
							dc->Ellipse(rgnrect);

							dc->SelectClipRgn(NULL);

						}
					}
					else if (!horz && (line->IsHorizontal()))
					{
						if (min(line->GetLeft(), line->GetRight()) < obj->GetLeft() &&
							max(line->GetLeft(), line->GetRight()) > obj->GetLeft() &&
							min(obj->GetTop(), obj->GetBottom()) < line->GetTop() &&
							max(obj->GetTop(), obj->GetBottom()) > line->GetTop())
						{
							CRect rect(round(obj->GetLeft() - x),
								round(line->GetTop() - y),
								round(obj->GetLeft() + x),
								round(line->GetTop() + y));
							rect.InflateRect(-1, -1);
							if (IsGridVisible())
								dc->SelectObject(&gridPen);
							else
								dc->SelectObject(&bgPen);

							CRect seg(rect);
							seg.top = round((double)seg.top * GetZoom());
							seg.bottom = round((double)seg.bottom * GetZoom());
							seg.left = round((double)seg.left * GetZoom());
							seg.right = round((double)seg.right * GetZoom());
							int top = round((double)line->GetTop() * GetZoom());
							dc->MoveTo(seg.left, top);
							dc->LineTo(seg.left + seg.Width() / 2, top);

							dc->MoveTo(seg.left + seg.Width() / 2 + 1, top);
							dc->LineTo(seg.right, top);

							CRgn rgn;
							CRect rgnrect(rect);
							VirtualToScreen(rgnrect);

							rgn.CreateRectRgn(rgnrect.left, rgnrect.top, rgnrect.right, rgnrect.top + rgnrect.Height() / 2);
							dc->SelectClipRgn(&rgn, RGN_AND);

							rgnrect.top += topscroll;
							rgnrect.bottom += topscroll;
							rgnrect.left += leftscroll;
							rgnrect.right += leftscroll;

							dc->SelectStockObject(BLACK_PEN);
							dc->Ellipse(rgnrect);
							dc->SelectClipRgn(NULL);

						}
					}
				}
			}
		}
	}

	dc->SelectStockObject(WHITE_BRUSH);
	dc->SelectStockObject(BLACK_PEN);

}

void CUMLEditor::StartDrawingObject(CDiagramEntity* obj)
/* ============================================================
	Function :		CUMLEditor::StartDrawingObject
	Description :	Starts drawing an object in the editor
	Access :		Public

	Return :		void
	Parameters :	CDiagramEntity* obj	-	Object to draw

	Usage :			Call to start drawing an object. The editor
					takes ownership of the object.

   ============================================================*/
{

	UnselectAll();

	CUMLEntity* uml = static_cast<CUMLEntity*>(obj);
	uml->SetPackage(GetPackage());
	uml->SetDisplayOptions(GetDisplayOptions());

	CDiagramEditor::StartDrawingObject(obj);

}

void CUMLEditor::OnObjectCommand(UINT nID)
/* ============================================================
	Function :		CUMLEditor::OnObjectCommand
	Description :	Command handler for the commands issued
					from an object popup menu.
	Access :		Protected

	Return :		void
	Parameters :	UINT nID	-	ID of command

	Usage :			Called from MFC. We check and execute the
					package Open command here.

   ============================================================*/
{

	CUMLEntityPackage* package = dynamic_cast<CUMLEntityPackage*>(GetSelectedObject());
	if (package && nID == CMD_OPEN)
	{
		UnselectAll();
		SetPackage(package->GetName());
		RedrawWindow();
	}
	else
		CDiagramEditor::OnObjectCommand(nID);

}

void CUMLEditor::SelectAll()
/* ============================================================
	Function :		CUMLEditor::SelectAll
	Description :	Selects all objects in the current package.
	Access :		Public

	Return :		void
	Parameters :	none

	Usage :			Call to select all objects.

   ============================================================*/
{

	int count = 0;
	CUMLEntity* obj;
	while ((obj = GetObjectAt(count++)))
		if (obj->GetPackage() == GetPackage())
			obj->Select(TRUE);

	RedrawWindow();

}

/////////////////////////////////////////////////////////////////////////////
// CUMLEditor message handlers
//

void CUMLEditor::OnLButtonDown(UINT nFlags, CPoint point)
/* ============================================================
	Function :		CUMLEditor::OnLButtonDown
	Description :	Command handler for the "WM_LBUTTONDOWN"
					message.
	Access :		Protected

	Return :		void
	Parameters :	UINT nFlags		-	Not used
					CPoint point	-	Mouse position

	Usage :			Called from MFC. Extra processing is made
					here regarding links and packages.

   ============================================================*/
{

	// Click on the arrow up icon
	CString package = GetPackage();
	int size = round(20 * GetZoom());
	if (package.GetLength() && point.x < size && point.y < size)
	{
		CUMLEntity* obj = GetAllNamedObject(package);
		if (obj)
			SetPackage(obj->GetPackage());
		else
			SetPackage(_T(""));

		RedrawWindow();
		return;
	}

	// Store the line being drawn, if any
	CPoint virtpoint(point);
	ScreenToVirtual(virtpoint);
	CUMLLineSegment* seg = NULL;
	if (m_drawingLine && GetInteractMode() == MODE_RESIZING)
		seg = dynamic_cast<CUMLLineSegment*>(GetSelectedObject());

	// If we are starting to draw a new link, we save the object 
	// link point under the cursor, as we want to set the start 
	// of the link to this object.
	int type = LINK_NONE;
	int savedtype = LINK_NONE;
	CUMLEntity* obj = NULL;
	CUMLEntity* mainobj = NULL;
	if (GetInteractMode() == MODE_DRAWING)
	{
		m_drawingLine = FALSE;
		CUMLLineSegment* seg = dynamic_cast<CUMLLineSegment*>(GetDrawingObject());
		if (seg)
		{
			int count = 0;
			while ((obj = GetObjectAt(count++)))
			{
				if (obj->GetPackage() == GetPackage())
				{
					type = obj->GetLinkCode(virtpoint);
					if (type & LINK_ALL)
					{
						mainobj = obj;
						m_drawingLine = TRUE;
						savedtype = type;
						count = -2;
					}
				}
			}

			// there was no object under the cursor, and we 
			// will not be able to draw a link
			if (!m_drawingLine)
				SetInteractMode(MODE_NONE);
		}
	}

	CDiagramEditor::OnLButtonDown(nFlags, point);

	if (GetInteractMode() == MODE_RESIZING && m_drawingLine && mainobj && savedtype != LINK_NONE)
	{
		// We have added a line segment, and set the link start point to it
		CUMLLineSegment* seg = dynamic_cast<CUMLLineSegment*>(GetSelectedObject());
		if (seg)
		{
			seg->SetLink(LINK_START, mainobj->GetName());
			seg->SetLinkType(LINK_START, savedtype);

			int diff = 0;
			if (savedtype == LINK_TOP || savedtype == LINK_BOTTOM)
			{
				diff = virtpoint.x - static_cast<int>(mainobj->GetLeft());
				if (GetSnapToGrid())
					diff = SnapX(diff);
			}
			else if (savedtype == LINK_LEFT || savedtype == LINK_RIGHT)
			{
				diff = virtpoint.y - static_cast<int>(mainobj->GetTop());
				if (GetSnapToGrid())
					diff = SnapY(diff);
			}

			seg->SetOffset(LINK_START, diff);

			if (savedtype == LINK_END)
			{
				CUMLLineSegment* line = dynamic_cast<CUMLLineSegment*>(mainobj);
				line->SetLink(LINK_END, seg->GetName());
				line->SetLinkType(LINK_END, LINK_START);
			}
			else
			{
				CSize minsize = mainobj->GetMinimumSize();
				if (savedtype == LINK_TOP || savedtype == LINK_BOTTOM)
				{
					if (minsize.cx < diff)
						mainobj->SetMinimumSize(CSize(diff, minsize.cy));
				}
				else
					if (minsize.cy < diff)
						mainobj->SetMinimumSize(CSize(minsize.cx, diff));
			}
		}
	}

	if (GetInteractMode() == MODE_RUBBERBANDING && m_drawingLine)
	{
		// Clicked outside the link point. Force continue drawing.
		if (!seg)
			m_drawingLine = FALSE;
		else
		{
			if (seg->IsHorizontal())
				seg->SetRight(virtpoint.x);
			else
				seg->SetBottom(virtpoint.y);
			seg->Select(TRUE);
			SetInteractMode(MODE_RESIZING, DEHT_BOTTOMRIGHT);
		}
	}

	SetCursor(point);

}

void CUMLEditor::OnLButtonUp(UINT nFlags, CPoint point)
/* ============================================================
	Function :		CUMLEditor::OnLButtonUp
	Description :	Handler for "WM_LBUTTONUP" message.
	Access :		Protected

	Return :		void
	Parameters :	UINT nFlags		-	Not used
					CPoint point	-	Position of mouse cursor

	Usage :			Called from MFC. Used to link lines if we
					are drawing lines.

   ============================================================*/
{

	// We get this before the base class implementation
	// to avoid any unselects in there. 
	CUMLEntityContainer* objs = GetUMLEntityContainer();
	CUMLLineSegment* selobj = dynamic_cast<CUMLLineSegment*>(GetSelectedObject());

	CDiagramEditor::OnLButtonUp(nFlags, point);

	if (selobj && m_drawingLine)
	{
		if (selobj->GetRight() == selobj->GetLeft() && selobj->GetTop() == selobj->GetBottom())
			// If we have a zero-size line, we force resizing
			SetInteractMode(MODE_RESIZING, DEHT_BOTTOMRIGHT);
		else
		{
			// We try to find a link point to attach this line to
			UnselectAll();

			// Check if we can attatch to a link point.
			BOOL attached = FALSE;
			int count = 0;
			CUMLEntity* obj;
			point = selobj->GetRect().BottomRight();

			while ((obj = GetObjectAt(count++)))
			{
				if (obj->GetPackage() == GetPackage())
				{
					int linkCode = obj->GetLinkCode(point);
					if (linkCode != LINK_NONE && obj != selobj)
					{
						if ((selobj->IsHorizontal() && (linkCode == LINK_LEFT || linkCode == LINK_RIGHT)) ||
							(!selobj->IsHorizontal() && (linkCode == LINK_TOP || linkCode == LINK_BOTTOM)))
						{
							// We have a hit
							// Attach to an object
							attached = TRUE;
							selobj->SetLink(LINK_END, obj->GetName());
							selobj->SetLinkType(LINK_END, linkCode);

							int diff = 0;
							if (linkCode == LINK_TOP || linkCode == LINK_BOTTOM)
							{
								diff = point.x - static_cast<int>(obj->GetLeft());
								if (GetSnapToGrid())
									diff = SnapX(diff);
							}
							else if (linkCode == LINK_LEFT || linkCode == LINK_RIGHT)
							{
								diff = point.y - static_cast<int>(obj->GetTop());
								if (GetSnapToGrid())
									diff = SnapY(diff);
							}

							selobj->SetOffset(LINK_END, diff);

							if (linkCode == LINK_START || linkCode == LINK_END)
							{
								CUMLLineSegment* line = dynamic_cast<CUMLLineSegment*>(obj);
								if (line)
								{
									line->SetLink(LINK_START, selobj->GetName());
									line->SetLinkType(LINK_START, LINK_END);
								}
							}
							else
							{
								CSize minsize = obj->GetMinimumSize();
								if (linkCode == LINK_TOP || linkCode == LINK_BOTTOM)
								{
									if (minsize.cx < diff)
										obj->SetMinimumSize(CSize(diff, minsize.cy));
								}
								else
									if (minsize.cy < diff)
										obj->SetMinimumSize(CSize(minsize.cx, diff));
							}

							objs->ReduceLine(selobj);
							objs->SetDefaultLineStyle(selobj);
							RedrawWindow();
							count = -2;
						}
					}
				}
			}

			if (!attached)
			{

				CUMLLineSegment* line = static_cast<CUMLLineSegment*>(GetDrawingObject()->Clone());
				CRect rect = selobj->GetRect();
				CPoint virtpoint(rect.BottomRight());
				line->SetRect(CRect(virtpoint.x, virtpoint.y, virtpoint.x, virtpoint.y));
				line->Select(TRUE);

				// Create a link between selobj and newobj
				selobj->SetLinkType(LINK_END, LINK_START);
				selobj->SetLink(LINK_END, line->GetName());

				line->SetLink(LINK_START, selobj->GetName());
				line->SetLinkType(LINK_START, LINK_END);

				objs->ReduceLine(selobj);

				// Add the object to the container
				objs->Add(line);
				SetInteractMode(MODE_RESIZING, DEHT_BOTTOMRIGHT);

			}
			else
				m_drawingLine = FALSE;
		}
	}

	if (GetInteractMode() == MODE_NONE || GetInteractMode() == MODE_RUBBERBANDING)
	{
		GetUMLEntityContainer()->DeleteDanglingLines();
		RedrawWindow();
	}

	SetCursor(point);

}

void CUMLEditor::OnMouseMove(UINT nFlags, CPoint point)
/* ============================================================
	Function :		CUMLEditor::OnMouseMove
	Description :	Handler for the "WM_MOUSEMOVE" message.
	Access :		Protected

	Return :		void
	Parameters :	UINT nFlags		-	Not used
					CPoint point	-	Not used

	Usage :			Called from MFC. We add handling
					to close screen redraws as we are moving
					lots of stuff (all the attached objects in
					addition to the primarily moved). We also
					do secondary movements here - that is,
					objects linked to the one(s) being moved
					must be moved as well, even if they are
					not selected.

   ============================================================*/
{

	SetCursor(point);

	// We want to skip mouse movement handling if a couple 
	// of lines are selected.
	if (!PartialLinesSelected())
	{
		CRect rect;
		CUMLLineSegment* savedobj = NULL;
		if ((GetInteractMode() == MODE_MOVING || GetInteractMode() == MODE_RESIZING))
		{
			SetRedraw(FALSE);
			if (GetSelectCount() == 1)
			{

				CUMLLineSegment* obj = dynamic_cast<CUMLLineSegment*>(GetSelectedObject());
				if (obj)
				{
					if (IsConnected(obj))
					{
						rect = obj->GetRect();
						savedobj = obj;
					}
				}
			}
		}

		CDiagramEditor::OnMouseMove(nFlags, point);

		// We want to stop movement of connected lines in the line direction.
		// This is accomplished here, by resetting either the horizontal or 
		// vertical coordinates.
		if ((GetInteractMode() == MODE_MOVING || GetInteractMode() == MODE_RESIZING))
		{
			if (savedobj)
			{
				// Forced move
				if (rect.top == rect.bottom)
				{
					savedobj->SetLeft(rect.left);
					savedobj->SetRight(rect.right);
				}
				else
				{
					savedobj->SetTop(rect.top);
					savedobj->SetBottom(rect.bottom);
				}
			}

			ModifyLinkedPositions();
			SetRedraw(TRUE);
			RedrawWindow();
		}
	}
}

void CUMLEditor::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
/* ============================================================
	Function :		CUMLEditor::OnKeyDown
	Description :	Handles the WM_KEYDOWN-message.
	Access :		Protected

	Return :		void
	Parameters :	UINT nChar		-	Not used
					UINT nRepCnt	-	Not used
					UINT nFlags		-	Not used

	Usage :			Called from MFC. We override this as
					objects might be moved and we will have
					to move unlinked objects as well.

   ============================================================*/
{

	if (!PartialLinesSelected() || nChar == VK_DELETE)
	{
		SetRedraw(FALSE);

		CRect rect;
		CUMLLineSegment* savedobj = NULL;
		if (GetSelectCount() == 1)
		{
			CUMLLineSegment* obj = dynamic_cast<CUMLLineSegment*>(GetSelectedObject());
			if (obj && IsConnected(obj))
			{
				rect = obj->GetRect();
				savedobj = obj;
			}
		}

		CDiagramEditor::OnKeyDown(nChar, nRepCnt, nFlags);

		if (nChar != VK_DELETE)
		{
			if (savedobj)
			{
				// Forced move
				if (rect.top == rect.bottom)
				{
					savedobj->SetLeft(rect.left);
					savedobj->SetRight(rect.right);
				}
				else
				{
					savedobj->SetTop(rect.top);
					savedobj->SetBottom(rect.bottom);
				}
			}
		}

		ModifyLinkedPositions();
		SetRedraw(TRUE);
		RedrawWindow();
	}

}

BOOL CUMLEditor::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
/* ============================================================
	Function :		CUMLEditor::OnSetCursor
	Description :	Handler for the "WM_SETCURSOR" message.
	Access :		Protected

	Return :		BOOL			-	"TRUE" if handled
	Parameters :	CWnd* pWnd		-	Not used
					UINT nHitTest	-	Not used
					UINT message	-	Not used

	Usage :			Called from MFC. We set out custom cursors.

   ============================================================*/
{

	BOOL result = FALSE;
	if (m_currentCursor)
	{
		::SetCursor(m_currentCursor);
		result = TRUE;
	}
	else
		result = CDiagramEditor::OnSetCursor(pWnd, nHitTest, message);

	return result;

}

void CUMLEditor::OnRButtonDown(UINT nFlags, CPoint point)
/* ============================================================
	Function :		CUMLEditor::OnRButtonDown
	Description :	Handler for the "WM_RBUTTONDOWN"-message.
	Access :		Protected

	Return :		void
	Parameters :	UINT nFlags		-	Not used
					CPoint point	-	Not used.

	Usage :			Called from MFC. We remove dangling lines.

   ============================================================*/
{

	// The default handler will select leave the current 
	// link - if any - dangling, and so we start by deleting it.
	if (GetSelectCount() == 1)
	{
		CUMLLineSegment* seg = dynamic_cast<CUMLLineSegment*>(GetSelectedObject());
		if (seg && !IsConnected(seg))
			GetDiagramEntityContainer()->Remove(seg);
	}

	CDiagramEditor::OnRButtonDown(nFlags, point);

}

void CUMLEditor::OnRButtonUp(UINT nFlags, CPoint point)
/* ============================================================
	Function :		CUMLEditor::OnRButtonUp
	Description :	Handler for the "WM_RBUTTONUP"-message.
	Access :		Protected

	Return :		void
	Parameters :	UINT nFlags		-	Not used
					CPoint point	-	Not used

	Usage :			Called from MFC. We end the line drawing
					mode.

   ============================================================*/
{

	if (!m_drawingLine)
		CDiagramEditor::OnRButtonUp(nFlags, point);

	m_drawingLine = FALSE;

}

/////////////////////////////////////////////////////////////////////////////
// CUMLEditor private helpers

void CUMLEditor::ModifyLinkedPositions()
/* ============================================================
	Function :		CUMLEditor::ModifyLinkedPositions
	Description :	Will modify the position of all unselected
					items linked to a selected one, with the
					difference between start and current as
					offset.
	Access :		Protected

	Return :		void
	Parameters :	none

	Usage :			Should be called after all operations that
					might move an object. Movement direction is
					dependent on the link type.

   ============================================================*/
{

	CUMLEntity* obj = NULL;
	int count = 0;
	while ((obj = GetObjectAt(count++)))
		if (obj->GetPackage() == GetPackage() && obj->IsSelected())
			GetUMLEntityContainer()->AdjustLinkedObjects(obj);

}

CUMLEntity* CUMLEditor::GetNamedObject(const CString& name) const
/* ============================================================
	Function :		CUMLEditor::GetNamedObject
	Description :	Returns the object with the name attribute
					name.
	Access :		Private

	Return :		CUMLEntity*			-	The object, or
											"NULL" if not found.
	Parameters :	const CString& name	-	The name of the
											object to find.

	Usage :			Call to get the object with the name name,
					if it exists.

   ============================================================*/
{

	CUMLEntity* result = NULL;
	CUMLEntity* obj;
	int count = 0;
	while ((obj = GetObjectAt(count++)))
		if (obj && obj->GetName() == name)
			result = obj;

	return result;

}

CUMLEntity* CUMLEditor::GetAllNamedObject(const CString& name) const
/* ============================================================
	Function :		CUMLEditor::GetNamedObject
	Description :	Returns the object with the name attribute
					name.
	Access :		Private

	Return :		CUMLEntity*			-	The object, or
											"NULL" if not found.
	Parameters :	const CString& name	-	The name of the
											object to find.

	Usage :			Call to get the object with the name name,
					if it exists.

   ============================================================*/
{

	CUMLEditor* const local = const_cast<CUMLEditor* const>(this);

	CString package = GetPackage();
	local->SetPackage(_T("all"));

	CUMLEntity* result = NULL;
	CUMLEntity* obj;
	int count = 0;
	while ((obj = GetObjectAt(count++)))
	{
		if (obj && obj->GetName() == name)
		{
			result = obj;
			count = -2;
		}
	}

	local->SetPackage(package);
	return result;

}

/////////////////////////////////////////////////////////////////////////////
// CUMLEditor implementation

BOOL CUMLEditor::IsConnected(CUMLLineSegment* link) const
/* ============================================================
	Function :		CUMLEditor::IsConnected
	Description :	Checks if the line that "link" is a segment
					of is connected at both ends.
	Access :		Private

	Return :		BOOL					-	"TRUE" if connected at both ends.
	Parameters :	CUMLLineSegment* link	-	Segment to check

	Usage :			Call to see if a line is connected.

   ============================================================*/
{

	CString name = link->GetName();
	BOOL result = FALSE;
	if (link->GetLink(LINK_START).GetLength() && link->GetLink(LINK_END).GetLength())
		result = TRUE;

	return result;

}

CString CUMLEditor::GetPackage() const
/* ============================================================
	Function :		CUMLEditor::GetPackage
	Description :	Gets the current package name.
	Access :		Public

	Return :		CString	-	Current package.
	Parameters :	none

	Usage :			Call to get the current package.

   ============================================================*/
{

	return GetUMLEntityContainer()->GetPackage();

}

void CUMLEditor::SetPackage(const CString& package)
/* ============================================================
	Function :		CUMLEditor::SetPackage
	Description :	Sets the current package name.
	Access :		Public

	Return :		void
	Parameters :	const CString& package	-	New package

	Usage :			Call to set the current package.

   ============================================================*/
{

	GetUMLEntityContainer()->SetPackage(package);

}

CUMLEntity* CUMLEditor::GetObjectAt(INT_PTR index) const
/* ============================================================
	Function :		CUMLEditor::GetObjectAt
	Description :	Gets the object at "index".
	Access :		Private

	Return :		CUMLEntity*	-	Object at index or "NULL" if
									out of bounds.
	Parameters :	int index	-	Index to get object from.

	Usage :			Convenience function to get an already
					casted object.

   ============================================================*/
{

	CUMLEntityContainer* objs = GetUMLEntityContainer();
	CUMLEntity* obj = static_cast<CUMLEntity*>(objs->GetAt(index));

	return obj;

}

BOOL CUMLEditor::PartialLinesSelected() const
/* ============================================================
	Function :		CUMLEditor::PartialLinesSelected
	Description :	Check if only part of a line is selected.
	Access :		Private

	Return :		BOOL	-	"TRUE" if only a part of a line
								is selected.
	Parameters :	none

	Usage :			Call to see if only a part of a line is
					selected.

   ============================================================*/
{

	CUMLEntity* obj;
	int count = 0;
	if (GetSelectCount() != GetObjectCount() && GetSelectCount() > 1)
	{
		while ((obj = GetObjectAt(count++)))
		{
			CUMLLineSegment* line = dynamic_cast<CUMLLineSegment*>(obj);
			if (line && line->IsSelected())
				if (!GetUMLEntityContainer()->LineSelected(line))
					return TRUE;
		}
	}

	return FALSE;

}

void CUMLEditor::SetCursor(CPoint point)
/* ============================================================
	Function :		CUMLEditor::SetCursor
	Description :	Selects the cursor for the position "point".
	Access :		Private

	Return :		void
	Parameters :	CPoint point	-	Position to check.

	Usage :			Call to see what cursor should be displayed
					when the mouse cursor is at position
					"point".

   ============================================================*/
{

	m_currentCursor = NULL;

	CUMLLineSegment* seg = dynamic_cast<CUMLLineSegment*>(GetSelectedObject());
	CUMLLineSegment* draw = dynamic_cast<CUMLLineSegment*>(GetDrawingObject());

	if (seg && GetInteractMode() == MODE_RESIZING)
	{
		ScreenToVirtual(point);

		CUMLEntity* obj;
		int count = 0;
		BOOL overOther = FALSE;
		while ((obj = GetObjectAt(count++)))
		{
			if (obj->GetPackage() == GetPackage())
			{
				CPoint hit = seg->GetRect().BottomRight();
				int linkCode = obj->GetLinkCode(hit);
				if (obj != seg)
				{
					if (linkCode != LINK_NONE)
					{
						if ((seg->IsHorizontal() && (linkCode == LINK_LEFT || linkCode == LINK_RIGHT)) ||
							(!seg->IsHorizontal() && (linkCode == LINK_TOP || linkCode == LINK_BOTTOM)))
						{
							overOther = TRUE;
							count = -2;
						}
					}
				}
			}
		}

		if (overOther)
			m_currentCursor = m_curAttach;
		else
			m_currentCursor = m_curDrawLine;

	}
	else if (draw && GetInteractMode() == MODE_DRAWING)
	{
		BOOL result = FALSE;
		ScreenToVirtual(point);

		CUMLEntity* obj;
		int count = 0;
		while ((obj = GetObjectAt(count++)))
		{
			if (obj->GetPackage() == GetPackage())
			{
				int linkCode = obj->GetLinkCode(point);
				if (linkCode & LINK_ALL)
				{
					m_currentCursor = m_curAttach;
					result = TRUE;
					count = -2;
				}
			}
		}

		if (!result)
			m_currentCursor = LoadCursor(NULL, IDC_ARROW);

	}
	else if (GetInteractMode() == MODE_NONE || GetInteractMode() == MODE_RUBBERBANDING)
	{
		CString package = GetPackage();
		int size = round(20 * GetZoom());
		if (package.GetLength() && point.x < size && point.y < size)
			m_currentCursor = m_curUp;
	}

	if (m_currentCursor)
		::SetCursor(m_currentCursor);

}

void CUMLEditor::SetDisplayOptions(int displayOption)
/* ============================================================
	Function :		CUMLEditor::SetDisplayOptions
	Description :	Sets the current diaply option.
	Access :		Public

	Return :		void
	Parameters :	int displayOption	-	New display options

	Usage :			Call to set the display option.
					It can be a combination of the following:
						"DISPLAY_ALL" Show everything
						"DISPLAY_ONLY_PUBLIC" Show only public attributes and operations.
						"DISPLAY_NO_MARKERS" Don't show access markers
						"DISPLAY_NO_ATTRIBUTES" Don't show attributes
						"DISPLAY_NO_OPERATIONS" Don't show operations.
						"DISPLAY_NO_OPERATION_ATTRIBUTE_NAMES" Don't show names for operation parameters

   ============================================================*/
{

	CUMLEntityContainer* objs = GetUMLEntityContainer();
	if (objs)
		objs->SetDisplayOptions(displayOption);

}

int CUMLEditor::GetDisplayOptions() const
/* ============================================================
	Function :		CUMLEditor::GetDisplayOptions
	Description :	Gets the current display options.
	Access :		Public

	Return :		int		-	Current display option(s)
	Parameters :	none

	Usage :			Call to get the current display options.
					It can be a combination of the following:
						"DISPLAY_ALL" Show everything
						"DISPLAY_ONLY_PUBLIC" Show only public attributes and operations.
						"DISPLAY_NO_MARKERS" Don't show access markers
						"DISPLAY_NO_ATTRIBUTES" Don't show attributes
						"DISPLAY_NO_OPERATIONS" Don't show operations.
						"DISPLAY_NO_OPERATION_ATTRIBUTE_NAMES" Don't show names for operation parameters

   ============================================================*/
{

	int result = 0;
	CUMLEntityContainer* objs = GetUMLEntityContainer();
	if (objs)
		result = objs->GetDisplayOptions();

	return result;

}

CUMLEntityContainer * CUMLEditor::GetUMLEntityContainer() const
/* ============================================================
	Function :		CUMLEditor::GetUMLEntityContainer
	Description :	Get the container as a "CUMLEntityContainer".
	Access :		Private

	Return :		CUMLEntityContainer *	-	The container
	Parameters :	none

	Usage :			Convenience function.

   ============================================================*/
{

	return static_cast<CUMLEntityContainer *>(GetDiagramEntityContainer());

}

CString CUMLEditor::BrowseForFolder()
/* ============================================================
	Function :		CUMLEditor::BrowseForFolder
	Description :	Displays the Windows folder browsing dialog.
	Access :		Private

	Return :		CString	-	Selected folder.
	Parameters :	none

	Usage :			Call to let the user enter a folder (to save
					c++-code to).

   ============================================================*/
{

	LPITEMIDLIST pidl;
	BROWSEINFO bi;
	ZeroMemory(&bi, sizeof(BROWSEINFO));
	CString str;

	CString title;
	title.LoadString(IDS_UML_SELECT_FOLDER);

	_TCHAR initialPath[_MAX_PATH];
	lstrcpy(initialPath, m_exportPath);

	_TCHAR buffer[_MAX_PATH];
	ZeroMemory(buffer, _MAX_PATH);

	bi.hwndOwner = AfxGetMainWnd()->m_hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = NULL;
	bi.lpszTitle = title;
	bi.ulFlags = 0;
	bi.lpfn = NULL;
	bi.lpfn = BFFCallbackProc;
	bi.lParam = (LPARAM)initialPath;

	if ((pidl = ::SHBrowseForFolder(&bi)) != NULL)
	{
		if (SUCCEEDED(::SHGetPathFromIDList(pidl, buffer)))
		{
			m_exportPath = buffer;
			str = buffer;
		}

		LPMALLOC smalloc;
		if (SUCCEEDED(SHGetMalloc(&smalloc)))
		{
			smalloc->Free(pidl);
			smalloc->Release();
		}
	}

	return str;

}

void CUMLEditor::ExportEMF(const CString& filename)
/* ============================================================
	Function :		CUMLEditor::ExportEMF
	Description :	Exports the current diagram to an EMF-file.
	Access :

	Return :		void
	Parameters :	none

	Usage :			Call to export the diagram to an EMF file
					for - for example - inclusion into a
					word-processing document.

   ============================================================*/
{

	CFileDialog dlg(FALSE, _T("emf"), filename, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("Enhanced MetaFile (*.emf)|*.emf|All Files (*.*)|*.*||"));
	if (dlg.DoModal() == IDOK)
	{

		CClientDC	dc(this);
		CMetaFileDC	metaDC;
		CRect rect(0, 0,
			GetVirtualSize().cx,
			GetVirtualSize().cy);

		// Determine the picture frame dimensions.
		// iWidthMM is the display width in millimeters.
		// iHeightMM is the display height in millimeters.
		// iWidthPels is the display width in pixels.
		// iHeightPels is the display height in pixels

		int iWidthMM = GetDeviceCaps(dc, HORZSIZE);
		int iHeightMM = GetDeviceCaps(dc, VERTSIZE);
		int iWidthPels = GetDeviceCaps(dc, HORZRES);
		int iHeightPels = GetDeviceCaps(dc, VERTRES);

		// Convert client coordinates to .01-mm units.
		// Use iWidthMM, iWidthPels, iHeightMM, and
		// iHeightPels to determine the number of
		// .01-millimeter units per pixel in the x-
		//  and y-directions.
		CRect r(
			(rect.left * iWidthMM * 100) / iWidthPels,
			(rect.top * iHeightMM * 100) / iHeightPels,
			(rect.right * iWidthMM * 100) / iWidthPels,
			(rect.bottom * iHeightMM * 100) / iHeightPels);

		metaDC.CreateEnhanced(&dc, dlg.GetPathName(), &r, _T("UMLEditor Drawing"));
		metaDC.SetAttribDC(dc.m_hDC);

		SetRedraw(FALSE);
		COLORREF col = GetBackgroundColor();

		SetBackgroundColor(RGB(255, 255, 255));
		Print(&metaDC, rect, 1);
		SetBackgroundColor(col);
		SetRedraw(TRUE);
		RedrawWindow();

		::DeleteEnhMetaFile(metaDC.CloseEnhanced());

	}

}

void CUMLEditor::ExportCPP(const CString& project, BOOL bHeaderOnly)
/* ============================================================
	Function :		CUMLEditor::ExportCPP
	Description :	Exports the current diagram to c++-code.
	Access :		Private

	Return :		void
	Parameters :	const CString& project	-	Project name

	Usage :			Call to export the diagram to c++-code.

   ============================================================*/
{

	CUMLEntityContainer* container = GetUMLEntityContainer();

	if (container)
	{
		CString location = BrowseForFolder();
		if (location.GetLength())
		{
			BOOL modified0 = container->BaseClassClasses();
			BOOL modified1 = container->AddInterfacesToClasses();
			BOOL modified2 = container->VirtualizeClasses();
			CStringArray stra;
			container->SetStripLeadingClassCharacter(GetStripLeadingClassCharacter());
			container->SetProjectName(project);
			container->SetProjectLocation(location);
			if (!bHeaderOnly)
				container->Export(stra, EXPORT_CPP);
			container->Export(stra, EXPORT_H);
			CString err = container->GetErrorMessage();
			if (err.GetLength())
				AfxMessageBox(err);
			else
			{
				if (modified0 || modified1 || modified2)
				{
					ModifyLinkedPositions();
					SetModified(TRUE);
					RedrawWindow();
				}

				AfxMessageBox(IDS_UML_EXPORT_FINISHED);

			}
		}
	}

}

void CUMLEditor::ExportHTML(const CString& filename)
/* ============================================================
	Function :		CUMLEditor::ExportHTML
	Description :	Export the current diagram to HTML-format.
	Access :		Private

	Return :		void
	Parameters :	const CString& project	-	Project name

	Usage :			Call to export the current diagram to HTML.

   ============================================================*/
{

	CFileDialog dlg(FALSE, _T("html"), filename, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("HTML File (*.html)|*.html;*.htm||All Files (*.*)|*.*||"));
	if (dlg.DoModal() == IDOK)
	{
		CUMLEntityContainer* container = GetUMLEntityContainer();
		if (container)
		{
			CStringArray stra;
			container->Export(stra, EXPORT_HTML);
			CString err = container->GetErrorMessage();
			if (err.IsEmpty())
			{
				CTextFile file(_T("html"), _T("\n"));
				if (!file.WriteTextFile(dlg.GetPathName(), stra))
				{
					if (file.GetErrorMessage().GetLength())
						AfxMessageBox(file.GetErrorMessage());
				}
				else
				{
					CString imagedir(dlg.GetPathName());
					int find = imagedir.ReverseFind(_TCHAR('\\'));
					if (find != -1)
						imagedir = imagedir.Left(find);
					imagedir += _T("\\images");
					CString source(GetApplicationDirectory() + _T("images"));
					CDiskObject cdo;
					if (!cdo.CopyDirectory(source, imagedir))
						AfxMessageBox(cdo.GetErrorMessage());
					else
						AfxMessageBox(IDS_UML_EXPORT_FINISHED);
				}
			}
			else
			{
				AfxMessageBox(err);
			}
		}
	}
}

void CUMLEditor::SetStripLeadingClassCharacter(BOOL stripLeadingClassLetter)
/* ============================================================
	Function :		CUMLEditor::SetStripLeadingClassCharacter
	Description :	Sets the flag for if the leading character
					of a class should be stripped away for the
					filename when exporting to cpp.
	Access :		Public

	Return :		void
	Parameters :	BOOL stripLeadingClassLetter	-	"TRUE" if char
														should be stripped

	Usage :			Call with "TRUE" to generate cpp-filenames
					without the first letter.

   ============================================================*/
{

	m_stripLeadingClassLetter = stripLeadingClassLetter;

}

BOOL CUMLEditor::GetStripLeadingClassCharacter() const
/* ============================================================
	Function :		CUMLEditor::GetStripLeadingClassCharacter
	Description :	Gets if the leading character should be
					removed when generating cpp-filenames.
	Access :		Public

	Return :		BOOL	-	If "TRUE", leading char will be
								removed.
	Parameters :	none

	Usage :			Call to check if the leading char should be
					removed from class filnames when exporting
					to c++.

   ============================================================*/
{

	return m_stripLeadingClassLetter;

}

BOOL CUMLEditor::IsLinkSelected() const
/* ============================================================
	Function :		CUMLEditor::IsLinkSelected
	Description :	Check if a link is selected.
	Access :		Public

	Return :		BOOL	-	"TRUE" if a link is selected
	Parameters :	none

	Usage :			Check to see if a link is selected. Returns
					TRUE if only segments of a single link is
					selected.

   ============================================================*/
{

	BOOL result = FALSE;

	if (GetUMLEntityContainer())
		result = GetUMLEntityContainer()->IsLinkSelected();

	return result;

}

void CUMLEditor::FlipLink()
/* ============================================================
	Function :		CUMLEditor::FlipLink
	Description :	Flips the direction of the currently
					selected link.
	Access :		Public

	Return :		void
	Parameters :	none

	Usage :			Call to flip the currently selected link.

   ============================================================*/
{

	if (IsLinkSelected())
		GetUMLEntityContainer()->FlipLink();

}

void CUMLEditor::Import()
/* ============================================================
	Function :		CUMLEditor::Import
	Description :	Imports data into the selected class object
					from a h-file.
	Access :		Public

	Return :		void
	Parameters :	none

	Usage :			Call to import data from a h-file to a
					selected "CUMLEntityClass".

   ============================================================*/
{

	if (GetUMLEntityContainer())
		GetUMLEntityContainer()->Import();

}

void CUMLEditor::Save(CString& filename)
/* ============================================================
	Function :		CUMLEditor::Save
	Description :	Saves the data to the file "filename"
	Access :

	Return :		void
	Parameters :	CString& filename	-	File to save to

	Usage :			Call to save data. If "filename" is empty,
					a standard Windows file dialog will be
					displayed, and the variable will contain
					the selected filename on return.

   ============================================================*/
{

	if (GetUMLEntityContainer())
		GetUMLEntityContainer()->Save(filename);

}

void CUMLEditor::Load(CString& filename)
/* ============================================================
	Function :		CUMLEditor::Load
	Description :	Loads data from the file "filename"
	Access :

	Return :		void
	Parameters :	CString& filename	-	File to load from

	Usage :			Call to load data. If "filename" is empty,
					a standard Windows file dialog will be
					displayed, and the variable will contain
					the selected filename on return.

   ============================================================*/
{

	if (GetUMLEntityContainer())
		GetUMLEntityContainer()->Load(filename);

}

#pragma warning( default : 4706 )
