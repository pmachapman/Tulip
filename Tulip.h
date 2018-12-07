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

// Tulip.h : main header file for the Tulip application
//
#pragma once

#ifndef __AFXWIN_H__
#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols

#include "FlowchartEditor/FlowchartClipboardHandler.h"
#include "NetworkEditor/NetworkEditorClipboardHandler.h"
#include "UMLEditor/UMLClipboardHandler.h"


// CTulipApp:
// See Tulip.cpp for the implementation of this class
//

class CTulipApp : public CWinAppEx
{
public:
	CTulipApp();

	CUMLClipboardHandler			m_umlClip;
	CNetworkEditorClipboardHandler	m_netClip;
	CFlowchartClipboardHandler	m_flowchartClip;

	// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	// Implementation
	UINT  m_nAppLook;
	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	afx_msg void OnFileNew();
	afx_msg void OnFileNewDialog();
	afx_msg void OnFileNewFlowchart();
	afx_msg void OnFileNewNet();
	afx_msg void OnFileNewUml();
	afx_msg void OnFileOpen();
	DECLARE_MESSAGE_MAP()
protected:
	CMultiDocTemplate* m_dialogDocTemplate;
	CMultiDocTemplate* m_flowchartDocTemplate;
	CMultiDocTemplate* m_netDocTemplate;
	CMultiDocTemplate* m_umlDocTemplate;
};

extern CTulipApp theApp;
