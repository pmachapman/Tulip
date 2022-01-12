/* ==========================================================================
	CFlowchartClipboardHandler

	Author :		Johan Rosengren, Abstrakt Mekanik AB

	Date :			2004-04-30

	Purpose :		CFlowchartClipboardHandler is a clipboard handler that
					includes links in the clipboard.

	Description :	Links are saved and loaded to/from  a separate
					CObArray.

	Usage :			Declare an instance of CFlowchartClipboardHandler in
					your document (for a SDI-app), dialog class (for a
					CDialog-derived app) or centrally for a MDI-application.
					Use CDiagramEntityContainer::SetClipboardHandler to
					attach it to application data. If you are not using
					an external CDiagramEntityContainer, but let editor
					create the data internally, call
					CDiagramEditor::GetGetDiagramEntityContainer()->SetClipboardHandler
					after the CDiagramEditor::Create-call.

   ========================================================================*/
#include "stdafx.h"
#include "FlowchartClipboardHandler.h"
#include "../UMLEditor/LinkFactory.h"
#include "../DiagramEditor/DiagramEntityContainer.h"
#include "FlowchartEntityContainer.h"
#include "FlowchartLink.h"

CFlowchartClipboardHandler::CFlowchartClipboardHandler()
/* ============================================================
	Function :		CFlowchartClipboardHandler::CFlowchartClipboardHandler
	Description :	constructor

	Return :		void
	Parameters :	none

	Usage :

   ============================================================*/
{
}

CFlowchartClipboardHandler::~CFlowchartClipboardHandler()
/* ============================================================
	Function :		CFlowchartClipboardHandler::~CFlowchartClipboardHandler
	Description :	destructor

	Return :		void
	Parameters :	none

	Usage :

   ============================================================*/
{

	ClearPaste();

}

void CFlowchartClipboardHandler::Copy(CDiagramEntity* obj)
/* ============================================================
	Function :		CFlowchartClipboardHandler::Copy
	Description :	Copies obj to the paste array

	Return :		void
	Parameters :	CDiagramEntity* obj	-	Object to copy.

	Usage :			Overridden to assign a new id.

   ============================================================*/
{

	if (obj)
	{
		ClearPaste();
		CDiagramEntity* newobj = obj->Clone();
		newobj->Select(TRUE);
		GetData()->Add(newobj);
	}

}

void CFlowchartClipboardHandler::Paste(CDiagramEntityContainer* container)
/* ============================================================
	Function :		CFlowchartClipboardHandler::Paste
	Description :	Pastes the contents of the paste array to
					the data array.

	Return :		void
	Parameters :	none

	Usage :			Overridden to paste links as well.

   ============================================================*/
{

	CDiagramClipboardHandler::Paste(container);
	INT_PTR max = m_pasteLinks.GetSize();
	CFlowchartEntityContainer* flow = static_cast<CFlowchartEntityContainer*>(container);
	for (INT_PTR t = 0; t < max; t++)
		flow->AddLink((static_cast<CFlowchartLink*>(m_pasteLinks[t]))->Clone());

}

void CFlowchartClipboardHandler::CopyAllSelected(CDiagramEntityContainer* container)
/* ============================================================
	Function :		CFlowchartClipboardHandler::CopyAllSelected
	Description :	Copies all the selected items to the paste
					array.

	Return :		void
	Parameters :	none

	Usage :			Overridden to add links as well. New ids
					are assigned to the copied objects, and the
					copied links are updated.

   ============================================================*/
{

	CDiagramClipboardHandler::CopyAllSelected(container);
	CFlowchartEntityContainer* flow = static_cast<CFlowchartEntityContainer*>(container);
	CObArray* links = flow->GetLinkArray();

	INT_PTR max = links->GetSize();
	for (INT_PTR t = 0; t < max; t++)
	{
		CFlowchartLink* link = static_cast<CFlowchartLink*>(links->GetAt(t));
		m_pasteLinks.Add(link->Clone());
	}

	CObArray* paste = GetData();
	max = paste->GetSize();

	for (INT_PTR t = 0; t < max; t++)
	{
		CDiagramEntity* obj = static_cast<CDiagramEntity*>(paste->GetAt(t));
		CString newID = CLinkFactory::GetID();

		INT_PTR maxlinks = m_pasteLinks.GetSize();
		for (INT_PTR i = 0; i < maxlinks; i++)
		{
			CFlowchartLink* link = static_cast<CFlowchartLink*>(m_pasteLinks[i]);
			if (link->from == obj->GetName())
				link->from = newID;
			if (link->to == obj->GetName())
				link->to = newID;
		}

		obj->SetName(newID);

	}

}

void CFlowchartClipboardHandler::ClearPaste()
/* ============================================================
	Function :		CFlowchartClipboardHandler::ClearPaste
	Description :	Clears the paste array.

	Return :		void
	Parameters :	none

	Usage :			Overridden to clear the paste link array as
					well.

   ============================================================*/
{

	CDiagramClipboardHandler::ClearPaste();

	INT_PTR max = m_pasteLinks.GetSize();
	for (INT_PTR t = max - 1; t >= 0; t--)
		delete m_pasteLinks[t];

	m_pasteLinks.RemoveAll();

}

