#if !defined(AFX_DIAGRAMCOMBOBOX_H__A34BFDFD_1CBE_463B_94EF_907D9123D614__INCLUDED_)
#define AFX_DIAGRAMCOMBOBOX_H__A34BFDFD_1CBE_463B_94EF_907D9123D614__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DiagramCombobox.h : header file
//

#include "DiagramEditor/DiagramEntity.h"
#include "EditPropertyDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CDiagramCombobox window

class CDiagramCombobox : public CDiagramEntity
{
	// Construction
public:
	CDiagramCombobox();

	static	CDiagramEntity* CreateFromString(const CString& str);

	// Overrides
	virtual CDiagramEntity* Clone();
	virtual void			Draw(CDC* dc, CRect rect);
	virtual CString	Export(UINT format = 0) const;

private:
	CEditPropertyDlg	m_dlg;


};

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_DIAGRAMCOMBOBOX_H__A34BFDFD_1CBE_463B_94EF_907D9123D614__INCLUDED_)
