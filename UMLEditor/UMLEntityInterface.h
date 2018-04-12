#ifndef _UMLENTITYINTERFACE_H_
#define _UMLENTITYINTERFACE_H_

#include "UMLEntity.h"
#include "UMLInterfacePropertyDialog.h"

class CUMLEntityInterface : public CUMLEntity
{
public:
	CUMLEntityInterface();
	virtual ~CUMLEntityInterface();

	virtual CDiagramEntity* Clone();
	static	CDiagramEntity* CreateFromString( const CString& str );
	virtual void Draw( CDC* dc, CRect rect );

	virtual int		GetLinkCode( CPoint point ) const;
	virtual void	DrawSelectionMarkers( CDC* dc, CRect rect ) const;
	virtual HCURSOR	GetCursor( int hit ) const;
	virtual CString	Export( UINT format = 0 ) const;

private:

	CUMLInterfacePropertyDialog	m_dlg;

};

#endif //_UMLENTITYINTERFACE_H_