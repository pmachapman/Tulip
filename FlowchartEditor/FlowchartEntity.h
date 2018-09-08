#if !defined(AFX_FLOWCHARTENTITY_H__8BBDE424_2729_426C_89EC_B3E3E1021F92__INCLUDED_)
#define AFX_FLOWCHARTENTITY_H__8BBDE424_2729_426C_89EC_B3E3E1021F92__INCLUDED_

#include "../DiagramEditor/DiagramEntity.h"
#include "../NetworkEditor/PropertyDialog.h"
#include "FlowchartLink.h"

class CFlowchartEntity : public CDiagramEntity
{

public:
	// Construction/initialization/destruction
	CFlowchartEntity();
	virtual	~CFlowchartEntity();
	virtual CDiagramEntity* Clone();
	static	CDiagramEntity* CreateFromString(const CString& str);

	// Overrides
	virtual int		GetHitCode(CPoint point) const;
	virtual HCURSOR GetCursor(int hit) const;

	// Implementation
	virtual int		AllowLink();
	virtual CPoint	GetLinkPosition(int type);

	void			SetMoved(BOOL moved);
	BOOL			GetMoved();

protected:
	// Overrides
	virtual void	DrawSelectionMarkers(CDC* dc, CRect rect) const;

private:
	// Private data
	CPropertyDialog	m_dlg;
	BOOL			m_moved;

};

#endif // !defined(AFX_FLOWCHARTENTITY_H__8BBDE424_2729_426C_89EC_B3E3E1021F92__INCLUDED_)
