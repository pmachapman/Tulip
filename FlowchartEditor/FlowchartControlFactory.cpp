/* ==========================================================================
	CFlowchartControlFactory

	Author :		Johan Rosengren, Abstrakt Mekanik AB

	Date :			2004-04-22

	Purpose :		CFlowchartControlFactory is a factory-class with one
					single member, allowing the creation of CDiagramEntity-
					derived objects from a string.

	Description :	CreateFromString tries to instantiate one of the control
					objects by testing the control object static function
					CreateFromString until it hits a valid one. The
					resulting pointer is then returned and can be added to
					the application data.

	Usage :			Call CFlowchartControlFactory::CreateFromString and if
					a non-NULL pointer is returned, add it to the
					editor data.

   ========================================================================*/
#include "stdafx.h"
#include "FlowchartControlFactory.h"

#include "FlowchartEntityTerminator.h"
#include "FlowchartEntityBox.h"
#include "FlowchartEntityCondition.h"
#include "FlowchartEntityConnector.h"
#include "FlowchartEntityIO.h"
#include "FlowchartLabel.h"
#include "FlowchartLineSegment.h"
#include "FlowchartLinkableLineSegment.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFlowchartControlFactory

CDiagramEntity* CFlowchartControlFactory::CreateFromString(const CString& str)
/* ============================================================
	Function :		CFlowchartControlFactory::CreateFromString
	Description :	The function returns an object from the
					parameter str.

	Return :		CDiagramEntity*		-	The new object, or
											NULL is str is not a
											valid representation.
	Parameters :	const CString& str	-	The string to create
											a new object from

	Usage :			Call this static function while reading
					string representations of objects from a
					text file. Note that the caller is
					responsible for the allocated memory.

   ============================================================*/
{
	CDiagramEntity* obj;

	obj = CFlowchartEntityTerminator::CreateFromString(str);
	if (!obj)
		obj = CFlowchartEntityBox::CreateFromString(str);
	if (!obj)
		obj = CFlowchartEntityConnector::CreateFromString(str);
	if (!obj)
		obj = CFlowchartEntityCondition::CreateFromString(str);
	if (!obj)
		obj = CFlowchartEntityIO::CreateFromString(str);
	if (!obj)
		obj = CFlowchartLineSegment::CreateFromString(str);
	if (!obj)
		obj = CFlowchartLinkableLineSegment::CreateFromString(str);
	if (!obj)
		obj = CFlowchartLabel::CreateFromString(str);

	return obj;
}