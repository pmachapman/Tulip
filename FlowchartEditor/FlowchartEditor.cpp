/* ==========================================================================
	CFlowchartEditor

	Author :		Johan Rosengren, Abstrakt Mekanik AB

	Date :			2004-05-01

	Purpose :		CFlowchartEditor is a flowchart editor, demonstrating
					several concepts of CDiagramEditor:

					1.	Linking objects using a CDiagramEntityContainer-
						derived class to manage data
					2.	Putting CDiagramEditor in a MDI-application, using
						a shared CDiagramClipboard-derived clipboard handler.
					3.	Exporting the diagram to an enhanced metafile that
						can be pasted into - for example - Word.

	Description :	CFlowchartEditor handles several messages and overrides
					drawing to manage links. Links are implemented as a
					special class, stored in a separate array in
					CFlowchartEntityContainer. Object drawing in
					CFlowchartEditor also draws the links.

					The objects derived from CFlowchartEntity has link
					points. Two selected and unlinked objects with free
					link points at appropriate position can be linked. If
					they are linked, a line is automatically drawn between
					the objects, with an arrow head representing the
					direction (which can be flipped).

					The link can have a label, that is, a text describing
					the link.

					When objects are moved, other objects linked to the
					moved ones might also be moved - depending on the links.
					For example, if two objects are linked horizontally and
					one is moved up or down, the linked object will also be
					moved.

					A special type of object is the linkable line. Linkable
					lines are lines that can be linked to other objects,
					even lines. They can be used to represent more complex
					flows, such as several links converging on a single spot.
					Lines will also be moved as other objects, but worth to
					notice is that they will not be resized, they will keep
					their original length and might therefore trigger
					movements far from the current one. Linked lines can
					only be drawn either vertically or horizontally, that is,
					no slant is allowed - this is enforced by the editor
					rather than the line class.

	Usage :			CFlowchartEditor should be instantiated in the same way
					as a CDiagramEditor. The two public member functions
					CanLink and IsLinked can be used for command enablers
					for the commands OnLink, OnUnlink, OnLinkDirection and
					OnLinkProperties. The commands link, unlink, reverse
					direction of the link and sets/edit the link label
					respectively.

					The package uses RTTI (run time type information) to
					identify object types, using dynamic_cast, so the
					project must enable this feature.

   ========================================================================*/

#include "stdafx.h"
#include "FlowchartEditor.h"
#include "FlowchartEntityContainer.h"
#include "FlowchartMenu.h"
#include "FlowchartLinkPropertiesDialog.h"
#include "FlowchartLinkableLineSegment.h"

#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFlowchartEditor

CFlowchartEditor::CFlowchartEditor()
/* ============================================================
	Function :		CFlowchartEditor::CFlowchartEditor
	Description :	constructor

	Return :		void
	Parameters :	none

	Usage :

   ============================================================*/
{

	SetBackgroundColor(RGB(250, 250, 230));
	ShowGrid(FALSE);
	SetSnapToGrid(TRUE);
	SetRestraints(RESTRAINT_VIRTUAL);
	SetPopupMenu(new CFlowchartMenu);

}

CFlowchartEditor::~CFlowchartEditor()
/* ============================================================
	Function :		CFlowchartEditor::~CFlowchartEditor
	Description :	destructor

	Return :		void
	Parameters :	none

	Usage :

   ============================================================*/
{
}

BEGIN_MESSAGE_MAP(CFlowchartEditor, CDiagramEditor)
	ON_COMMAND(IDM_FLOWCHART_LINK, OnLink)
	ON_COMMAND(IDM_FLOWCHART_UNLINK, OnUnlink)
	ON_COMMAND(IDM_FLOWCHART_LINK_DIRECTION, OnLinkDirection)
	ON_COMMAND(IDM_FLOWCHART_LINK_PROPERTIES, OnLinkProperties)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFlowchartEditor overrides

