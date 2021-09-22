/* ==========================================================================
	Class :			CTokenizer

	Date :			06/18/04

	Purpose :		"CTokenizer" is a very simple class to tokenize a string
					into a string array.

	Description :	The string is chopped up and put into an internal
					"CStringArray". With "GetAt", different types of data can
					be read from the different elements.

	Usage :			Create an instance of "CTokenizer" on the stack. Set
					the string to parse in the "ctor", and the delimiter if
					not a comma. The "ctor" automatically tokenize the input
					string.

					Call "GetSize" to get the number of
					tokens, and "GetAt" to get a specific token.

					You can also reuse the tokenizer by calling "Init".
   ========================================================================
	Changes :		28/8  2004	Changed a char to TCHAR to allow UNICODE
								building (Enrico Detoma)
   ========================================================================*/

#include "stdafx.h"
#include "Tokenizer.h"


   ////////////////////////////////////////////////////////////////////
   // Public functions
   //
CTokenizer::CTokenizer(CString strInput, const CString& strDelimiter, const CString& strEscapeStart, const CString& strEscapeEnd)
/* ============================================================
	Function :		CTokenizer::CTokenizer
	Description :	Constructor.
	Access :		Public

	Return :		void
	Parameters :	CString strInput				-	String to
														tokenize
					const CString & strDelimiter	-	Delimiter, defaults to comma
					const CString & strEscapeStart	-	Starting Escape Character, defaults to empty string
					const CString & strEscapeEnd	-	Ending Escape Character, defaults to empty string
	Usage :			Should normally be created on the stack.

   ============================================================*/
{

	Init(strInput, strDelimiter, strEscapeStart, strEscapeEnd);

}

void CTokenizer::Init(const CString& strInput, const CString& strDelimiter, const CString& strEscapeStart, const CString& strEscapeEnd)
/* ============================================================
	Function :		CTokenizer::Init
	Description :	Reinitializes and tokenizes the tokenizer
					with "strInput". "strDelimiter" is the
					delimiter to use.
	Access :		Public

	Return :		void
	Parameters :	const CString & strInput		-	New string
					const CString & strDelimiter	-	Delimiter, defaults to comma
					const CString & strEscapeStart	-	Starting Escape Character, defaults to empty string
					const CString & strEscapeEnd	-	Ending Escape Character, defaults to empty string

	Usage :			Call to reinitialize the tokenizer.

   ============================================================*/
{
	// Set up the copy of the string we will work with
	CString copy(strInput);
	m_stra.RemoveAll();

	// Get the first escape character, if it is set
	int escapeStart = -1;
	if (strEscapeStart != _T(""))
	{
		escapeStart = copy.Find(strEscapeStart);
	}

	// Get the end escape character, if it is set and there is a start escape character
	int escapeEnd = -1;
	if (escapeStart > -1 && strEscapeEnd != _T(""))
	{
		escapeEnd = copy.Find(strEscapeEnd, escapeStart);
	}

	// Look for the delimiter
	int nFound = copy.Find(strDelimiter);

	// Make sure the delimiter is not between the escape start and end
	if (nFound > escapeStart && nFound < escapeEnd)
	{
		nFound = copy.Find(strDelimiter, escapeEnd);
	}

	// While we can find the delimiter
	while (nFound != -1)
	{
		// Get the token, and remove it from our string copy
		CString strLeft;
		strLeft = copy.Left(nFound);
		copy = copy.Right(copy.GetLength() - (nFound + 1));

		// Add the token to the list
		m_stra.Add(strLeft);

		// Get the first escape character, if it is set
		if (strEscapeStart != _T(""))
		{
			escapeStart = copy.Find(strEscapeStart);
		}

		// Get the end escape character, if it is set and there is a start escape character
		if (escapeStart > -1 && strEscapeEnd != _T(""))
		{
			escapeEnd = copy.Find(strEscapeEnd, escapeStart);
		}
		else
		{
			escapeEnd = -1;
		}

		// Look for the next occurence of the delimiter
		nFound = copy.Find(strDelimiter);

		// Make sure the delimiter is not between the escape start and end
		if (nFound > escapeStart && nFound < escapeEnd)
		{
			nFound = copy.Find(strDelimiter, escapeEnd);
		}
	}

	// Add what is left of our string copy as a token to the list
	m_stra.Add(copy);
}

INT_PTR CTokenizer::GetSize() const
/* ============================================================
	Function :		CTokenizer::GetSize
	Description :	Gets the number of tokens in the tokenizer.
	Access :		Public

	Return :		int	-	Number of tokens.
	Parameters :	none

	Usage :			Call to get the number of tokens in the
					tokenizer.

   ============================================================*/
{

	return m_stra.GetSize();

}

void CTokenizer::GetAt(INT_PTR nIndex, CString& str) const
/* ============================================================
	Function :		CTokenizer::GetAt
	Description :	Get the token at "nIndex" and put it in
					"str".
	Access :		Public

	Return :		void
	Parameters :	int nIndex		- Index to get token from
					CString & str	- Result

	Usage :			Call to get the value of the token at
					"index".

   ============================================================*/
{

	if (nIndex < m_stra.GetSize())
		str = m_stra.GetAt(nIndex);
	else
		str = _T("");

}

void CTokenizer::GetAt(INT_PTR nIndex, int& var) const
/* ============================================================
	Function :		CTokenizer::GetAt
	Description :	Get the token at "nIndex" and put it in
					"var".
	Access :		Public

	Return :		void
	Parameters :	int nIndex	- Index to get token from
					int & var	- Result

	Usage :			Call to get the value of the token at
					"index".

   ============================================================*/
{

	if (nIndex < m_stra.GetSize())
		var = _ttoi(m_stra.GetAt(nIndex));
	else
		var = 0;

}

void CTokenizer::GetAt(INT_PTR nIndex, WORD& var) const
/* ============================================================
	Function :		CTokenizer::GetAt
	Description :	Get the token at "nIndex" and put it in
					"var".
	Access :		Public

	Return :		void
	Parameters :	int nIndex	- Index to get token from
					WORD & var	- Result

	Usage :			Call to get the value of the token at
					"index".

   ============================================================*/
{

	if (nIndex < m_stra.GetSize())
		var = static_cast<WORD>(_ttoi(m_stra.GetAt(nIndex)));
	else
		var = 0;

}

void CTokenizer::GetAt(INT_PTR nIndex, double& var) const
/* ============================================================
	Function :		CTokenizer::GetAt
	Description :	Get the token at "nIndex" and put it in
					"var".
	Access :		Public

	Return :		void
	Parameters :	int nIndex		- Index to get token from
					double & var	- Result

	Usage :			Call to get the value of the token at
					"index".

   ============================================================*/
{

	TCHAR* stop;
	if (nIndex < m_stra.GetSize())
		var = _tcstod(m_stra.GetAt(nIndex), &stop);
	else
		var = 0.0;

}

void CTokenizer::GetAt(INT_PTR nIndex, DWORD& var) const
/* ============================================================
	Function :		CTokenizer::GetAt
	Description :	Get the token at "nIndex" and put it in
					"var".
	Access :		Public

	Return :		void
	Parameters :	int nIndex	- Index to get token from
					DWORD & var	- Result

	Usage :			Call to get the value of the token at
					"index".

   ============================================================*/
{

	if (nIndex < m_stra.GetSize())
		var = static_cast<DWORD>(_ttoi(m_stra.GetAt(nIndex)));
	else
		var = 0;

}
