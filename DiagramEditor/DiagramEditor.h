#if !defined(AFX_DIAGRAMEDITOR_H__B8C35A07_CDC4_4D85_9FAE_2C9BE81EA911__INCLUDED_)
#define AFX_DIAGRAMEDITOR_H__B8C35A07_CDC4_4D85_9FAE_2C9BE81EA911__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DiagramEditor.h : header file
//
#include "DiagramEntity.h"
#include "DiagramEntityContainer.h"
#include "DiagramMenu.h"

// Current mouse mode
#define MODE_NONE			0
#define MODE_RUBBERBANDING	1
#define MODE_MOVING			2
#define MODE_RESIZING		3
#define MODE_DRAWING		4
#define MODE_BGRESIZING		5

#define KEY_NONE			  0
#define KEY_ARROW			  1 // Arrow keys
#define KEY_PGUPDOWN		  4	// Pg up & pg down
#define KEY_DELETE			  8	// Delete key
#define KEY_ESCAPE			 16	// Escape key
#define KEY_INSERT			 32	// Insert key
#define KEY_PLUSMINUS		 64	// Plus- and minus key
#define KEY_CTRL			128	// Ctrl+A,Z,X,C,V, Enter

#define KEY_ALL				0xFFFFFFFF

#define WHEEL_SCROLL	0
#define WHEEL_ZOOM		1

/////////////////////////////////////////////////////////////////////////////
// CDiagramEditor window

class CDiagramEditor : public CWnd
{
	friend class CDiagramMenu;

public:
	// Construction/destruction/initialization
	CDiagramEditor();
	virtual ~CDiagramEditor();

	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, CDiagramEntityContainer* data = NULL);

	virtual void Clear();

	void SetDiagramEntityContainer(CDiagramEntityContainer* objs);
	CDiagramEntityContainer* GetDiagramEntityContainer() const;

	// Visuals
	virtual void	Draw(CDC* dc, CRect rect);
	virtual void	Print(CDC* dc, CRect rect, double zoom);

protected:
	virtual void EraseBackground(CDC* dc, CRect rect) const;
	virtual void DrawBackground(CDC* dc, CRect rect, double zoom) const;
	virtual void DrawGrid(CBitmap* bmp, CRect rect, double zoom) const;
	virtual void DrawMargins(CDC* dc, CRect rect, double zoom) const;
	virtual void DrawObjects(CDC* dc, double zoom) const;
	virtual void DrawSelectionMarkers(CDC* dc) const;

public:
	// Property Accessors
	void		SetVirtualSize(const CSize& size);
	CSize		GetVirtualSize() const;

	void		SetBackgroundColor(COLORREF col);
	COLORREF	GetBackgroundColor() const;
	void		SetNonClientColor(COLORREF col);

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

	void		SetResize(BOOL bgresize);
	BOOL		GetResize() const;
	void		SetResizeZone(int bgresize);
	int			GetResizeZone() const;

	void		SetMargins(int left, int top, int right, int bottom);
	void		GetMargins(int& left, int& top, int& right, int& bottom) const;
	void		SetMarginColor(COLORREF marginColor);
	COLORREF	GetMarginColor() const;
	void		ShowMargin(BOOL show);
	BOOL		IsMarginVisible() const;

	int			GetRestraints() const;
	void		SetRestraints(int restraint);

	BOOL		GetMultidraw() const;
	void		SetMultidraw(BOOL multidraw);

	virtual void	SetZoom(double zoom);
	double			GetZoom() const;
	void			SetZoomFactor(double zoomfactor);
	double			GetZoomFactor() const;
	void			SetZoomMax(double zoommax);
	double			GetZoomMax() const;
	void			SetZoomMin(double zoommin);
	double			GetZoomMin() const;

	void		ZoomToFitScreen();
	BOOL		ZoomToFit(CPoint start, CSize size);
	BOOL		ZoomToFit(CSize size);
	void		ScrollIntoView();
	void		ScrollIntoView(CDiagramEntity* obj);

	CSize		GetMarkerSize() const;
	void		SetMarkerSize(CSize markerSize);

	UINT		GetKeyboardInterface() const;
	void		SetKeyboardInterface(int keyInterface);

	void		SetPopupMenu(CDiagramMenu* popupmenu);
	CDiagramMenu* GetPopupMenu() const;

	BOOL		IsModified() const;
	void		SetModified(BOOL dirty);

	// Data access
	virtual void	AddObject(CDiagramEntity* obj);
	virtual void	MoveObject(CDiagramEntity* obj, const CRect& rect);

	INT_PTR			GetObjectCount() const;
	INT_PTR			GetSelectCount() const;
	CDiagramEntity* GetObject(int index) const;
	BOOL			IsDrawing() const;
	CDiagramEntity* GetSelectedObject() const;
	BOOL			IsAnyObjectSelected() const;

	virtual void	StartDrawingObject(CDiagramEntity* obj);

	// Group object operations
	virtual void	New();
	virtual void	SelectAll();
	virtual void	UnselectAll();
	void			DeleteAll();
	virtual void	DeleteAllSelected();

	virtual void	LeftAlignSelected();
	virtual void	RightAlignSelected();
	virtual void	TopAlignSelected();
	virtual void	BottomAlignSelected();
	virtual void	MakeSameSizeSelected();

	// Copy/paste
	virtual void		Cut();
	virtual void		Copy();
	virtual void		Paste();
	virtual void		Undo();
	virtual void		Redo();

	// Group 
	void		Group();
	void		Ungroup();

	// Panning
	BOOL			GetPanning() const;
	CPoint			GetPanningOrigin() const;
	void			SetPanning(BOOL panning);
	void			SetPanningOrigin(CPoint point);
	virtual void	DrawPanning(CDC* dc) const;

	// Scroll-wheel
	void			SetScrollWheelMode(int mode);
	int				GetScrollWheelMode() const;

	// Single object operations
	void		Duplicate();
	void		Up();
	void		Down();
	void		Front();
	void		Bottom();

	// Background resizing
	virtual int		GetHitCode(CPoint point);
	virtual CRect	GetSelectionMarkerRect(UINT marker) const;
	virtual HCURSOR GetCursor(int hit) const;

	// Command enablers for Doc/View apps
	void		UpdateCut(CCmdUI* pCmdUI) const;
	void		UpdateCopy(CCmdUI* pCmdUI) const;
	void		UpdatePaste(CCmdUI* pCmdUI) const;
	void		UpdateUndo(CCmdUI* pCmdUI) const;
	void		UpdateRedo(CCmdUI* pCmdUI) const;
	void		UpdateGroup(CCmdUI* pCmdUI) const;
	void		UpdateUngroup(CCmdUI* pCmdUI) const;

	// Property handling
	void		ShowProperties();

	// Saving and loading
	virtual void Save(CStringArray& stra);
	virtual BOOL FromString(const CString& str);

