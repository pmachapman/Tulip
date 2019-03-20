/* ==========================================================================
	CFlowchartEntityContainer

	Author :		Johan Rosengren, Abstrakt Mekanik AB

	Date :			2004-04-29

	Purpose :		CFlowchartEntityContainer is a CDiagramEntityContainer-
					derived class, holding the data for a CFlowchartEditor.
					In addition to CDiagramEntityContainer, this class keeps
					and manages a list of links. This includes copy/paste
					and undo-handling.

	Description :	The class uses a derived CDiagramClipboardHandler.

	Usage :			Use as a normal CDiagramEntityContainer class. The
					editor class exposes functions for command enabling.

   ========================================================================*/
#include "stdafx.h"
#include "FlowchartEntityContainer.h"
#include <math.h>

CFlowchartEntityContainer::CFlowchartEntityContainer()
/* ============================================================
	Function :		CFlowchartEntityContainer::CFlowchartEntityContainer
	Description :	constructor

	Return :		void
	Parameters :	none

	Usage :

   ============================================================*/
{
}

CFlowchartEntityContainer::~CFlowchartEntityContainer()
/* ============================================================
	Function :		CFlowchartEntityContainer::~CFlowchartEntityContainer
	Description :	destructor

	Return :		void
	Parameters :	none

	Usage :

   ============================================================*/
{
	ClearLinks();
	ClearUndo();
	ClearRedo();
}

BOOL CFlowchartEntityContainer::CreateLink(CFlowchartEntity* from, CFlowchartEntity* to, const CString& title)
/* ============================================================
	Function :		CFlowchartEntityContainer::CreateLink
	Description :	Creates a link between from and to and
					puts it into the link-array.

	Return :		BOOL					-	TRUE if ok.
	Parameters :	CFlowchartEntity* from	-	From-object.
					CFlowchartEntity* to	-	To-object.
					const CString& title	-	Title of link.

	Usage :			Call to create a link between from and to.

   ============================================================*/
{

	BOOL result = FALSE;
	int	fromtype = 0;
	int	totype = 0;
	CPoint source;
	CPoint target;

	if (FindClosestLink(from, to, fromtype, totype))
	{

		CFlowchartLink* link = new CFlowchartLink;

		link->from = from->GetName();
		link->to = to->GetName();
		link->fromtype = fromtype;
		link->totype = totype;
		link->title = title;

		m_links.Add(link);
		result = TRUE;
	}

	return result;

}

BOOL CFlowchartEntityContainer::HasLinks(CFlowchartEntity* obj1, CFlowchartEntity* obj2)
/* ============================================================
	Function :		CFlowchartEntityContainer::HasLinks
	Description :	Returns TRUE if obj1 and obj2 are linked
					to each other or another object using the
					link-point closest between them..

	Return :		BOOL					-	TRUE if obj1 and
												obj2 has a link
	Parameters :	CFlowchartEntity* obj1	-	First object to
												test
					CFlowchartEntity* obj2	-	Second object
												to test

	Usage :			Call to see if it is possible to link two
					objects, or they already have one or both
					link-points attached.

   ============================================================*/
{

	BOOL result = FALSE;
	CString n1 = obj1->GetName();
	CString n2 = obj2->GetName();

	int	fromtype = 0;
	int	totype = 0;
	if (FindClosestLink(obj1, obj2, fromtype, totype))
	{
		if (fromtype & LINK_ALL && totype & LINK_ALL)
		{
			// Box-objects
			INT_PTR max = m_links.GetSize();
			for (INT_PTR t = 0; t < max; t++)
			{
				CFlowchartLink* link = static_cast<CFlowchartLink*>(m_links.GetAt(t));
				if ((link->fromtype == fromtype && link->from == n1) || (link->totype == totype && link->to == n2))
					result = TRUE;
			}
		}
		else
		{
			// Line-objects
			INT_PTR max = m_links.GetSize();
			for (INT_PTR t = 0; t < max; t++)
			{
				CFlowchartLink* link = static_cast<CFlowchartLink*>(m_links.GetAt(t));
				if (((link->fromtype == fromtype && link->from == n1) && (link->totype == totype && link->to == n2)) ||
					((link->fromtype == totype && link->to == n1) && (link->totype == fromtype && link->from == n2)))
					result = TRUE;
			}
		}
	}

	return result;

}

