// DialogEditorDoc.h : interface of the CDialogEditorDoc class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "DiagramEditor/DiagramEntityContainer.h"

class CDialogEditorDoc : public CDocument
{
protected: // create from serialization only
	CDialogEditorDoc();
	DECLARE_DYNCREATE(CDialogEditorDoc)

	// Attributes
public:

	// --- DiagramEditor ---
	CDiagramEntityContainer*	GetData();

	// Operations
public:

	// --- DiagramEditor ---
	void	ExportHTML(CString filename);

	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CDialogEditorDoc)
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
protected:
	virtual BOOL SaveModified();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDialogEditorDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// Generated message map functions
protected:
	//{{AFX_MSG(CDialogEditorDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	// --- DiagramEditor ---
private:
	CDiagramEntityContainer	m_objs; // The object data

};
