/* ==========================================================================
	Class :			CUMLEntityPackage

	Author :		Johan Rosengren, Abstrakt Mekanik AB

	Date :			2004-06-21

	Purpose :		"CUMLEntityPackage" is derived from "CUMLEntity" and 
					represents an UML package object.

	Description :	The "CUMLEntity"-derived objects are the drawing objects 
					of the UML-editor.
					The name ("m_name") of the package is used as the 
					current package name in the editor, so that if you 
					right-click the package and select Open, only the objects 
					in this package will be displayed. Packages can be nested 
					inside each other.

	Usage :			When loading, create with "CUMLControlFactory::CreateFromString". 
					Otherwise, create instances in the application view or dialog and 
					add them to the editor with "StartDrawingObject". The 
					editor takes responsibility for the object.

   ========================================================================*/
#include "stdafx.h"
#include "UMLEntityPackage.h"
#include "DiagramEditor/Tokenizer.h"
#include "UMLEntityContainer.h"
#include "StringHelpers.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CUMLEntityPackage::CUMLEntityPackage()
/* ============================================================
	Function :		CUMLEntityPackage::CUMLEntityPackage
	Description :	Constructor
	Access :		Public

	Return :		void
	Parameters :	none

	Usage :			Create either through "CUMLControlFactory" or
					by calling "new" and adding the pointer to the
					editor via "StartDrawingObject"

   ============================================================*/
{

	SetDefaultSize( CSize( 112, 80 ) );

	SetPropertyDialog( &m_dlg, CUMLPackagePropertyDialog::IDD );

	CString title;
	title.LoadString( IDS_UML_PACKAGE );
	SetTitle( title );

	SetType( _T( "uml_package" ) );
	SetConstraints( GetDefaultSize(), CSize( -1, -1 ) );

	SetBkColor( RGB( 221,221,255 ) );
	CalcRestraints();

}

CUMLEntityPackage::~CUMLEntityPackage()
/* ============================================================
	Function :		CUMLEntityPackage::~CUMLEntityPackage
	Description :	Destructor
	Access :		Public

	Return :		void
	Parameters :	none

	Usage :			Note that objects will normally be deleted by
					the container.

   ============================================================*/
{

	if( m_dlg.m_hWnd )
		m_dlg.DestroyWindow();

}

CDiagramEntity* CUMLEntityPackage::Clone()
/* ============================================================
	Function :		CUMLEntityPackage::Clone
	Description :	Creates a new object of this type, copies 
					the data from this object, and returns the 
					new one.
	Access :		Public

	Return :		CDiagramEntity*	-	New object
	Parameters :	none

	Usage :			Call to clone this object

   ============================================================*/
{

	CUMLEntityPackage* obj = new CUMLEntityPackage;
	obj->Copy( this );
	return obj;

}

CDiagramEntity* CUMLEntityPackage::CreateFromString( const CString& str )
/* ============================================================
	Function :		CUMLEntityPackage::CreateFromString
	Description :	Static factory function that creates and 
					returns an instance of this class if "str" 
					is a valid representation.
	Access :		Public

	Return :		CDiagramEntity*		-	The object, or "NULL" 
											if "str" is not a 
											representation of 
											this type.
	Parameters :	const CString& str	-	The string to create 
											from.

	Usage :			Can be used as a factory for text file loads. 

   ============================================================*/
{

	CUMLEntityPackage* obj = new CUMLEntityPackage;
	if(!obj->FromString( str ) )
	{
		delete obj;
		obj = NULL;
	}

	return obj;

}