BOOL CFlowchartEntityContainer::FindClosestLink(CFlowchartEntity* obj1, CFlowchartEntity* obj2, int& fromtype, int& totype)
/* ============================================================
	Function :		CFlowchartEntityContainer::FindClosestLink
	Description :	Finds the closet link types between two
					objects.

	Return :		BOOL					-	TRUE if the
												objects can be
												linked.
	Parameters :	CFlowchartEntity* obj1	-	First object
												to link
					CFlowchartEntity* obj2	-	Second object
												to link
					int& fromtype			-	Type of link for the first object
					int& totype				-	Type of link for the second object.

	Usage :			The link types can be:
					LINK_TOP		Top of the object.
					LINK_BOTTOM		Bottom of the object.
					LINK_LEFT		To the left of the object.
					LINK_RIGHT		To the right of the object.
					LINK_START		To the start of the line (normally
									the top-left corner of
									the non-normalized bounding
									rect).
					LINK_END		To the end of the line
									(normally the bottom-right
									corner of the non-normalized
									bounding rect).

   ============================================================*/
{

	BOOL result = TRUE;
	CPoint start;
	CPoint end;
	double diff2 = 0;
	double diff1 = 0x7FFFFFFF;

	// We go through all the allowed links for obj1, and get the 
	// distance between the inspected link point and the allowed 
	// link points of obj2. Shortest distance wins!

	if ((obj1->AllowLink() & LINK_LEFT))
	{
		start = obj1->GetLinkPosition(LINK_LEFT);
		if ((obj2->AllowLink() & LINK_START) && (obj2->AllowLink() & LINK_END))
		{
			end = obj2->GetLinkPosition(LINK_START);
			diff2 = Dist(start, end);
			if (diff2 <= diff1 && start.x > end.x)
			{
				fromtype = LINK_LEFT;
				totype = LINK_START;
				diff1 = diff2;
			}

			end = obj2->GetLinkPosition(LINK_END);
			diff2 = Dist(start, end);
			if (diff2 <= diff1 && start.x > end.x)
			{
				fromtype = LINK_LEFT;
				totype = LINK_END;
				diff1 = diff2;
			}
		}
		else
		{
			end = obj2->GetLinkPosition(LINK_RIGHT);
			diff2 = Dist(start, end);
			if (diff2 <= diff1 && start.x > end.x)
			{
				fromtype = LINK_LEFT;
				totype = LINK_RIGHT;
				diff1 = diff2;
			}
		}
	}

	if ((obj1->AllowLink() & LINK_RIGHT))
	{
		start = obj1->GetLinkPosition(LINK_RIGHT);
		if ((obj2->AllowLink() & LINK_START) && (obj2->AllowLink() & LINK_END))
		{
			end = obj2->GetLinkPosition(LINK_START);
			diff2 = Dist(start, end);
			if (diff2 <= diff1 && end.x > start.x)
			{
				fromtype = LINK_RIGHT;
				totype = LINK_START;
				diff1 = diff2;
			}

			end = obj2->GetLinkPosition(LINK_END);
			diff2 = Dist(start, end);
			if (diff2 <= diff1 && end.x > start.x)
			{
				fromtype = LINK_RIGHT;
				totype = LINK_END;
				diff1 = diff2;
			}
		}
		else
		{
			end = obj2->GetLinkPosition(LINK_LEFT);
			diff2 = Dist(start, end);
			if (diff2 <= diff1 && end.x > start.x)
			{
				fromtype = LINK_RIGHT;
				totype = LINK_LEFT;
				diff1 = diff2;
			}
		}
	}

	if ((obj1->AllowLink() & LINK_TOP))
	{
		start = obj1->GetLinkPosition(LINK_TOP);
		if ((obj2->AllowLink() & LINK_START) && (obj2->AllowLink() & LINK_END))
		{
			end = obj2->GetLinkPosition(LINK_START);
			diff2 = Dist(start, end);
			if (diff2 < diff1 && start.y > end.y)
			{
				fromtype = LINK_TOP;
				totype = LINK_START;
				diff1 = diff2;
			}
			end = obj2->GetLinkPosition(LINK_END);
			diff2 = Dist(start, end);
			if (diff2 < diff1 && start.y > end.y)
			{
				fromtype = LINK_TOP;
				totype = LINK_END;
				diff1 = diff2;
			}
		}
		else
		{
			end = obj2->GetLinkPosition(LINK_BOTTOM);
			diff2 = Dist(start, end);
			if (diff2 < diff1 && start.y > end.y)
			{
				fromtype = LINK_TOP;
				totype = LINK_BOTTOM;
				diff1 = diff2;
			}
		}
	}

	if ((obj1->AllowLink() & LINK_BOTTOM))
	{
		start = obj1->GetLinkPosition(LINK_BOTTOM);
		if ((obj2->AllowLink() & LINK_START) && (obj2->AllowLink() & LINK_END))
		{
			end = obj2->GetLinkPosition(LINK_START);
			diff2 = Dist(start, end);
			if (diff2 <= diff1 && end.y > start.y)
			{
				fromtype = LINK_BOTTOM;
				totype = LINK_START;
				diff1 = diff2;
			}
			end = obj2->GetLinkPosition(LINK_END);
			diff2 = Dist(start, end);
			if (diff2 <= diff1 && end.y > start.y)
			{
				fromtype = LINK_BOTTOM;
				totype = LINK_END;
				diff1 = diff2;
			}
		}
		else
		{
			end = obj2->GetLinkPosition(LINK_TOP);
			diff2 = Dist(start, end);
			if (diff2 <= diff1 && end.y > start.y)
			{
				fromtype = LINK_BOTTOM;
				totype = LINK_TOP;
				diff1 = diff2;
			}
		}
	}

	int sum2 = 0;
	if ((obj1->AllowLink() & LINK_START))
	{
		start = obj1->GetLinkPosition(LINK_START);

		if (obj2->AllowLink() & LINK_START)
		{
			end = obj2->GetLinkPosition(LINK_START);
			diff2 = Dist(start, end);
			if (diff2 < diff1)
			{
				fromtype = LINK_START;
				totype = LINK_START;
				diff1 = diff2;
			}
		}

		if (obj2->AllowLink() & LINK_END)
		{
			end = obj2->GetLinkPosition(LINK_END);
			diff2 = Dist(start, end);
			if (diff2 < diff1)
			{
				fromtype = LINK_START;
				totype = LINK_END;
				diff1 = diff2;
			}
		}

		if (obj2->AllowLink() & LINK_LEFT)
		{
			end = obj2->GetLinkPosition(LINK_LEFT);
			diff2 = Dist(start, end);
			if (diff2 < diff1 && start.x <= end.x)
			{
				fromtype = LINK_START;
				totype = LINK_LEFT;
				diff1 = diff2;
			}
		}

		if (obj2->AllowLink() & LINK_RIGHT)
		{
			end = obj2->GetLinkPosition(LINK_RIGHT);
			diff2 = Dist(start, end);
			if (diff2 < diff1 && start.x >= end.x)
			{
				fromtype = LINK_START;
				totype = LINK_RIGHT;
				diff1 = diff2;
			}
		}

		if (obj2->AllowLink() & LINK_TOP)
		{
			end = obj2->GetLinkPosition(LINK_TOP);
			diff2 = Dist(start, end);
			if (diff2 < diff1 && start.y <= end.y)
			{
				fromtype = LINK_START;
				totype = LINK_TOP;
				diff1 = diff2;
			}
		}

		if (obj2->AllowLink() & LINK_BOTTOM)
		{
			end = obj2->GetLinkPosition(LINK_BOTTOM);
			diff2 = Dist(start, end);
			if (diff2 < diff1 && start.y >= end.y)
			{
				fromtype = LINK_START;
				totype = LINK_BOTTOM;
				diff1 = diff2;
			}
		}

	}

	if ((obj1->AllowLink() & LINK_END))
	{
		start = obj1->GetLinkPosition(LINK_END);
		if (obj2->AllowLink() & LINK_START)
		{
			end = obj2->GetLinkPosition(LINK_START);
			diff2 = Dist(start, end);
			if (diff2 < diff1)
			{
				fromtype = LINK_END;
				totype = LINK_START;
				diff1 = diff2;
			}
		}

		if (obj2->AllowLink() & LINK_END)
		{
			end = obj2->GetLinkPosition(LINK_END);
			diff2 = Dist(start, end);
			if (diff2 < diff1)
			{
				fromtype = LINK_END;
				totype = LINK_END;
				diff1 = diff2;
			}
		}

		if (obj2->AllowLink() & LINK_LEFT)
		{
			end = obj2->GetLinkPosition(LINK_LEFT);
			diff2 = Dist(start, end);
			if (diff2 < diff1 && start.x <= end.x)
			{
				fromtype = LINK_END;
				totype = LINK_LEFT;
				diff1 = diff2;
			}
		}

		if (obj2->AllowLink() & LINK_RIGHT)
		{
			end = obj2->GetLinkPosition(LINK_RIGHT);
			diff2 = Dist(start, end);
			if (diff2 < diff1 && start.x >= end.x)
			{
				fromtype = LINK_END;
				totype = LINK_RIGHT;
				diff1 = diff2;
			}
		}

		if (obj2->AllowLink() & LINK_TOP)
		{
			end = obj2->GetLinkPosition(LINK_TOP);
			diff2 = Dist(start, end);
			if (diff2 < diff1 && start.y <= end.y)
			{
				fromtype = LINK_END;
				totype = LINK_TOP;
				diff1 = diff2;
			}
		}

		if (obj2->AllowLink() & LINK_BOTTOM)
		{
			end = obj2->GetLinkPosition(LINK_BOTTOM);
			diff2 = Dist(start, end);
			if (diff2 < diff1 && start.y >= end.y)
			{
				fromtype = LINK_END;
				totype = LINK_BOTTOM;
				diff1 = diff2;
			}
		}

	}

	// To be really, really sure
	if (!(obj1->AllowLink() & fromtype))
	{
		result = FALSE;
		fromtype = 0;
	}

	if (!(obj2->AllowLink() & totype))
	{
		result = FALSE;
		totype = 0;
	}

	return result;

}

