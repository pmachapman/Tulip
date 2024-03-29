/* ==========================================================================
	CDiagramControlFactory

	Author :		Johan Rosengren, Abstrakt Mekanik AB

	Date :			2004-03-31

	Purpose :		CDiagramControlFactory is a factory-class with one
					single member, allowing the creation of CDiagramEntity-
					derived objects from a string.

	Description :	CreateFromString tries to instantiate one of the control
					objects by testing the control object static function
					CreateFromString until it hits a valid one. The
					resulting pointer is then returned and can be added to
					the application data.

	Usage :			This is an example on how to handle loading from a text
					file to a CDiagramEditor. Just call
					CDiagramControlFactory::CreateFromString and if a non-
					NULL pointer is returned, add it to the CDiagramEditor
					data.

   ========================================================================*/
#include "stdafx.h"
#include "DiagramControlFactory.h"

#include "DiagramButton.h"
#include "DiagramEdit.h"
#include "DiagramStatic.h"
#include "DiagramCheckbox.h"
#include "DiagramRadiobutton.h"
#include "DiagramCombobox.h"
#include "DiagramListbox.h"
#include "DiagramGroupbox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDiagramControlFactory

CDiagramEntity* CDiagramControlFactory::CreateFromString(const CString& str)
{
	CDiagramEntity* obj;

	obj = CDiagramButton::CreateFromString(str);
	if (!obj)
		obj = CDiagramEdit::CreateFromString(str);
	if (!obj)
		obj = CDiagramStatic::CreateFromString(str);
	if (!obj)
		obj = CDiagramCheckbox::CreateFromString(str);
	if (!obj)
		obj = CDiagramRadiobutton::CreateFromString(str);
	if (!obj)
		obj = CDiagramCombobox::CreateFromString(str);
	if (!obj)
		obj = CDiagramListbox::CreateFromString(str);
	if (!obj)
		obj = CDiagramGroupbox::CreateFromString(str);

	return obj;
}