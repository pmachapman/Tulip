#if !defined(AFX_CLASSDISPLAYPROPERTYDIALOG_H__94871CF8_C790_4030_9EC1_DE85F2119CA1__INCLUDED_)
#define AFX_CLASSDISPLAYPROPERTYDIALOG_H__94871CF8_C790_4030_9EC1_DE85F2119CA1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ClassDisplayPropertyDialog.h : header file
//

#include "../resource.h"

/////////////////////////////////////////////////////////////////////////////
// CClassDisplayPropertyDialog dialog

class CClassDisplayPropertyDialog : public CDialog
{
// Construction
public:
	CClassDisplayPropertyDialog(CWnd* pParent = NULL);

// Dialog Data
	//{{AFX_DATA(CClassDisplayPropertyDialog)
	enum { IDD = IDD_UML_DIALOG_PROPERTY_CLASS_DISPLAY };
	BOOL	m_noattributenames;
	BOOL	m_noattributes;
	BOOL	m_nomarkers;
	BOOL	m_nooperations;
	BOOL	m_onlypublic;
	//}}AFX_DATA


// Overrides
	//{{AFX_VIRTUAL(CClassDisplayPropertyDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CClassDisplayPropertyDialog)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLASSDISPLAYPROPERTYDIALOG_H__94871CF8_C790_4030_9EC1_DE85F2119CA1__INCLUDED_)
