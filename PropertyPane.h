#pragma once


// CPropertyPane

class CPropertyPane : public CDockablePane
{
	DECLARE_DYNAMIC(CPropertyPane)

public:
	CPropertyPane();
	virtual ~CPropertyPane();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
private:
	CMFCPropertyGridCtrl m_grid;
};


