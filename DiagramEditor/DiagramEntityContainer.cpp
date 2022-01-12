/* ==========================================================================
	Class :			CDiagramEntityContainer

	Author :		Johan Rosengren, Abstrakt Mekanik AB

	Date :			2004-03-29

	Purpose :		"CDiagramEntityContainer" contains the data for a
					"CDiagramEditor". It manages mass operations such as
					copying, pasting and undo. It is completely separated
					from "CDiagramEditor" to allow the package to be used
					in a doc/view app. This is also the reason why some
					functionality is accessible in both this class and ïn
					"CDiagramEditor".

	Description :	The class contains a "CObArray" with the instances of
					"CDiagramEntity"-derived classes that is the current data
					for an editor. It also contains a pointer to a
					"CDiagramClipboardHandler"-instance that works as the
					'clipboard' for an editor. Furthermore, It contains an
					"CObArray" of "CObArray"s that is the undo stack.

					The Undo-functionality is implemented as a simple FILO-stack
					of "CObArray" pointers. Before any change that should be
					undoable, "Snapshot" is called to add new entries to
					the Undo-stack. This is normally managed by the editor,
					and need only be done manually for added functionality.

					Note that the "CDiagramEntityContainer" should normally
					not call "Snapshot" itself - in the case of, for example,
					additions to "m_objs", the container can not and should
					not know if it is an undoable operation.

	Usage :			Normally, this class need not be derived from. A
					"CDiagramEditor" needs an instance of
					"CDiagramEntityContainer" to hold the object data. This
					instance can either be external, as for a doc/view app
					where the container belongs to the document, or
					internal, as for a dialog application where the editor
					will manage all of the data. In the first case, a
					"CDiagramEntityContainer" member should be declared in
					the document class, and a pointer to it submitted to
					the "Create"-call of the "CDiagramEditor" (or by calling
					"CDiagramEditor::SetCDiagramEntityContainer"). In the
					second case, nothing special need to be done - the
					"CDiagramEntityContainer" will be created during the
					"CDiagramEditor::Create" call automatically if no pointer
					is submitted.

					The container is not using the Windows clipboard
					(because of instantiation questions on derived
					entities), but rather an external clipboard handler
					derived from "CDiagramClipboardHandler". This handler is
					set calling "SetClipboardHandler", and several containers
					can share the same handler. If no clipboard handler is
					set, a default internal one will be used.

					"CDiagramEntityContainer" manages all data internally,
					all internal objects are deleted in the class "dtor".

   ========================================================================
	Changes :		19/4 2004	Made RemoveAt virtual.
					20/4 2004	Made several Undo- and Copy/Paste functions
								virtual. Added array accessors for derived
								classes. Moved the member function Find to
								protected.
					30/4 2004	Copy/paste-handling removed to a separate
								class to allow several containers to share
								the same clipboard.
					30/4 2004	Changed c-style casts to static_cast
   ========================================================================
					20/5 2004	Made GetAt virtual
					30/5 2004	RemoveAll, added check to see if there are
								any items in the object array.
					30/5 2004	Made RemoveAll access data container objects
								directly, to avoid chained deletes in
								derived classes.
   ========================================================================
					26/6 2004	Added group handling (Unruled Boy).
					3/7  2004	Made Add and Remove virtual
					3/7  2004	Added a GetSelectCount
   ========================================================================
					4/8  2004	Added SelectAll and UnselectAll
   ========================================================================
					11/12 2004	Made UnselectAll virtual (Grisha Vinevich)
   ========================================================================
					22/1  2005	Added PopUndo function to pop the latest
								undo item from the stack
								Made IsUndoPossible const.
   ========================================================================
					16/3  2019	Added Redo support, removed PopUndo
   ========================================================================
					13/1  2022	Added auto-generated name support
   ========================================================================*/

#include "stdafx.h"
#include "DiagramEntityContainer.h"
#include "DiagramEntity.h"
#include "Tokenizer.h"
#include "GroupFactory.h"
#include "../UMLEditor/LinkFactory.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma warning( disable : 4706 )

/////////////////////////////////////////////////////////////////////////////
// CDiagramEntityContainer construction/destruction/initialization

CDiagramEntityContainer::CDiagramEntityContainer(CDiagramClipboardHandler* clip)
/* ============================================================
	Function :		CDiagramEntityContainer::CDiagramEntityContainer
	Description :	Constructor
	Access :		Public

	Return :		void
	Parameters :	none

	Usage :

   ============================================================*/
{

	m_clip = clip;

	SetUndoStackSize(0);
	Clear();
	SetVirtualSize(CSize(0, 0));
	m_color = RGB(0, 0, 0);

	m_gridCol = RGB(192, 192, 192);
	m_grid = TRUE;
	m_gridStyle = PS_SOLID;
	m_gridSize = CSize(8, 8);

	m_snap = TRUE;

	m_restraint = RESTRAINT_VIRTUAL;

	m_leftMargin = 8;
	m_topMargin = 8;
	m_rightMargin = 8;
	m_bottomMargin = 16;
	m_margin = FALSE;
	m_marginColor = RGB(128, 128, 255);
}

CDiagramEntityContainer::~CDiagramEntityContainer()
/* ============================================================
	Function :		CDiagramEntityContainer::~CDiagramEntityContainer
	Description :	Destructor
	Access :		Public

	Return :		void
	Parameters :	none

	Usage :

   ============================================================*/
{

	Clear();

}

void CDiagramEntityContainer::Clear()
/* ============================================================
	Function :		CDiagramEntityContainer::Clear
	Description :	Removes all data from the data and undo.
	Access :		Public

	Return :		void
	Parameters :	none

	Usage :			Call to remove data from the container. The
					Paste-array will be kept.

   ============================================================*/
{

	RemoveAll();
	ClearUndo();
	ClearRedo();
	SetModified(FALSE);

}

