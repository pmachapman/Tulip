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
	virtual void DrawGrid(CDC* dc, CRect rect, double zoom) const;

};

/////////////////////////////////////////////////////////////////////////////