CFlowchartLink* CFlowchartEntityContainer::GetLinkAt(INT_PTR index)
/* ============================================================
	Function :		CFlowchartEntityContainer::GetLinkAt
	Description :	Returns the CFlowchartLink object at
					position index in the internal data array.

	Return :		CFlowchartLink*	-	Object at index. NULL
										if not a CFlowchartLink
										or out of bounds.
	Parameters :	int index		-	The index in the data
										array.

	Usage :			Call to get the object at index, or to check
					if the object is a CFlowchartLink-object.

   ============================================================*/
{

	CFlowchartLink* result = NULL;
	if (index < GetLinks())
		result = static_cast<CFlowchartLink*>(m_links[index]);

	return result;

}

INT_PTR CFlowchartEntityContainer::GetLinks()
/* ============================================================
	Function :		CFlowchartEntityContainer::GetLinks
	Description :	Returns the number of links in the link
					array.

	Return :		int		-	The current number of links.
	Parameters :	none

	Usage :			Call to get the current number of links.

   ============================================================*/
{

	return m_links.GetSize();

}

void CFlowchartEntityContainer::RemoveAt(int index)
/* ============================================================
	Function :		CFlowchartEntityContainer::RemoveAt
	Description :	Removes the object at index. Will also
					remove all links refering to this object.

	Return :		void
	Parameters :	int index	-	Index for object to remove.

	Usage :			Overridden to remove links as well.

   ============================================================*/
{

	CDiagramEntity* obj = GetAt(index);
	if (obj)
	{
		CString name = obj->GetName();
		CDiagramEntityContainer::RemoveAt(index);

		INT_PTR max = m_links.GetUpperBound();
		for (INT_PTR t = max; t >= 0; t--)
		{
			CFlowchartLink* link = static_cast<CFlowchartLink*>(m_links.GetAt(t));
			if (link->to == name || link->from == name)
			{
				delete link;
				m_links.RemoveAt(t);
			}
		}
	}

}