CString CDiagramEntityContainer::GetString() const
/* ============================================================
	Function :		CDiagramEntityContainer::GetString
	Description :	Returns a string representation of the
					virtual paper size and current paper color.
	Access :		Public

	Return :		CString	-	Resulting string
	Parameters :	none

	Usage :			Call to get a string representing the paper
					size of the container. The format is
					"paper:x,y,color;" where "x" and "y" are the
					horisontal and vertical sizes, and "color" is
					the paper color.

   ============================================================*/
{
	int leftMargin;
	int topMargin;
	int rightMargin;
	int bottomMargin;
	GetMargins(leftMargin, topMargin, rightMargin, bottomMargin);

	CString str;
	str.Format(_T("paper:%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i;"),
		GetVirtualSize().cx,
		GetVirtualSize().cy,
		static_cast<int>(GetColor()),
		GetSnapToGrid(),
		IsGridVisible(),
		GetGridPenStyle(),
		GetGridSize().cx,
		GetGridSize().cy,
		static_cast<int>(GetGridColor()),
		IsMarginVisible(),
		GetRestraints(),
		static_cast<int>(GetMarginColor()),
		leftMargin,
		topMargin,
		rightMargin,
		bottomMargin
	);
	return str;
}


BOOL CDiagramEntityContainer::FromString(const CString& str)
/* ============================================================
	Function :		CDiagramEntityContainer::FromString
	Description :	Sets the virtual paper size from a string.
	Access :		Public

	Return :		BOOL				-	"TRUE" if the string
											represented a
											paper.
	Parameters :	const CString& str	-	The string
											representation.

	Usage :			Call to set the paper size of the container
					from a string. The format is "paper:x,y,color;"
					where "x" and "y" are the horisontal and
					vertical sizes, "color" the background color.

   ============================================================*/
{
	BOOL result = FALSE;

	CTokenizer main(str, _T(":"));
	CString header;
	CString data;
	if (main.GetSize() == 2)
	{
		main.GetAt(0, header);
		main.GetAt(1, data);
		header.TrimLeft();
		header.TrimRight();
		data.TrimLeft();
		data.TrimRight();
		if (header == _T("paper"))
		{
			CTokenizer tok(data.Left(data.GetLength() - 1));
			INT_PTR size = tok.GetSize();
			if (size == 16)
			{
				int right;
				int bottom;
				int color;
				int snap;
				int grid;
				int gridStyle;
				int gridRight;
				int gridBottom;
				int gridColor;
				int margin;
				int restraint;
				int marginColor;
				int leftMargin;
				int topMargin;
				int rightMargin;
				int bottomMargin;

				tok.GetAt(0, right);
				tok.GetAt(1, bottom);
				tok.GetAt(2, color);
				tok.GetAt(3, snap);
				tok.GetAt(4, grid);
				tok.GetAt(5, gridStyle);
				tok.GetAt(6, gridRight);
				tok.GetAt(7, gridBottom);
				tok.GetAt(8, gridColor);
				tok.GetAt(9, margin);
				tok.GetAt(10, restraint);
				tok.GetAt(11, marginColor);
				tok.GetAt(12, leftMargin);
				tok.GetAt(13, topMargin);
				tok.GetAt(14, rightMargin);
				tok.GetAt(15, bottomMargin);

				SetVirtualSize(CSize(right, bottom));
				SetColor(static_cast<COLORREF>(color));
				SetSnapToGrid(snap);
				ShowGrid(grid);
				SetGridPenStyle(gridStyle);
				SetGridSize(CSize(gridRight, gridBottom));
				SetGridColor(static_cast<COLORREF>(gridColor));
				ShowMargin(margin);
				SetRestraints(restraint);
				SetMarginColor(static_cast<COLORREF>(marginColor));
				SetMargins(leftMargin, topMargin, rightMargin, bottomMargin);
				SetModified(TRUE);
				result = TRUE;
			}
		}
	}

	return result;
}

void CDiagramEntityContainer::Export(CStringArray& stra, UINT format) const
/* ============================================================
	Function :		CDiagramEntityContainer::Export
	Description :	Exports all objects to format format.
	Access :		Public

	Return :		void
	Parameters :	CStringArray& stra	-	"CStingArray" that
											will be filled with
											data on return.
					UINT format			-	Format to save to.

	Usage :			Call to export the contents of the container
					to a "CStringArray". "Export" will - of course -
					have to be defined for the derived objects.

   ============================================================*/
{

	INT_PTR max = GetSize();
	for (INT_PTR t = 0; t < max; t++)
	{
		CDiagramEntity* obj = GetAt(t);
		stra.Add(obj->Export(format));
	}

}

/////////////////////////////////////////////////////////////////////////////
// CDiagramEntityContainer data access

INT_PTR CDiagramEntityContainer::GetSize() const
/* ============================================================
	Function :		CDiagramEntityContainer::GetSize
	Description :	Returns the number of objects in the data
					container.
	Access :		Public

	Return :		int		-	The number of objects.
	Parameters :	none

	Usage :			Call to get the number of objects currently
					in the data array of the container.

   ============================================================*/
{

	return m_objs.GetSize();

}

void CDiagramEntityContainer::Add(CDiagramEntity* obj)
/* ============================================================
	Function :		CDiagramEntityContainer::Add
	Description :	Add an object to the data.
	Access :		Public

	Return :		void
	Parameters :	CDiagramEntity* obj	-	The object to add.

	Usage :			Call to add a new object to the container.

   ============================================================*/
{

	obj->SetParent(this);
	m_objs.Add(obj);
	SetModified(TRUE);

}

CDiagramEntity* CDiagramEntityContainer::GetAt(INT_PTR index) const
/* ============================================================
	Function :		CDiagramEntityContainer::GetAt
	Description :	Gets the object at position "index".
	Access :		Public

	Return :		CDiagramEntity*	-	The object or "NULL" if
										out of range.
	Parameters :	int index		-	The index to get data
										from

	Usage :			Call to get a specific object from the
					container.

   ============================================================*/
{

	CDiagramEntity* result = NULL;
	if (index < m_objs.GetSize() && index >= 0)
		result = static_cast<CDiagramEntity*>(m_objs.GetAt(index));
	return result;

}

void CDiagramEntityContainer::SetAt(INT_PTR index, CDiagramEntity* obj)
/* ============================================================
	Function :		CDiagramEntityContainer::SetAt
	Description :	Sets an object at position "index".
	Access :		Public

	Return :		void
	Parameters :	int index			-	Index to set data
											at.
					CDiagramEntity* obj	-	Object to set.

	Usage :			Internal function. Used by "Swap".

   ============================================================*/
{

	m_objs.SetAt(index, obj);
	SetModified(TRUE);

}

