#ifndef _UMLENTITYNOTE_H_
#define _UMLENTITYNOTE_H_

#include "UMLEntity.h"
#include "UMLNotePropertyDialog.h"

class CUMLEntityNote : public CUMLEntity
{
public:
	CUMLEntityNote();
	virtual ~CUMLEntityNote();

	virtual CDiagramEntity* Clone();
	static	CDiagramEntity* CreateFromString( const CString& str );
	virtual void Draw( CDC* dc, CRect rect );

	virtual void	SetTitle( CString title );
	virtual void	SetRect( CRect rect );
	virtual void	SetRect( double left, double top, double right, double bottom );
	virtual CString	Export( UINT format = 0 ) const;

	virtual void	CalcRestraints();

private:
	CRect			GetTextRect( const CRect& inrect ) const;

	CUMLNotePropertyDialog	m_dlg;

	CString ExportHTML() const;

};

#endif //_UMLENTITYNOTE_H_