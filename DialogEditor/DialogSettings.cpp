// DialogSettings.cpp : implementation file
//

#include "stdafx.h"
#include "DialogSettings.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogSettings dialog


CDialogSettings::CDialogSettings(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogSettings::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogSettings)
	m_height = 0;
	m_width = 0;
	m_gridHeight = 0;
	m_gridWidth = 0;
	m_marginLeft = 0;
	m_marginTop = 0;
	m_marginRight = 0;
	m_marginBottom = 0;
	//}}AFX_DATA_INIT
}


void CDialogSettings::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogSettings)
	DDX_Text(pDX, IDC_DIALOG_EDIT_HEIGHT, m_height);
	DDX_Text(pDX, IDC_DIALOG_EDIT_WIDTH, m_width);
	DDX_Text(pDX, IDC_DIALOG_EDIT_GRID_HEIGHT, m_gridHeight);
	DDX_Text(pDX, IDC_DIALOG_EDIT_GRID_WIDTH, m_gridWidth);
	DDX_Text(pDX, IDC_DIALOG_EDIT_LEFT, m_marginLeft);
	DDX_Text(pDX, IDC_DIALOG_EDIT_TOP, m_marginTop);
	DDX_Text(pDX, IDC_DIALOG_EDIT_RIGHT, m_marginRight);
	DDX_Text(pDX, IDC_DIALOG_EDIT_BOTTOM, m_marginBottom);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogSettings, CDialog)
	//{{AFX_MSG_MAP(CDialogSettings)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogSettings message handlers
