#pragma once


// ExportDialog dialog

class CExportDialog : public CDialog
{
	DECLARE_DYNAMIC(CExportDialog)

public:
	CExportDialog(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CExportDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_EXPORT };
#endif
	CStatic	m_stripInfoCtrl;
	CButton	m_stripCtrl;
	CButton	m_onlyhCtrl;
	int		m_format;
	BOOL	m_strip;
	BOOL	m_onlyh;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	afx_msg void OnRadioEmf();
	afx_msg void OnRadioCpp();
	afx_msg void OnRadioHtml();

	DECLARE_MESSAGE_MAP()
};
