#if !defined(AFX_UMLTEMPLATEPROPERTYDIALOG_H__BE264CA5_9335_48B3_8807_4100EAD87453__INCLUDED_)
#define AFX_UMLTEMPLATEPROPERTYDIALOG_H__BE264CA5_9335_48B3_8807_4100EAD87453__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UMLTemplatePropertyDialog.h : header file
//
#include "DiagramEditor/DiagramPropertyDlg.h"
#include "../resource.h"
#include "ExListbox.h"
#include "PropertyContainer.h"

/////////////////////////////////////////////////////////////////////////////
// CUMLTemplatePropertyDialog dialog

class CUMLTemplatePropertyDialog : public CDiagramPropertyDlg
{
// Construction
public:
	CUMLTemplatePropertyDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CUMLTemplatePropertyDialog)
	enum { IDD = IDD_UML_DIALOG_PROPERTY_TEMPLATE };
	CExListBox	m_operation;
	CExListBox	m_attribute;
	CString	m_name;
	CString	m_parameter;
	CString	m_propertylist;
	CEdit	m_nameCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUMLTemplatePropertyDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
	virtual void SetValues();

protected:

	// Generated message map functions
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnButtonAddAttribute();
	afx_msg void OnButtonAddOperation();
	afx_msg void OnButtonFont();
	afx_msg void OnButtonColor();
	afx_msg void OnButtonPropertyList();
	afx_msg void OnButtonAutoGenerate();
	afx_msg void OnButtonVisibility();
	afx_msg void OnButtonAutoGenerate2();
	//}}AFX_MSG
	LRESULT OnListboxDblClick( WPARAM id, LPARAM );
	LRESULT OnListboxDelete( WPARAM id, LPARAM );

	DECLARE_MESSAGE_MAP()

private:

	CString		m_font;
	COLORREF	m_color;

	void FillListboxes();
	void ClearListboxes();
	void RefreshListboxes();

	void DeleteAttribute();
	void DeleteOperation();
	void EditAttribute();
	void EditOperation();

	void CreateAttributes();
	void CreateOperations();

	CPropertyContainer	m_properties;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UMLTEMPLATEPROPERTYDIALOG_H__BE264CA5_9335_48B3_8807_4100EAD87453__INCLUDED_)
