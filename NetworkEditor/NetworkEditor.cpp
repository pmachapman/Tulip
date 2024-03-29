/* ==========================================================================
	CNetworkEditor

	Author :		Johan Rosengren, Abstrakt Mekanik AB

	Date :			2004-05-04

	Purpose :		CNetworkEditor derives from CDiagramEditor, and is an
					example on how to handle links between objects.

	Description :	The class overrides object drawing (to draw links as
					well). The class also handles linking and unlinking of
					objects.

	Usage :

   ========================================================================*/
#include "stdafx.h"
#include "NetworkEditor.h"
#include "NetworkEntityContainer.h"
#include "../resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CNetworkEditor::CNetworkEditor()
/* ============================================================
	Function :		CNetworkEditor::CNetworkEditor
	Description :	constructor

	Return :		void
	Parameters :	none

	Usage :

   ============================================================*/
{
}

CNetworkEditor::~CNetworkEditor()
/* ============================================================
	Function :		CNetworkEditor::~CNetworkEditor
	Description :	destructor

	Return :		void
	Parameters :	none

	Usage :

   ============================================================*/
{
}

BEGIN_MESSAGE_MAP(CNetworkEditor, CDiagramEditor)
	ON_COMMAND(IDM_NETWORKEDITOR_LINK, OnLink)
	ON_COMMAND(IDM_NETWORKEDITOR_UNLINK, OnUnlink)
END_MESSAGE_MAP()

void CNetworkEditor::DrawObjects(CDC* dc, double zoom) const
/* ============================================================
	Function :		CNetworkEditor::DrawObjects
	Description :	Draws the objects and links.

	Return :		void
	Parameters :	CDC* dc		-	CDC to draw to
					double zoom	-	Current zoom level

	Usage :			Overridden to draw links as well.

   ============================================================*/
{
	// Draw links
	dc->SelectStockObject(BLACK_PEN);

	CNetworkEntityContainer* objs = static_cast<CNetworkEntityContainer*> (GetDiagramEntityContainer());
	if (objs)
	{
		INT_PTR max = objs->GetLinks();
		for (INT_PTR t = 0; t < max; t++)
		{
			CNetworkLink* link = objs->GetLinkAt(t);
			CNetworkSymbol* from = GetNamedObject(link->from);
			CNetworkSymbol* to = GetNamedObject(link->to);
			if (from && to)
			{
				CPoint start = from->GetLinkPosition();
				CPoint end = to->GetLinkPosition();
				start.x = round((double)start.x * zoom);
				start.y = round((double)start.y * zoom);
				end.x = round((double)end.x * zoom);
				end.y = round((double)end.y * zoom);
				dc->MoveTo(start);
				dc->LineTo(end);
			}
			else
			{
				// Delete this link
				objs->DeleteLink(link);

				// Decrement the counter and reset the max
				t--;
				max = objs->GetLinks();
			}
		}
	}

	// Draw objects
	CDiagramEditor::DrawObjects(dc, zoom);
}

BOOL CNetworkEditor::CanLink()
/* ============================================================
	Function :		CNetworkEditor::CanLink
	Description :	Checks if it's currently possible to link.

	Return :		BOOL	-	TRUE if it is possible to link.
	Parameters :	none

	Usage :			Intended as a command enabler for a Link
					command.

   ============================================================*/
{
	BOOL result = FALSE;
	CNetworkEntityContainer* objs = dynamic_cast<CNetworkEntityContainer*>(GetDiagramEntityContainer());
	if (objs)
		result = objs->CanLink();

	return result;
}

BOOL CNetworkEditor::IsLinked()
/* ============================================================
	Function :		CNetworkEditor::IsLinked
	Description :	Checks if two linked objects are currently
					selected.

	Return :		BOOL	-	TRUE if two linked objects are
								selected.
	Parameters :	none

	Usage :			Intended as a command enabler for an Unlink
					command.

   ============================================================*/
{
	BOOL result = FALSE;
	CNetworkEntityContainer* objs = dynamic_cast<CNetworkEntityContainer*>(GetDiagramEntityContainer());
	if (objs)
		result = objs->IsLinked();
	return result;
}

