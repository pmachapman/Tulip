#if !defined(AFX_CLASSOPERATIONPROPERTYDIALOG_H__3497CFE6_762F_4167_91CC_D8A71E16379F__INCLUDED_)
#define AFX_CLASSOPERATIONPROPERTYDIALOG_H__3497CFE6_762F_4167_91CC_D8A71E16379F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ClassOperationPropertyDialog.h : header file
//

#include "../resource.h"
#include "StringHelpers.h"
#include "ExListBox.h"
#include "PropertyContainer.h"
#include "ParameterContainer.h"
#include "Operation.h"

/////////////////////////////////////////////////////////////////////////////
// CClassOperationPropertyDialog dialog

class CClassOperationPropertyDialog : public CDialog
{
// Construction
public:
	COperation* GetOperation() const;
	void SetOperation( COperation* operation );
	CClassOperationPropertyDialog(CWnd* pParent = NULL);

// Dialog Data
	//{{AFX_DATA(CClassOperationPropertyDialog)
	enum { IDD = IDD_UML_DIALOG_PROPERTY_CLASS_OPERATION };
	CEdit	m_typeCtrl;
	CEdit	m_nameCtrl;
	BOOL	m_abstract;
	BOOL	m_static;
	BOOL	m_const;
	CString	m_name;
	CString	m_type;
	CString m_propertylist;
	int		m_access;
	CExListBox	m_parameter;
	//}}AFX_DATA

// Overrides
	//{{AFX_VIRTUAL(CClassOperationPropertyDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CClassOperationPropertyDialog)
	afx_msg void OnButtonAdd();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonPropertyList();
	afx_msg void OnCheckConst();
	//}}AFX_MSG
	LRESULT OnListboxDblClick( WPARAM id, LPARAM );
	LRESULT OnListboxDelete( WPARAM id, LPARAM );

	DECLARE_MESSAGE_MAP()
private:
	// Private data
	COperation* m_operation;
	CPropertyContainer	m_properties;
	CParameterContainer	m_parameters;

	// Private helpers
	void	FillParameterList();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLASSOPERATIONPROPERTYDIALOG_H__3497CFE6_762F_4167_91CC_D8A71E16379F__INCLUDED_)
