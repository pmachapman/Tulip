#if !defined(AFX_GETTERSETTERDIALOG_H__096C8819_CC5A_4A28_9DFC_DED7E85D4323__INCLUDED_)
#define AFX_GETTERSETTERDIALOG_H__096C8819_CC5A_4A28_9DFC_DED7E85D4323__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GetterSetterDialog.h : header file
//

#include "../resource.h"

/////////////////////////////////////////////////////////////////////////////
// CGetterSetterDialog dialog

class CGetterSetterDialog : public CDialog
{
// Construction
public:
	CGetterSetterDialog(CWnd* pParent = NULL);

// Dialog Data
	//{{AFX_DATA(CGetterSetterDialog)
	enum { IDD = IDD_UML_DIALOG_GETTER_SETTER };
	CEdit	m_setterCtrl;
	CEdit	m_getterCtrl;
	CString	m_getter;
	CString	m_setter;
	//}}AFX_DATA

	BOOL m_noset;
	BOOL m_noget;

// Overrides
	//{{AFX_VIRTUAL(CGetterSetterDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CGetterSetterDialog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GETTERSETTERDIALOG_H__096C8819_CC5A_4A28_9DFC_DED7E85D4323__INCLUDED_)