void CFlowchartEditor::DrawObjects(CDC* dc, double zoom) const
/* ============================================================
	Function :		CFlowchartEditor::DrawObjects
	Description :	Draws the object

	Return :		void
	Parameters :	CDC* dc		-	CDC to draw to
					double zoom	-	Zoom level (use this value
									instead of GetZoom())

	Usage :			Overridden to draw links.

   ============================================================*/
{

	CFlowchartEntityContainer* objs = dynamic_cast<CFlowchartEntityContainer *>(GetDiagramEntityContainer());
	if (objs)
	{
		CFont font;
		font.CreateFont(-round(12.0 * zoom), 0, 0, 0, FW_NORMAL, 0, 0, 0, 0, 0, 0, 0, 0, _T("Courier New"));

		INT_PTR count = GetObjectCount();

		INT_PTR max = objs->GetLinks();
		int seg = round((double)GetMarkerSize().cx * zoom / 2);
		POINT pts[3];

		for (INT_PTR i = 0; i < count; i++)
		{

			CDiagramEntity* main = static_cast<CDiagramEntity*>(objs->GetAt(i));
			CFlowchartEntity* obj = dynamic_cast<CFlowchartEntity*>(objs->GetAt(i));
			if (obj)
			{
				for (int t = 0; t < max; t++)
				{
					CFlowchartLink* link = objs->GetLinkAt(t);
					if (link)
					{
						// Get the to/from objects
						CFlowchartEntity* to = GetNamedObject(link->to);
						CFlowchartEntity* from = GetNamedObject(link->from);

						// See if this link needs to be deleted
						if (!to || !from)
						{
							objs->DeleteLink(link);
						}
						else if (link->from == obj->GetName())
						{
							// Otherwise, if this is the link we are looking for
							dc->SelectStockObject(BLACK_PEN);
							dc->SelectStockObject(BLACK_BRUSH);

							BOOL drawArrow = TRUE;
							CPoint start;
							CPoint end;
							start = obj->GetLinkPosition(link->fromtype);
							end = to->GetLinkPosition(link->totype);

							start.x = round((double)start.x * zoom);
							start.y = round((double)start.y * zoom);
							end.x = round((double)end.x * zoom);
							end.y = round((double)end.y * zoom);

							switch (link->fromtype)
							{
							case LINK_RIGHT:
								pts[1].x = end.x - seg * 2;
								pts[1].y = end.y - seg;
								pts[2].x = end.x - seg * 2;
								pts[2].y = end.y + seg;
								break;

							case LINK_LEFT:
								pts[1].x = end.x + seg * 2;
								pts[1].y = end.y - seg;
								pts[2].x = end.x + seg * 2;
								pts[2].y = end.y + seg;
								break;

							case LINK_TOP:
								pts[1].x = end.x - seg;
								pts[1].y = end.y + seg * 2;
								pts[2].x = end.x + seg;
								pts[2].y = end.y + seg * 2;
								break;

							case LINK_BOTTOM:
								pts[1].x = end.x - seg;
								pts[1].y = end.y - seg * 2;
								pts[2].x = end.x + seg;
								pts[2].y = end.y - seg * 2;

								break;

							default:
							{
								switch (link->totype)
								{
								case LINK_RIGHT:
									pts[1].x = end.x + seg * 2;
									pts[1].y = end.y - seg;
									pts[2].x = end.x + seg * 2;
									pts[2].y = end.y + seg;
									break;

								case LINK_LEFT:
									pts[1].x = end.x - seg * 2;
									pts[1].y = end.y - seg;
									pts[2].x = end.x - seg * 2;
									pts[2].y = end.y + seg;
									break;

								case LINK_TOP:
									pts[1].x = end.x - seg;
									pts[1].y = end.y - seg * 2;
									pts[2].x = end.x + seg;
									pts[2].y = end.y - seg * 2;
									break;

								case LINK_BOTTOM:
									pts[1].x = end.x - seg;
									pts[1].y = end.y + seg * 2;
									pts[2].x = end.x + seg;
									pts[2].y = end.y + seg * 2;
									break;

								default:
									drawArrow = FALSE;
									break;
								}
							}
							break;
							}

							dc->MoveTo(start);
							dc->LineTo(end);

							pts[0].x = end.x;
							pts[0].y = end.y;

							if (drawArrow)
								dc->Polygon(pts, 3);

							CString str = link->title;
							if (str.GetLength())
							{
								dc->SelectObject(&font);
								int mode = dc->SetBkMode(TRANSPARENT);

								CRect rect(start, end);
								rect.NormalizeRect();
								int cy = round(14.0 * zoom);
								int cut = round((double)GetMarkerSize().cx * zoom / 2);
								CRect r(rect.right - cut, rect.top, rect.right - (rect.Width() + cut), rect.bottom);
								if (rect.top == rect.bottom)
								{
									CRect r(rect.left, rect.top - (cy + cut), rect.right, rect.bottom);
									r.NormalizeRect();
									dc->DrawText(str, r, DT_NOPREFIX | DT_SINGLELINE | DT_VCENTER | DT_CENTER);
								}
								else
								{
									CRect r(rect.right - cut, rect.top, rect.right - (cy * str.GetLength() + cut), rect.bottom);
									r.NormalizeRect();
									dc->DrawText(str, r, DT_NOPREFIX | DT_SINGLELINE | DT_VCENTER | DT_RIGHT);
								}

								dc->SetBkMode(mode);
								dc->SelectStockObject(DEFAULT_GUI_FONT);
							}
						}
					}
				}
			}

			main->DrawObject(dc, zoom);
		}
	}

}

