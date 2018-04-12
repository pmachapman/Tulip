#if !defined(AFX_UMLLINESEGMENT_H__8BBDE424_2729_426C_89EC_B3E3E1021F92__INCLUDED_)
#define AFX_UMLLINESEGMENT_H__8BBDE424_2729_426C_89EC_B3E3E1021F92__INCLUDED_

#include "UMLEntity.h"

// Line styles
#define STYLE_NONE				0
#define STYLE_ARROWHEAD			1
#define STYLE_FILLED_ARROWHEAD	2
#define STYLE_CIRCLECROSS		4
#define STYLE_FILLED_DIAMOND	8
#define STYLE_DASHED			16
#define STYLE_INVISIBLE			32

#include "UMLLinkPropertyDialog.h"

class CUMLLineSegment : public CUMLEntity
{

public:

// Creation/initialization/destruction
	CUMLLineSegment();
	virtual ~CUMLLineSegment();
	virtual	CDiagramEntity* Clone();

	static	CDiagramEntity* CreateFromString( const CString& str );

// Overrides
	virtual void		Draw( CDC* dc, CRect rect );
	virtual int			GetHitCode( CPoint point ) const;
	virtual HCURSOR		GetCursor( int hit ) const;
	virtual void		SetRect( CRect rect );
	virtual BOOL		BodyInRect( CRect rect ) const;
	virtual CPoint		GetLinkPosition( int type ) const;
	virtual CString		GetString() const;
	virtual BOOL		FromString( const CString& str );
	virtual CString		Export( UINT format = 0 ) const;
	virtual BOOL		DoMessage( UINT msg, CDiagramEntity* sender, CWnd* from );
	virtual void		ShowPopup( CPoint point, CWnd* parent );
	virtual void		SetRect( double left, double top, double right, double bottom );
	virtual void		Copy( CDiagramEntity* obj );

	// Link style
	void				SetStyle( int style );
	int					GetStyle() const;
	void				SetLineStyle( int style );
	void				AddLineStyle( int style );
	int					GetLineStyle() const;
	void				RemoveLineStyle( int style );

	// Other line information
	BOOL				IsHorizontal() const;
	BOOL				IsSingleLineSegment() const;

	// Accessors
	CString				GetStartLabel() const;
	void				SetStartLabel( const CString& label );
	CString				GetEndLabel() const;
	void				SetEndLabel( const CString& label );
	CString				GetSecondaryStartLabel() const;
	void				SetSecondaryStartLabel( const CString& label );
	CString				GetSecondaryEndLabel() const;
	void				SetSecondaryEndLabel( const CString& label );


	// Links
	virtual void		SetLink( int type, const CString& name );
	virtual void		SetLinkType( int type, int targetType );
	virtual CString		GetLink( int type ) const;
	virtual int			GetLinkType( int type ) const;
	virtual int			GetLinkCode( CPoint point ) const;

	int					GetOffset( int type ) const;
	void				SetOffset( int type, int linkOffset );

	// Operations
	void				Flip();

protected:
	virtual void		DrawSelectionMarkers( CDC* dc, CRect rect ) const;

private:

	// Line style
	int			m_style;

	// Labels
	CString		m_startLabel;
	CString		m_endLabel;
	CString		m_secondaryStartLabel;
	CString		m_secondaryEndLabel;	

	CString		m_start;				// Object linked to the start
	CString		m_end;					// Object linked to the end

	int			m_startType;			// Type of link at the start
	int 		m_endType;				// Type of link at the end

	int			m_startOffset;			// Offset from top/left at the start
	int			m_endOffset;			// Offset from top/left at the end

	CUMLLinkPropertyDialog	m_dlg;		// Property dialog

	// HTML export functions
	CString ExportHTML() const;
	CString GetArrowHeadHTML() const;
	CString GetCircleCrossHTML() const;
	CString GetFilledArrowHeadHTML() const;
	CString GetFilledDiamondHTML() const;

	// Drawing
	void	DrawDiamond( CDC* dc, const CRect& rect );
	void	DrawInheritanceArrow( CDC* dc );
	void	DrawDirectionArrow( CDC* dc );
	CPoint	GetStyleMarkerRect( int node, const CSize& size ) const;

};

#endif // !defined(AFX_UMLLINESEGMENT_H__8BBDE424_2729_426C_89EC_B3E3E1021F92__INCLUDED_)