void CFlowchartEntityContainer::AddLink(CFlowchartLink* link)
/* ============================================================
	Function :		CFlowchartEntityContainer::AddLink
	Description :	Adds a links to the link array.

	Return :		void
	Parameters :	CFlowchartLink* link	-	Link to add

	Usage :			Call to add a link to the link array.

   ============================================================*/
{

	m_links.Add(link);

}

CFlowchartLink* CFlowchartEntityContainer::FindLink(CDiagramEntity* obj1, CDiagramEntity* obj2)
/* ============================================================
	Function :		CFlowchartEntityContainer::FindLink
	Description :	Finds a link between obj1 and obj2.

	Return :		CFlowchartLink*			-	Link between obj1
												and obj2, or NULL
												if they are not
												linked.
	Parameters :	CDiagramEntity* obj1	-	First object to
												test.
					CDiagramEntity* obj2	-	Second object
												to test.

	Usage :			Call to get the link between obj1 and obj2,
					or to check if obj1 and obj2 are linked.

   ============================================================*/
{

	CFlowchartLink* result = NULL;

	if (obj1 && obj2)
	{

		INT_PTR max = m_links.GetUpperBound();
		for (INT_PTR t = max; t >= 0; t--)
		{
			CFlowchartLink* link = static_cast<CFlowchartLink*>(m_links.GetAt(t));
			if ((link->from == obj1->GetName() && link->to == obj2->GetName()) || (link->from == obj2->GetName() && link->to == obj1->GetName()))
				result = link;
		}
	}

	return result;

}

