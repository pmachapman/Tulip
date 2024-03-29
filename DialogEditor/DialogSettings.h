#if !defined(AFX_DIALOGSETTINGS_H__3C8BA03C_49B4_44BE_ACC5_0D255108C583__INCLUDED_)
#define AFX_DIALOGSETTINGS_H__3C8BA03C_49B4_44BE_ACC5_0D255108C583__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogSettings.h : header file
//
#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CDialogSettings dialog

class CDialogSettings : public CDialog
{
	// Construction
public:
	CDialogSettings(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogSettings)
	enum { IDD = IDD_DIALOG_SETTINGS };
	int		m_height;
	int		m_width;
	int		m_gridHeight;
	int		m_gridWidth;
	int		m_marginLeft;
	int		m_marginTop;
	int		m_marginBottom;
	int		m_marginRight;
	//}}AFX_DATA

	bool		m_disableColor;
	COLORREF	m_color;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogSettings)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogSettings)
	afx_msg void OnButtonColor();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGSETTINGS_H__3C8BA03C_49B4_44BE_ACC5_0D255108C583__INCLUDED_)
