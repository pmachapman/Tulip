#if !defined(AFX_FLOWCHARTLINKABLELINESEGMENT_H__8BBDE424_2729_426C_89EC_B3E3E1021F92__INCLUDED_)
#define AFX_FLOWCHARTLINKABLELINESEGMENT_H__8BBDE424_2729_426C_89EC_B3E3E1021F92__INCLUDED_

#include "FlowchartEntity.h"
#include "../NetworkEditor/PropertyDialog.h"

class CFlowchartLinkableLineSegment : public CFlowchartEntity
{

public:

	// Creation/initialization/destruction
	CFlowchartLinkableLineSegment();
	virtual ~CFlowchartLinkableLineSegment();
	virtual	CDiagramEntity* Clone();

	static	CDiagramEntity* CreateFromString(const CString& str);

	// Overrides
	virtual void		Draw(CDC* dc, CRect rect);
	virtual int			GetHitCode(CPoint point) const;
	virtual HCURSOR		GetCursor(int hit) const;
	virtual void		SetRect(CRect rect);
	virtual BOOL		BodyInRect(CRect rect) const;
	virtual CPoint		GetLinkPosition(int type);
	virtual int			AllowLink();

protected:

	virtual void		DrawSelectionMarkers(CDC* dc, CRect rect) const;

};

#endif // !defined(AFX_FLOWCHARTLINKABLELINESEGMENT_H__8BBDE424_2729_426C_89EC_B3E3E1021F92__INCLUDED_)
