// MyVisualManagerOffice2007.cpp : implementation file
//

#include "stdafx.h"
#include "MyVisualManagerOffice2007.h"


// CMyVisualManagerOffice2007

IMPLEMENT_DYNCREATE(CMyVisualManagerOffice2007, CMFCVisualManagerOffice2007);

CMyVisualManagerOffice2007::CMyVisualManagerOffice2007()
{
}

CMyVisualManagerOffice2007::~CMyVisualManagerOffice2007()
{
}

BOOL CMyVisualManagerOffice2007::DrawTextOnGlass(CDC * pDC, CString strText, CRect rect, DWORD dwFlags, int nGlowSize, COLORREF clrText)
{
	return CMFCVisualManagerOffice2007::DrawTextOnGlass(pDC, strText, rect, dwFlags, nGlowSize, RGB(0, 0, 0));
}


// CMyVisualManagerOffice2007 member functions