void CDiagramEntityContainer::RemoveAt(INT_PTR index)
/* ============================================================
	Function :		CDiagramEntityContainer::RemoveAt
	Description :	Removes the object at index.
	Access :		Public

	Return :		void
	Parameters :	int index	-	The index of the object
									to remove.

	Usage :			Call to remove a specific object. Memory is
					freed.

   ============================================================*/
{

	CDiagramEntity* obj = GetAt(index);
	if (obj)
	{
		delete obj;
		m_objs.RemoveAt(index);
		SetModified(TRUE);
	}

}

void CDiagramEntityContainer::RemoveAll()
/* ============================================================
	Function :		CDiagramEntityContainer::RemoveAll
	Description :	Removes all data objects
	Access :		Public

	Return :		void
	Parameters :	none

	Usage :			Call to remove all data objects in the
					container. Undo- and paste arrays are not
					emptied.
					Allocated memory is released. Undo and
					paste not deleted.

   ============================================================*/
{

	INT_PTR max = m_objs.GetSize();
	if (max)
	{

		for (INT_PTR t = 0; t < max; t++)
		{
			CDiagramEntity* obj = static_cast<CDiagramEntity*>(m_objs.GetAt(t));
			delete obj;
		}

		m_objs.RemoveAll();
		SetModified(TRUE);

	}

}

void CDiagramEntityContainer::RemoveAllSelected()
/* ============================================================
	Function :		CDiagramEntityContainer::RemoveAllSelected
	Description :	Removes all selected objects
	Access :		Public

	Return :		void
	Parameters :	none

	Usage :			Call to remove all selected objects from the
					container. Releases allocated data

   ============================================================*/
{

	INT_PTR max = m_objs.GetSize() - 1;
	for (INT_PTR t = max; t >= 0; t--)
		if (GetAt(t)->IsSelected())
			RemoveAt(t);

}

/////////////////////////////////////////////////////////////////////////////
// CDiagramEntityContainer property access

void CDiagramEntityContainer::SetVirtualSize(CSize size)
/* ============================================================
	Function :		CDiagramEntityContainer::SetVirtualSize
	Description :	Sets the current virtual paper size.
	Access :		Public

	Return :		void
	Parameters :	CSize size	-	The size to set

	Usage :			Call to set the paper size. Note that
					"SetModified( TRUE )" might have to be called
					as well.

   ============================================================*/
{

	m_virtualSize = size;

}

CSize CDiagramEntityContainer::GetVirtualSize() const
/* ============================================================
	Function :		CDiagramEntityContainer::GetVirtualSize
	Description :	Gets the virtual paper size.
	Access :		Public

	Return :		CSize	-	The current size
	Parameters :	none

	Usage :			Call to get the current paper size.

   ============================================================*/
{

	return m_virtualSize;

}

void CDiagramEntityContainer::SetColor(COLORREF color)
/* ============================================================
	Function :		CDiagramEntityContainer::SetColor
	Description :	Sets the color of the container.
	Access :		Public

	Return :		void
	Parameters :	COLORREF color	-	New color

	Usage :			The color is used as background color for
					the editor and will be saved/loaded with
					the rest of the container data.

   ============================================================*/
{

	m_color = color;

}

COLORREF CDiagramEntityContainer::GetColor() const
/* ============================================================
	Function :		CDiagramEntityContainer::GetColor
	Description :	Gets the container color
	Access :		Public

	Return :		COLORREF	-	Container color
	Parameters :	none

	Usage :			The color is used as background color for
					the editor and will be saved/loaded with
					the rest of the container data.

   ============================================================*/
{

	return m_color;

}

void CDiagramEntityContainer::ShowGrid(BOOL grid)
/* ============================================================
	Function :		CDiagramEntityContainer::ShowGrid
	Description :	Sets grid visibility.
	Access :		Public

	Return :		void
	Parameters :	BOOL grid	-	"TRUE" to show the grid, "FALSE"
									to hide.

	Usage :			If the grid is visible, it will be drawn
					using the grid pen style and color. The
					grid lines will not be scaled with the
					zoom (the space between them will of
					course be, however)

   ============================================================*/
{
	m_grid = grid;
}

BOOL CDiagramEntityContainer::IsGridVisible() const
/* ============================================================
	Function :		CDiagramEntityContainer::IsGridVisible
	Description :	Returns the visibility state of the grid.
	Access :		Public

	Return :		BOOL	-	"TRUE" if grid is visible.
	Parameters :	none

	Usage :			If the grid is visible, it will be drawn
					using the grid pen style and color. The
					grid lines will not be scaled with the
					zoom (the space between them will of
					course be, however)

   ============================================================*/
{
	return m_grid;
}

void CDiagramEntityContainer::SetGridColor(COLORREF col)
/* ============================================================
	Function :		CDiagramEntityContainer::SetGridColor
	Description :	Sets a new grid pen color.
	Access :		Public

	Return :		void
	Parameters :	COLORREF col	-	New grid pen color.

	Usage :			If the grid is visible, it will be drawn
					using the grid pen style and color. The
					grid lines will not be scaled with the
					zoom (the space between them will of
					course be, however)

   ============================================================*/
{
	m_gridCol = col;
}

COLORREF CDiagramEntityContainer::GetGridColor() const
/* ============================================================
	Function :		CDiagramEntityContainer::GetGridColor
	Description :	Returns the current grid pen color.
	Access :		Public

	Return :		COLORREF	-	The current grid color.
	Parameters :	none

	Usage :			If the grid is visible, it will be drawn
					using the grid pen style and color. The
					grid lines will not be scaled with the
					zoom (the space between them will of
					course be, however)

   ============================================================*/
{
	return m_gridCol;
}

void CDiagramEntityContainer::SetGridSize(CSize size)
/* ============================================================
	Function :		CDiagramEntityContainer::SetGridSize
	Description :	Sets a new grid size.
	Access :		Public

	Return :		void
	Parameters :	CSize size	-	The new grid size.

	Usage :			If snap to grid is on, added, moved and
					resized objects snap to the closest grid
					position. If the background is resized, it
					will also snap to the grid.

   ============================================================*/
{
	m_gridSize = size;
}

CSize CDiagramEntityContainer::GetGridSize() const
/* ============================================================
	Function :		CDiagramEntityContainer::GetGridSize
	Description :	Gets the current grid size.
	Access :		Public

	Return :		CSize	-	The current grid size.
	Parameters :	none

	Usage :			If snap to grid is on, added, moved and
					resized objects snap to the closest grid
					position. If the background is resized, it
					will also snap to the grid.

   ============================================================*/
{
	return m_gridSize;
}

