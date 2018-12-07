#ifndef _DIAGRAMENTITYCONTAINER_H_
#define _DIAGRAMENTITYCONTAINER_H_
class CDiagramEntity;
#include "DiagramClipboardHandler.h"
#include "UndoItem.h"

// Restraint modes
#define RESTRAINT_NONE		0
#define RESTRAINT_VIRTUAL	1
#define RESTRAINT_MARGIN	2

class CDiagramEntityContainer {

public:

	// Construction/destruction/initialization
	CDiagramEntityContainer(CDiagramClipboardHandler* clip = NULL);
	virtual ~CDiagramEntityContainer();
	void Clear();
	virtual CString	GetString() const;
	virtual BOOL FromString(const CString& str);
	virtual void Export(CStringArray& stra, UINT format = 0) const;
	virtual void SetClipboardHandler(CDiagramClipboardHandler* clip);
	virtual CDiagramClipboardHandler* GetClipboardHandler();

	// Data access
	virtual CDiagramEntity* GetAt(INT_PTR index) const;
	CObArray*		GetData();
	INT_PTR			GetSize() const;
	virtual void	Add(CDiagramEntity* obj);
	virtual void	RemoveAt(INT_PTR index);
	virtual void	RemoveAll();
	virtual void	RemoveAllSelected();
	virtual void	Remove(CDiagramEntity* obj);

	void			SetVirtualSize(CSize size);
	CSize			GetVirtualSize() const;

	void			SetColor(COLORREF color);
	COLORREF		GetColor() const;

	void		ShowGrid(BOOL grid);
	BOOL		IsGridVisible() const;

	void		SetGridColor(COLORREF col);
	COLORREF	GetGridColor() const;

	void		SetGridPenStyle(int style);
	int			GetGridPenStyle() const;

	void		SetGridSize(CSize size);
	CSize		GetGridSize() const;

	void		SetSnapToGrid(BOOL snap);
	BOOL		GetSnapToGrid() const;

	void		SetMargins(int left, int top, int right, int bottom);
	void		GetMargins(int& left, int& top, int& right, int& bottom) const;
	void		SetMarginColor(COLORREF marginColor);
	COLORREF	GetMarginColor() const;
	void		ShowMargin(BOOL show);
	BOOL		IsMarginVisible() const;

	int			GetRestraints() const;
	void		SetRestraints(int restraint);

	void			SetModified(BOOL dirty);
	BOOL			IsModified() const;

	virtual void	SelectAll();
	virtual void	UnselectAll();
	INT_PTR			GetSelectCount() const;

	// Undo handling
	virtual void	Undo();
	virtual void	Snapshot();
	BOOL			IsUndoPossible() const;
	virtual void	ClearUndo();
	void			SetUndoStackSize(int maxstacksize);
	int				GetUndoStackSize() const;
	void			PopUndo();

	// Group handling
	virtual void	Group();
	virtual void	Ungroup();

	// Single object handlers
	virtual void			Duplicate(CDiagramEntity* obj);
	virtual void			Cut(CDiagramEntity* obj);
	virtual void			Copy(CDiagramEntity* obj);
	virtual void			Up(CDiagramEntity* obj);
	virtual void			Down(CDiagramEntity* obj);
	virtual void			Front(CDiagramEntity* obj);
	virtual void			Bottom(CDiagramEntity* obj);
	virtual void			Paste();

	// Copy/paste
	virtual void			CopyAllSelected();
	virtual INT_PTR			ObjectsInPaste();
	virtual void			ClearPaste();

	// Message handling
	virtual void			SendMessageToObjects(int command, BOOL selected = TRUE, CDiagramEntity* sender = NULL, CWnd* from = NULL);

	// Positional information
	CSize					GetTotalSize();
	CPoint					GetStartPoint();

protected:
	CObArray * GetPaste();
	CObArray*				GetUndo();
	int						Find(CDiagramEntity* obj);

private:

	// Data
	CObArray		m_objs;
	CObArray		m_undo;
	int				m_maxstacksize;
	CSize			m_virtualSize;
	COLORREF		m_color; // Current bg color

	BOOL		m_snap;				// TRUE if we should snap to grid
	BOOL		m_grid;				// TRUE if the background grid should be displayed
	int			m_gridStyle;		// Grid style
	CSize		m_gridSize;			// Size of a grid cell
	COLORREF	m_gridCol;			// Color of the grid

	BOOL		m_margin;			// TRUE if margins should be drawn
	COLORREF	m_marginColor;		// Color of the margin
	int			m_leftMargin;		// Left margin in pixels
	int			m_topMargin;		// Top margin in pixels
	int			m_rightMargin;		// Right margin in pixels
	int			m_bottomMargin;		// Bottom margin in pixels

	int			m_restraint;		// Restraint mode ( none, virtual or margin )

	CDiagramClipboardHandler*	m_clip;
	CDiagramClipboardHandler	m_internalClip;

	// State
	BOOL			m_dirty;

	// Helpers
	void			Swap(INT_PTR index1, INT_PTR index2);
	void			SetAt(INT_PTR index, CDiagramEntity* obj);

};

#endif // _DIAGRAMENTITYCONTAINER_H_
