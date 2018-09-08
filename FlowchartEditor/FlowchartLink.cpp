/* ==========================================================================
	CFlowchartLink

	Author :		Johan Rosengren, Abstrakt Mekanik AB

	Date :			2004-05-01

	Purpose :		This class represents a link object in the flowchart
					data.

	Description :	The class contains data for the from- and to-parts of the
					link, as well as functions to save, load and clone it.

	Usage :			Instances of this class are managed by the flowchart
					container class. Link data is public.

   ========================================================================*/
#include "stdafx.h"
#include "FlowchartLink.h"
#include "../DiagramEditor/Tokenizer.h"

CFlowchartLink::CFlowchartLink()
/* ============================================================
	Function :		CFlowchartLink::CFlowchartLink
	Description :	constructor

	Return :		void
	Parameters :	none

	Usage :

   ============================================================*/
{

	fromtype = 0;
	totype = 0;

}

CFlowchartLink::~CFlowchartLink()
/* ============================================================
	Function :		CFlowchartLink::~CFlowchartLink
	Description :	destructor

	Return :		void
	Parameters :	none

	Usage :

   ============================================================*/
{
}

CFlowchartLink* CFlowchartLink::Clone() const
/* ============================================================
	Function :		CFlowchartLink::Clone
	Description :	Clone this object to a new object.

	Return :		CDiagramEntity*	-	The new object.
	Parameters :	none

	Usage :			Call to create a clone of the object. The
					caller will have to delete the object.

   ============================================================*/
{

	CFlowchartLink* obj = new CFlowchartLink;
	obj->from = from;
	obj->to = to;
	obj->title = title;
	obj->fromtype = fromtype;
	obj->totype = totype;

	return obj;

}

CString CFlowchartLink::GetString() const
/* ============================================================
	Function :		CFlowchartLink::GetString
	Description :	Returns this object as a string

	Return :		CString	-	String representation of this
								link.
	Parameters :	none

	Usage :			Call to save the object to a file.

   ============================================================*/
{

	CString str;
	CString writetitle = title;
	writetitle.Replace(_T(":"), _T("\\colon"));
	writetitle.Replace(_T(","), _T("\\comma"));
	writetitle.Replace(_T(";"), _T("\\semicolon"));
	writetitle.Replace(_T("\r\n"), _T("\\newline"));

	str.Format(_T("flowchart_link:%i,%i,%s,%s,%s;"), fromtype, totype, writetitle, from, to);
	return str;

}

BOOL CFlowchartLink::FromString(const CString& str)
/* ============================================================
	Function :		CFlowchartLink::FromString
	Description :	Creates a CFlowchartLink-object from a
					string.

	Return :		BOOL				-	TRUE if str is a
											representation of
											a link
	Parameters :	const CString& str	-	String representation
											of a link.

	Usage :			Call to load data from a text stream.

   ============================================================*/
{

	BOOL result = FALSE;
	CTokenizer main(str, _T(":"));
	CString header;
	CString data;
	if (main.GetSize() == 2)
	{
		main.GetAt(0, header);
		main.GetAt(1, data);
		header.TrimLeft();
		header.TrimRight();
		data.TrimLeft();
		data.TrimRight();
		if (header == _T("flowchart_link"))
		{
			CTokenizer tok(data.Left(data.GetLength() - 1));
			INT_PTR size = tok.GetSize();
			if (size == 5)
			{
				int readtype;
				int readantitype;
				CString readtitle;
				CString readfrom;
				CString readto;

				tok.GetAt(0, readtype);
				tok.GetAt(1, readantitype);
				tok.GetAt(2, readtitle);
				tok.GetAt(3, readfrom);
				tok.GetAt(4, readto);

				readtitle.Replace(_T("\\semicolon"), _T(";"));
				readtitle.Replace(_T("\\comma"), _T(","));
				readtitle.Replace(_T("\\colon"), _T(":"));
				readtitle.Replace(_T("\\newline"), _T("\r\n"));

				fromtype = readtype;
				totype = readantitype;
				title = readtitle;
				from = readfrom;
				to = readto;

				result = TRUE;
			}
		}
	}

	return result;

}

int CFlowchartLink::ReverseLink(int link)
/* ============================================================
	Function :		CFlowchartLink::ReverseLink
	Description :	This is a static function returning the
					reverse of link.

	Return :		int			-	Reverse type, or the same
									as the input if none exist.
	Parameters :	int link	-	Link type to test.

	Usage :			Call this as a convenience to get the
					reverse of a link type.

   ============================================================*/
{

	int result = link;
	if (link == LINK_LEFT)
		result = LINK_RIGHT;
	if (link == LINK_RIGHT)
		result = LINK_LEFT;
	if (link == LINK_TOP)
		result = LINK_BOTTOM;
	if (link == LINK_BOTTOM)
		result = LINK_TOP;

	return result;

}

