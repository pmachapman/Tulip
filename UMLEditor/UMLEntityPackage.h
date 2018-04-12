#ifndef _UMLENTITYPACKAGE_H_
#define _UMLENTITYPACKAGE_H_

#include "UMLEntity.h"
#include "UMLPackagePropertyDialog.h"

class CUMLEntityPackage : public CUMLEntity
{
public:
	CUMLEntityPackage();
	virtual ~CUMLEntityPackage();

	virtual CDiagramEntity* Clone();
	static	CDiagramEntity* CreateFromString( const CString& str );
	virtual void Draw( CDC* dc, CRect rect );

	virtual CPoint	GetLinkPosition( int type ) const;
	virtual void	ShowPopup( CPoint point, CWnd* parent );
	virtual void	SetTitle( CString title );
	virtual CString	Export( UINT format = 0 ) const;

	BOOL			ModifyTitle( const CString& title );
	virtual void	CalcRestraints();

private:
	CUMLPackagePropertyDialog	m_dlg;

	CString			ExportHTML() const;

};

#endif //_UMLENTITYPACKAGE_H_