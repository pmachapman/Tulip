#if !defined(AFX_PROPERTYLISTEDITORDIALOG_H__5F0E7F1B_9386_4958_9694_D7F41CD0EA19__INCLUDED_)
#define AFX_PROPERTYLISTEDITORDIALOG_H__5F0E7F1B_9386_4958_9694_D7F41CD0EA19__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyListEditorDialog.h : header file
//

#include "../resource.h"
#include "PropertyContainer.h"
#include "ExListBox.h"

/////////////////////////////////////////////////////////////////////////////
// CPropertyListEditorDialog dialog

class CPropertyListEditorDialog : public CDialog
{
// Construction
public:
	CPropertyListEditorDialog(CWnd* pParent = NULL);

// Dialog Data
	//{{AFX_DATA(CPropertyListEditorDialog)
	enum { IDD = IDD_UML_DIALOG_PROPERTY_LIST_EDITOR };
	CEdit	m_tagCtrl;
	CExListBox	m_tags;
	CString	m_tag;
	CString	m_value;
	//}}AFX_DATA

	void SetProperties( CPropertyContainer& container );
	CPropertyContainer* GetProperties();

// Overrides
	//{{AFX_VIRTUAL(CPropertyListEditorDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CPropertyListEditorDialog)
	virtual void OnOK();
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonDelete();
	afx_msg void OnButtonUpdate();
	afx_msg void OnButtonClose();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	LRESULT OnListboxDelete( WPARAM id, LPARAM );
	LRESULT	OnListboxSelChange( WPARAM id, LPARAM );

	DECLARE_MESSAGE_MAP()

private:
	CPropertyContainer	m_container;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYLISTEDITORDIALOG_H__5F0E7F1B_9386_4958_9694_D7F41CD0EA19__INCLUDED_)
