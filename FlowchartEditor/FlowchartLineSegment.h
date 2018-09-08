#if !defined(AFX_FLOWCHARTLINESEGMENT_H__8BBDE424_2729_426C_89EC_B3E3E1021F92__INCLUDED_)
#define AFX_FLOWCHARTLINESEGMENT_H__8BBDE424_2729_426C_89EC_B3E3E1021F92__INCLUDED_

#include "../DiagramEditor/DiagramLine.h"
#include "../NetworkEditor/PropertyDialog.h"

class CFlowchartLineSegment : public CDiagramLine
{

public:
	// Construction/initialization/destruction
	CFlowchartLineSegment();
	virtual ~CFlowchartLineSegment();
	virtual	CDiagramEntity* Clone();
	static	CDiagramEntity* CreateFromString(const CString& str);

	// Overrides
	virtual void	Draw(CDC* dc, CRect rect);

private:
	// Private data
	CPropertyDialog	m_dlg;

};

#endif // !defined(AFX_FLOWCHARTLINESEGMENT_H__8BBDE424_2729_426C_89EC_B3E3E1021F92__INCLUDED_)
