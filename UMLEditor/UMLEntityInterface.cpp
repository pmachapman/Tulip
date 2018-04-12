/* ==========================================================================
	Class :			CUMLEntityInterface

	Author :		Johan Rosengren, Abstrakt Mekanik AB

	Date :			2004-06-21

	Purpose :		"CUMLEntityInterface" is derived from "CUMLEntity" and 
					represents an UML interface object.

	Description :	The "CUMLEntity"-derived objects are the drawing objects 
					of the UML-editor. "CUMLEntityInterface" is linkable at 
					the top, left, right and bottom middle points. A 
					non-selectable title is displayed above the UML-symbol 
					for the interface. The interface will currently not 
					generate any code.

	Usage :			When loading, create with "CUMLControlFactory::CreateFromString". 
					Otherwise, create instances in the application view or dialog and 
					add them to the editor with "StartDrawingObject". The 
					editor takes responsibility for the object.

   ========================================================================*/
#include "stdafx.h"
#include "UMLEntityInterface.h"
#include "DiagramEditor/Tokenizer.h"
#include "UMLEntityContainer.h"
#include "StringHelpers.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CUMLEntityInterface::CUMLEntityInterface()
/* ============================================================
	Function :		CUMLEntityInterface::CUMLEntityInterface
	Description :	Constructor
	Access :		Public

	Return :		void
	Parameters :	none

	Usage :			Create either through "CUMLControlFactory" or
					by calling "new" and adding the pointer to the
					editor via "StartDrawingObject"

   ============================================================*/
{

	SetDefaultSize( CSize( 32, 32 ) );

	CString title;
	title.LoadString( IDS_UML_INTERFACE );
	SetTitle( title );

	SetType( _T( "uml_interface" ) );
	SetConstraints( GetDefaultSize(), GetDefaultSize() );

	SetBkColor( RGB( 255, 255, 217 ) );

	SetPropertyDialog( &m_dlg, CUMLInterfacePropertyDialog::IDD );

}

CUMLEntityInterface::~CUMLEntityInterface()
/* ============================================================
	Function :		CUMLEntityInterface::~CUMLEntityInterface
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

CDiagramEntity* CUMLEntityInterface::Clone()
/* ============================================================
	Function :		CUMLEntityInterface::Clone
	Description :	Creates a new object of this type, copies 
					the data from this object, and returns the 
					new one.
	Access :		Public

	Return :		CDiagramEntity*	-	New object
	Parameters :	none

	Usage :			Call to clone this object

   ============================================================*/
{

	CUMLEntityInterface* obj = new CUMLEntityInterface;
	obj->Copy( this );
	return obj;

}

CDiagramEntity* CUMLEntityInterface::CreateFromString( const CString& str )
/* ============================================================
	Function :		CUMLEntityInterface::CreateFromString
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

	CUMLEntityInterface* obj = new CUMLEntityInterface;
	if(!obj->FromString( str ) )
	{
		delete obj;
		obj = NULL;
	}

	return obj;

}

void CUMLEntityInterface::Draw( CDC* dc, CRect rect )
/* ============================================================
	Function :		CUMLEntityInterface::Draw
	Description :	Draws this object.
	Access :		Public

	Return :		void
	Parameters :	CDC* dc		-	"CDC" to draw to
					CRect rect	-	Rectangle to draw to
					
	Usage :			Call to draw the object.

   ============================================================*/
{

	int cutoff = round( static_cast< double >( GetMarkerSize().cx ) * GetZoom() );
	int height = round( 12.0 * GetZoom() );
	dc->SelectStockObject( BLACK_PEN );
	CBrush bk;
	bk.CreateSolidBrush( GetBkColor() );
	dc->SelectObject( &bk );

	dc->Ellipse( rect );

	CString str = GetTitle();
	if( str )
	{
		CFont font;
		dc->SetBkMode( TRANSPARENT );
		font.CreateFont( -height, 0,0,0,FW_BOLD,0,0,0,0,0,0,0,0, GetFont() );
		CFont* oldfont = dc->SelectObject( &font );

		CRect textRect( rect );
		textRect.bottom = textRect.top;
		textRect.top -= round( 14.0 * GetZoom() );

		int width = dc->GetTextExtent( str ).cx + cutoff * 2;
		int diff = width - textRect.Width();
		if( diff > 0 )
		{
			textRect.left -= diff / 2;
			textRect.right += diff / 2;
		}

		dc->DrawText( str, textRect, DT_SINGLELINE | DT_CENTER );
		dc->SelectObject( oldfont );
	}

	dc->SelectStockObject( BLACK_PEN );
	dc->SelectStockObject( WHITE_BRUSH );

}

