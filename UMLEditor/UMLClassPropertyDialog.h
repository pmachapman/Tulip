#if !defined(AFX_UMLCLASSPROPERTYDIALOG_H__9B1B631A_258B_4C45_87BC_8249B0BC474B__INCLUDED_)
#define AFX_UMLCLASSPROPERTYDIALOG_H__9B1B631A_258B_4C45_87BC_8249B0BC474B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UMLClassPropertyDialog.h : header file
//

#include "DiagramEditor/DiagramPropertyDlg.h"
#include "../resource.h"
#include "ExListbox.h"
#include "PropertyContainer.h"

/////////////////////////////////////////////////////////////////////////////
// CUMLClassPropertyDialog dialog

class CUMLClassPropertyDialog : public CDiagramPropertyDlg
{
// Construction
public:
	CUMLClassPropertyDialog(CWnd* pParent = NULL);

	virtual void SetValues();

// Dialog Data
	//{{AFX_DATA(CUMLClassPropertyDialog)
	enum { IDD = IDD_UML_DIALOG_PROPERTY_CLASS };
	CExListBox	m_operation;
	CExListBox	m_attribute;
	CString	m_name;
	CString	m_stereotype;
	CString	m_propertylist;
	CEdit	m_nameCtrl;
	//}}AFX_DATA


// Overrides
	//{{AFX_VIRTUAL(CUMLClassPropertyDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CUMLClassPropertyDialog)
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

#endif // !defined(AFX_UMLCLASSPROPERTYDIALOG_H__9B1B631A_258B_4C45_87BC_8249B0BC474B__INCLUDED_)
