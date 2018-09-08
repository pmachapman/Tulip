#if !defined(AFX_LABELPROPERTYDIALOG_H__2FB127AA_9D6C_46CD_82AA_25DECE00338B__INCLUDED_)
#define AFX_LABELPROPERTYDIALOG_H__2FB127AA_9D6C_46CD_82AA_25DECE00338B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LabelPropertyDialog.h : header file
//

#include "../DiagramEditor/DiagramPropertyDlg.h"
#include "../resource.h"

/////////////////////////////////////////////////////////////////////////////
// CLabelPropertyDialog dialog

class CLabelPropertyDialog : public CDiagramPropertyDlg
{

	// Construction
public:
	CLabelPropertyDialog(CWnd* pParent = NULL);   // standard constructor
	~CLabelPropertyDialog();

	virtual void SetValues();

	// Dialog Data
		//{{AFX_DATA(CLabelPropertyDialog)
	enum { IDD = IDD_FLOWCHART_DIALOG_PROPERTY_LABEL };
	CString	m_text;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLabelPropertyDialog)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLabelPropertyDialog)
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LABELPROPERTYDIALOG_H__2FB127AA_9D6C_46CD_82AA_25DECE00338B__INCLUDED_)