void CUMLEntityPackage::Draw( CDC* dc, CRect rect )
/* ============================================================
	Function :		CUMLEntityPackage::Draw
	Description :	Draws this object.
	Access :		Public

	Return :		void
	Parameters :	CDC* dc		-	"CDC" to draw to
					CRect rect	-	Rectangle to draw to
					
	Usage :			Call to draw the object.

   ============================================================*/
{

	int cutoff = round( static_cast< double >( GetMarkerSize().cx ) * GetZoom() );
	CRect tabbox( rect );
	CRect bigbox( rect );

	tabbox.left += cutoff;
	tabbox.bottom = tabbox.top + cutoff * 2;
	tabbox.right = tabbox.left + cutoff * 8;
	if( tabbox.right > rect.right )
		tabbox.right = rect.right - cutoff;

	bigbox.top = tabbox.bottom - 1;

	dc->SelectStockObject( BLACK_PEN );
	CBrush bk;
	bk.CreateSolidBrush( GetBkColor() );
	dc->SelectObject( &bk );

	dc->Rectangle( tabbox );
	dc->Rectangle( bigbox );

	if( GetTitle().GetLength() )
	{
		bigbox.top += cutoff;
		CFont font;
		dc->SetBkMode( OPAQUE );
		dc->SetBkColor( GetBkColor() );
		font.CreateFont( -round( 12.0 * GetZoom() ), 0,0,0,FW_BOLD,0,0,0,0,0,0,0,0, GetFont() );
		CFont* oldfont = dc->SelectObject( &font );
		dc->DrawText( GetTitle(), bigbox, DT_SINGLELINE | DT_CENTER );
		dc->SelectObject( oldfont );
	}

	dc->SelectStockObject( BLACK_PEN );
	dc->SelectStockObject( WHITE_BRUSH );

}

CPoint CUMLEntityPackage::GetLinkPosition( int type ) const
/* ============================================================
	Function :		CUMLEntityPackage::GetLinkPosition
	Description :	Returns the position of a link.
	Access :		Public
					
	Return :		CPoint		-	The position of the link,
									-1, -1 if the link is not
									allowed.
	Parameters :	int type	-	The type of the link.
					
	Usage :			The possible link types are:
						"LINK_TOP" Link at the top
						"LINK_BOTTOM" Link at the bottom
						"LINK_LEFT" Link at the left
						"LINK_RIGHT" Link at the right
						"LINK_START" Link at the start point of a line
						"LINK_END" Link at the end point of a line.

   ============================================================*/
{

	int cutoff = round( static_cast< double >( GetMarkerSize().cx ) * GetZoom() );
	CPoint point( -1, -1 );
	CRect rect = GetRect();
	switch( type )
	{
		case LINK_LEFT:
			point.x = rect.left;
			point.y = rect.top + round( static_cast< double >( rect.Height() ) / 2 );
			break;
		case LINK_RIGHT:
			point.x = rect.right;
			point.y = rect.top + round( static_cast< double >( rect.Height() ) / 2 );
			break;
		case LINK_TOP:
			point.x = rect.left + cutoff * 5;
			point.y = rect.top;
			break;
		case LINK_BOTTOM:
			point.x = rect.left + round( static_cast< double >( rect.Width() ) / 2 );
			point.y = rect.bottom;
			break;
	}

	return point;

}

void CUMLEntityPackage::ShowPopup( CPoint point, CWnd* parent )
/* ============================================================
	Function :		CUMLEntityPackage::ShowPopup
	Description :	Shows the popup menu for the object.
	Access :		Public

	Return :		void
	Parameters :	CPoint point	-	The point to track.
					CWnd* parent	-	The parent "CWnd" of the 
										menu (should be the 
										"CDiagramEditor")

	Usage :			Called from the editor.

   ============================================================*/
{

	CMenu menu;
	menu.LoadMenu( IDR_UML_MENU_PACKAGE_POPUP );
	menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, parent );

}

void CUMLEntityPackage::CalcRestraints()
/* ============================================================
	Function :		CUMLEntityPackage::CalcRestraints
	Description :	Calculates, depending on the contents, the 
					minimum size for this object.
	Access :		Public

	Return :		void
	Parameters :	none

	Usage :			Call when the contents change.

   ============================================================*/
{

	CString title = GetTitle();

	int cutoff = GetMarkerSize().cx;
	CRect rect( 0, 0, GetDefaultSize().cx, GetDefaultSize().cy );
	if( title.GetLength() )
	{
		CDC* dc = CWnd::GetDesktopWindow()->GetDC();
		CFont font;
		font.CreateFont( -12, 0,0,0,FW_BOLD,0,0,0,0,0,0,0,0, GetFont() );
		CFont* oldfont = dc->SelectObject( &font );
		dc->DrawText( GetTitle(), rect, DT_WORDBREAK | DT_CALCRECT );
		dc->SelectObject( oldfont );
		CWnd::GetDesktopWindow()->ReleaseDC( dc );
	}

	int minwidth = max( rect.Width() + cutoff * 2, GetDefaultSize().cx );
	CUMLEntityContainer* container = GetUMLContainer();
	if( container )
	{
		CSize sz = container->CalcMinimumRestraints( GetName() );
		if( sz.cy > minwidth )
			minwidth = sz.cy;
	}

	int steps = round( static_cast< double >( minwidth ) / 8 );
	minwidth = steps * 8;

	if( GetMinimumSize().cx != minwidth )
	{
		SetMinimumSize( CSize( minwidth,GetDefaultSize().cy ) );
		CRect rc = GetRect();
		if( rc.Width() < minwidth )
		{
			SetRect( GetLeft(), GetTop(), GetLeft() + minwidth, GetBottom() );
			if( GetUMLContainer() )
				GetUMLContainer()->AdjustLinkedObjects( this );
		}
	}
	else
		SetMinimumSize( GetDefaultSize() );

}

