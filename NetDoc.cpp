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

// NetDoc.cpp : implementation of the CNetDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Tulip.h"
#endif

#include "NetDoc.h"

#include "NetworkEditor/NetworkLink.h"
#include "NetworkEditor/NetworkControlFactory.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CNetDoc

IMPLEMENT_DYNCREATE(CNetDoc, CDocument)

BEGIN_MESSAGE_MAP(CNetDoc, CDocument)
END_MESSAGE_MAP()


// CNetDoc construction/destruction

CNetDoc::CNetDoc()
{
}

CNetDoc::~CNetDoc()
{
}

// Called when a new document is created.
BOOL CNetDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	m_objs.Clear();

	return TRUE;
}




// CNetDoc serialization

void CNetDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar.WriteString(m_objs.GetString() + _T("\r\n"));
		INT_PTR count = 0;
		CDiagramEntity* obj;
		while ((obj = m_objs.GetAt(count++)))
			ar.WriteString(obj->GetString() + _T("\r\n"));

		INT_PTR max = m_objs.GetLinks();
		for (INT_PTR t = 0; t < max; t++)
		{
			CNetworkLink* link = m_objs.GetLinkAt(t);
			if (link)
				ar.WriteString(link->GetString() + _T("\r\n"));
		}

		m_objs.SetModified(FALSE);
	}
	else
	{

		m_objs.Clear();
		CString str;
		while (ar.ReadString(str))
		{

			if (!m_objs.FromString(str))
			{
				CDiagramEntity* obj = CNetworkControlFactory::CreateFromString(str);
				if (obj)
					m_objs.Add(obj);
				else
				{
					CNetworkLink* link = new CNetworkLink;
					if (link->FromString(str))
						m_objs.AddLink(link);
					else
						delete link;
				}
			}
		}

		m_objs.SetModified(FALSE);

	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CNetDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
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
void CNetDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data.
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CNetDoc::SetSearchContent(const CString& value)
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

// CNetDoc diagnostics

#ifdef _DEBUG
// Asserts the validity of the current object.
void CNetDoc::AssertValid() const
{
	CDocument::AssertValid();
}

// Dumps the current object state.
void CNetDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CNetDoc commands

// Called before a check is made to see if the data is modified.
BOOL CNetDoc::SaveModified()
{
	SetModifiedFlag(m_objs.IsModified());
	return CDocument::SaveModified();
}
