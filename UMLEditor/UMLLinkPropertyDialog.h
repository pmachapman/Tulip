#if !defined(AFX_UMLLINKPROPERTYDIALOG_H__240BC1A0_5124_427A_B390_4AF299AA0910__INCLUDED_)
#define AFX_UMLLINKPROPERTYDIALOG_H__240BC1A0_5124_427A_B390_4AF299AA0910__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LinkPropertyDialog.h : header file
//
#include "../resource.h"
#include "DiagramEditor/DiagramPropertyDlg.h"

#define TYPE_ASSOCIATION	0
#define TYPE_DEPENDANCY		1
#define TYPE_INHERITANCE	2
#define TYPE_COMPOSITION	3
#define TYPE_PARTOF			4
#define TYPE_INVISIBLE		5
#define TYPE_INTERFACE		6

/////////////////////////////////////////////////////////////////////////////
// CUMLLinkPropertyDialog dialog

class CUMLLinkPropertyDialog : public CDiagramPropertyDlg
{
// Construction
public:
	CUMLLinkPropertyDialog(CWnd* pParent = NULL);

	virtual void SetValues();

// Dialog Data
	//{{AFX_DATA(CUMLLinkPropertyDialog)
	enum { IDD = IDD_UML_DIALOG_PROPERTY_LINK };
	BOOL	m_unidirectional;
	CString	m_endlabel;
	CString	m_startlabel;
	CString	m_secondaryendlabel;
	CString	m_secondarystartlabel;
	int		m_linktype;
	CString	m_title;
	//}}AFX_DATA


// Overrides
	//{{AFX_VIRTUAL(CUMLLinkPropertyDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CUMLLinkPropertyDialog)
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	afx_msg void OnButtonFont();
	DECLARE_MESSAGE_MAP()

private:

	CString		m_font;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UMLLINKPROPERTYDIALOG_H__240BC1A0_5124_427A_B390_4AF299AA0910__INCLUDED_)
