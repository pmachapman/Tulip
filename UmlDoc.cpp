// This MFC Samples source code demonstrates using MFC Microsoft Office Fluent User Interface
// (the "Fluent UI") and is provided only as referential material to supplement the
// Microsoft Foundation Classes Reference and related electronic documentation
// included with the MFC C++ library software.
// License terms to copy, use or distribute the Fluent UI are available separately.
// To learn more about our Fluent UI licensing program, please visit
// https://go.microsoft.com/fwlink/?LinkId=238214.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// UmlDoc.cpp : implementation of the CUmlDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Tulip.h"
#endif

#include "UmlDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CUmlDoc

IMPLEMENT_DYNCREATE(CUmlDoc, CDocument)

BEGIN_MESSAGE_MAP(CUmlDoc, CDocument)
END_MESSAGE_MAP()


// CUmlDoc construction/destruction

CUmlDoc::CUmlDoc()
{
	m_objs.SetClipboardHandler(&theApp.m_umlClip);
}

CUmlDoc::~CUmlDoc()
{
}

// Called when a new document is created.
BOOL CUmlDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	m_objs.Clear();

	return TRUE;
}




// CUmlDoc serialization

void CUmlDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		m_objs.Save(ar);
	}
	else
	{
		m_objs.Load(ar);
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CUmlDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CUmlDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data.
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CUmlDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CUmlDoc diagnostics

#ifdef _DEBUG
// Asserts the validity of the current object.
void CUmlDoc::AssertValid() const
{
	CDocument::AssertValid();
}

// Dumps the current object state.
void CUmlDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CUmlDoc commands

// Called before a check is made to see if the data is modified.
BOOL CUmlDoc::SaveModified()
{
	SetModifiedFlag(m_objs.IsModified());
	return CDocument::SaveModified();
}

// Accessor for the internal data container.
CUMLEntityContainer* CUmlDoc::GetData()
{
	return &m_objs;
}
