#if !defined(AFX_UMLEDITOR_H__609E7C31_5443_4059_8ED9_DF9A743CFD2F__INCLUDED_)
#define AFX_UMLEDITOR_H__609E7C31_5443_4059_8ED9_DF9A743CFD2F__INCLUDED_

#include "DiagramEditor\DiagramEditor.h"

#include "UMLEntity.h"
#include "UMLLineSegment.h"
#include "UMLEntityContainer.h"

class CUMLEditor : public CDiagramEditor
{

public:
// Construction/initialization/destruction
	CUMLEditor();
	virtual ~CUMLEditor();
	virtual BOOL Create( DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, CDiagramEntityContainer* data = NULL );

// Operations
	BOOL	IsLinkSelected() const;
	void	FlipLink();

	CString GetPackage() const;
	void SetPackage( const CString& package );
	void SetDisplayOptions( int displayOption );
	int GetDisplayOptions() const;
	void SetColor( COLORREF col );
	CUMLEntity* GetAllNamedObject( const CString& name ) const;

	virtual void	StartDrawingObject( CDiagramEntity* obj );
	virtual void	SelectAll();

	void Save( CString& filename );
	void Load( CString& filename );

	void ExportCPP( const CString& project, BOOL bHeaderOnly = FALSE );
	void ExportHTML();
	void ExportEMF();

	void SetStripLeadingClassCharacter( BOOL stripLeadingClassLetter );
	BOOL GetStripLeadingClassCharacter() const;

	void	Import();

protected:
// Overrides:
	virtual void DrawObjects( CDC* dc, double zoom ) const;
	virtual afx_msg void OnObjectCommand( UINT nID );

// Message handlers
	virtual afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	virtual afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	virtual afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	virtual afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	virtual afx_msg void OnRButtonUp(UINT nFlags, CPoint point);

private:
// Private helpers
	void			ModifyLinkedPositions();
	void			AdjustLinkedObjects( CUMLEntity* parent, CUMLEntity* filter = NULL );
	CUMLEntity*		GetNamedObject( const CString& name ) const;

	BOOL			IsConnected( CUMLLineSegment* link ) const;
	CUMLEntity*		GetObjectAt( int index ) const;

	BOOL			PartialLinesSelected() const;
	CString			BrowseForFolder();

// Private data
	BOOL	m_drawingLine;

	HCURSOR	m_curDrawLine;
	HCURSOR	m_curAttach;
	HCURSOR	m_curUp;

	CUMLEntityContainer*	GetUMLEntityContainer() const;
	void	SetCursor( CPoint point );

	HICON	m_currentCursor;

	BOOL	m_stripLeadingClassLetter;

	CUMLEntityContainer*	m_data;
	CString					m_exportPath;

};

#endif // !defined(AFX_UMLEDITOR_H__609E7C31_5443_4059_8ED9_DF9A743CFD2F__INCLUDED_)
