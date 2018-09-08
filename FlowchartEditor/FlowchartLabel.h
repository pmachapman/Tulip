#if !defined(AFX_FLOWCHARTLABEL_H__8BBDE424_2729_426C_89EC_B3E3E1021F92__INCLUDED_)
#define AFX_FLOWCHARTLABEL_H__8BBDE424_2729_426C_89EC_B3E3E1021F92__INCLUDED_

#include "../DiagramEditor/DiagramEntity.h"
#include "LabelPropertyDialog.h"

class CFlowchartLabel : public CDiagramEntity
{

public:
	// Construction/initialization/destruction
	CFlowchartLabel();
	virtual ~CFlowchartLabel();
	virtual CDiagramEntity* Clone();
	static	CDiagramEntity* CreateFromString(const CString& str);

	// Overrides
	virtual void Draw(CDC* dc, CRect rect);

private:
	// Private data
	CLabelPropertyDialog	m_dlg;

};

#endif // !defined(AFX_FLOWCHARTLABEL_H__8BBDE424_2729_426C_89EC_B3E3E1021F92__INCLUDED_)