void CUMLEntityPackage::SetTitle( CString title )
/* ============================================================
	Function :		CUMLEntityPackage::SetTitle
	Description :	Sets the object title
	Access :		Public

	Return :		void
	Parameters :	CString title	-	New title
					
	Usage :			Will remove some whitespace and recalculate
					the object restraints.

   ============================================================*/
{

	title.TrimLeft();
	title.TrimRight();
	title.Remove( TCHAR( '\r' ) );
	title.Remove( TCHAR( '\n' ) );
	title.Remove( TCHAR( ' ' ) );

	CDiagramEntity::SetTitle( title );
	CalcRestraints();

}

BOOL CUMLEntityPackage::ModifyTitle( const CString& title )
/* ============================================================
	Function :		CUMLEntityPackage::ModifyTitle
	Description :	Modifies the title of the package.
	Access :		Public

	Return :		BOOL					-	"TRUE" if modified
	Parameters :	const CString& title	-	New title
					
	Usage :			Call to modify the title of the package. 
					The container will be checked for the 
					existance of another package with this name 
					first.

   ============================================================*/
{

	BOOL result = TRUE;
	CUMLEntityContainer* objs = GetUMLContainer();
	CString package = GetPackage();
	CString deftitle;
	deftitle.LoadString( IDS_UML_PACKAGE );
	if( objs && title.GetLength() && title != deftitle )
	{
		if( objs->PackageExists( title, this ) )
		{
			AfxMessageBox( IDS_UML_PACKAGE_EXISTS );
			result = FALSE;;
		}
		else
		{
			CString oldtitle = GetTitle();
			if( title != oldtitle )
				SetTitle( title );
		}
	}

	return result;

}

CString CUMLEntityPackage::Export( UINT format ) const
/* ============================================================
	Function :		CUMLEntityPackage::Export
	Description :	Exports this object to the desired format.
	Access :		Public
					
	Return :		CString		-	Result
	Parameters :	UINT format	-	Format to export to
					
	Usage :			"format" can be one of the following:
						"EXPORT_CPP" Export to cpp-files
						"EXPORT_H" Export to header files
						"EXPORT_HTML" Export to HTML-files

   ============================================================*/
{

	CString result;

	switch( format )
	{
		case EXPORT_HTML:
			result = ExportHTML();
			break;
	}

	return result;

}

CString CUMLEntityPackage::ExportHTML() const
/* ============================================================
	Function :		CUMLEntityPackage::ExportHTML
	Description :	Exports this object to HTML
	Access :		Private

	Return :		CString	-	Resulting HTML
	Parameters :	none

	Usage :			Called from the export-functionality.

   ============================================================*/
{

	CString result;
	CRect rect = GetRect();

	int font_size = round( 12.0 * GetZoom() );
	int cut = round( static_cast< double >( GetMarkerSize().cx ) * GetZoom() );

	CString color = ColorrefToString( GetBkColor() );

	result.Format( _T( "<div style='position:absolute;left:%i;top:%i;width:%i;height:%i;border:1px solid black;background-color:#%s;'>&nbsp;</div><div style='position:absolute;left:%i;top:%i;width:%i;height:%i;border:1px solid black;background-color:#%s;text-align:center;font-family:%s;font-size:%i;overflow:hidden;'><b>%s</b></div>" ),
		rect.left + cut, rect.top, cut * 10, cut * 2, color, rect.left, rect.top + cut * 2, rect.Width(), rect.Height() - cut * 2, color, GetFont(), font_size, GetTitle() );

	return result;

}
