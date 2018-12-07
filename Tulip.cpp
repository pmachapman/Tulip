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

// Tulip.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "Tulip.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "DialogEditorDoc.h"
#include "DialogEditorView.h"
#include "FlowchartDoc.h"
#include "FlowchartView.h"
#include "NetDoc.h"
#include "NetView.h"
#include "UmlDoc.h"
#include "UmlView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTulipApp

BEGIN_MESSAGE_MAP(CTulipApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CTulipApp::OnAppAbout)
	// Standard file based document commands
	// TODO: ID_FILE_NEW should create a new of whatever document is open
	ON_COMMAND(ID_FILE_NEW, &CTulipApp::OnFileNew)
	ON_COMMAND(ID_FILE_NEW_DIALOG, &CTulipApp::OnFileNewDialog)
	ON_COMMAND(ID_FILE_NEW_FLOWCHART, &CTulipApp::OnFileNewFlowchart)
	ON_COMMAND(ID_FILE_NEW_NET, &CTulipApp::OnFileNewNet)
	ON_COMMAND(ID_FILE_NEW_UML, &CTulipApp::OnFileNewUml)
	ON_COMMAND(ID_FILE_OPEN, &CTulipApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()


// CTulipApp construction

CTulipApp::CTulipApp()
{
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// If the application is built using Common Language Runtime support (/clr):
	//     1) This additional setting is needed for Restart Manager support to work properly.
	//     2) In your project, you must add a reference to System.Windows.Forms in order to build.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// Recommended format for string is CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("Conglomo.Tulip.1.0"));

	// Place all significant initialization in InitInstance
}

// The one and only CTulipApp object

CTulipApp theApp;


// CTulipApp initialization

BOOL CTulipApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();


	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	EnableTaskbarInteraction(TRUE);

	// AfxInitRichEdit2() is required to use RichEdit control
	// AfxInitRichEdit2();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	SetRegistryKey(_T("Conglomo"));
	LoadStdProfileSettings(9);  // Load standard INI file options (including MRU)

	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views

	m_dialogDocTemplate = new CMultiDocTemplate(
		IDR_DIALOGTYPE,
		RUNTIME_CLASS(CDialogEditorDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CDialogEditorView));
	if (!m_dialogDocTemplate)
		return FALSE;
	AddDocTemplate(m_dialogDocTemplate);

	m_flowchartDocTemplate = new CMultiDocTemplate(
		IDR_FLOWCHTYPE,
		RUNTIME_CLASS(CFlowchartDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CFlowchartView));
	if (!m_flowchartDocTemplate)
		return FALSE;
	AddDocTemplate(m_flowchartDocTemplate);

	m_netDocTemplate = new CMultiDocTemplate(
		IDR_NETWORTYPE,
		RUNTIME_CLASS(CNetDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CNetView));
	if (!m_netDocTemplate)
		return FALSE;
	AddDocTemplate(m_netDocTemplate);
	
	m_umlDocTemplate = new CMultiDocTemplate(IDR_UMLTYPE,
		RUNTIME_CLASS(CUmlDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CUmlView));
	if (!m_umlDocTemplate)
		return FALSE;
	AddDocTemplate(m_umlDocTemplate);

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;

	// call DragAcceptFiles only if there's a suffix
	//  In an MDI app, this should occur immediately after setting m_pMainWnd
	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// The main window has been initialized, so show and update it
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	return TRUE;
}

int CTulipApp::ExitInstance()
{
	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
}

// CTulipApp message handlers


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// App command to run the dialog
void CTulipApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

void CTulipApp::OnFileNewDialog()
{
	m_dialogDocTemplate->OpenDocumentFile(NULL);
}

void CTulipApp::OnFileNew()
{
	CWinApp::OnFileNew();
}

void CTulipApp::OnFileNewFlowchart()
{
	m_flowchartDocTemplate->OpenDocumentFile(NULL);
}

void CTulipApp::OnFileNewNet()
{
	m_netDocTemplate->OpenDocumentFile(NULL);
}

void CTulipApp::OnFileNewUml()
{
	m_umlDocTemplate->OpenDocumentFile(NULL);
}

void CTulipApp::OnFileOpen()
{
	LPCTSTR pszFilter =
		_T("All Tulip Files (*.dialog;*.flowdiagram;*.netdiagram;*.umldiagram)|*.dialog;*.flowdiagram;*.netdiagram;*.umldiagram|")
		_T("Dialog (*.dialog)|*.dialog|")
		_T("Flowchart Diagram (*.flowdiagram)|*.flowdiagram|")
		_T("Network Diagram (*.netdiagram)|*.netdiagram|")
		_T("UML Diagram (*.umldiagram)|*.umldiagram|")
		_T("All Files (*.*)|*.*||");

	CFileDialog dlgFile(TRUE, NULL, NULL,
		OFN_HIDEREADONLY | OFN_FILEMUSTEXIST,
		pszFilter,
		AfxGetMainWnd());

	if (dlgFile.DoModal() == IDOK)
	{
		OpenDocumentFile(dlgFile.GetPathName());
	}
}

// CTulipApp customization load/save methods

void CTulipApp::PreLoadState()
{
}

void CTulipApp::LoadCustomState()
{
}

void CTulipApp::SaveCustomState()
{
}

// CTulipApp message handlers



