// PropertyPane.cpp : implementation file
//

#include "stdafx.h"
#include "Tulip.h"
#include "PropertyPane.h"


// CPropertyPane

IMPLEMENT_DYNAMIC(CPropertyPane, CDockablePane)

CPropertyPane::CPropertyPane()
{

}

CPropertyPane::~CPropertyPane()
{
}


BEGIN_MESSAGE_MAP(CPropertyPane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()



// CPropertyPane message handlers




int CPropertyPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	DWORD style = TVS_HASLINES | TVS_HASBUTTONS | TVS_LINESATROOT |
		WS_CHILD | WS_VISIBLE | TVS_SHOWSELALWAYS | TVS_FULLROWSELECT;
	CRect dump(0, 0, 0, 0);
	if (!m_grid.Create(style, dump, this, IDC_PROPERTY_GRID))
	{
		return -1;
	}
	else
	{
		return 0;
	}
}


void CPropertyPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	m_grid.SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOACTIVATE | SWP_NOZORDER);
}
