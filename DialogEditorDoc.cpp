// DialogEditorDoc.cpp : implementation of the CDialogEditorDoc class
//

#include "stdafx.h"
#include "Tulip.h"

#include "DialogEditorDoc.h"

// --- DiagramEditor ---
#include "DialogEditor/DiagramControlFactory.h"
#include "TextFile/TextFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogEditorDoc

IMPLEMENT_DYNCREATE(CDialogEditorDoc, CDocument)

BEGIN_MESSAGE_MAP(CDialogEditorDoc, CDocument)
	//{{AFX_MSG_MAP(CDialogEditorDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogEditorDoc construction/destruction

CDialogEditorDoc::CDialogEditorDoc()
{
}

CDialogEditorDoc::~CDialogEditorDoc()
{
}

BOOL CDialogEditorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// --- DiagramEditor ---
	// Removing the current data
	m_objs.Clear();

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CDialogEditorDoc serialization

void CDialogEditorDoc::Serialize(CArchive& ar)
{
	// --- DiagramEditor ---
	// Saving and loading to/from a text file
	if (ar.IsStoring())
	{
		ar.WriteString(m_objs.GetString() + _T("\r\n"));
		int count = 0;
		CDiagramEntity* obj;
		while ((obj = m_objs.GetAt(count++)))
			ar.WriteString(obj->GetString() + _T("\r\n"));

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
				CDiagramEntity* obj = CDiagramControlFactory::CreateFromString(str);
				if (obj)
					m_objs.Add(obj);
			}
		}
		m_objs.SetModified(TRUE);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CDialogEditorDoc diagnostics

#ifdef _DEBUG
void CDialogEditorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDialogEditorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDialogEditorDoc commands

// --- DiagramEditor ---
CDiagramEntityContainer* CDialogEditorDoc::GetData()
{
	// Data accessor
	return &m_objs;
}

BOOL CDialogEditorDoc::SaveModified()
{

	// We override this to get the dirty-
	// handling right
	SetModifiedFlag(m_objs.IsModified());
	return CDocument::SaveModified();

}

void CDialogEditorDoc::ExportHTML(CString filename)
{
	// Exporting to HTML
	CStringArray stra;

	// Header. I don't want this in the CDiagramEntityContainer class
	// where it would normally belong, as it should not be necessary 
	// to derive a class from CDiagramEntityContainer.

	stra.Add(_T("<html>"));
	stra.Add(_T("<head>"));
	stra.Add(_T("<style>"));
	stra.Add(_T("\t.controls { font-family:MS Sans Serif;font-size:12; }"));
	stra.Add(_T("\tbody { background-color:#c0c0c0; }"));
	stra.Add(_T("</style>"));
	stra.Add(_T("<script>"));
	stra.Add(_T("function buttonHandler( obj )"));
	stra.Add(_T("{"));
	stra.Add(_T("\talert( obj.name )"));
	stra.Add(_T("}"));
	stra.Add(_T("function checkboxHandler( obj )"));
	stra.Add(_T("{"));
	stra.Add(_T("\talert( obj.name )"));
	stra.Add(_T("}"));
	stra.Add(_T("function radiobuttonHandler( obj )"));
	stra.Add(_T("{"));
	stra.Add(_T("\talert( obj.name )"));
	stra.Add(_T("}"));
	stra.Add(_T("function listboxHandler( obj )"));
	stra.Add(_T("{"));
	stra.Add(_T("\talert( obj.name )"));
	stra.Add(_T("}"));
	stra.Add(_T("function comboboxHandler( obj )"));
	stra.Add(_T("{"));
	stra.Add(_T("\talert( obj.name )"));
	stra.Add(_T("}"));
	stra.Add(_T("</script>"));
	stra.Add(_T("</head>"));
	stra.Add(_T("<body topmargin=0 leftmargin=0>"));

	CRect rect(0, 0, m_objs.GetVirtualSize().cx + 1, m_objs.GetVirtualSize().cy + 1);
	CString input1(_T("<div style='position:absolute;left:%i;top:%i;width:%i;height:%i;border:1 solid black;'>"));
	CString input2(_T("<div style='position:absolute;left:0;top:0;width:%i;height:%i;border-top:1 solid white;border-left:1 solid white;border-right:1 solid gray;border-bottom:1 solid gray;'>"));
	CString str;

	str.Format(input1, rect.left, rect.top, rect.Width(), rect.Height());
	stra.Add(str);

	rect.InflateRect(-1, -1);
	str.Format(input2, rect.Width(), rect.Height());
	stra.Add(str);

	// The export itself
	m_objs.Export(stra);

	// The footer
	stra.Add(_T("</div>"));
	stra.Add(_T("</div>"));

	stra.Add(_T("</body>"));
	stra.Add(_T("</html>"));

	// Writing it to a file
	CTextFile tf(_T("html"), _T("\n"));
	if (!tf.WriteTextFile(filename, stra))
		if (filename.GetLength())
			AfxMessageBox(tf.GetErrorMessage());

}
