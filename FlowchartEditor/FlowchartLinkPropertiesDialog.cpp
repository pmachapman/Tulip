/* ==========================================================================
	CFlowchartLinkPropertiesDialog

	Author :		Johan Rosengren, Abstrakt Mekanik AB

	Date :			2004-04-29

	Purpose :		CFlowchartLinkPropertiesDialog is a CDialog-derived
					class used to display the dialog when link-titles are
					edited.

	Description :	Class wizard-generated.

	Usage :			Called internally when "Edit link title" is selected
					from the editor popup-menu.

   ========================================================================*/

#include "stdafx.h"
#include "FlowchartLinkPropertiesDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFlowchartLinkPropertiesDialog dialog


CFlowchartLinkPropertiesDialog::CFlowchartLinkPropertiesDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CFlowchartLinkPropertiesDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFlowchartLinkPropertiesDialog)
	m_linkTitle = _T("");
	//}}AFX_DATA_INIT
}


void CFlowchartLinkPropertiesDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFlowchartLinkPropertiesDialog)
	DDX_Text(pDX, IDC_FLOWCHART_EDIT_LINK_TITLE, m_linkTitle);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFlowchartLinkPropertiesDialog, CDialog)
	//{{AFX_MSG_MAP(CFlowchartLinkPropertiesDialog)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFlowchartLinkPropertiesDialog message handlers
