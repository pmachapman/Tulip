#if !defined(AFX_UMLINTERFACEPROPERTYDIALOG_H__0F34B3BB_CEDC_435F_BC4C_256D4DDD548C__INCLUDED_)
#define AFX_UMLINTERFACEPROPERTYDIALOG_H__0F34B3BB_CEDC_435F_BC4C_256D4DDD548C__INCLUDED_

#include "DiagramEditor/DiagramPropertyDlg.h"
#include "../resource.h"

/////////////////////////////////////////////////////////////////////////////
// CUMLInterfacePropertyDialog dialog

class CUMLInterfacePropertyDialog : public CDiagramPropertyDlg
{

// Construction
public:
	CUMLInterfacePropertyDialog(CWnd* pParent = NULL);
	~CUMLInterfacePropertyDialog();

	virtual void SetValues();

// Dialog Data
	//{{AFX_DATA(CUMLInterfacePropertyDialog)
	enum { IDD = IDD_UML_DIALOG_PROPERTY_INTERFACE };
	CString	m_text;
	//}}AFX_DATA

// Overrides
	//{{AFX_VIRTUAL(CUMLInterfacePropertyDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CUMLInterfacePropertyDialog)
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

#endif // !defined(AFX_UMLINTERFACEPROPERTYDIALOG_H__0F34B3BB_CEDC_435F_BC4C_256D4DDD548C__INCLUDED_)
