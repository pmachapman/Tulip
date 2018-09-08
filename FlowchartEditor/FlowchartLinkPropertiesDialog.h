#if !defined(AFX_FLOWCHARTLINKPROPERTIESDIALOG_H__4336784A_125E_4A99_9E98_EC47C9AFE0EA__INCLUDED_)
#define AFX_FLOWCHARTLINKPROPERTIESDIALOG_H__4336784A_125E_4A99_9E98_EC47C9AFE0EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FlowchartLinkPropertiesDialog.h : header file
//
#include "../resource.h"

/////////////////////////////////////////////////////////////////////////////
// CFlowchartLinkPropertiesDialog dialog

class CFlowchartLinkPropertiesDialog : public CDialog
{
	// Construction
public:
	CFlowchartLinkPropertiesDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFlowchartLinkPropertiesDialog)
	enum { IDD = IDD_FLOWCHART_DIALOG_LINK_TITLE };
	CString	m_linkTitle;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFlowchartLinkPropertiesDialog)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFlowchartLinkPropertiesDialog)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FLOWCHARTLINKPROPERTIESDIALOG_H__4336784A_125E_4A99_9E98_EC47C9AFE0EA__INCLUDED_)