void CDiagramEntityContainer::SetGridPenStyle(int style)
/* ============================================================
	Function :		CDiagramEntityContainer::SetGridPenStyle
	Description :	Sets the new grid pen style.
	Access :		Public

	Return :		void
	Parameters :	int style	-	The new pen style, one of
									the style constants for
									"CreatePen".

	Usage :			The grid (if visible) is drawn with a pen
					created with the grid pen style. The grid
					lines will not be scaled with the zoom
					(the space between them will of course be,
					however)

   ============================================================*/
{
	m_gridStyle = style;
}

int CDiagramEntityContainer::GetGridPenStyle() const
/* ============================================================
	Function :		CDiagramEntityContainer::GetGridPenStyle
	Description :	Returns the pen style for the grid.
	Access :		Public

	Return :		int		-	The pen style, one of the style
								constants for "CreatePen".
	Parameters :	none

	Usage :			The grid (if visible) is drawn with a pen
					created with the grid pen style. The grid
					lines will not be scaled with the zoom
					(the space between them will of course be,
					however)

   ============================================================*/
{
	return m_gridStyle;
}

void CDiagramEntityContainer::SetSnapToGrid(BOOL snap)
/* ============================================================
	Function :		CDiagramEntityContainer::SetSnapToGrid
	Description :	Enable/disable snap to grid.
	Access :		Public

	Return :		void
	Parameters :	BOOL snap	-	"TRUE" if objects should
									snap to grid.

	Usage :			If snap to grid is on, added, moved and
					resized objects snap to the closest grid
					position. If the background is resized, it
					will also snap to the grid.

   ============================================================*/
{
	m_snap = snap;
}

BOOL CDiagramEntityContainer::GetSnapToGrid() const
/* ============================================================
	Function :		CDiagramEntityContainer::GetSnapToGrid
	Description :	Gets the state of the snap-to-grid state.
	Access :		Public

	Return :		BOOL	-	"TRUE" if snap is on.
	Parameters :	none

	Usage :			If snap to grid is on, added, moved and
					resized objects snap to the closest grid
					position. If the background is resized, it
					will also snap to the grid.

   ============================================================*/
{
	return m_snap;
}

void CDiagramEntityContainer::SetMargins(int left, int top, int right, int bottom)
/* ============================================================
	Function :		CDiagramEntityContainer::SetMargins
	Description :	Sets margins for the virtual screen.
	Access :		Public

	Return :		void
	Parameters :	int left	-	New left margin.
					int top		-	New top margin.
					int right	-	New right margin.
					int bottom	-	New bottom margin.

	Usage :			Call to set new margins for the editor.

   ============================================================*/
{
	m_leftMargin = left;
	m_topMargin = top;
	m_rightMargin = right;
	m_bottomMargin = bottom;
}

void CDiagramEntityContainer::GetMargins(int& left, int& top, int& right, int& bottom) const
/* ============================================================
	Function :		CDiagramEntityContainer::GetMargins
	Description :	Return the current margin.
	Access :		Public

	Return :		void
	Parameters :	int& left	-	Current left margin.
					int& top	-	Current top margin.
					int& right	-	Current right margin.
					int& bottom	-	Current bottom margin.

	Usage :			Call to get the margins of the editor.

   ============================================================*/
{
	left = m_leftMargin;
	top = m_topMargin;
	right = m_rightMargin;
	bottom = m_bottomMargin;
}

void CDiagramEntityContainer::SetMarginColor(COLORREF marginColor)
/* ============================================================
	Function :		CDiagramEntityContainer::SetMarginColor
	Description :	Set current margin colors.
	Access :		Public

	Return :		void
	Parameters :	COLORREF marginColor	-	The new color.

	Usage :			Call to set the margin color.

   ============================================================*/
{
	m_marginColor = marginColor;
}

COLORREF CDiagramEntityContainer::GetMarginColor() const
/* ============================================================
	Function :		CDiagramEntityContainer::GetMarginColor
	Description :	Returns the current margin colors.
	Access :		Public

	Return :		COLORREF	-	Margin colors.
	Parameters :	none

	Usage :			Call to get the margin color.

   ============================================================*/
{
	return m_marginColor;
}

void CDiagramEntityContainer::ShowMargin(BOOL show)
/* ============================================================
	Function :		CDiagramEntityContainer::ShowMargin
	Description :	Show/hide margins.
	Access :		Public

	Return :		void
	Parameters :	BOOL show	-	"TRUE" to show margins, "FALSE"
									to hide.

	Usage :			Call to show/hide the margins.

   ============================================================*/
{
	m_margin = show;
}

BOOL CDiagramEntityContainer::IsMarginVisible() const
/* ============================================================
	Function :		CDiagramEntityContainer::IsMarginVisible
	Description :	Returns the visibility state of the
					margins.
	Access :		Public

	Return :		BOOL	-	"TRUE" if the margins are
								visible.
	Parameters :	none

	Usage :			Call to se if the margin visibility flag is
					on.

   ============================================================*/
{
	return m_margin;
}

void CDiagramEntityContainer::SetRestraints(int restraint)
/* ============================================================
	Function :		CDiagramEntityContainer::SetRestraints
	Description :	Sets current restraint mode
	Access :		Public

	Return :		void
	Parameters :	BOOL restraint	-	restraint mode, see Usage
										below

	Usage :			The restraint mode can be one of the
					following ( defined in DialogEditor.h):
						"RESTRAINT_NONE" No restraints.
						"RESTRAINT_VIRTUAL" No objects can be moved outside the virtual rectangle.
						"RESTRAINT_MARGIN" No objects can be moved outside the background margins.

   ============================================================*/
{
	m_restraint = restraint;
}

int CDiagramEntityContainer::GetRestraints() const
/* ============================================================
	Function :		CDiagramEntityContainer::GetRestraints
	Description :	Returns the current restraint mode.
	Access :		Public

	Return :		int	-	The current restraint mode. See
							Usage below

	Parameters :	none

	Usage :			The restraint mode can be one of the
					following ( defined in DialogEditor.h):
						"RESTRAINT_NONE" No restraints.
						"RESTRAINT_VIRTUAL" No objects can be moved outside the virtual rectangle.
						"RESTRAINT_MARGIN" No objects can be moved outside the background margins.

   ============================================================*/
{

	return m_restraint;

}

BOOL CDiagramEntityContainer::IsModified() const
/* ============================================================
	Function :		CDiagramEntityContainer::IsModified
	Description :	Returns the state of the modified-flag.
	Access :		Public

	Return :		BOOL	-	"TRUE" if data is changed
	Parameters :	none

	Usage :			Call to see if data is modified.

   ============================================================*/
{

	return m_dirty;

}

void CDiagramEntityContainer::SetModified(BOOL dirty)
/* ============================================================
	Function :		CDiagramEntityContainer::SetModified
	Description :	Sets the state of the modified flag
	Access :		Public

	Return :		void
	Parameters :	BOOL dirty	-	"TRUE" if data is changed.

	Usage :			Call to mark the data as modified.

   ============================================================*/
{

	m_dirty = dirty;

}

/////////////////////////////////////////////////////////////////////////////
// CDiagramEntityContainer single object handlers

void CDiagramEntityContainer::Remove(CDiagramEntity* obj)
/* ============================================================
	Function :		CDiagramEntityContainer::Remove
	Description :	Removes the object.
	Access :		Public

	Return :		void
	Parameters :	CDiagramEntity* obj	-	The object to
											remove.

	Usage :			Call to remove "obj" - if it exists - from the
					container. Allocated memory is released.

   ============================================================*/
{

	int index = Find(obj);
	if (index != -1)
		RemoveAt(index);

}

void CDiagramEntityContainer::Duplicate(CDiagramEntity* obj)
/* ============================================================
	Function :		CDiagramEntityContainer::Duplicate
	Description :	Duplicates the object and adds the new
					one 10 pixels offset down and right.
	Access :		Public

	Return :		void
	Parameters :	CDiagramEntity* obj	-	The object to
											duplicate.

	Usage :			Call to create a copy of the selected
					element.

   ============================================================*/
{
	int index = Find(obj);
	if (index != -1)
	{
		// Clone the object
		CDiagramEntity* newobj = obj->Clone();

		// Set the name if it is to be auto-generated
		if (newobj->AutoGenerateName())
		{
			newobj->SetName(CLinkFactory::GetID());
		}

		// Move it by the grid size
		CSize gridSize = GetGridSize();
		newobj->SetRect(newobj->GetLeft() + gridSize.cx, newobj->GetTop() + gridSize.cy, newobj->GetRight() + gridSize.cx, newobj->GetBottom() + gridSize.cy);

		// Ensure it is within the boundaries
		if (static_cast<long>(newobj->GetRight()) > m_virtualSize.cx
			|| static_cast<long>(newobj->GetBottom()) > m_virtualSize.cy)
		{
			double height = obj->GetBottom() - newobj->GetTop();
			double width = obj->GetRight() - newobj->GetLeft();
			newobj->SetTop(0);
			newobj->SetLeft(0);
			newobj->SetBottom(height);
			newobj->SetRight(width);
		}

		// If it is still not within the boundaries, shrink it
		if (static_cast<long>(newobj->GetRight()) > m_virtualSize.cx
			|| static_cast<long>(newobj->GetBottom()) > m_virtualSize.cy)
		{
			newobj->SetRight(static_cast<double>(m_virtualSize.cx));
			newobj->SetBottom(static_cast<double>(m_virtualSize.cy));
		}

		// Add the object
		Add(newobj);
	}
}

void CDiagramEntityContainer::Cut(CDiagramEntity* obj)
/* ============================================================
	Function :		CDiagramEntityContainer::Cut
	Description :	Cuts out the object and puts it into the
					'clipboard'
	Access :		Public

	Return :		void
	Parameters :	CDiagramEntity* obj	-	The object to cut.

	Usage :			Call in response to a Cut-command. See also
					the functions for copy/paste below.

   ============================================================*/
{

	Copy(obj);
	Remove(obj);

}

void CDiagramEntityContainer::Copy(CDiagramEntity* obj)
/* ============================================================
	Function :		CDiagramEntityContainer::Copy
	Description :	Copies the object to the 'clipboard'.
	Access :		Public

	Return :		void
	Parameters :	CDiagramEntity* obj	-	The object to copy.

	Usage :			Call in response to a Copy-command. Note
					that obj will only be copied to the
					clipboard, not the screen. See also the
					functions for copy/paste below.

   ============================================================*/
{

	ASSERT(obj);

	if (m_clip == NULL)
		m_clip = &m_internalClip;

	if (obj)
		m_clip->Copy(obj);

}

void CDiagramEntityContainer::Up(CDiagramEntity* obj)
/* ============================================================
	Function :		CDiagramEntityContainer::Up
	Description :	Moves the object one step up in the z-
					order.
	Access :		Public

	Return :		void
	Parameters :	CDiagramEntity* obj	-	The object to move.

	Usage :			Call to move "obj" in the z-order.

   ============================================================*/
{

	INT_PTR index = Find(obj);
	Swap(index, index + 1);

}

void CDiagramEntityContainer::Down(CDiagramEntity* obj)
/* ============================================================
	Function :		CDiagramEntityContainer::Down
	Description :	Moves the object one step down in the z-
					order.
	Access :		Public

	Return :		void
	Parameters :	CDiagramEntity* obj	-	The object to move.

	Usage :			Call to move "obj" in the z-order.

   ============================================================*/
{

	INT_PTR index = Find(obj);
	Swap(index, index - 1);

}

void CDiagramEntityContainer::Front(CDiagramEntity* obj)
/* ============================================================
	Function :		CDiagramEntityContainer::Front
	Description :	Moves "obj" to the top of the z-order.
	Access :		Public

	Return :		void
	Parameters :	CDiagramEntity* obj	-	The object to move.

	Usage :			Call to move "obj" in the z-order.

   ============================================================*/
{

	int index = Find(obj);
	if (index < m_objs.GetSize() - 1)
	{
		m_objs.RemoveAt(index);
		m_objs.Add(obj);
		SetModified(TRUE);
	}

}

