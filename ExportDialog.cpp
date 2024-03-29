// ExportDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Tulip.h"
#include "ExportDialog.h"
#include "afxdialogex.h"


// ExportDialog dialog

IMPLEMENT_DYNAMIC(CExportDialog, CDialog)

CExportDialog::CExportDialog(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_EXPORT, pParent)
{

}

CExportDialog::~CExportDialog()
{
}

void CExportDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_STRIP_INFO, m_stripInfoCtrl);
	DDX_Control(pDX, IDC_CHECK_STRIP_LEADING, m_stripCtrl);
	DDX_Control(pDX, IDC_CHECK_ONLY_H, m_onlyhCtrl);
	DDX_Radio(pDX, IDC_RADIO_EMF, m_format);
	DDX_Check(pDX, IDC_CHECK_STRIP_LEADING, m_strip);
	DDX_Check(pDX, IDC_CHECK_ONLY_H, m_onlyh);
	if (m_format == 2)
	{
		m_stripCtrl.EnableWindow(TRUE);
		m_stripInfoCtrl.EnableWindow(TRUE);
		m_onlyhCtrl.EnableWindow(TRUE);
	}
}


BEGIN_MESSAGE_MAP(CExportDialog, CDialog)
	ON_BN_CLICKED(IDC_RADIO_EMF, OnRadioEmf)
	ON_BN_CLICKED(IDC_RADIO_CPP, OnRadioCpp)
	ON_BN_CLICKED(IDC_RADIO_HTML, OnRadioHtml)
END_MESSAGE_MAP()


// ExportDialog message handlers

// Handler for the Export to CPP button
void CExportDialog::OnRadioCpp()
{
	m_stripCtrl.EnableWindow(TRUE);
	m_stripInfoCtrl.EnableWindow(TRUE);
	m_onlyhCtrl.EnableWindow(TRUE);
}

// Handler for the Export to HTML button
void CExportDialog::OnRadioHtml()
{
	m_stripCtrl.EnableWindow(FALSE);
	m_stripInfoCtrl.EnableWindow(FALSE);
	m_onlyhCtrl.EnableWindow(FALSE);
}

// Handler for the Export to EMF button
void CExportDialog::OnRadioEmf()
{
	m_stripCtrl.EnableWindow(FALSE);
	m_stripInfoCtrl.EnableWindow(FALSE);
	m_onlyhCtrl.EnableWindow(FALSE);
}