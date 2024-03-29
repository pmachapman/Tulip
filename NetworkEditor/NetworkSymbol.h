#ifndef _NETWORKSYMBOL_H_
#define _NETWORKSYMBOL_H_

#include "../DiagramEditor/DiagramEntity.h"
#include "PropertyDialog.h"

class CNetworkSymbol : public CDiagramEntity
{
public:
	CNetworkSymbol(UINT resid);
	~CNetworkSymbol();

	virtual CDiagramEntity* Clone();
	static	CDiagramEntity* CreateFromString(const CString& str);

	// Overrides
	virtual int		GetHitCode(CPoint point) const;
	virtual CPoint	GetLinkPosition();
	virtual void	Draw(CDC* dc, CRect rect);
	virtual void	Copy(CDiagramEntity* obj);
	virtual void	DrawSelectionMarkers(CDC* dc, CRect rect) const;
	virtual BOOL	FromString(const CString& str);
	virtual CString	GetString() const;

	void			SetSymbol(UINT resid);
	UINT			GetSymbol() const;

	virtual BOOL	AutoGenerateName() const;

private:
	UINT	m_symbol;

	CPropertyDialog	m_dlg;

};

#endif // _NETWORKSYMBOL_H_