void CFlowchartEntityContainer::DeleteLink(CFlowchartLink* inlink)
/* ============================================================
	Function :		CFlowchartEntityContainer::DeleteLink
	Description :	Finds and deletes inlink from the internal
					link array.

	Return :		void
	Parameters :	CFlowchartLink* inlink	-	Link to delete

	Usage :			Call to delete a link from the link array.

   ============================================================*/
{

	INT_PTR max = m_links.GetUpperBound();
	for (INT_PTR t = max; t >= 0; t--)
	{
		CFlowchartLink* link = static_cast<CFlowchartLink*>(m_links.GetAt(t));
		if (link == inlink)
		{
			delete link;
			m_links.RemoveAt(t);
		}
	}

}

void CFlowchartEntityContainer::Undo()
/* ============================================================
	Function :		CFlowchartEntityContainer::Undo
	Description :	Undo the latest operation

	Return :		void
	Parameters :	none

	Usage :			Overridden to also undo link operations.

   ============================================================*/
{
	if (m_undoLinks.GetSize())
	{
		// Push the current state to the redo links stack
		PushLinks(&m_redoLinks);

		// Undo any non-link changes
		CDiagramEntityContainer::Undo();

		// Remove all links
		ClearLinks();

		CObArray* undo = static_cast<CObArray*>(m_undoLinks.GetAt(m_undoLinks.GetUpperBound()));
		INT_PTR count = undo->GetSize();
		for (INT_PTR t = 0; t < count; t++)
		{
			CFlowchartLink* obj = static_cast<CFlowchartLink*>(undo->GetAt(t));
			AddLink(obj->Clone());
		}

		INT_PTR max = undo->GetSize();
		for (INT_PTR t = max - 1; t >= 0; t--)
			delete undo->GetAt(t);
		undo->RemoveAll();
		delete undo;

		m_undoLinks.RemoveAt(m_undoLinks.GetUpperBound());
	}
}

