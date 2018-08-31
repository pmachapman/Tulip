#ifndef _CTOKENIZER_H_9BBD5DD2_819A_40CE_92574222C14D
#define _CTOKENIZER_H_9BBD5DD2_819A_40CE_92574222C14D

///////////////////////////////////////////////////////////
// File :		Tokenizer.h
// Created :	06/18/04
//


class CTokenizer
{
public:
	// Construction/destruction
	CTokenizer(CString strInput, const CString & strDelimiter = _T(","));

	// Operations
	void Init(const CString & strInput, const CString & strDelimiter = _T(","));
	INT_PTR GetSize() const;
	void GetAt(INT_PTR nIndex, CString & str) const;
	void GetAt(INT_PTR nIndex, int & var) const;
	void GetAt(INT_PTR nIndex, WORD & var) const;
	void GetAt(INT_PTR nIndex, double & var) const;
	void GetAt(INT_PTR nIndex, DWORD & var) const;


	// Attributes

private:
	CStringArray m_stra;


};
#endif //_CTOKENIZER_H_9BBD5DD2_819A_40CE_92574222C14D