void CDiagramEntityContainer::Bottom(CDiagramEntity* obj)
/* ============================================================
	Function :		CDiagramEntityContainer::Bottom
	Description :	Moves "obj" to the bottom of the z-order.
	Access :		Public

	Return :		void
	Parameters :	CDiagramEntity* obj	-	The object to move.

	Usage :			Call to move "obj" in the z-order.

   ============================================================*/
{

	int index = Find(obj);
	if (index > 0)
	{
		m_objs.RemoveAt(index);
		m_objs.InsertAt(0, obj);
		SetModified(TRUE);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CDiagramEntityContainer copy/paste is implemented as separate class.

void CDiagramEntityContainer::SetClipboardHandler(CDiagramClipboardHandler* clip)
/* ============================================================
	Function :		CDiagramEntityContainer::SetClipboardHandler
	Description :	Sets the container clipboard class.
	Access :		Public

	Return :		void
	Parameters :	CDiagramClipboardHandler* clip	-	A pointer
														to the
														class

	Usage :			Call to set the clipboard handler for this
					container. The same clipboard handler
					instance can be used for several containers
					to allow several editors (in an MDI-
					application) to share the same clipboard.

   ============================================================*/
{

	m_clip = clip;

}

CDiagramClipboardHandler* CDiagramEntityContainer::GetClipboardHandler()
/* ============================================================
	Function :		CDiagramEntityContainer::GetClipboardHandler
	Description :	Returns a pointer to the current clipboard
					handler.
	Access :		Public

	Return :		CDiagramClipboardHandler*	-	Current handler.
	Parameters :	none

	Usage :			Call to get a pointer to the current handler.

   ============================================================*/
{

	return m_clip;

}

void CDiagramEntityContainer::CopyAllSelected()
/* ============================================================
	Function :		CDiagramEntityContainer::CopyAllSelected
	Description :	Clones all selected object to the paste
					array.
	Access :		Public

	Return :		void
	Parameters :	none

	Usage :			Call to copy all selected objects to the
					clipboard. "Paste" will put them on screen.

   ============================================================*/
{

	if (m_clip == NULL)
		m_clip = &m_internalClip;

	m_clip->CopyAllSelected(this);

}

INT_PTR CDiagramEntityContainer::ObjectsInPaste()
/* ============================================================
	Function :		CDiagramEntityContainer::ObjectsInPaste
	Description :	Returns the number of objects in the paste
					array.
	Access :		Public

	Return :		int		-	The number of objects.
	Parameters :	none

	Usage :			Call to get the number of objects in the
					clipboard.

   ============================================================*/
{

	if (m_clip == NULL)
		m_clip = &m_internalClip;

	return m_clip->ObjectsInPaste();

}

void CDiagramEntityContainer::ClearPaste()
/* ============================================================
	Function :		CDiagramEntityContainer::ClearPaste
	Description :	Clears the paste-array.
	Access :		Public

	Return :		void
	Parameters :	none

	Usage :			Call to clear the clipboard. All memory is
					released.

   ============================================================*/
{

	if (m_clip == NULL)
		m_clip = &m_internalClip;

	m_clip->ClearPaste();

}

void CDiagramEntityContainer::Paste()
/* ============================================================
	Function :		CDiagramEntityContainer::Paste
	Description :	Clones the contents of the paste array
					into the container data array.
	Access :		Public

	Return :		void
	Parameters :	none

	Usage :			Call to paste the contents of the clipboard
					to screen.

   ============================================================*/
{

	if (m_clip == NULL)
		m_clip = &m_internalClip;

	m_clip->Paste(this);

}

/////////////////////////////////////////////////////////////////////////////
// CDiagramEntityContainer message handling

void CDiagramEntityContainer::SendMessageToObjects(int command, BOOL selected, CDiagramEntity* sender, CWnd* from)
/* ============================================================
	Function :		CDiagramEntityContainer::SendMessageToObjects
	Description :	Sends "command" to objects.
	Access :		Public

	Return :		void
	Parameters :	int command				-	The command to send.
					BOOL selected			-	If "TRUE", the command
												will only be sent to
												selected objects,
												otherwise, it will be
												sent to all objects.
					CDiagramEntity* sender	-	Original sender
												or "NULL" if not
												an object.

	Usage :			Call this member to send messages to
					(selected) objects in the range "CMD_START"
					to "CMD_END" inclusively (defined in
					DiagramEntity.h). Calls the object "DoCommand".

   ============================================================*/
{

	BOOL stop = FALSE;
	INT_PTR max = m_objs.GetSize();
	for (INT_PTR t = 0; t < max; t++)
	{
		CDiagramEntity* obj = GetAt(t);
		if (!stop && (!selected || obj->IsSelected()))
		{
			stop = obj->DoMessage(command, sender, from);
			SetModified(TRUE);
		}
	}

}

/////////////////////////////////////////////////////////////////////////////
// CDiagramEntityContainer private helpers

int CDiagramEntityContainer::Find(CDiagramEntity* testobj)
/* ============================================================
	Function :		CDiagramEntityContainer::Find
	Description :	Finds the index of object "testobj" in the
					data array.
	Access :		Protected

	Return :		int						-	Index of the
												object or -1
												if not found.
	Parameters :	CDiagramEntity* testobj	-	Object to find.

	Usage :			Internal function.

   ============================================================*/
{

	int index = -1;
	CDiagramEntity* obj;
	int count = 0;
	while ((obj = GetAt(count)))
	{
		if (obj == testobj)
			index = count;
		count++;
	}

	return index;

}

void CDiagramEntityContainer::Swap(INT_PTR index1, INT_PTR index2)
/* ============================================================
	Function :		CDiagramEntityContainer::Swap
	Description :	Swaps the elements at "index1" and "index2".
	Access :		Private

	Return :		void
	Parameters :	int index1	-	First object to swap
					int index2	-	Second object to swap

	Usage :			Internal function. Used to move objects up
					or down in the z-order.

   ============================================================*/
{

	INT_PTR max = m_objs.GetSize();
	if (index1 >= 0 && index1 < max && index2 >= 0 && index2 < max)
	{
		CDiagramEntity* obj1 = GetAt(index1);
		CDiagramEntity* obj2 = GetAt(index2);
		SetAt(index1, obj2);
		SetAt(index2, obj1);
	}

}

void CDiagramEntityContainer::Undo()
/* ============================================================
	Function :		CDiagramEntityContainer::Undo
	Description :	Sets the container data to the last entry
					in the undo stack.
	Access :		Public

	Return :		void
	Parameters :	none

	Usage :			Call to undo the last operation

   ============================================================*/
{
	if (m_undo.GetSize())
	{
		// Push the current state to the redo stack
		Push(&m_redo);

		// We remove all current data
		RemoveAll();

		// We get the last entry from the undo-stack
		// and clone it into the container data
		CUndoItem* undo = static_cast<CUndoItem*>(m_undo.GetAt(m_undo.GetUpperBound()));
		INT_PTR count = (undo->arr).GetSize();
		for (INT_PTR t = 0; t < count; t++)
		{
			CDiagramEntity* obj = static_cast<CDiagramEntity*>((undo->arr).GetAt(t));
			Add(obj->Clone());
		}

		// Set the background color
		SetColor(undo->col);

		// Set the saved virtual size as well
		SetVirtualSize(undo->pt);

		// We remove the entry from the undo-stack
		delete undo;

		m_undo.RemoveAt(m_undo.GetUpperBound());
	}
}

void CDiagramEntityContainer::Snapshot()
/* ============================================================
	Function :		CDiagramEntityContainer::Snapshot
	Description :	Copies the current state of the data to
					the undo-stack.
	Access :		Public

	Return :		void
	Parameters :	none

	Usage :			Call to add the current state to the undo-stack.
					If the undo stack has a maximum size and
					the stack will grow above the stack limit,
					the first undo array will be removed.

   ============================================================*/
{
	// Clear the redo stack
	ClearRedo();

	// Push the current state to the undo stack
	Push(&m_undo);
}

void CDiagramEntityContainer::ClearUndo()
/* ============================================================
	Function :		CDiagramEntityContainer::ClearUndo
	Description :	Remove all undo arrays from the undo stack
	Access :		Public

	Return :		void
	Parameters :	none

	Usage :			Call to clear the undo-stack. All memory will
					be deleted.

   ============================================================*/
{

	INT_PTR count = m_undo.GetSize() - 1;
	for (INT_PTR t = count; t >= 0; t--)
	{
		CUndoItem* undo = static_cast<CUndoItem*>(m_undo.GetAt(t));
		// Remove the stack entry itself.
		delete undo;
	}

	m_undo.RemoveAll();

}

BOOL CDiagramEntityContainer::IsUndoPossible() const
/* ============================================================
	Function :		CDiagramEntityContainer::IsUndoPossible
	Description :	Check if it is possible to undo.
	Access :		Public

	Return :		BOOL	-	"TRUE" if undo is possible.
	Parameters :	none

	Usage :			Use this call for command enabling

   ============================================================*/
{

	return m_undo.GetSize() > 0;

}


void CDiagramEntityContainer::SetUndoStackSize(int maxstacksize)
/* ============================================================
	Function :		CDiagramEntityContainer::SetUndoStackSize
	Description :	Sets the size of the undo stack.
	Access :		Public

	Return :		void
	Parameters :	int maxstacksize	-	New size. -1 means
											no limit, 0 no undo.

	Usage :			Call to set the max undo stack size.

   ============================================================*/
{

	m_maxstacksize = maxstacksize;

}

int CDiagramEntityContainer::GetUndoStackSize() const
/* ============================================================
	Function :		CDiagramEntityContainer::GetUndoStackSize
	Description :	Returns the size of the undo-stack
	Access :		Public

	Return :		int	-	Current size
	Parameters :	none

	Usage :			Call to get the max undo stack size.

   ============================================================*/
{

	return m_maxstacksize;

}

void CDiagramEntityContainer::Redo()
/* ============================================================
	Function :		CDiagramEntityContainer::Redo
	Description :	Sets the container data to the last entry
					in the redo stack.
	Access :		Public

	Return :		void
	Parameters :	none

	Usage :			Call to redo the last operation

   ============================================================*/
{

	if (m_redo.GetSize())
	{
		// Push the current state to the undo stack
		Push(&m_undo);

		// We remove all current data
		RemoveAll();

		// We get the last entry from the redo-stack
		// and clone it into the container data
		CUndoItem* redo = static_cast<CUndoItem*>(m_redo.GetAt(m_redo.GetUpperBound()));
		INT_PTR count = (redo->arr).GetSize();
		for (INT_PTR t = 0; t < count; t++)
		{
			CDiagramEntity* obj = static_cast<CDiagramEntity*>((redo->arr).GetAt(t));
			Add(obj->Clone());
		}

		// Set the background color
		SetColor(redo->col);

		// Set the saved virtual size as well
		SetVirtualSize(redo->pt);

		// We remove the entry from the redo-stack
		delete redo;

		m_redo.RemoveAt(m_redo.GetUpperBound());
	}
}

void CDiagramEntityContainer::Push(CObArray* stack)
/* ============================================================
	Function :		CDiagramEntityContainer::PushRedo
	Description :	Copies the current state of the data to
					the specified stack.
	Access :		Public

	Return :		void
	Parameters :	none

	Usage :			Call to add the current state to the stack.
					If the stack has a maximum size and
					the stack will grow above the stack limit,
					the first array will be removed.

   ============================================================*/
{
	if (m_maxstacksize > 0 && stack->GetSize() == m_maxstacksize)
	{
		delete stack->GetAt(0);
		stack->RemoveAt(0);
	}

	CUndoItem* item = new CUndoItem;

	while (!item && stack->GetSize())
	{
		// We seem - however unlikely - to be out of memory.
		// Remove first element in stack and try again
		delete stack->GetAt(0);
		stack->RemoveAt(0);
		item = new CUndoItem;
	}

	if (item)
	{
		// Save the background color
		item->col = GetColor();

		// Save current virtual size
		item->pt = GetVirtualSize();

		// Save all objects
		INT_PTR count = m_objs.GetSize();
		for (INT_PTR t = 0; t < count; t++)
			(item->arr).Add(GetAt(t)->Clone());

		// Add to stack
		stack->Add(item);
	}
}

void CDiagramEntityContainer::ClearRedo()
/* ============================================================
	Function :		CDiagramEntityContainer::ClearRedo
	Description :	Remove all redo arrays from the redo stack
	Access :		Public

	Return :		void
	Parameters :	none

	Usage :			Call to clear the redo-stack. All memory will
					be deleted.

   ============================================================*/
{

	INT_PTR count = m_redo.GetSize() - 1;
	for (INT_PTR t = count; t >= 0; t--)
	{
		CUndoItem* redo = static_cast<CUndoItem*>(m_redo.GetAt(t));
		// Remove the stack entry itself.
		delete redo;
	}

	m_redo.RemoveAll();

}

BOOL CDiagramEntityContainer::IsRedoPossible() const
/* ============================================================
	Function :		CDiagramEntityContainer::IsRedoPossible
	Description :	Check if it is possible to redo.
	Access :		Public

	Return :		BOOL	-	"TRUE" if redo is possible.
	Parameters :	none

	Usage :			Use this call for command enabling

   ============================================================*/
{

	return m_redo.GetSize() > 0;

}

CObArray* CDiagramEntityContainer::GetData()
/* ============================================================
	Function :		CDiagramEntityContainer::GetData
	Description :	Accessor for the internal data array
	Access :		Public

	Return :		CObArray*	-	A pointer to the internal
									data array.
	Parameters :	none

	Usage :			Call to access the internal data array. To
					be used in derived classes.

   ============================================================*/
{

	return &m_objs;

}

CObArray* CDiagramEntityContainer::GetPaste()
/* ============================================================
	Function :		CDiagramEntityContainer::GetPaste
	Description :	Accessor for the internal paste array
	Access :		Protected

	Return :		CObArray*	-	A pointer to the paste
									array
	Parameters :	none

	Usage :			Call to access the internal paste array. To
					be used in derived classes.

   ============================================================*/
{

	CObArray* arr = NULL;
	if (m_clip)
		arr = m_clip->GetData();

	return arr;

}

CObArray* CDiagramEntityContainer::GetUndo()
/* ============================================================
	Function :		CDiagramEntityContainer::GetUndo
	Description :	Accessor for the internal undo array
	Access :		Protected

	Return :		CObArray*	-	A pointer to the undo
									array
	Parameters :	none

	Usage :			Call to access the internal undo array. To
					be used in derived classes.

   ============================================================*/
{

	return &m_undo;

}

CObArray* CDiagramEntityContainer::GetRedo()
/* ============================================================
	Function :		CDiagramEntityContainer::GetRedo
	Description :	Accessor for the internal redo array
	Access :		Protected

	Return :		CObArray*	-	A pointer to the redo
									array
	Parameters :	none

	Usage :			Call to access the internal redo array. To
					be used in derived classes.

   ============================================================*/
{

	return &m_redo;

}

void CDiagramEntityContainer::Group()
/* ============================================================
	Function :		CDiagramEntityContainer::Group
	Description :	Groups the currently selected objects into
					the same group.
	Access :		Public

	Return :		void
	Parameters :	none

	Usage :			Call to group all selected items into the
					same group.
					Grouped objects can be moved as a
					single entity. Technically, when one object
					in a group is selected, all other objects
					are also selected automatically.

   ============================================================*/
{

	CDiagramEntity* obj;
	int count = 0;
	int group = CGroupFactory::GetNewGroup();
	while ((obj = GetAt(count)))
	{
		if (obj->IsSelected())
			obj->SetGroup(group);
		count++;
	}

}

void CDiagramEntityContainer::Ungroup()
/* ============================================================
	Function :		CDiagramEntityContainer::Ungroup
	Description :	Ungroups the currently selected objects.
	Access :		Public

	Return :		void
	Parameters :	none

	Usage :			Call to ungroup all selected items.
					Grouped objects can be moved as a
					single entity. Technically, when one object
					in a group is selected, all other objects
					are also selected automatically.

   ============================================================*/
{

	CDiagramEntity* obj;
	int count = 0;
	while ((obj = GetAt(count)))
	{
		if (obj->IsSelected())
			obj->SetGroup(0);
		count++;
	}

}

CSize CDiagramEntityContainer::GetTotalSize()
/* ============================================================
	Function :		CDiagramEntityContainer::GetTotalSize
	Description :	Gets the minimum bounding size for the
					objects in the container.
	Access :

	Return :		CSize	-	Minimum bounding size
	Parameters :	none

	Usage :			Call to get the screen size of the objects
					in the container.

   ============================================================*/
{
	CPoint start = GetStartPoint();
	double width = 0;
	double height = 0;

	CDiagramEntity* obj;
	int count = 0;
	while ((obj = GetAt(count)))
	{

		width = max(width, obj->GetLeft());
		width = max(width, obj->GetRight());
		height = max(height, obj->GetTop());
		height = max(height, obj->GetBottom());

		count++;

	}

	return CSize(round(width - start.x), round(height - start.y));

}

CPoint CDiagramEntityContainer::GetStartPoint()
/* ============================================================
	Function :		CDiagramEntityContainer::GetStartPoint
	Description :	Gets the starting screen position of the
					objects in the container (normally the
					top-left corner of the top-left object).
	Access :

	Return :		CPoint	-	Top-left position of the
								objects.
	Parameters :	none

	Usage :			Call to get the starting point on screen of
					the objects.

   ============================================================*/
{

	double startx = 2000.0;
	double starty = 2000.0;

	CDiagramEntity* obj;
	int count = 0;

	while ((obj = GetAt(count)))
	{

		startx = min(startx, obj->GetLeft());
		startx = min(startx, obj->GetRight());
		starty = min(starty, obj->GetTop());
		starty = min(starty, obj->GetBottom());

		count++;

	}

	return CPoint(round(startx), round(starty));

}

INT_PTR	CDiagramEntityContainer::GetSelectCount() const
/* ============================================================
	Function :		int	CDiagramEntityContainer::GetSelectCount
	Description :	Gets the number of currently selected
					objects in the container.
	Access :

	Return :		int		-	Currently selected objects.
	Parameters :	none

	Usage :			Call to get the number of selected objects.

   ============================================================*/
{

	INT_PTR max = GetSize();
	INT_PTR count = 0;
	for (INT_PTR t = 0; t < max; t++)
		if (GetAt(t)->IsSelected())
			count++;

	return count;

}

void CDiagramEntityContainer::SelectAll()
{
	INT_PTR max = GetSize();
	for (INT_PTR t = 0; t < max; t++)
		GetAt(t)->Select(TRUE);
}

void CDiagramEntityContainer::UnselectAll()
{
	INT_PTR max = GetSize();
	for (INT_PTR t = 0; t < max; t++)
		GetAt(t)->Select(FALSE);
}

#pragma warning( default : 4706 )
