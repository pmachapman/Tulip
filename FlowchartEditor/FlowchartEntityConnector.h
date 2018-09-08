#if !defined(AFX_FLOWCHARTENTITYCONNECTOR_H__8BBDE424_2729_426C_89EC_B3E3E1021F92__INCLUDED_)
#define AFX_FLOWCHARTENTITYCONNECTOR_H__8BBDE424_2729_426C_89EC_B3E3E1021F92__INCLUDED_

#include "FlowchartEntity.h"

class CFlowchartEntityConnector : public CFlowchartEntity
{

public:
// Construction/initialization/destruction
	CFlowchartEntityConnector();
	virtual ~CFlowchartEntityConnector();
	virtual CDiagramEntity* Clone();
	static	CDiagramEntity* CreateFromString( const CString& str );

// Overrides
	virtual void Draw( CDC* dc, CRect rect );
	virtual BOOL BodyInRect( CRect rect ) const;
	virtual int	 GetHitCode( CPoint point ) const;

};

#endif // !defined(AFX_FLOWCHARTENTITYCONNECTOR_H__8BBDE424_2729_426C_89EC_B3E3E1021F92__INCLUDED_)
