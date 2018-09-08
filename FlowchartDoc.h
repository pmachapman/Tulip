// FlowchartDoc.h : interface of the CFlowchartDoc class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "FlowchartEditor/FlowchartEntityContainer.h"

class CFlowchartDoc : public CDocument
{
protected: // create from serialization only
	CFlowchartDoc();
	DECLARE_DYNCREATE(CFlowchartDoc)

	// Attributes
public:

	// Operations
public:

	CFlowchartEntityContainer*	GetData();

	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CFlowchartDoc)
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
protected:
	virtual BOOL SaveModified();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFlowchartDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// Generated message map functions
protected:
	//{{AFX_MSG(CFlowchartDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CFlowchartEntityContainer	m_objs;

};