protected:
	virtual void	SaveObjects(CStringArray& stra);

	virtual void	SetInteractMode(int interactMode, int subMode = 0);
	virtual int		GetInteractMode() const;
	virtual			CDiagramEntity* GetDrawingObject();

	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CDiagramEditor)
		//}}AFX_VIRTUAL

		// Generated message map functions
protected:
	//{{AFX_MSG(CDiagramEditor)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	virtual afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	virtual afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	virtual afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	virtual afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	virtual afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg UINT OnGetDlgCode();
	virtual afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	virtual afx_msg void OnObjectCommand(UINT nID);
	virtual afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	virtual afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	afx_msg void OnEditUndo();
	afx_msg void OnEditRedo();
	afx_msg void OnEditCut();
	afx_msg void OnEditCopy();
	afx_msg void OnEditPaste();
	afx_msg void OnEditDelete();
	afx_msg void OnEditSelectAll();

	DECLARE_MESSAGE_MAP()

	void SetInternalDiagramEntityContainer(CDiagramEntityContainer* objs);
	virtual HCURSOR GetCursor();

	virtual void SetHScroll(int pos);
	virtual void SetVScroll(int pos);

private:

	// Run-time states/data
	double		m_zoom;				// Current zoom level
	double		m_zoomFactor;		// Zoom factor for +/- keys
	double		m_zoomMax;			// Max zoom level
	double		m_zoomMin;			// Min zoom level
	BOOL		m_dirty;			// TRUE if data is modified

	int			m_interactMode;		// Current mouse-mode
	int			m_subMode;			// Sub-mode for resizing (corner)
	CRect		m_selectionRect;	// Rect to draw for rubberbanding
	CPoint		m_deltaPoint;		// Offset to object when moving
	BOOL		m_drawing;			// We are currently drawing
	BOOL		m_multiDraw;		// If the drawing mode is continued 
									// after an object is added.

	CDiagramEntity* m_multiSelObj;	// Primary object when moving multiple

	// Properties
	COLORREF	m_nonClientBkgndCol;// Background of non-paper area

	int			m_bgResizeZone;		// Size, in pixels, of resize zone
	BOOL		m_bgResize;			// TRUE if the paper can be resized
	BOOL		m_bgResizeSelected;	// TRUE if we are resizing the background

	CSize		m_markerSize;		// Size of selection marker

	UINT		m_keyInterface;		// Flags for the keys the editor will handle

	CPoint		m_panOrigin;
	BOOL		m_panning;
	UINT_PTR	m_panningTimer;

	// Data pointer
	CDiagramEntityContainer*	m_objs;	// Pointer to data

	CDiagramMenu*				m_popupMenu;

	// Misc data
	CDiagramEntity* m_drawObj;	// Temporary pointer to object that should be drawn
	CDiagramEntityContainer* m_internalData;	// Internal data pointer - if external data is not submitted

public:
	// Coordinate conversions
	void		ScreenToVirtual(CRect& rect) const;
	void		ScreenToVirtual(CPoint& point) const;
	void		ScreenToVirtual(CSize& size) const;
	void		VirtualToScreen(CRect& rect) const;
	void		VirtualToScreen(CPoint& point) const;

protected:
	// Scroll
	void		SetupScrollbars();
	int			HScroll(int scroll);
	int			VScroll(int scroll);
	CPoint		ScrollPoint(CPoint point);

	// Coordinate modifications
	int			SnapX(int coord) const;
	int			SnapY(int coord) const;
	CSize		GetContainingSize() const;
	void		InsideRestraints(double& x, double& y);
	void		AdjustForRestraints(double& left, double& top, double& right, double& bottom);
	void		AdjustForRestraints(double& xpos, double& ypos);
	BOOL		OutsideRestraints(CPoint point);

private:
	// Misc internal functions
	void		SetInternalBackgroundColor(COLORREF col);
	void		SetInternalVirtualSize(const CSize& size);
	void		RemoveUnselectedPropertyDialogs();
	void		ShowPopup(CPoint point);

	// Panning cursors
	HCURSOR		m_cursorNorth;
	HCURSOR		m_cursorNorthEast;
	HCURSOR		m_cursorEast;
	HCURSOR		m_cursorSouthEast;
	HCURSOR		m_cursorSouth;
	HCURSOR		m_cursorSouthWest;
	HCURSOR		m_cursorWest;
	HCURSOR		m_cursorNorthWest;

	int			m_scrollwheel;		// Scroll wheel mode, scroll or zoom

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIAGRAMEDITOR_H__B8C35A07_CDC4_4D85_9FAE_2C9BE81EA911__INCLUDED_)