void CFlowchartEditor::ExportEMF(const CString& filename)
/* ============================================================
	Function :		CFlowchartEditor::ExportEMF
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

		// Himetric rect
		CRect r(0, 0, 8 * 2540, 11 * 2540);

		metaDC.CreateEnhanced(&dc, dlg.GetPathName(), &r, _T("FlowchartEditor Drawing"));
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

void CFlowchartEditor::SaveObjects(CStringArray& stra)
/* ============================================================
	Function :		CFlowchartEditor::SaveObjects
	Description :	Saves all the objects to a CStringArray.

	Return :		void
	Parameters :	CStringArray& stra	-	CStringArray to
											save to.

	Usage :			Overridden to save the links as well.

   ============================================================*/
{

	CDiagramEditor::SaveObjects(stra);
	CFlowchartEntityContainer* objs = dynamic_cast<CFlowchartEntityContainer *>(GetDiagramEntityContainer());
	if (objs)
	{
		INT_PTR max = objs->GetLinks();
		for (INT_PTR t = 0; t < max; t++)
		{
			CFlowchartLink* link = objs->GetLinkAt(t);
			stra.Add(link->GetString());
		}
	}

}

/////////////////////////////////////////////////////////////////////////////
// CFlowchartEditor message handlers

void CFlowchartEditor::OnMouseMove(UINT nFlags, CPoint point)
/* ============================================================
	Function :		CFlowchartEditor::OnMouseMove
	Description :	Handler for WM_MOUSEMOVE. We add handling
					to close screen redraws as we are moving
					lots of stuff (all the attached objects in
					addition to the primarily moved). We also
					do secondary movements here - that is,
					objects linked to the one(s) being moved
					must be moved as well, even if they are
					not selected.

	Return :		void
	Parameters :	UINT nFlags		-	Not used
					CPoint point	-	Not used

	Usage :			Called from MFC

   ============================================================*/
{
	if ((GetInteractMode() == MODE_MOVING) || (GetInteractMode() == MODE_RESIZING && GetSelectCount() == 1))
		SetRedraw(FALSE);

	CDiagramEditor::OnMouseMove(nFlags, point);

	if (GetInteractMode() == MODE_RESIZING)
	{
		if (GetSelectCount() == 1)
		{
			CFlowchartLinkableLineSegment* obj = dynamic_cast<CFlowchartLinkableLineSegment*>(GetSelectedObject());
			if (obj)
			{
				if (obj->GetLeft() != obj->GetRight() && obj->GetTop() != obj->GetBottom())
				{
					if (fabs(obj->GetLeft() - obj->GetRight()) > fabs(obj->GetTop() - obj->GetBottom()))
						obj->SetBottom(obj->GetTop());
					else
						obj->SetRight(obj->GetLeft());
				}
			}
		}

	}

	if (GetInteractMode() == MODE_MOVING || GetInteractMode() == MODE_RESIZING)
	{
		ModifyLinkedPositions();
		SetRedraw(TRUE);
		RedrawWindow();
	}

}

