#ifndef _OPERATIONCONTAINER_H_5DE26980_B9B9_4899_8993C9B614
#define _OPERATIONCONTAINER_H_5DE26980_B9B9_4899_8993C9B614

///////////////////////////////////////////////////////////
// File :		COperationContainer.h
// Created :	06/08/04
//
#include "Operation.h"

class COperationContainer
{
public:
	// Construction/destruction
	COperationContainer();
	virtual ~COperationContainer();

	// Operations
	void Copy(COperationContainer & operations);
	INT_PTR GetSize() const;
	COperation* GetAt(INT_PTR index) const;
	void RemoveAt(INT_PTR index);
	void RemoveAll();
	void Add(COperation * operation);
	CString GetString(int format = STRING_FORMAT_SAVE) const;
	void SetSize(INT_PTR size);
	void SetAt(INT_PTR index, COperation* obj);

	// Attributes

private:
	CObArray m_operations;


};
#endif //_OPERATIONCONTAINER_H_5DE26980_B9B9_4899_8993C9B614
