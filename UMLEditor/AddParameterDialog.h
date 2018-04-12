#if !defined(AFX_ADDPARAMETERDIALOG_H__AC07C8A9_94E8_4A75_9845_82FA872C6815__INCLUDED_)
#define AFX_ADDPARAMETERDIALOG_H__AC07C8A9_94E8_4A75_9845_82FA872C6815__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AddParameterDialog.h : header file
//
#include "../resource.h"
#include "Parameter.h"

/////////////////////////////////////////////////////////////////////////////
// CAddParameterDialog dialog

class CAddParameterDialog : public CDialog
{
// Construction
public:
	CAddParameterDialog(CWnd* pParent = NULL);

// Dialog Data
	//{{AFX_DATA(CAddParameterDialog)
	enum { IDD = IDD_UML_DIALOG_PROPERTY_CLASS_OPERATION_PARAMETER };
	CEdit	m_typeCtrl;
	CEdit	m_nameCtrl;
	CString	m_name;
	CString	m_type;
	CString	m_defaultvalue;
	BOOL	m_in;
	BOOL	m_out;
	BOOL	m_reference;
	//}}AFX_DATA

	void			SetParameter( CParameter* parameter );
	CParameter*		GetParameter();

// Overrides
	//{{AFX_VIRTUAL(CAddParameterDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

// Implementation
protected:

	//{{AFX_MSG(CAddParameterDialog)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CParameter*	m_parameter;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADDPARAMETERDIALOG_H__AC07C8A9_94E8_4A75_9845_82FA872C6815__INCLUDED_)
