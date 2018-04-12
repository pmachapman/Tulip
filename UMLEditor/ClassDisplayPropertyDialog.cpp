/* ==========================================================================
	Class :			CClassDisplayPropertyDialog

	Author :		Johan Rosengren, Abstrakt Mekanik AB

	Date :			2004-06-21

	Purpose :		"CClassDisplayPropertyDialog" derives from "CDialog" 
					and is a wrapper for the class display properties 
					dialog.

	Description :	A standard Wizard-created dialog box class.

	Usage :			Call to edit the display properties of the class 
					object. The display properties are for example if access 
					markers should be displayed, if only public attributes/
					operation should be displayed etc.

   ========================================================================*/

#include "stdafx.h"
#include "ClassDisplayPropertyDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CClassDisplayPropertyDialog dialog

CClassDisplayPropertyDialog::CClassDisplayPropertyDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CClassDisplayPropertyDialog::IDD, pParent)
/* ============================================================
	Function :		CClassDisplayPropertyDialog::CClassDisplayPropertyDialog
	Description :	Constructor
	Access :		Public

	Return :		void
	Parameters :	CWnd* pParent	-	Dialog parent

	Usage :			Call to edit the display setting of a class
					object.

   ============================================================*/
{
	//{{AFX_DATA_INIT(CClassDisplayPropertyDialog)
	m_noattributenames = FALSE;
	m_noattributes = FALSE;
	m_nomarkers = FALSE;
	m_nooperations = FALSE;
	m_onlypublic = FALSE;
	//}}AFX_DATA_INIT
}


void CClassDisplayPropertyDialog::DoDataExchange(CDataExchange* pDX)
/* ============================================================
	Function :		CClassDisplayPropertyDialog::DoDataExchange
	Description :	Data exchange handler.
	Access :		Protected

	Return :		void
	Parameters :	CDataExchange* pDX	-	Pointer to exchange object
					
	Usage :			Called from MFC to exchange and validate 
					dialog data.

   ============================================================*/
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CClassDisplayPropertyDialog)
	DDX_Check(pDX, IDC_CHECK_NO_ATTRIBUTE_NAMES, m_noattributenames);
	DDX_Check(pDX, IDC_CHECK_NO_ATTRIBUTES, m_noattributes);
	DDX_Check(pDX, IDC_CHECK_NO_MARKERS, m_nomarkers);
	DDX_Check(pDX, IDC_CHECK_NO_OPERATIONS, m_nooperations);
	DDX_Check(pDX, IDC_CHECK_ONLY_PUBLIC, m_onlypublic);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CClassDisplayPropertyDialog, CDialog)
	//{{AFX_MSG_MAP(CClassDisplayPropertyDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

