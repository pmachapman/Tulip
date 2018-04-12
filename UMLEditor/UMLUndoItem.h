#ifndef _CUMLUNDOITEM_H_68A37D5B_34FC_40A6_8D1D2B9DEAFA

#define _CUMLUNDOITEM_H_68A37D5B_34FC_40A6_8D1D2B9DEAFA

///////////////////////////////////////////////////////////
// CUMLUndoItem.h
// Created: 06/02/04
//
#include "DiagramEditor/UndoItem.h"

class CUMLUndoItem : public CUndoItem
{
public:
	// Construction/destruction
	CUMLUndoItem();
	virtual ~CUMLUndoItem();

// Attributes
	COLORREF col;
	CString package;

};

#endif //_CUMLUNDOITEM_H_68A37D5B_34FC_40A6_8D1D2B9DEAFA
