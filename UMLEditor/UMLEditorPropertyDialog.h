#if !defined(AFX_UMLEDITORPROPERTYDIALOG_H__83AAFD4E_EF83_49F4_BEEF_DEE7C59AF7CE__INCLUDED_)
#define AFX_UMLEDITORPROPERTYDIALOG_H__83AAFD4E_EF83_49F4_BEEF_DEE7C59AF7CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UMLEditorPropertyDialog.h : header file
//

#include "../resource.h"

/////////////////////////////////////////////////////////////////////////////
// CUMLEditorPropertyDialog dialog

class CUMLEditorPropertyDialog : public CDialog
{
// Construction
public:
	CUMLEditorPropertyDialog(CWnd* pParent = NULL);

// Dialog Data
	//{{AFX_DATA(CUMLEditorPropertyDialog)
	enum { IDD = IDD_UML_DIALOG_PROPERTY_EDITOR };
	BOOL	m_grid;
	BOOL	m_nomarkers;
	BOOL	m_onlypublic;
	BOOL	m_showgrid;
	BOOL	m_noattributes;
	BOOL	m_nooperations;
	BOOL	m_nooperationattributenames;
	BOOL	m_showmargin;
	//}}AFX_DATA

	COLORREF	m_color;

// Overrides
	//{{AFX_VIRTUAL(CUMLEditorPropertyDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CUMLEditorPropertyDialog)
	afx_msg void OnButtonColor();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UMLEDITORPROPERTYDIALOG_H__83AAFD4E_EF83_49F4_BEEF_DEE7C59AF7CE__INCLUDED_)