void CFlowchartEditor::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
/* ============================================================
	Function :		CFlowchartEditor::OnKeyDown
	Description :	Handles the WM_KEYDOWN-message. We
					override this as objects might be moved and
					we will have to move unlinked objects as
					well.

	Return :		void
	Parameters :	UINT nChar		-	Not used
					UINT nRepCnt	-	Not used
					UINT nFlags		-	Not used

	Usage :			Called from MFC

   ============================================================*/
{

	SetRedraw(FALSE);
	CDiagramEditor::OnKeyDown(nChar, nRepCnt, nFlags);

	ModifyLinkedPositions();
	SetRedraw(TRUE);
	RedrawWindow();

}

/////////////////////////////////////////////////////////////////////////////
// CFlowchartEditor private helpers

void CFlowchartEditor::ModifyLinkedPositions()
/* ============================================================
	Function :		CFlowchartEditor::ModifyLinkedPositions
	Description :	Will modify the position of all unselected
					items linked to a selected one, with the
					difference between start and current as
					offset. Movement direction is dependent on
					the link type.

	Return :		void
	Parameters :	none

	Usage :			Should be called after all operations that
					might move an object.

   ============================================================*/
{

	INT_PTR max = GetObjectCount();
	CFlowchartEntity* obj;
	for (int t = 0; t < max; t++)
	{
		obj = dynamic_cast<CFlowchartEntity*>(GetObject(t));
		if (obj && obj->IsSelected())
			AdjustLinkedObjects(obj);
	}

	max = GetObjectCount();
	for (int t = 0; t < max; t++)
	{
		obj = dynamic_cast<CFlowchartEntity*>(GetObject(t));
		if (obj)
			obj->SetMoved(FALSE);
	}

}

