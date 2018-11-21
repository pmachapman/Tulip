// DEditor.h : header file
//
#pragma once
#include "../DiagramEditor/DiagramEditor.h"

/////////////////////////////////////////////////////////////////////////////
// CDialogEditor window

class CDialogEditor : public CDiagramEditor
{
public:
	// Construction/initialization/destruction
	CDialogEditor();
	virtual ~CDialogEditor();

	virtual void DrawBackground(CDC* dc, CRect rect, double zoom) const;
	void ExportEMF(const CString& filename, const bool noDialog = false);

};

/////////////////////////////////////////////////////////////////////////////