void CNetworkEditor::OnLink()
/* ============================================================
	Function :		CNetworkEditor::OnLink
	Description :	Command handler for the Link command.

	Return :		void
	Parameters :	none

	Usage :			Called internally.
   ============================================================*/
{

	if (CanLink())
	{
		// Get the two selected objects 
		CNetworkEntityContainer* objs = dynamic_cast<CNetworkEntityContainer *>(GetDiagramEntityContainer());
		CNetworkSymbol* primary = objs->GetPrimarySelected();
		CNetworkSymbol* secondary = objs->GetSecondarySelected();

		GetDiagramEntityContainer()->Snapshot();

		// Link them
		if (primary && secondary)
			if (objs->CreateLink(primary, secondary))
				RedrawWindow();
	}
}

void CNetworkEditor::OnUnlink()
/* ============================================================
	Function :		CNetworkEditor::OnUnlink
	Description :	Command handler for the Break link command.

	Return :		void
	Parameters :	none

	Usage :			Called internally.

   ============================================================*/
{

	if (IsLinked())
	{
		// Get the two selected objects 
		CNetworkEntityContainer* objs = dynamic_cast<CNetworkEntityContainer *>(GetDiagramEntityContainer());
		CDiagramEntity* primary = objs->GetPrimaryLink();;
		CDiagramEntity* secondary = objs->GetSecondaryLink();
		if (primary && secondary)
		{
			CNetworkLink* link = objs->FindLink(primary, secondary);

			GetDiagramEntityContainer()->Snapshot();
			objs->DeleteLink(link);
			RedrawWindow();
		}
	}
}

CNetworkSymbol* CNetworkEditor::GetNamedObject(const CString& name) const
/* ============================================================
	Function :		CNetworkEditor::GetNamedObject
	Description :	Returns the object with the name attribute
					name.

	Return :		CNetworkSymbol*	-	The object, or NULL
											if not found.
	Parameters :	const CString& name	-	The name of the
											object to find.

	Usage :			Call to get the object with the name name,
					if it exists.

   ============================================================*/
{

	CDiagramEntity* result = NULL;

	INT_PTR count = GetObjectCount();
	CDiagramEntity* obj;
	for (int t = 0; t < count; t++)
	{
		obj = GetObject(t);
		if (obj && obj->GetName() == name)
			result = obj;
	}

	return dynamic_cast<CNetworkSymbol*>(result);

}

void CNetworkEditor::ExportEMF(const CString& filename)
/* ============================================================
	Function :		CNetworkEditor::ExportEMF
	Description :	Exports the current diagram to an EMF-file.
	Access :

	Return :		void
	Parameters :	none

	Usage :			Call to export the diagram to an EMF file
					for - for example - inclusion into a
					word-processing document.

   ============================================================*/
{

	CFileDialog dlg(FALSE, _T("emf"), filename, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("Enhanced MetaFile (*.emf)|*.emf|All Files (*.*)|*.*||"));
	if (dlg.DoModal() == IDOK)
	{

		CClientDC	dc(this);
		CMetaFileDC	metaDC;
		CRect rect(0, 0,
			GetVirtualSize().cx,
			GetVirtualSize().cy);

		// Determine the picture frame dimensions.
		// iWidthMM is the display width in millimeters.
		// iHeightMM is the display height in millimeters.
		// iWidthPels is the display width in pixels.
		// iHeightPels is the display height in pixels

		int iWidthMM = GetDeviceCaps(dc, HORZSIZE);
		int iHeightMM = GetDeviceCaps(dc, VERTSIZE);
		int iWidthPels = GetDeviceCaps(dc, HORZRES);
		int iHeightPels = GetDeviceCaps(dc, VERTRES);

		// Convert client coordinates to .01-mm units.
		// Use iWidthMM, iWidthPels, iHeightMM, and
		// iHeightPels to determine the number of
		// .01-millimeter units per pixel in the x-
		//  and y-directions.
		CRect r(
			(rect.left * iWidthMM * 100) / iWidthPels,
			(rect.top * iHeightMM * 100) / iHeightPels,
			(rect.right * iWidthMM * 100) / iWidthPels,
			(rect.bottom * iHeightMM * 100) / iHeightPels);

		metaDC.CreateEnhanced(&dc, dlg.GetPathName(), &r, _T("NetworkEditor Drawing"));
		metaDC.SetAttribDC(dc.m_hDC);

		SetRedraw(FALSE);
		COLORREF col = GetBackgroundColor();

		SetBackgroundColor(RGB(255, 255, 255));
		Print(&metaDC, rect, 1);
		SetBackgroundColor(col);
		SetRedraw(TRUE);
		RedrawWindow();

		::DeleteEnhMetaFile(metaDC.CloseEnhanced());

	}

}