void CFlowchartEditor::AdjustLinkedObjects(CFlowchartEntity* parent, CFlowchartEntity* filter)
/* ============================================================
	Function :		CFlowchartEditor::AdjustLinkedObjects
	Description :	The function adjusts all objects attached
					to parent, except filter (filter is assumed
					to be last iteration's parent).

	Return :		void
	Parameters :	CFlowchartEntity* parent	-	The object to
													find attached
													links to.
					CFlowchartEntity* filter	-	Object attached
													to parent that
													is not to be
													moved.

	Usage :			The function is called to recursively adjust
					the positions of objects attached to an
					object being moved.

   ============================================================*/
{

	parent->SetMoved(TRUE);
	BOOL moved = FALSE;

	CString name1 = parent->GetName();
	CString name2;

	double sizediff = 0.0;

	CFlowchartEntityContainer* objs = dynamic_cast<CFlowchartEntityContainer *>(GetDiagramEntityContainer());
	if (objs)
	{
		INT_PTR max = objs->GetLinks();
		for (INT_PTR t = 0; t < max; t++)
		{
			CFlowchartLink* link = objs->GetLinkAt(t);
			if (link)
			{
				int fromtype = link->fromtype;
				int totype = link->totype;

				name2 = _T("");
				if (link->from == parent->GetName())
					name2 = link->to;
				else if (link->to == parent->GetName())
				{
					fromtype = link->totype;
					totype = link->fromtype;
					name2 = link->from;
				}

				if (name2.GetLength())
				{
					CFlowchartEntity* obj = GetNamedObject(name2);
					if (obj && obj != filter && !obj->GetMoved())
					{
						CPoint source;
						CPoint target;

						source = parent->GetLinkPosition(fromtype);
						target = obj->GetLinkPosition(totype);

						moved = FALSE;
						switch (fromtype)
						{
						case LINK_LEFT:
							sizediff = source.y - target.y;
							obj->MoveRect(0, source.y - target.y);
							break;
						case LINK_RIGHT:
							sizediff = source.y - target.y;
							obj->MoveRect(0, source.y - target.y);
							break;
						case LINK_TOP:
							sizediff = source.x - target.x;
							obj->MoveRect(source.x - target.x, 0);
							break;
						case LINK_BOTTOM:
							sizediff = source.x - target.x;
							obj->MoveRect(source.x - target.x, 0);
							break;
						default:
							switch (totype)
							{
							case LINK_LEFT:
								sizediff = source.y - target.y;
								obj->MoveRect(0, source.y - target.y);
								break;
							case LINK_RIGHT:
								sizediff = source.y - target.y;
								obj->MoveRect(0, source.y - target.y);
								break;
							case LINK_TOP:
								sizediff = source.x - target.x;
								obj->MoveRect(source.x - target.x, 0);
								break;
							case LINK_BOTTOM:
								sizediff = source.x - target.x;
								obj->MoveRect(source.x - target.x, 0);
								break;
							}
							break;
						}

						if (sizediff)
							AdjustLinkedObjects(obj, parent);

					}
				}
			}
		}
	}

}

CFlowchartEntity* CFlowchartEditor::GetNamedObject(const CString& name) const
/* ============================================================
	Function :		CFlowchartEditor::GetNamedObject
	Description :	Returns the object with the name attribute
					name.

	Return :		CFlowchartEntity*	-	The object, or NULL
											if not found.
	Parameters :	const CString& name	-	The name of the
											object to find.

	Usage :			Call to get the object with the name name,
					if it exists.

   ============================================================*/
{

	CDiagramEntity* result = NULL;

	INT_PTR count = GetObjectCount();
	CDiagramEntity* obj;
	for (int t = 0; t < count; t++)
	{
		obj = GetObject(t);
		if (obj && obj->GetName() == name)
			result = obj;
	}

	return dynamic_cast<CFlowchartEntity*>(result);

}

/////////////////////////////////////////////////////////////////////////////
// CFlowchartEditor command handlers

void CFlowchartEditor::OnLink()
/* ============================================================
	Function :		CFlowchartEditor::OnLink
	Description :	Command handler for the Link command.

	Return :		void
	Parameters :	none

	Usage :			Called internally.

   ============================================================*/
{

	if (CanLink())
	{
		// Get the two selected objects 
		CFlowchartEntityContainer* objs = dynamic_cast<CFlowchartEntityContainer *>(GetDiagramEntityContainer());
		CFlowchartEntity* primary = objs->GetPrimarySelected();
		CFlowchartEntity* secondary = objs->GetSecondarySelected();

		if (primary && secondary)
		{
			GetDiagramEntityContainer()->Snapshot();

			// Link them
			if (objs->CreateLink(primary, secondary, _T("")))
			{
				// Move other objects already linked to the secondary
				AdjustLinkedObjects(primary);

				INT_PTR max = GetObjectCount();
				for (int t = 0; t < max; t++)
				{
					CFlowchartEntity* obj = dynamic_cast<CFlowchartEntity*>(GetObject(t));
					if (obj)
						obj->SetMoved(FALSE);
				}

				RedrawWindow();
			}
		}
	}
}

