#if !defined(AFX_FLOWCHARTENTITYTERMINATOR_H__8BBDE424_2729_426C_89EC_B3E3E1021F92__INCLUDED_)
#define AFX_FLOWCHARTENTITYTERMINATOR_H__8BBDE424_2729_426C_89EC_B3E3E1021F92__INCLUDED_

#include "FlowchartEntity.h"

class CFlowchartEntityTerminator : public CFlowchartEntity
{

public:
	// Construction/initialization/destruction
	CFlowchartEntityTerminator();
	virtual	~CFlowchartEntityTerminator();
	virtual CDiagramEntity* Clone();
	static	CDiagramEntity* CreateFromString(const CString& str);

	// Overrides
	virtual void			Draw(CDC *dc, CRect rect);

};

#endif // !defined(AFX_FLOWCHARTENTITYTERMINATOR_H__8BBDE424_2729_426C_89EC_B3E3E1021F92__INCLUDED_)
