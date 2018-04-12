#if !defined(AFX_CLASSATTRIBUTEPROPERTYDIALOG_H__EFB515DB_9276_4445_B25D_B6272411C3E5__INCLUDED_)
#define AFX_CLASSATTRIBUTEPROPERTYDIALOG_H__EFB515DB_9276_4445_B25D_B6272411C3E5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ClassAttributePropertyDialog.h : header file
//
#include "../resource.h"
#include "Attribute.h"
#include "PropertyContainer.h"

/////////////////////////////////////////////////////////////////////////////
// CClassAttributePropertyDialog dialog

class CClassAttributePropertyDialog : public CDialog
{
// Construction
public:
	CClassAttributePropertyDialog(CWnd* pParent = NULL);

// Dialog Data
	//{{AFX_DATA(CClassAttributePropertyDialog)
	enum { IDD = IDD_UML_DIALOG_PROPERTY_CLASS_ATTRIBUTE };
	CEdit	m_typeCtrl;
	CEdit	m_nameCtrl;
	CString	m_name;
	CString	m_type;
	int		m_access;
	BOOL	m_static;
	CString m_multiplicity;
	CString m_defaultvalue;
	CString m_propertylist;
	//}}AFX_DATA

	void SetAttribute( CAttribute* attribute );
	CAttribute* GetAttribute() const;

// Overrides
	//{{AFX_VIRTUAL(CClassAttributePropertyDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CClassAttributePropertyDialog)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonPropertyList();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	// Private data
	CAttribute* m_attribute;
	CPropertyContainer	m_properties;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLASSATTRIBUTEPROPERTYDIALOG_H__EFB515DB_9276_4445_B25D_B6272411C3E5__INCLUDED_)