void CFlowchartEditor::OnUnlink()
/* ============================================================
	Function :		CFlowchartEditor::OnUnlink
	Description :	Command handler for the Break link command.

	Return :		void
	Parameters :	none

	Usage :			Called internally.

   ============================================================*/
{

	if (IsLinked())
	{
		// Get the two selected objects 
		CFlowchartEntityContainer* objs = dynamic_cast<CFlowchartEntityContainer *>(GetDiagramEntityContainer());
		CDiagramEntity* primary = objs->GetPrimaryLink();;
		CDiagramEntity* secondary = objs->GetSecondaryLink();
		if (primary && secondary)
		{
			CFlowchartLink* link = objs->FindLink(primary, secondary);
			if (link)
			{
				GetDiagramEntityContainer()->Snapshot();
				objs->DeleteLink(link);
				RedrawWindow();
			}
		}
	}

}

void CFlowchartEditor::OnLinkDirection()
/* ============================================================
	Function :		CFlowchartEditor::OnLinkDirection
	Description :	Command handler for the Flip link direction
					command.

	Return :		void
	Parameters :	none

	Usage :			Called internally.

   ============================================================*/
{

	if (IsLinked())
	{
		// Get the two selected objects 
		CFlowchartEntityContainer* objs = dynamic_cast<CFlowchartEntityContainer *>(GetDiagramEntityContainer());
		CFlowchartEntity* primary = objs->GetPrimaryLink();
		CFlowchartEntity* secondary = objs->GetSecondaryLink();
		if (primary && secondary)
		{
			CFlowchartLink* link = objs->FindLink(primary, secondary);

			if (link)
			{
				GetDiagramEntityContainer()->Snapshot();

				CString saved = link->from;
				link->from = link->to;
				link->to = saved;

				int fromtype = link->fromtype;
				int totype = link->totype;
				link->fromtype = totype;
				link->totype = fromtype;

				RedrawWindow();
			}
		}
	}
}

void CFlowchartEditor::OnLinkProperties()
/* ============================================================
	Function :		CFlowchartEditor::OnLinkProperties
	Description :	Command handler for the Link title command.

	Return :		void
	Parameters :	none

	Usage :			Called internally.

   ============================================================*/
{

	if (IsLinked())
	{
		// Get the two selected objects 
		CFlowchartEntityContainer* objs = dynamic_cast<CFlowchartEntityContainer *>(GetDiagramEntityContainer());
		CFlowchartEntity* primary = objs->GetPrimaryLink();
		CFlowchartEntity* secondary = objs->GetSecondaryLink();
		if (primary && secondary)
		{
			CFlowchartLink* link = objs->FindLink(primary, secondary);
			if (link)
			{
				CFlowchartLinkPropertiesDialog	dlg;
				dlg.m_linkTitle = link->title;
				if (dlg.DoModal())
				{
					GetDiagramEntityContainer()->Snapshot();
					link->title = dlg.m_linkTitle;

					RedrawWindow();
					SetFocus();
				}
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CFlowchartEditor implementation

BOOL CFlowchartEditor::CanLink()
/* ============================================================
	Function :		CFlowchartEditor::CanLink
	Description :	Will check if the currently selected items
					can be linked to each other.

	Return :		BOOL	-	TRUE if the currently selected
								items can be linked.
	Parameters :	none

	Usage :			Call from - for example - a command enabler
					to see if linking is currently possible.

   ============================================================*/
{

	BOOL result = FALSE;
	CFlowchartEntityContainer* objs = dynamic_cast<CFlowchartEntityContainer *>(GetDiagramEntityContainer());
	if (objs)
		result = objs->CanLink();

	return result;

}

BOOL CFlowchartEditor::IsLinked()
/* ============================================================
	Function :		CFlowchartEditor::IsLinked
	Description :	Checks if the currently selected items are
					linked to each other.

	Return :		BOOL	-	TRUE if they are linked.
	Parameters :	none

	Usage :			Call from - for example - a command enabler
					to see if the currently selected items are
					linked.

   ============================================================*/
{

	BOOL result = FALSE;
	CFlowchartEntityContainer* objs = dynamic_cast<CFlowchartEntityContainer *>(GetDiagramEntityContainer());
	if (objs)
		result = objs->IsLinked();
	return result;

}
