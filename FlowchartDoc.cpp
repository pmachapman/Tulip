// FlowchartDoc.cpp : implementation of the CFlowchartDoc class
//

#include "stdafx.h"

#include "Tulip.h"
#include "FlowchartDoc.h"
#include "FlowchartEditor/FlowchartControlFactory.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma warning( disable : 4706 )

/////////////////////////////////////////////////////////////////////////////
// CFlowchartDoc

IMPLEMENT_DYNCREATE(CFlowchartDoc, CDocument)

BEGIN_MESSAGE_MAP(CFlowchartDoc, CDocument)
	//{{AFX_MSG_MAP(CFlowchartDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFlowchartDoc construction/destruction

CFlowchartDoc::CFlowchartDoc()
{

}

CFlowchartDoc::~CFlowchartDoc()
{
}

BOOL CFlowchartDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	m_objs.Clear();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CFlowchartDoc serialization

void CFlowchartDoc::Serialize(CArchive& ar)
{

	if (ar.IsStoring())
	{
		ar.WriteString(m_objs.GetString() + _T("\r\n"));
		int count = 0;
		CDiagramEntity* obj;
		while ((obj = m_objs.GetAt(count++)))
			ar.WriteString(obj->GetString() + _T("\r\n"));

		INT_PTR max = m_objs.GetLinks();
		for (INT_PTR t = 0; t < max; t++)
		{
			CFlowchartLink* link = m_objs.GetLinkAt(t);
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
				CDiagramEntity* obj = CFlowchartControlFactory::CreateFromString(str);
				if (obj)
					m_objs.Add(obj);
				else
				{
					CFlowchartLink* link = new CFlowchartLink;
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

/////////////////////////////////////////////////////////////////////////////
// CFlowchartDoc diagnostics

#ifdef _DEBUG
void CFlowchartDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CFlowchartDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFlowchartDoc commands

BOOL CFlowchartDoc::SaveModified()
{

	SetModifiedFlag(m_objs.IsModified());
	return CDocument::SaveModified();

}

CFlowchartEntityContainer* CFlowchartDoc::GetData()
{
	return &m_objs;
}

