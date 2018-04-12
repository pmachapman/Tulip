#ifndef _UMLENTITYLABEL_H_
#define _UMLENTITYLABEL_H_

#include "UMLEntity.h"
#include "UMLLabelPropertyDialog.h"

class CUMLEntityLabel : public CUMLEntity
{
public:
	CUMLEntityLabel();
	virtual ~CUMLEntityLabel();

	virtual CDiagramEntity* Clone();
	static	CDiagramEntity* CreateFromString( const CString& str );
	virtual void Draw( CDC* dc, CRect rect );

	virtual CString GetString() const;
	virtual BOOL	FromString( const CString& str );
	virtual void	SetTitle( CString title );
	virtual void	SetRect( CRect rect );
	virtual void	SetRect( double left, double top, double right, double bottom );
	virtual int		GetLinkCode( CPoint point ) const;
	virtual CString	Export( UINT format = 0 ) const;

	virtual void	CalcRestraints();

	void			SetPointsize( int pointsize );
	void			SetBold( BOOL bold );
	void			SetItalic( BOOL italic );
	void			SetUnderline( BOOL underline );
	int				GetPointsize() const;
	BOOL			GetBold() const;
	BOOL			GetItalic() const;
	BOOL			GetUnderline() const;

private:

	CUMLLabelPropertyDialog	m_dlg;


	CString ExportHTML() const;

	int		m_pointsize;
	BOOL	m_bold;
	BOOL	m_italic;
	BOOL	m_underline;

};

#endif //_UMLENTITYNOTE_H_