void CFlowchartEntityContainer::Snapshot()
/* ============================================================
	Function :		CFlowchartEntityContainer::Snapshot
	Description :	Creates a snapshot of the current data
					state for the undo-functionality.

	Return :		void
	Parameters :	none

	Usage :			Overridden to save the link state as well.

   ============================================================*/
{
	CDiagramEntityContainer::Snapshot();
	PushLinks(&m_undoLinks);
}

void CFlowchartEntityContainer::ClearUndo()
/* ============================================================
	Function :		CFlowchartEntityContainer::ClearUndo
	Description :	Clears the undo-array

	Return :		void
	Parameters :	none

	Usage :			Overridden to also clear the link undo
					states.

   ============================================================*/
{
	CDiagramEntityContainer::ClearUndo();
	INT_PTR count = m_undoLinks.GetSize() - 1;
	for (INT_PTR t = count; t >= 0; t--)
	{
		CObArray* undo = static_cast<CObArray*>(m_undoLinks.GetAt(t));

		// Remove all objects in the stack entry
		INT_PTR max = undo->GetSize();
		for (INT_PTR i = 0; i < max; i++)
			delete undo->GetAt(i);
		undo->RemoveAll();

		// Remove the stack entry itself.
		delete undo;
	}

	m_undoLinks.RemoveAll();
}

void CFlowchartEntityContainer::Redo()
/* ============================================================
	Function :		CFlowchartEntityContainer::Undo
	Description :	Redo the latest operation

	Return :		void
	Parameters :	none

	Usage :			Overridden to also redo link operations.

   ============================================================*/
{
	if (m_redoLinks.GetSize())
	{
		// Push the current state to the undo links stack
		PushLinks(&m_undoLinks);

		// Redo any non-link changes
		CDiagramEntityContainer::Redo();

		// Remove all links
		ClearLinks();

		CObArray* redo = static_cast<CObArray*>(m_redoLinks.GetAt(m_redoLinks.GetUpperBound()));
		INT_PTR count = redo->GetSize();
		for (INT_PTR t = 0; t < count; t++)
		{
			CFlowchartLink* obj = static_cast<CFlowchartLink*>(redo->GetAt(t));
			AddLink(obj->Clone());
		}

		INT_PTR max = redo->GetSize();
		for (INT_PTR t = max - 1; t >= 0; t--)
			delete redo->GetAt(t);
		redo->RemoveAll();
		delete redo;

		m_redoLinks.RemoveAt(m_redoLinks.GetUpperBound());
	}
}

void CFlowchartEntityContainer::PushLinks(CObArray* stack)
/* ============================================================
	Function :		CFlowchartEntityContainer::PushLinks
	Description :	Creates a snapshot of the current link
					state for the undo/redo functionality.

	Return :		void
	Parameters :	none

   ============================================================*/
{
	if (GetUndoStackSize() > 0 && stack->GetSize() == GetUndoStackSize())
	{
		delete stack->GetAt(0);
		stack->RemoveAt(0);
	}

	CObArray* item = new CObArray;

	while (!item && stack->GetSize())
	{
		delete stack->GetAt(0);
		stack->RemoveAt(0);
		item = new CObArray;
	}

	if (item)
	{
		// Save all objects
		INT_PTR count = m_links.GetSize();
		for (INT_PTR t = 0; t < count; t++)
			item->Add(GetLinkAt(t)->Clone());

		// Add to the stack
		stack->Add(item);
	}
}

