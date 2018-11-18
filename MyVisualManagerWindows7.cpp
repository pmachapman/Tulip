// MyVisualManagerWindows7.cpp : implementation file
//

#include "stdafx.h"
#include "MyVisualManagerWindows7.h"


// CMyVisualManagerWindows7

IMPLEMENT_DYNCREATE(CMyVisualManagerWindows7, CMFCVisualManagerWindows7);

CMyVisualManagerWindows7::CMyVisualManagerWindows7()
{
}

CMyVisualManagerWindows7::~CMyVisualManagerWindows7()
{
}

BOOL CMyVisualManagerWindows7::DrawTextOnGlass(CDC * pDC, CString strText, CRect rect, DWORD dwFlags, int nGlowSize, COLORREF clrText)
{
	return CMFCVisualManagerWindows7::DrawTextOnGlass(pDC, strText, rect, dwFlags, nGlowSize, RGB(0, 0, 0));
}


// CMyVisualManagerWindows7 member functions