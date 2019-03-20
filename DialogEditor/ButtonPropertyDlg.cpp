/* ==========================================================================
	CButtonPropertyDlg

	Author :		Johan Rosengren, Abstrakt Mekanik AB

	Date :			2004-03-31

	Purpose :		CButtonPropertyDlg is a CDiagramPropertyDlg derived
					class to edit objects having a title and a name
					(surprisingly enough for example CDiagramButton...)

	Description :	Implements an update function (OnApply) and the pure
					virtual member SetValues.

	Usage :			See CDiagramPropertyDlg.

   ========================================================================*/

#include "stdafx.h"
#include "ButtonPropertyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CButtonPropertyDlg dialog


CButtonPropertyDlg::CButtonPropertyDlg(CWnd* pParent /*=NULL*/)
	: CDiagramPropertyDlg(CButtonPropertyDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CButtonPropertyDlg)
	//}}AFX_DATA_INIT
}


void CButtonPropertyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CButtonPropertyDlg)
	DDX_Control(pDX, IDC_DIALOG_EDIT_TITLE, m_editTitle);
	DDX_Control(pDX, IDC_DIALOG_EDIT_NAME, m_editName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CButtonPropertyDlg, CDialog)
	//{{AFX_MSG_MAP(CButtonPropertyDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CButtonPropertyDlg message handlers

void CButtonPropertyDlg::OnCancel()
/* ============================================================
	Function :		CButtonPropertyDlg::OnCancel
	Description :	Sets the focus to the editor

	Return :		void
	Parameters :	none

	Usage :			Called from MFC

   ============================================================*/
{
	CDialog::OnCancel();
	GetRedrawWnd()->SetFocus();
}

void CButtonPropertyDlg::OnOK()
/* ============================================================
	Function :		CButtonPropertyDlg::OnOK
	Description :	Updates the object from the dialog box,
					redrawing the editor window.

	Return :		void
	Parameters :	none

	Usage :			Called from MFC

   ============================================================*/
{
	if (m_hWnd)
	{
		if (GetEntity())
		{
			CString title;
			CString name;
			m_editTitle.GetWindowText(title);
			m_editName.GetWindowText(name);

			if (GetEntity()->GetTitle() != title || GetEntity()->GetName() != name)
			{
				GetEntity()->SetTitle(title);
				GetEntity()->SetName(name);
				GetParent()->RedrawWindow();
			}

			Redraw();
			ShowWindow(SW_HIDE);
			GetRedrawWnd()->SetFocus();
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CButtonPropertyDlg virtuals

void CButtonPropertyDlg::SetValues()
/* ============================================================
	Function :		CButtonPropertyDlg::SetValues
	Description :	Sets the values of the dialog box fields
					from the attached object.

	Return :		void
	Parameters :	none

	Usage :			Called from CDiagramEditor.

   ============================================================*/
{
	if (m_hWnd)
	{
		if (GetEntity())
		{
			m_editTitle.SetWindowText(GetEntity()->GetTitle());
			m_editName.SetWindowText(GetEntity()->GetName());
		}

	}
}

