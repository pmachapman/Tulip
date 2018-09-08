#if !defined(AFX_FLOWCHARTENTITYIO_H__8BBDE424_2729_426C_89EC_B3E3E1021F92__INCLUDED_)
#define AFX_FLOWCHARTENTITYIO_H__8BBDE424_2729_426C_89EC_B3E3E1021F92__INCLUDED_

#include "FlowchartEntity.h"

class CFlowchartEntityIO : public CFlowchartEntity
{

public:
	// Construction/initialization/destruction
	CFlowchartEntityIO();
	virtual ~CFlowchartEntityIO();
	virtual CDiagramEntity* Clone();
	static	CDiagramEntity* CreateFromString(const CString& str);

	// Overrides
	virtual void Draw(CDC* dc, CRect rect);

};

#endif // !defined(AFX_FLOWCHARTENTITYIO_H__8BBDE424_2729_426C_89EC_B3E3E1021F92__INCLUDED_)
