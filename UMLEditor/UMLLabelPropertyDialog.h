#if !defined(AFX_UMLLABELPROPERTYDIALOG_H__2FB127AA_9D6C_46CD_82AA_25DECE00338B__INCLUDED_)
#define AFX_UMLLABELPROPERTYDIALOG_H__2FB127AA_9D6C_46CD_82AA_25DECE00338B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UMLLabelPropertyDialog.h : header file
//

#include "DiagramEditor/DiagramPropertyDlg.h"
#include "../resource.h"

/////////////////////////////////////////////////////////////////////////////
// CUMLLabelPropertyDialog dialog

class CUMLLabelPropertyDialog : public CDiagramPropertyDlg
{

// Construction
public:
	CUMLLabelPropertyDialog(CWnd* pParent = NULL);
	~CUMLLabelPropertyDialog();

	virtual void SetValues();

// Dialog Data
	//{{AFX_DATA(CUMLLabelPropertyDialog)
	enum { IDD = IDD_UML_DIALOG_PROPERTY_LABEL };
	CString	m_text;
	//}}AFX_DATA

// Overrides
	//{{AFX_VIRTUAL(CUMLLabelPropertyDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CUMLLabelPropertyDialog)
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	afx_msg void OnButtonFont();
	DECLARE_MESSAGE_MAP()

private:

	CString		m_font;
	int			m_pointsize;
	BOOL		m_bold;
	BOOL		m_italic;
	BOOL		m_underline;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UMLLABELPROPERTYDIALOG_H__2FB127AA_9D6C_46CD_82AA_25DECE00338B__INCLUDED_)