int CUMLEntityInterface::GetLinkCode( CPoint point ) const
/* ============================================================
	Function :		CUMLEntityInterface::GetLinkCode
	Description :	Returns the link code of this object for 
					the position "point".
	Access :		Public

	Return :		int				-	Link code of position
	Parameters :	CPoint point	-	Position to text
					
	Usage :			
					The possible link types are:
						"LINK_TOP" Link at the top
						"LINK_BOTTOM" Link at the bottom
						"LINK_LEFT" Link at the left
						"LINK_RIGHT" Link at the right
						"LINK_START" Link at the start point of a line
						"LINK_END" Link at the end point of a line.

   ============================================================*/
{

	int result = LINK_NONE;
	CRect rect;

	rect = GetLinkMarkerRect( LINK_LEFT );
	if( rect.PtInRect( point ) )
		result = LINK_LEFT;

	rect = GetLinkMarkerRect( LINK_TOP );
	if( rect.PtInRect( point ) )
		result = LINK_TOP;

	rect = GetLinkMarkerRect( LINK_RIGHT );
	if( rect.PtInRect( point ) )
		result = LINK_RIGHT;

	rect = GetLinkMarkerRect( LINK_BOTTOM );
	if( rect.PtInRect( point ) )
		result = LINK_BOTTOM;

	return result;

}

HCURSOR CUMLEntityInterface::GetCursor( int hit ) const
/* ============================================================
	Function :		CDiagramEntity::GetCursor
	Description :	Returns the cursor for the given hit point.
	Access :		Public
					
	Return :		HCURSOR	-	The cursor to show
	Parameters :	int hit	-	The hit point constant ("DEHT_") 
								to get the cursor for.

	Usage :			We will only return the cursor for the body.
					"hit" can be one of the following:
						"DEHT_NONE" No hit-point
						"DEHT_BODY" Inside object body
						"DEHT_TOPLEFT" Top-left corner
						"DEHT_TOPMIDDLE" Middle top-side
						"DEHT_TOPRIGHT" Top-right corner
						"DEHT_BOTTOMLEFT" Bottom-left corner
						"DEHT_BOTTOMMIDDLE" Middle bottom-side
						"DEHT_BOTTOMRIGHT" Bottom-right corner
						"DEHT_LEFTMIDDLE" Middle left-side
						"DEHT_RIGHTMIDDLE" Middle right-side

   ============================================================*/
{

	HCURSOR cursor = LoadCursor( NULL, IDC_ARROW );
	if( hit == DEHT_BODY )
		cursor = LoadCursor( NULL, IDC_SIZEALL );

	return cursor;

}

void CUMLEntityInterface::DrawSelectionMarkers( CDC* dc, CRect rect ) const
/* ============================================================
	Function :		CDiagramEntity::DrawSelectionMarkers
	Description :	Draws the selection markers for the 
					object.
	Access :		Public
					
	Return :		void
	Parameters :	CDC* dc		-	The "CDC" to draw to
					CRect rect	-	The real object rectangle.
					
	Usage :			rect is the true rectangle (zoomed) of the 
					object. We will only draw the markers in the 
					corners.

   ============================================================*/
{

	// Draw selection markers
	CRect rectSelect;

	dc->SelectStockObject( BLACK_BRUSH );
	rectSelect = GetSelectionMarkerRect( DEHT_TOPLEFT, rect );
	dc->Rectangle( rectSelect );

	rectSelect = GetSelectionMarkerRect( DEHT_TOPRIGHT, rect );
	dc->Rectangle( rectSelect );

	rectSelect = GetSelectionMarkerRect( DEHT_BOTTOMLEFT, rect );
	dc->Rectangle( rectSelect );

	rectSelect = GetSelectionMarkerRect( DEHT_BOTTOMRIGHT, rect );
	dc->Rectangle( rectSelect );

}

CString CUMLEntityInterface::Export( UINT format ) const
/* ============================================================
	Function :		CUMLEntityInterface::Export
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

	if( format == EXPORT_HTML )
	{
		CRect rect = GetRect();

		int font_size = 12;
		int cut = GetMarkerSize().cx;

		CString color = ColorrefToString( GetBkColor() );

		CRect textRect( rect );
		textRect.bottom = textRect.top;
		textRect.top -= font_size + 2;

		CDC* dc = CWnd::GetDesktopWindow()->GetDC();
		CFont font;
		font.CreateFont( -font_size, 0,0,0,FW_BOLD,0,0,0,0,0,0,0,0, GetFont() );
		CFont* oldfont = dc->SelectObject( &font );
		int width = dc->GetTextExtent( GetTitle() ).cx + cut * 2;
		dc->SelectObject( oldfont );
		CWnd::GetDesktopWindow()->ReleaseDC( dc );

		int diff = width - textRect.Width();
		if( diff > 0 )
		{
			textRect.left -= diff / 2;
			textRect.right += diff / 2;
		}

		result.Format( _T( "<div style='position:absolute;left:%i;top:%i;width:32;height:32;background-color:#%s;background-image:url(\"images/interface.gif\");background-repeat:no-repeat;'>&nbsp;</div>\n<div style='position:absolute;left:%i;top:%i;width:%i;height:%i;font-family:%s;font-size:%i;font-weight:bold;text-align:center;'>%s</div>" ),
			rect.left, rect.top, color, textRect.left, textRect.top, textRect.Width(), textRect.Height(), GetFont(), font_size, GetTitle() );
	}

	return result;

}