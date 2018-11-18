#pragma once


class CMyVisualManagerWindows7 : public CMFCVisualManagerWindows7
{
	DECLARE_DYNCREATE(CMyVisualManagerWindows7);

public:
	CMyVisualManagerWindows7();
	virtual ~CMyVisualManagerWindows7();
	BOOL DrawTextOnGlass(
		CDC* pDC,
		CString strText,
		CRect rect,
		DWORD dwFlags,
		int nGlowSize = 0,
		COLORREF clrText = (COLORREF)-1) override;
};