void CFlowchartEntityContainer::ClearRedo()
/* ============================================================
	Function :		CFlowchartEntityContainer::ClearRedo
	Description :	Clears the redo-array

	Return :		void
	Parameters :	none

	Usage :			Overridden to also clear the link redo
					states.

   ============================================================*/
{
	CDiagramEntityContainer::ClearRedo();
	INT_PTR count = m_redoLinks.GetSize() - 1;
	for (INT_PTR t = count; t >= 0; t--)
	{
		CObArray* redo = static_cast<CObArray*>(m_redoLinks.GetAt(t));

		// Remove all objects in the stack entry
		INT_PTR max = redo->GetSize();
		for (INT_PTR i = 0; i < max; i++)
			delete redo->GetAt(i);
		redo->RemoveAll();

		// Remove the stack entry itself.
		delete redo;
	}

	m_redoLinks.RemoveAll();
}

void CFlowchartEntityContainer::ClearLinks()
/* ============================================================
	Function :		CFlowchartEntityContainer::ClearLinks
	Description :	Clears the link array.

	Return :		void
	Parameters :	none

	Usage :			Call to remove all links.

   ============================================================*/
{

	INT_PTR max = m_links.GetSize();
	for (INT_PTR t = 0; t < max; t++)
		delete m_links[t];

	m_links.RemoveAll();

}

double CFlowchartEntityContainer::Dist(CPoint point1, CPoint point2)
/* ============================================================
	Function :		CFlowchartEntityContainer::Dist
	Description :	Calculates the distance between point1 and
					point2.

	Return :		double			-	Resulting distance
	Parameters :	CPoint point1	-	First point to test
					CPoint point2	-	Second point to test

	Usage :			Used to find the closest link points between
					two objects.

   ============================================================*/
{

	double width = abs(point1.x - point2.x);
	double height = abs(point1.y - point2.y);

	double hyp = _hypot(width, height);

	return hyp;

}

CFlowchartEntity* CFlowchartEntityContainer::GetPrimaryLink()
/* ============================================================
	Function :		CFlowchartEntityContainer::GetPrimaryLink
	Description :	Returns the primary object of the two
					currently selected and linked.

	Return :		CFlowchartEntity*	-	Primary object or
											NULL if none.
	Parameters :	none

	Usage :			Returns NULL if not exactly two objects are
					selected, or they are not linked.

   ============================================================*/
{

	CFlowchartEntity* result = NULL;

	if (GetSelectCount() == 2)
	{
		INT_PTR max = GetSize();
		CFlowchartEntity* primary = NULL;
		CFlowchartEntity* secondary = NULL;

		for (INT_PTR t = 0; t < max; t++)
		{
			CFlowchartEntity* obj = dynamic_cast<CFlowchartEntity*>(GetAt(t));
			if (obj && obj->IsSelected())
			{
				if (primary == NULL)
					primary = obj;
				else
					secondary = obj;
			}
		}

		if (primary && secondary)
		{
			CFlowchartLink* link = FindLink(primary, secondary);
			if (link)
			{
				if (primary->GetName() == link->from)
					result = primary;
				else
					result = secondary;
			}
		}
	}

	return result;

}

CFlowchartEntity* CFlowchartEntityContainer::GetSecondaryLink()
/* ============================================================
	Function :		CFlowchartEntityContainer::GetSecondaryLink
	Description :	Returns the secondary object of the two
					currently selected and linked.

	Return :		CFlowchartEntity*	-	Secondary object or
											NULL if none.
	Parameters :	none

	Usage :			Returns NULL if not exactly two objects are
					selected, or they are not linked.

   ============================================================*/
{

	CFlowchartEntity* result = NULL;

	if (GetSelectCount() == 2)
	{
		INT_PTR max = GetSize();
		CFlowchartEntity* primary = NULL;
		CFlowchartEntity* secondary = NULL;

		for (INT_PTR t = 0; t < max; t++)
		{
			CFlowchartEntity* obj = dynamic_cast<CFlowchartEntity*>(GetAt(t));
			if (obj && obj->IsSelected())
			{
				if (primary == NULL)
					primary = obj;
				else
					secondary = obj;
			}
		}

		if (primary && secondary)
		{
			CFlowchartLink* link = FindLink(primary, secondary);
			if (link)
			{
				if (primary->GetName() == link->from)
					result = secondary;
				else
					result = primary;
			}
		}
	}

	return result;

}

