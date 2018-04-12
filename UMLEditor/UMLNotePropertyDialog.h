#if !defined(AFX_UMLNOTEPROPERTYDIALOG_H__2FB127AA_9D6C_46CD_82AA_25DECE00338B__INCLUDED_)
#define AFX_UMLNOTEPROPERTYDIALOG_H__2FB127AA_9D6C_46CD_82AA_25DECE00338B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UMLNotePropertyDialog.h : header file
//

#include "DiagramEditor/DiagramPropertyDlg.h"
#include "../resource.h"

/////////////////////////////////////////////////////////////////////////////
// CUMLNotePropertyDialog dialog

class CUMLNotePropertyDialog : public CDiagramPropertyDlg
{

// Construction
public:
	CUMLNotePropertyDialog(CWnd* pParent = NULL);
	~CUMLNotePropertyDialog();

	virtual void SetValues();

// Dialog Data
	//{{AFX_DATA(CUMLNotePropertyDialog)
	enum { IDD = IDD_UML_DIALOG_PROPERTY_NOTE };
	CString	m_text;
	//}}AFX_DATA

// Overrides
	//{{AFX_VIRTUAL(CUMLNotePropertyDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CUMLNotePropertyDialog)
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	afx_msg void OnButtonFont();
	afx_msg void OnButtonColor();
	DECLARE_MESSAGE_MAP()

private:

	CString		m_font;
	COLORREF	m_color;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UMLNOTEPROPERTYDIALOG_H__2FB127AA_9D6C_46CD_82AA_25DECE00338B__INCLUDED_)