CFlowchartEntity* CFlowchartEntityContainer::GetPrimarySelected()
/* ============================================================
	Function :		CFlowchartEntityContainer::GetPrimarySelected
	Description :	Returns the primary object of the two
					currently selected.

	Return :		CFlowchartEntity*	-	Primary object or
											NULL if none.
	Parameters :	none

	Usage :			Returns NULL if not exactly two objects are
					selected.

   ============================================================*/
{

	CFlowchartEntity* result = NULL;

	if (GetSelectCount() == 2)
	{
		INT_PTR max = GetSize();

		for (INT_PTR t = 0; t < max; t++)
		{
			CFlowchartEntity* obj = dynamic_cast<CFlowchartEntity*>(GetAt(t));
			if (obj && obj->IsSelected())
			{
				if (result == NULL)
					result = obj;
			}
		}
	}

	return result;

}

CFlowchartEntity* CFlowchartEntityContainer::GetSecondarySelected()
/* ============================================================
	Function :		CFlowchartEntityContainer::GetSecondarySelected
	Description :	Returns the secondary object of the two
					currently selected.

	Return :		CFlowchartEntity*	-	secondary object or
											NULL if none.
	Parameters :	none

	Usage :			Returns NULL if not exactly two objects are
					selected.

   ============================================================*/
{

	CFlowchartEntity* result = NULL;

	if (GetSelectCount() == 2)
	{
		INT_PTR max = GetSize();

		for (INT_PTR t = 0; t < max; t++)
		{
			CFlowchartEntity* obj = dynamic_cast<CFlowchartEntity*>(GetAt(t));
			if (obj && obj->IsSelected())
				result = obj;
		}
	}

	return result;

}

int	CFlowchartEntityContainer::GetSelectCount()
/* ============================================================
	Function :		int	CFlowchartEntityContainer::GetSelectCount
	Description :	Returns the number of currently selected
					objects.

	Return :		int		-	The number of selected objects.
	Parameters :	none

	Usage :			Call to see how many objects are selected.

   ============================================================*/
{

	int count = 0;
	INT_PTR max = GetSize();

	for (INT_PTR t = 0; t < max; t++)
	{
		CFlowchartEntity* obj = dynamic_cast<CFlowchartEntity*>(GetAt(t));

		if (obj && obj->IsSelected())
			count++;
	}

	return count;

}

BOOL CFlowchartEntityContainer::IsLinked()
/* ============================================================
	Function :		CFlowchartEntityContainer::IsLinked
	Description :	Check if exactly two objects are selected
					and linked.

	Return :		BOOL	-	TRUE if two objects are
								selected and linked.
	Parameters :	none

	Usage :			Call as a command enabler for commands
					where the selected items must be two and
					linked (such as Flip link direction).

   ============================================================*/
{

	BOOL result = FALSE;

	if (GetSelectCount() == 2)
	{
		CFlowchartEntity* primary = GetPrimaryLink();
		CFlowchartEntity* secondary = GetSecondaryLink();

		result = (primary && secondary);
	}

	return result;

}

BOOL CFlowchartEntityContainer::CanLink()
/* ============================================================
	Function :		CFlowchartEntityContainer::CanLink
	Description :	Returns TRUE if two objects are selected,
					but not linked and it is possible to link
					them.

	Return :		BOOL	-	TRUE if two objects are
								selected and can be linked.
	Parameters :	none

	Usage :			Call as a command enabler for the Link
					command.

   ============================================================*/
{

	BOOL result = FALSE;

	if (GetSelectCount() == 2)
	{
		CFlowchartEntity* primary = GetPrimarySelected();
		CFlowchartEntity* secondary = GetSecondarySelected();

		if (primary && secondary)
			result = !HasLinks(primary, secondary);
	}

	return result;

}