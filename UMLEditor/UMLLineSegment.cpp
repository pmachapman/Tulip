/* ==========================================================================
	Class :			CUMLLineSegment

	Author :		Johan Rosengren, Abstrakt Mekanik AB

	Date :			2004-04-29

	Purpose :		"CUMLLineSegment" is a "CUMLEntity"-
					derived class, representing a line that can be linked 
					to other "CUMLEntity"-derived objects.

	Description :	The implementation is based on "CDiagramLine", even though 
					not derived from it. The class allows two link point, 
					"LINK_START" and "LINK_END" - the top-left and bottom-right 
					corners. 
					The links can be attached to any link point on some 
					objects, and contains offset values to the top or left 
					to make it possible to move the segment along with the 
					linked object.

	Usage :			Create with "CUMLControlFactory::CreateFromString" or 
					add an instance to the editor "StartDrawingObject".

   ========================================================================
	Changes :
		8/7 2004	Support for the Interface line type added.
		5/8 2004	Fixed bug in GetOffset - zoom should *not* be applied.
   ========================================================================
		28/8 2004	Not calling CDiagramEntity::SetRect when setting rects, 
					to keep non-normalization.
   ========================================================================*/
#include "stdafx.h"
#include "UMLLineSegment.h"
#include "UMLEntityContainer.h"
#include "DiagramEditor/DiagramLine.h"
#include "DiagramEditor/Tokenizer.h"
#include "StringHelpers.h"

#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////
// LineDDA callbacks from CDiagramLine
//

VOID CALLBACK HitTest( int X, int Y, LPARAM data );
VOID CALLBACK HitTestRect( int X, int Y, LPARAM data );

CUMLLineSegment::CUMLLineSegment()
/* ============================================================
	Function :		CUMLLineSegment::CUMLLineSegment
	Description :	Constructor
	Access :		Public
					
	Return :		void
	Parameters :	none

	Usage :			Create either through "CUMLControlFactory" or
					by calling "new" and adding the pointer to the
					editor via "StartDrawingObject"

   ============================================================*/
{

	SetPropertyDialog( &m_dlg, CUMLLinkPropertyDialog::IDD );

	SetLinkType( LINK_START, 0 );
	SetLinkType( LINK_END, 0 );

	SetMinimumSize( CSize( -1, -1 ) );
	SetMaximumSize( CSize( -1, -1 ) );
	SetType( _T( "uml_line" ) );
	SetTitle( _T( "" ) );
	SetStyle( 0 );
	SetStartLabel( _T( "" ) );
	SetEndLabel( _T( "" ) );

	SetOffset( LINK_START, 0 );
	SetOffset( LINK_END, 0 );

}

CUMLLineSegment::~CUMLLineSegment()
/* ============================================================
	Function :		CUMLLineSegment::~CUMLLineSegment
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

CDiagramEntity* CUMLLineSegment::Clone()
/* ============================================================
	Function :		CUMLLineSegment::Clone
	Description :	Clone this object to a new object.
	Access :		Public
					
	Return :		CDiagramEntity*	-	The new object.
	Parameters :	none

	Usage :			Call to create a clone of the object. The 
					caller will have to delete the object.

   ============================================================*/
{

	CUMLLineSegment* obj = new CUMLLineSegment;
	obj->Copy( this );
	return obj;

}

void CUMLLineSegment::Draw( CDC* dc, CRect rect )
/* ============================================================
	Function :		CUMLLineSegment::Draw
	Description :	Draws the object.
	Access :		Public

	Return :		void
	Parameters :	CDC* dc		-	The "CDC" to draw to. 
					CRect rect	-	The real rectangle of the 
									object.

	Usage :			The function should clean up all selected 
					objects. Note that the "CDC" is a memory "CDC", 
					so creating a memory "CDC" in this function 
					will probably not speed up the function.

   ============================================================*/
{

	int mode = dc->SetBkMode( TRANSPARENT );

	if( !( GetStyle() & STYLE_INVISIBLE ) )
	{
		CPen pen;
		if( GetStyle() & STYLE_DASHED )
			pen.CreatePen( PS_DOT, 0, RGB( 0, 0, 0 ) );
		else
			pen.CreatePen( PS_SOLID, 0, RGB( 0, 0, 0 ) );

		CPen* oldpen = dc->SelectObject( &pen );

		// Draw line
		dc->MoveTo( rect.TopLeft() );
		dc->LineTo( rect.BottomRight() );

		dc->SelectObject( oldpen );
	}

	dc->SelectStockObject( BLACK_PEN );
	dc->SelectStockObject( BLACK_BRUSH );

	int cut = round( static_cast< double >( GetMarkerSize().cx ) * GetZoom() / 2 );
	int cy = round( 14.0 * GetZoom() );

	CFont font;
	font.CreateFont( -round( 12.0 * GetZoom() ), 0,0,0,FW_NORMAL,0,0,0,0,0,0,0,0, GetFont() );
	CFont* oldfont = dc->SelectObject( &font );

	/////////////////////////////////////////////////////////////
	// Draw title

	CString str = GetTitle();
	if( str.GetLength() )
	{

		CRect rectTemp( rect );
		rectTemp.NormalizeRect();
		CRect r( rect.right - cut, rect.top, rect.right - ( rectTemp.Width() + cut ), rect.bottom );
		if( IsHorizontal() )
		{
			CRect r( rect.left, rect.top - ( cy + cut ), rect.right, rect.bottom );
			r.NormalizeRect();
			dc->DrawText( str, r, DT_NOPREFIX | DT_SINGLELINE | DT_CENTER );
		}
		else
		{
			CRect r( rect.right - ( dc->GetTextExtent( str ).cx + cut * 2 ), rect.top, rect.right - cut, rect.bottom );
			r.NormalizeRect();
			dc->DrawText( str, r, DT_NOPREFIX | DT_SINGLELINE | DT_VCENTER | DT_RIGHT );
		}

	}

	/////////////////////////////////////////////////////////////
	// Draw labels

	str = GetStartLabel();
	if( str.GetLength() )
	{
		CRect rectTemp( rect );
		if( IsHorizontal() )
		{
			rectTemp.bottom = rectTemp.top - cut;
			rectTemp.top -= cy + cut;

			if( rectTemp.left < rectTemp.right )
			{
				rectTemp.left += cut;
				rectTemp.right -= cut;
				dc->DrawText( str, rectTemp, DT_NOPREFIX | DT_SINGLELINE | DT_VCENTER | DT_LEFT );
			}
			else
			{
				int temp = rectTemp.left;
				rectTemp.left = rectTemp.right + cut;
				rectTemp.right = temp - cut;
				dc->DrawText( str, rectTemp, DT_NOPREFIX | DT_SINGLELINE | DT_VCENTER | DT_RIGHT );
			}

		}
		else
		{
			rectTemp.left -= dc->GetTextExtent( str ).cx + 2 * cut;
			rectTemp.right -= cut;
			if( rectTemp.top < rectTemp.bottom )
				rectTemp.bottom = rectTemp.top + cy;
			else
			{
				rectTemp.top -= cy;
				rectTemp.bottom = rectTemp.top + cy;
			}

			dc->DrawText( str, rectTemp, DT_NOPREFIX | DT_SINGLELINE | DT_VCENTER | DT_RIGHT );
		}

	}

	str = GetSecondaryStartLabel();
	if( str.GetLength() )
	{
		CRect rectTemp( rect );
		if( IsHorizontal() )
		{
			rectTemp.bottom += cy + cut;
			rectTemp.top += cut;

			if( rectTemp.left < rectTemp.right )
			{
				rectTemp.left += cut;
				rectTemp.right -= cut;
				dc->DrawText( str, rectTemp, DT_NOPREFIX | DT_SINGLELINE | DT_VCENTER | DT_LEFT );
			}
			else
			{
				int temp = rectTemp.left;
				rectTemp.left = rectTemp.right + cut;
				rectTemp.right = temp - cut;
				dc->DrawText( str, rectTemp, DT_NOPREFIX | DT_SINGLELINE | DT_VCENTER | DT_RIGHT );
			}

		}
		else
		{
			rectTemp.right += dc->GetTextExtent( str ).cx + 2 * cut;
			rectTemp.left += cut;

			if( rectTemp.top < rectTemp.bottom )
				rectTemp.bottom = rectTemp.top + cy;
			else
			{
				rectTemp.top -= cy;
				rectTemp.bottom = rectTemp.top + cy;
			}

			dc->DrawText( str, rectTemp, DT_NOPREFIX | DT_SINGLELINE | DT_VCENTER | DT_RIGHT );
		}

	}

	str = GetEndLabel();
	if( str.GetLength() )
	{
		CRect rectTemp( rect );
		if( IsHorizontal() )
		{
			rectTemp.bottom = rectTemp.top - cut;
			rectTemp.top -= cy + cut;

			if( rectTemp.left < rectTemp.right )
			{
				rectTemp.left += cut;
				rectTemp.right -= cut;
				dc->DrawText( str, rectTemp, DT_NOPREFIX | DT_SINGLELINE | DT_VCENTER | DT_RIGHT );
			}
			else
			{
				int temp = rectTemp.left;
				rectTemp.left = rectTemp.right + cut;
				rectTemp.right = temp - cut;
				dc->DrawText( str, rectTemp, DT_NOPREFIX | DT_SINGLELINE | DT_VCENTER | DT_LEFT );
			}

		}
		else
		{
			rectTemp.left -= dc->GetTextExtent( str ).cx + 2 * cut;
			rectTemp.right -= cut;
			if( rectTemp.top < rectTemp.bottom )
				rectTemp.top = rectTemp.bottom - cy;
			else
			{
				rectTemp.top = rectTemp.bottom;
				rectTemp.bottom = rectTemp.top + cy;
			}
			dc->DrawText( str, rectTemp, DT_NOPREFIX | DT_SINGLELINE | DT_VCENTER | DT_RIGHT );
		}
	}

	str = GetSecondaryEndLabel();
	if( str.GetLength() )
	{
		CRect rectTemp( rect );
		if( IsHorizontal() )
		{
			rectTemp.bottom += cy + cut;
			rectTemp.top += cut;

			if( rectTemp.left < rectTemp.right )
			{
				rectTemp.left += cut;
				rectTemp.right -= cut;
				dc->DrawText( str, rectTemp, DT_NOPREFIX | DT_SINGLELINE | DT_VCENTER | DT_RIGHT );
			}
			else
			{
				int temp = rectTemp.left;
				rectTemp.left = rectTemp.right + cut;
				rectTemp.right = temp - cut;
				dc->DrawText( str, rectTemp, DT_NOPREFIX | DT_SINGLELINE | DT_VCENTER | DT_LEFT );
			}

		}
		else
		{
			rectTemp.right += dc->GetTextExtent( str ).cx + 2 * cut;
			rectTemp.left += cut;
			if( rectTemp.top < rectTemp.bottom )
				rectTemp.top = rectTemp.bottom - cy;
			else
			{
				rectTemp.top = rectTemp.bottom;
				rectTemp.bottom = rectTemp.top + cy;
			}
			dc->DrawText( str, rectTemp, DT_NOPREFIX | DT_SINGLELINE | DT_VCENTER | DT_RIGHT );
		}
	}

	/////////////////////////////////////////////////////////////
	// Draw markers

	int hgt = GetMarkerSize().cy / 2;
	if( GetStyle() & STYLE_FILLED_DIAMOND )
	{
		CRect diamond;

		double x2 = GetLeft();
		double x1 = GetRight();
		double y2 = GetTop();
		double y1 = GetBottom();

		if( !( GetLinkType( LINK_END ) & LINK_ALL ) && 
			( GetLinkType( LINK_START ) & LINK_ALL ) )
		{
			x2 = GetRight();
			x1 = GetLeft();
			y2 = GetBottom();
			y1 = GetTop();
		}

		if( IsHorizontal() )
		{
			diamond.left = round( x1 * GetZoom() );
			diamond.top = round( ( y1 - hgt ) * GetZoom() );
			diamond.bottom = diamond.top + round( hgt * 2 * GetZoom() );
			if( x1 < x2 )
				diamond.right = diamond.left + round( hgt * 4 * GetZoom() );
			else
				diamond.right = diamond.left - round( hgt * 4 * GetZoom() );

			diamond.NormalizeRect();

		}
		else
		{

			diamond.top = round( y1 * GetZoom() );
			diamond.left = round( ( x1 - hgt ) * GetZoom() );
			diamond.right = diamond.left + round( hgt * 2 * GetZoom() );
			if( y1 < y2 )
				diamond.bottom = diamond.top + round( hgt * 4 * GetZoom() );
			else
				diamond.bottom = diamond.top - round( hgt * 4 * GetZoom() );

			rect.NormalizeRect();

		}

		DrawDiamond( dc, diamond );
	}

	if( GetStyle() & STYLE_CIRCLECROSS )
	{

		CSize marker( 16, 16 );
		CPoint pos = GetStyleMarkerRect( LINK_END, marker );
		CRect circ( round( static_cast< double >( pos.x ) * GetZoom() ), round( static_cast< double >( pos.y ) * GetZoom() ), round( static_cast< double >( pos.x + marker.cx ) * GetZoom() ), round( static_cast< double >( pos.y + marker.cy ) * GetZoom() ) );

		dc->SelectStockObject( BLACK_PEN );
		dc->SelectStockObject( WHITE_BRUSH );

		dc->Ellipse( &circ );

		dc->MoveTo( circ.left + circ.Width() / 2, circ.top );
		dc->LineTo( circ.left + circ.Width() / 2, circ.bottom );

		dc->MoveTo( circ.left, circ.top + circ.Height() / 2 );
		dc->LineTo( circ.right, circ.top + circ.Height() / 2 );

	}

	if( GetStyle() & STYLE_ARROWHEAD )
		DrawInheritanceArrow( dc );

	if( GetStyle() & STYLE_FILLED_ARROWHEAD )
		DrawDirectionArrow( dc );

	/////////////////////////////////////////////////////////////
	// Cleaning up

	dc->SetBkMode( mode );
	dc->SelectObject( oldfont );
	dc->SelectStockObject( WHITE_BRUSH );

}

CDiagramEntity* CUMLLineSegment::CreateFromString( const CString& str )
/* ============================================================
	Function :		CUMLLineSegment::CreateFromString
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
					Each object type should have its own 
					version - the default one is a model 
					implementation.

   ============================================================*/
{

	CUMLLineSegment* obj = new CUMLLineSegment;
	if(!obj->FromString( str ) )
	{
		delete obj;
		obj = NULL;
	}

	return obj;

}

int CUMLLineSegment::GetHitCode( CPoint point ) const
/* ============================================================
	Function :		CUMLLineSegment::GetHitCode
	Description :	Returns the hit point constant for point.
	Access :		Public
					
	Return :		int				-	The hit point, 
										"DEHT_NONE" if none.
	Parameters :	CPoint point	-	The point to check
					
	Usage :			Call to see in what part of the object point 
					lies. The hit point can be one of the following:
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

	int result = DEHT_NONE;

	CRect rect = GetRect();

	hitParams hit;
	hit.hit = FALSE;
	hit.x = point.x;
	hit.y = point.y;

	LineDDA(	static_cast< int >( GetLeft() ), 
				static_cast< int >( GetTop() ), 
				static_cast< int >( GetRight() ), 
				static_cast< int >( GetBottom() ), 
				HitTest, 
				(LPARAM) &hit );

	if( hit.hit )
		result = DEHT_BODY;

	CRect rectTest;

	rectTest = GetSelectionMarkerRect( DEHT_TOPLEFT, rect );
	if( rectTest.PtInRect( point ) )
		result = DEHT_TOPLEFT;

	rectTest = GetSelectionMarkerRect( DEHT_BOTTOMRIGHT, rect );
	if( rectTest.PtInRect( point ) )
		result = DEHT_BOTTOMRIGHT;

	return result;

}

HCURSOR CUMLLineSegment::GetCursor( int hit ) const
/* ============================================================
	Function :		CUMLLineSegment::GetCursor
	Description :	Returns the cursor for the given hit point.
	Access :		Public
					
	Return :		HCURSOR	-	The cursor to show
	Parameters :	int hit	-	The hit point constant ("DEHT_") 
								to get the cursor for.

	Usage :			Call to get the cursor for a specific hit 
					point constant.
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

	HCURSOR cursor = NULL;
	switch( hit )
	{
		case DEHT_BODY:
		case DEHT_TOPLEFT:
		case DEHT_BOTTOMRIGHT:
		cursor = LoadCursor( NULL, IDC_SIZEALL );
		break;
	}

	return cursor;

}

void CUMLLineSegment::DrawSelectionMarkers( CDC* dc, CRect rect ) const
/* ============================================================
	Function :		CUMLLineSegment::DrawSelectionMarkers
	Description :	Draws the selection markers for the 
					object.
	Access :		Public
					
	Return :		void
	Parameters :	CDC* dc		-	The "CDC" to draw to
					CRect rect	-	The real object rectangle.
					
	Usage :			"rect" is the true rectangle (zoomed) of the 
					object.

   ============================================================*/
{

	CRect rectSelect;

	dc->SelectStockObject( BLACK_PEN );
	dc->SelectStockObject( BLACK_BRUSH );

	rectSelect = GetSelectionMarkerRect( DEHT_TOPLEFT, rect );
	dc->Rectangle( rectSelect );

	rectSelect = GetSelectionMarkerRect( DEHT_BOTTOMRIGHT, rect );
	dc->Rectangle( rectSelect );


}

void CUMLLineSegment::SetRect( CRect rect )
/* ============================================================
	Function :		CUMLLineSegment::SetRect
	Description :	Sets the rect of the object.
	Access :		Public
					
	Return :		void
	Parameters :	CRect rect	-	Rectangle to set
					
	Usage :			Overriden to avoid normalization and force 
					the line to be orthogonal.

   ============================================================*/
{

	if( GetLeft() - GetRight() != 0 || GetTop() - GetBottom() != 0 )
	{
		if( IsHorizontal() )
		{
			if( rect.top == static_cast< int >( GetTop() ) )
				rect.bottom = rect.top;
			else
				rect.top = rect.bottom;
		}
		else
		{
			if( rect.left == static_cast< int >( GetLeft() ) )
				rect.right = rect.left;
			else
				rect.left = rect.right;
		}
	}

	SetLeft( rect.left );
	SetTop( rect.top );
	SetRight( rect.right );
	SetBottom( rect.bottom );

}

BOOL CUMLLineSegment::BodyInRect( CRect rect ) const
/* ============================================================
	Function :		CUMLLineSegment::BodyInRect
	Description :	Used to see if any part of the object lies 
					in rect.
	Access :		Public
					
	Return :		BOOL		-	"TRUE" if any part of the 
									object lies inside rect.
	Parameters :	CRect rect	-	The rect to check.
					
	Usage :			Call to see if the object overlaps - for 
					example - a selection rubberband.

   ============================================================*/
{

	BOOL result = FALSE;
	hitParamsRect hit;
	hit.rect = rect;
	hit.hit = FALSE;

	LineDDA(	static_cast< int >( GetLeft() ), 
				static_cast< int >( GetTop() ), 
				static_cast< int >( GetRight() ), 
				static_cast< int >( GetBottom() ), 
				HitTestRect, 
				( LPARAM ) &hit );

	if( hit.hit )
		result = TRUE;

	return result;

}

CPoint CUMLLineSegment::GetLinkPosition( int type ) const
/* ============================================================
	Function :		CUMLLineSegment::GetLinkPosition
	Description :	Returns the position of a link.
	Access :		Public
					
	Return :		CPoint		-	The position of the link,
									-1, -1 if the link is not
									allowed.
	Parameters :	int type	-	The type of the link.
					
	Usage :			The possible link types are:
					"LINK_START" Links are allowed to the start of a line (the top-left corner).
					"LINK_END" Links are allowed to the end of a line (the bottom-right corner).

   ============================================================*/
{

	CPoint point( -1, -1 );
	CRect rect = GetRect();

	switch( type )
	{
		case LINK_START:
			point.x = rect.left;
			point.y = rect.top;
			break;
		case LINK_END:
			point.x = rect.right;
			point.y = rect.bottom;
			break;
	}

	return point;

}

void CUMLLineSegment::SetStyle( int style )
/* ============================================================
	Function :		CUMLLineSegment::SetStyle
	Description :	Sets the style for this object.
	Access :		Public

	Return :		void
	Parameters :	int style	-	New style.
					
	Usage :			"style" can be one of the following:
						"STYLE_NONE" Black line
						"STYLE_ARROWHEAD" White arrowhead
						"STYLE_FILLED_ARROWHEAD" Black arrowhead
						"STYLE_CIRCLECROSS" A circle with a cross
						"STYLE_FILLED_DIAMOND" Black diamond
						"STYLE_DASHED" Dashed line
						"STYLE_INVISIBLE" Invisible line

   ============================================================*/
{

	m_style = style;

}

void CUMLLineSegment::SetLineStyle( int style )
/* ============================================================
	Function :		CUMLLineSegment::SetLineStyle
	Description :	Clears the line style and sets "style" to 
					the complete line.
	Access :		Public
					
	Return :		void
	Parameters :	int style	-	Style to set
					
	Usage :			Call to set a style to the complete line.
					"style" can be one of the following:
						"STYLE_NONE" Black line
						"STYLE_ARROWHEAD" White arrowhead
						"STYLE_FILLED_ARROWHEAD" Black arrowhead
						"STYLE_CIRCLECROSS" A circle with a cross
						"STYLE_FILLED_DIAMOND" Black diamond
						"STYLE_DASHED" Dashed line
						"STYLE_INVISIBLE" Invisible line

   ============================================================*/
{

	CUMLEntityContainer* objs = GetUMLContainer();
	if( objs )
	{

		CUMLLineSegment* start = objs->GetStartSegment( this );
		while( start )
		{
			start->m_style = STYLE_NONE;
			start = objs->GetNextSegment( start );
		}

		AddLineStyle( style );

	}
	else
		m_style = style;

}

void CUMLLineSegment::AddLineStyle( int style )
/* ============================================================
	Function :		CUMLLineSegment::AddLineStyle
	Description :	Adds a style to the line.
	Access :		Public
					
	Return :		void
	Parameters :	int style	-	Style to add.
					
	Usage :			Adds the style to the complete line or the 
					appropriate start or end segment.
					"style" can be one of the following:
						"STYLE_NONE" Black line
						"STYLE_ARROWHEAD" White arrowhead
						"STYLE_FILLED_ARROWHEAD" Black arrowhead
						"STYLE_CIRCLECROSS" A circle with a cross
						"STYLE_FILLED_DIAMOND" Black diamond
						"STYLE_DASHED" Dashed line
						"STYLE_INVISIBLE" Invisible line
   ============================================================*/
{

	CUMLEntityContainer* objs = GetUMLContainer();
	if( objs )
	{

		if( style & STYLE_FILLED_ARROWHEAD )
		{
			CUMLLineSegment* start = objs->GetStartSegment( this );
			start->m_style |= style;
		}

		if( style & STYLE_FILLED_DIAMOND || 
			style & STYLE_ARROWHEAD ||
			style & STYLE_CIRCLECROSS )
		{
			CUMLLineSegment* end = objs->GetEndSegment( this );
			end->m_style |= style;
		}

		if( style & STYLE_DASHED || style & STYLE_INVISIBLE )
		{
			CUMLLineSegment* start = objs->GetStartSegment( this );
			while( start )
			{
				start->m_style |= style;
				start = objs->GetNextSegment( start );
			}
		}
	}
	else
		m_style |= style;

}

void CUMLLineSegment::RemoveLineStyle( int style )
/* ============================================================
	Function :		CUMLLineSegment::RemoveLineStyle
	Description :	Removes a style from this and all other 
					segments of the line.
	Access :		Public
					
	Return :		void
	Parameters :	int style	-	Style to remove
					
	Usage :			Call to remove a style from the complete line.
					"style" can be one of the following:
						"STYLE_NONE" Black line
						"STYLE_ARROWHEAD" White arrowhead
						"STYLE_FILLED_ARROWHEAD" Black arrowhead
						"STYLE_CIRCLECROSS" A circle with a cross
						"STYLE_FILLED_DIAMOND" Black diamond
						"STYLE_DASHED" Dashed line
						"STYLE_INVISIBLE" Invisible line
   ============================================================*/
{

	CUMLEntityContainer* objs = GetUMLContainer();
	if( objs )
	{
		if( style & STYLE_FILLED_DIAMOND ||
			style & STYLE_FILLED_ARROWHEAD )
		{
			CUMLLineSegment* start = objs->GetStartSegment( this );
			start->m_style &= ~style;
		}
		if( style & STYLE_ARROWHEAD ||
			style & STYLE_CIRCLECROSS )
		{
			CUMLLineSegment* end = objs->GetEndSegment( this );
			end->m_style &= ~style;
		}

		if( style & STYLE_DASHED || style & STYLE_INVISIBLE )
		{
			CUMLLineSegment* start = objs->GetStartSegment( this );
			while( start )
			{
				start->m_style &= ~style;
				start = objs->GetNextSegment( start );
			}
		}
	}
	else
		m_style &= ~style;

}

int CUMLLineSegment::GetStyle() const
/* ============================================================
	Function :		CUMLLineSegment::GetStyle
	Description :	Gets the style of the line segment
	Access :		Public

	Return :		int	-	Style
	Parameters :	none

	Usage :			"style" can be one of the following:
						"STYLE_NONE" Black line
						"STYLE_ARROWHEAD" White arrowhead
						"STYLE_FILLED_ARROWHEAD" Black arrowhead
						"STYLE_CIRCLECROSS" A circle with a cross
						"STYLE_FILLED_DIAMOND" Black diamond
						"STYLE_DASHED" Dashed line
						"STYLE_INVISIBLE" Invisible line

   ============================================================*/
{

	return m_style;

}

BOOL CUMLLineSegment::FromString( const CString& str )
/* ============================================================
	Function :		CUMLLineSegment::FromString
	Description :	Sets the data fro this object from "str".
	Access :		Public
					
	Return :		BOOL				-	"TRUE" if str represents 
											an object of this type.
	Parameters :	const CString& str	-	String representation
					
	Usage :			Called from the load-functionality.

   ============================================================*/
{

	BOOL result = FALSE;
	CString data( str );

	if( LoadFromString( data ) )
	{

		CTokenizer tok( data );

		CString package;
		CString stereotype;

		CString startLink;
		CString endLink;

		int startLinkType;
		int endLinkType;

		CString	fontName;
		int		bkColor;

		int style;
		CString startLabel;
		CString endLabel;
		CString secondaryStartLabel;
		CString secondaryEndLabel;

		int startOffset;
		int endOffset;

		int count = 0;

		tok.GetAt( count++, package );
		tok.GetAt( count++, stereotype );
		tok.GetAt( count++, startLink );
		tok.GetAt( count++, endLink );
		tok.GetAt( count++, startLinkType );
		tok.GetAt( count++, endLinkType );
		tok.GetAt( count++, style );
		tok.GetAt( count++, fontName );
		tok.GetAt( count++, bkColor );
		tok.GetAt( count++, startLabel );
		tok.GetAt( count++, endLabel );
		tok.GetAt( count++, startOffset );
		tok.GetAt( count++, endOffset );
		tok.GetAt( count++, secondaryStartLabel );
		tok.GetAt( count++, secondaryEndLabel );

		UnmakeSaveString( startLabel );
		UnmakeSaveString( endLabel );
		UnmakeSaveString( secondaryStartLabel );
		UnmakeSaveString( secondaryEndLabel );
		UnmakeSaveString( package );
		UnmakeSaveString( stereotype );

		SetPackage( package );
		SetStereotype( stereotype );
		SetLink( LINK_START, startLink );
		SetLink( LINK_END, endLink );
		SetLinkType( LINK_START, startLinkType );
		SetLinkType( LINK_END, endLinkType );
		SetStyle( style );
		SetFont( fontName );
		SetBkColor( static_cast< COLORREF >( bkColor ) );
		SetStartLabel( startLabel );
		SetEndLabel( endLabel );
		SetOffset( LINK_START, startOffset );
		SetOffset( LINK_END, endOffset );
		SetSecondaryStartLabel( secondaryStartLabel );
		SetSecondaryEndLabel( secondaryEndLabel );

		result = TRUE;

	}

	return result;

}

CString CUMLLineSegment::GetString() const
/* ============================================================
	Function :		CUMLLineSegment::GetString
	Description :	Get the object as a string for saving.
	Access :		Public

	Return :		CString	-	Object as a string
	Parameters :	none

	Usage :			Called from the save functionality.

   ============================================================*/
{

	CString str;

	CString package = GetPackage();
	CString stereotype = GetStereotype();
	CString startLabel1 = GetStartLabel();
	CString startLabel2 = GetSecondaryStartLabel();
	CString endLabel1 = GetEndLabel();
	CString endLabel2 = GetSecondaryEndLabel();

	MakeSaveString( package );
	MakeSaveString( stereotype );
	MakeSaveString( startLabel1 );
	MakeSaveString( startLabel2 );
	MakeSaveString( endLabel1 );
	MakeSaveString( endLabel2 );

	str.Format( _T( ",%s,%s,%s,%s,%i,%i,%i,%s,%i,%s,%s,%i,%i,%s,%s;" ), 
			package,
			stereotype,
			GetLink( LINK_START ), 
			GetLink( LINK_END ), 
			GetLinkType( LINK_START ), 
			GetLinkType( LINK_END ),
			GetStyle(),
			GetFont(),
			static_cast< int >( GetBkColor() ),
			startLabel1,
			endLabel1,
			GetOffset( LINK_START ),
			GetOffset( LINK_END ),
			startLabel2,
			endLabel2
		);

	str = GetDefaultGetString() + str;
	return str;

}

void CUMLLineSegment::DrawDiamond( CDC* dc, const CRect& rect )
/* ============================================================
	Function :		CUMLLineSegment::DrawDiamond
	Description :	Draws the composition diamond.
	Access :		Private
					
	Return :		void
	Parameters :	CDC* dc				-	The "CDC" to draw to
					const CRect& rect	-	The rect of the diamond
					
	Usage :			Called from "Draw".

   ============================================================*/
{

	dc->SelectStockObject( BLACK_PEN );
	dc->SelectStockObject( BLACK_BRUSH );

	POINT pts[4];
	pts[0].x = rect.left + ( ( rect.right - rect.left ) / 2 );
	pts[0].y = rect.top;
	pts[1].x = rect.right;
	pts[1].y = rect.top + ( ( rect.bottom - rect.top ) / 2 );
	pts[2].x = pts[0].x;
	pts[2].y = rect.bottom;
	pts[3].x = rect.left;
	pts[3].y = pts[1].y;

	dc->Polygon( pts, 4 );

}

CString CUMLLineSegment::GetStartLabel() const
/* ============================================================
	Function :		CUMLLineSegment::SetSecondaryEndLabel
	Description :	Gets the start label.
	Access :		Public
					
	Return :		void
	Parameters :	const CString& label	-	The label
					
	Usage :			Gets the top or left label at the line
					start.

   ============================================================*/
{

	return m_startLabel;

}

CString CUMLLineSegment::GetEndLabel() const
/* ============================================================
	Function :		CUMLLineSegment::SetSecondaryEndLabel
	Description :	Gets the end label.
	Access :		Public
					
	Return :		void
	Parameters :	const CString& label	-	The label
					
	Usage :			Gets the top or left label at the line
					end. 

   ============================================================*/
{

	return m_endLabel;

}

CString CUMLLineSegment::GetSecondaryStartLabel() const
/* ============================================================
	Function :		CUMLLineSegment::SetSecondaryEndLabel
	Description :	Gets the secondary start label.
	Access :		Public
					
	Return :		void
	Parameters :	const CString& label	-	The label
					
	Usage :			Gets the bottom or right label at the line
					start. 

   ============================================================*/
{

	return m_secondaryStartLabel;

}

CString CUMLLineSegment::GetSecondaryEndLabel() const
/* ============================================================
	Function :		CUMLLineSegment::SetSecondaryEndLabel
	Description :	Gets the secondary end label.
	Access :		Public
					
	Return :		void
	Parameters :	const CString& label	-	The label
					
	Usage :			Gets the bottom or right label at the line
					end. 

   ============================================================*/
{

	return m_secondaryEndLabel;

}

void CUMLLineSegment::SetStartLabel( const CString& label )
/* ============================================================
	Function :		CUMLLineSegment::SetSecondaryEndLabel
	Description :	Sets the start label.
	Access :		Public
					
	Return :		void
	Parameters :	const CString& label	-	The label
					
	Usage :			Sets the top or left label at the line
					start. 

   ============================================================*/
{

	m_startLabel = label;

}

void CUMLLineSegment::SetEndLabel( const CString& label )
/* ============================================================
	Function :		CUMLLineSegment::SetSecondaryEndLabel
	Description :	Sets the end label.
	Access :		Public
					
	Return :		void
	Parameters :	const CString& label	-	The label
					
	Usage :			Sets the top or left label at the line
					end. 

   ============================================================*/
{

	m_endLabel = label;

}

void CUMLLineSegment::SetSecondaryStartLabel( const CString& label )
/* ============================================================
	Function :		CUMLLineSegment::SetSecondaryEndLabel
	Description :	Sets the secondary start label.
	Access :		Public
					
	Return :		void
	Parameters :	const CString& label	-	The label
					
	Usage :			Sets the bottom or right label at the line
					start. 

   ============================================================*/
{

	m_secondaryStartLabel = label;

}

void CUMLLineSegment::SetSecondaryEndLabel( const CString& label )
/* ============================================================
	Function :		CUMLLineSegment::SetSecondaryEndLabel
	Description :	Sets the secondary end label.
	Access :		Public
					
	Return :		void
	Parameters :	const CString& label	-	The label
					
	Usage :			Sets the bottom or right label at the line
					end. 

   ============================================================*/
{

	m_secondaryEndLabel = label;

}

BOOL CUMLLineSegment::IsHorizontal() const
/* ============================================================
	Function :		CUMLLineSegment::IsHorizontal
	Description :	Check if the line is horizontal
	Access :		Public
					
	Return :		BOOL	-	"TRUE" if horizontal
	Parameters :	none

	Usage :			Call to see if the segment is horizontal.

   ============================================================*/
{

	return ( GetTop() == GetBottom() );

}

void CUMLLineSegment::SetRect( double left, double top, double right, double bottom )
/* ============================================================
	Function :		CUMLLineSegment::SetRect
	Description :	Sets the object rectangle.
	Access :		Public
					
	Return :		void
	Parameters :	double left		-	Left pos
					double top		-	Top pos
					double right	-	Right pos
					double bottom	-	Bottom pos.
					
	Usage :			Overridden to force the line to be 
					orthogonal.

   ============================================================*/
{

	if( GetLeft() - GetRight() != 0 || GetTop() - GetBottom() != 0 )
	{
		if( IsHorizontal() )
		{
			if( top == GetTop() )
				bottom = top;
			else
				top = bottom;
		}
		else
		{
			if( left == GetLeft() )
				right = left;
			else
				left = right;
		}
	}

	SetTop( top );
	SetLeft( left );
	SetRight( right );
	SetBottom( bottom );

}

void CUMLLineSegment::Copy( CDiagramEntity* obj )
/* ============================================================
	Function :		CUMLLineSegment::Copy
	Description :	Copies from obj to this
	Access :		Public
					
	Return :		void
	Parameters :	CDiagramEntity* obj	-	Object to copy from
					
	Usage :			Overridden to copy the rest of the attributes.

   ============================================================*/
{

	CUMLEntity::Copy( obj );

	CUMLLineSegment* uml = dynamic_cast< CUMLLineSegment* >( obj );
	if( uml )
	{

		SetStyle( uml->GetStyle() );
		SetStartLabel( uml->GetStartLabel() );
		SetEndLabel( uml->GetEndLabel() );
		SetOffset( LINK_START, uml->GetOffset( LINK_START ) );
		SetOffset( LINK_END, uml->GetOffset( LINK_END ) );
		SetLink( LINK_START, uml->GetLink( LINK_START ) );
		SetLink( LINK_END, uml->GetLink( LINK_END ) );
		SetLinkType( LINK_START, uml->GetLinkType( LINK_START ) );
		SetLinkType( LINK_END, uml->GetLinkType( LINK_END ) );

	}

}

int CUMLLineSegment::GetLineStyle() const
/* ============================================================
	Function :		CUMLLineSegment::GetLineStyle
	Description :	Returns the style for this and all the 
					other segments making up this line.
	Access :		Public
					
	Return :		int		-	The line style.
	Parameters :	none

	Usage :			Call to get the line style. The line style can be ORed from the following:
						"STYLE_NONE" No special style
						"STYLE_ARROWHEAD" Inheritance (empty arrowhead)
						"STYLE_FILLED_ARROWHEAD" Direction (filled arrowhead)
						"STYLE_CIRCLECROSS" Part-of circle (circle with cross)
						"STYLE_FILLED_DIAMOND" Composition (filled diamond)
						"STYLE_DASHED" Dependency (dashed line)
						"STYLE_INVISIBLE" Invisible (to connect objects)


   ============================================================*/
{

	int result = m_style;
	CUMLEntityContainer* objs = GetUMLContainer();
	if( objs )
	{

		CUMLLineSegment* start = objs->GetStartSegment( ( CUMLLineSegment *const ) this );
		CUMLLineSegment* end = objs->GetEndSegment( ( CUMLLineSegment *const ) this );

		result |= start->GetStyle();
		result |= end->GetStyle();

	}

	return result;

}

void CUMLLineSegment::SetLink( int type, const CString& name )
/* ============================================================
	Function :		CUMLLineSegment::SetLink
	Description :	Set that name of the object that is 
					attached to the start or end of this line 
					segment.
	Access :		Public
					
	Return :		void
	Parameters :	int type			-	"LINK_START" or "LINK_END"
					const CString& name	-	Name of object
					
	Usage :			Call to attach an object to this line segment.

   ============================================================*/
{

	switch( type )
	{
		case LINK_START:
			m_start = name;
			break;
		case LINK_END:
			m_end = name;
			break;
	}

}

CString CUMLLineSegment::GetLink( int type ) const
/* ============================================================
	Function :		CUMLLineSegment::GetLink
	Description :	Get the name of the object attached to the 
					start or end of this line segment.
	Access :		Public
					
	Return :		CString		-	Name of object, or empty if 
									none.
	Parameters :	int type	-	"LINK_START" or "LINK_END"
					
	Usage :			Call to get the name of the object attached 
					to either end of the line segment.

   ============================================================*/
{

	CString result;

	switch( type )
	{
		case LINK_START:
			result = m_start;
			break;
		case LINK_END:
			result = m_end;
			break;
	}

	return result;

}

void CUMLLineSegment::SetLinkType( int type, int targetType )
/* ============================================================
	Function :		CUMLLineSegment::SetLinkType
	Description :	Set the link type for the start or end of 
					the line segment.
	Access :		Public
					
	Return :		void
	Parameters :	int type		-	"LINK_START" or "LINK_END"
					int targetType	-	The type of the link on 
										the object this line 
										segment will be attached 
										to.
					
	Usage :			Call to set a new link type for the line 
					segment start or end. The possible link 
					types for "targetType" are:
						"LINK_TOP" Link at the top
						"LINK_BOTTOM" Link at the bottom
						"LINK_LEFT" Link at the left
						"LINK_RIGHT" Link at the right

   ============================================================*/
{

	switch( type )
	{
		case LINK_START:
			m_startType = targetType;
			break;
		case LINK_END:
			m_endType = targetType;
			break;
	}

}

int CUMLLineSegment::GetLinkType( int type ) const
/* ============================================================
	Function :		CUMLLineSegment::GetLinkType
	Description :	Returns the type of link the start or end 
					point is attached to.
	Access :		Public
					
	Return :		int			-	Any of the "LINK_"-constants
	Parameters :	int type	-	"LINK_START" or "LINK_END" for 
									the start or end point 
									respectively.
					
	Usage :			Call to get the type of link the start or 
					end of the line is attached to.The possible 
					link types are:
						"LINK_TOP" Link at the top
						"LINK_BOTTOM" Link at the bottom
						"LINK_LEFT" Link at the left
						"LINK_RIGHT" Link at the right
						"LINK_START" Link at the start point of a line
						"LINK_END" Link at the end point of a line.

   ============================================================*/
{

	int result = LINK_NONE;

	switch( type )
	{
		case LINK_START:
			result = m_startType;
			break;
		case LINK_END:
			result = m_endType;
			break;
	}

	return result;

}

int CUMLLineSegment::GetOffset( int type) const
/* ============================================================
	Function :		CUMLLineSegment::GetOffset
	Description :	Returns the offset for a line end point.
	Access :		Public
					
	Return :		int			-	Offset to the top or left.
	Parameters :	int type	-	Type of point - "LINK_START" 
									or "LINK_END" for the start 
									and end respectively.
					
	Usage :			As lines can be attached anywhere on an 
					object side, the offset from the top or left 
					side is saved for the two end points.

   ============================================================*/
{

	int result;

	if( type == LINK_START )
		result = m_startOffset;
	else
		result = m_endOffset;

	return result;

}

void CUMLLineSegment::SetOffset( int type, int linkOffset )
/* ============================================================
	Function :		CUMLLineSegment::SetOffset
	Description :	Sets the offset to the x- or y-position on 
					the object this line is linked to.
	Access :		Public
					
	Return :		void
	Parameters :	int type		-	"LINK_START" or "LINK_END" 
										for the appropriate end 
										point of the line.
					int linkOffset	-	Offset on the side (from 
										the top or left).
					
	Usage :			As lines can be attached anywhere on an 
					object side, the offset from the top or left 
					side is saved for the two end points.

   ============================================================*/
{

	if( type == LINK_START )
		m_startOffset = linkOffset;
	else
		m_endOffset = linkOffset;

}

int CUMLLineSegment::GetLinkCode( CPoint point ) const
/* ============================================================
	Function :		CUMLLineSegment::GetLinkCode
	Description :	Get the link code for point.
	Access :		Public
					
	Return :		int				-	Link code
	Parameters :	CPoint point	-	The point to check
					
	Usage :			Call to hit-test the object. The function 
					returns "LINK_START" for the start of the 
					line, "LINK_END" for the end.

   ============================================================*/
{

	int result = LINK_NONE;

	CRect rectTest;
	rectTest = GetLinkMarkerRect( LINK_START );
	if( rectTest.PtInRect( point ) )
		result = LINK_START;

	rectTest = GetLinkMarkerRect( LINK_END );
	if( rectTest.PtInRect( point ) )
		result = LINK_END;

	return result;

}

CString CUMLLineSegment::Export( UINT format ) const
/* ============================================================
	Function :		CUMLLineSegment::Export
	Description :	Exports this object to the desired format.
	Access :		Public
					
	Return :		CString		-	Result
	Parameters :	UINT format	-	Format to export to
					
	Usage :			Called from the editor/container to export 
					objects. "format" can be one of the following:
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
		case EXPORT_CPP:
			break;
		case EXPORT_H:
			break;
	}

	return result;

}

void CUMLLineSegment::Flip()
/* ============================================================
	Function :		CUMLLineSegment::Flip
	Description :	Flip the direction of the link
	Access :		Public
					
	Return :		void
	Parameters :	none

	Usage :			Called from flip-commands in the editor/
					container.

   ============================================================*/
{

	CString tempString;
	int tempInt;

	tempString = GetLink( LINK_END );
	SetLink( LINK_END, GetLink( LINK_START ) );
	SetLink( LINK_START, tempString );

	tempInt = GetLinkType( LINK_END );
	SetLinkType( LINK_END, GetLinkType( LINK_START ) );
	SetLinkType( LINK_START, tempInt );

	tempInt = GetOffset( LINK_END );
	SetOffset( LINK_END, GetOffset( LINK_START ) );
	SetOffset( LINK_START, tempInt );

	CRect rect = GetRect();
	tempInt = rect.right;
	rect.right = rect.left;
	rect.left = tempInt;

	tempInt = rect.bottom;
	rect.bottom = rect.top;
	rect.top = tempInt;

	SetRect( rect );

}

BOOL CUMLLineSegment::IsSingleLineSegment() const
/* ============================================================
	Function :		CUMLLineSegment::IsSingleLineSegment
	Description :	Check if the line is a single-segment line
	Access :		Public
					
	Return :		BOOL	-	"TRUE" if single-segment
	Parameters :	none

	Usage :			Call to see if this line consists of more 
					segments.

   ============================================================*/
{

	BOOL result = FALSE;

	if( ( GetLinkType( LINK_START ) & LINK_ALL ) && 
		( GetLinkType( LINK_END ) & LINK_ALL ) ) 
		result = TRUE;

	return result;

}

void CUMLLineSegment::DrawDirectionArrow( CDC* dc )
/* ============================================================
	Function :		CUMLLineSegment::DrawDirectionArrow
	Description :	Draws the single direction arrow to the 
					screen.
	Access :		Private
					
	Return :		void
	Parameters :	CDC* dc	-	"CDC" to draw to
					
	Usage :			Called from the "Draw" function

   ============================================================*/
{
	int hgt = GetMarkerSize().cy / 2;

	double x2 = GetLeft();
	double x1 = GetRight();
	double y2 = GetTop();
	double y1 = GetBottom();

	if( !( GetLinkType( LINK_START ) & LINK_ALL ) && 
		( GetLinkType( LINK_END ) & LINK_ALL ) )
	{
		x2 = GetRight();
		x1 = GetLeft();
		y2 = GetBottom();
		y1 = GetTop();
	}

	POINT pts[ 3 ];
	if( IsHorizontal() )
	{
		if( x1 < x2 )
		{
			pts[ 0 ].x = round( ( x2 - hgt * 4 ) * GetZoom() );
			pts[ 0 ].y = round( ( y1 - hgt ) * GetZoom() );
			pts[ 1 ].x = round( x2 * GetZoom() );
			pts[ 1 ].y = round( y1 * GetZoom() );
			pts[ 2 ].x = pts[ 0 ].x;
			pts[ 2 ].y = round( ( y1 + hgt ) * GetZoom() );
		}
		else
		{
			pts[ 0 ].x = round( ( x2 + hgt * 4 ) * GetZoom() );
			pts[ 0 ].y = round( ( y1 - hgt ) * GetZoom() );
			pts[ 1 ].x = round( x2 * GetZoom() );
			pts[ 1 ].y = round( y1 * GetZoom() );
			pts[ 2 ].x = pts[ 0 ].x;
			pts[ 2 ].y = round( ( y1 + hgt ) * GetZoom() );
		}
	}
	else
	{
		if( y1 < y2 )
		{
			pts[ 0 ].x = round( ( x2 - hgt ) * GetZoom() );
			pts[ 0 ].y = round( ( y2 - hgt * 4 ) * GetZoom() );

			pts[ 1 ].x = round( x2 * GetZoom() );
			pts[ 1 ].y = round( y2 * GetZoom() );

			pts[ 2 ].x = round( ( x2 + hgt ) * GetZoom() );
			pts[ 2 ].y = pts[ 0 ].y;
		}
		else
		{
			pts[ 0 ].x = round( ( x2 - hgt ) * GetZoom() );
			pts[ 0 ].y = round( ( y2 + hgt * 4 ) * GetZoom() );

			pts[ 1 ].x = round( x2 * GetZoom() );
			pts[ 1 ].y = round( y2 * GetZoom() );

			pts[ 2 ].x = round( ( x2 + hgt ) * GetZoom() );
			pts[ 2 ].y = pts[ 0 ].y;
		}
	}

	dc->SelectStockObject( BLACK_PEN );
	dc->SelectStockObject( BLACK_BRUSH );

	dc->Polygon( pts, 3 );

}

void CUMLLineSegment::DrawInheritanceArrow( CDC* dc )
/* ============================================================
	Function :		CUMLLineSegment::DrawInheritanceArrow
	Description :	Draws the inheritance arrow to the screen
	Access :		Private
					
	Return :		void
	Parameters :	CDC* dc	-	"CDC" to draw to
					
	Usage :			Called from the "Draw" function.

   ============================================================*/
{

	int hgt = GetMarkerSize().cy / 2;

	double x1 = GetLeft();
	double x2 = GetRight();
	double y1 = GetTop();
	double y2 = GetBottom();

	if( ( GetLinkType( LINK_START ) & LINK_ALL ) && 
		!( GetLinkType( LINK_END ) & LINK_ALL ) )
	{
		x1 = GetRight();
		x2 = GetLeft();
		y1 = GetBottom();
		y2 = GetTop();
	}

	POINT pts[ 3 ];
	if( IsHorizontal() )
	{
		if( x1 < x2 )
		{
			pts[ 0 ].x = round( ( x2 - hgt * 4 ) * GetZoom() );
			pts[ 0 ].y = round( ( y1 - hgt ) * GetZoom() );
			pts[ 1 ].x = round( x2 * GetZoom() );
			pts[ 1 ].y = round( y1 * GetZoom() );
			pts[ 2 ].x = pts[ 0 ].x;
			pts[ 2 ].y = round( ( y1 + hgt ) * GetZoom() );
		}
		else
		{
			pts[ 0 ].x = round( ( x2 + hgt * 4 ) * GetZoom() );
			pts[ 0 ].y = round( ( y1 - hgt ) * GetZoom() );
			pts[ 1 ].x = round( x2 * GetZoom() );
			pts[ 1 ].y = round( y1 * GetZoom() );
			pts[ 2 ].x = pts[ 0 ].x;
			pts[ 2 ].y = round( ( y1 + hgt ) * GetZoom() );
		}
	}
	else
	{
		if( y1 < y2 )
		{
			pts[ 0 ].x = round( ( x2 - hgt ) * GetZoom() );
			pts[ 0 ].y = round( ( y2 - hgt * 4 ) * GetZoom() );

			pts[ 1 ].x = round( x2 * GetZoom() );
			pts[ 1 ].y = round( y2 * GetZoom() );

			pts[ 2 ].x = round( ( x2 + hgt ) * GetZoom() );
			pts[ 2 ].y = pts[ 0 ].y;
		}
		else
		{
			pts[ 0 ].x = round( ( x2 - hgt ) * GetZoom() );
			pts[ 0 ].y = round( ( y2 + hgt * 4 ) * GetZoom() );

			pts[ 1 ].x = round( x2 * GetZoom() );
			pts[ 1 ].y = round( y2 * GetZoom() );

			pts[ 2 ].x = round( ( x2 + hgt ) * GetZoom() );
			pts[ 2 ].y = pts[ 0 ].y;
		}
	}

	dc->SelectStockObject( BLACK_PEN );
	dc->SelectStockObject( WHITE_BRUSH );

	dc->Polygon( pts, 3 );

}

CString CUMLLineSegment::ExportHTML() const
/* ============================================================
	Function :		CUMLLineSegment::ExportHTML
	Description :	Exports this object to HTML
	Access :		Private
					
	Return :		CString	-	The HTML result
	Parameters :	none

	Usage :			Called from the export-mechanism

   ============================================================*/
{

	CString result;
	if( !( GetStyle() & STYLE_INVISIBLE ) )
	{

		CRect	rect = GetRect();
		int style = GetStyle();

		rect.NormalizeRect();
		int width = max( 1, rect.Width() );
		int height = max( 1, rect.Height() );

		CString linetype ( _T( "solid" ) );
		if( style & STYLE_DASHED )
			linetype = _T( "dashed" );

		CString orientation( _T( "left" ) );
		if( IsHorizontal() )
			orientation = _T( "top" );

		CString line;
		line.Format( _T( "border-%s:1px %s black" ), orientation, linetype );

		result.Format( _T( "<div style='position:absolute;left:%i;top:%i;width:%i;height:%i;%s;'>&nbsp;</div>" ),
							rect.left,rect.top,width,height,line);

		if( GetStyle() & STYLE_ARROWHEAD )
			result += GetArrowHeadHTML();

		if( GetStyle() & STYLE_CIRCLECROSS )
			result += GetCircleCrossHTML();

		if( GetStyle() & STYLE_FILLED_ARROWHEAD )
			result += GetFilledArrowHeadHTML();

		if( GetStyle() & STYLE_FILLED_DIAMOND )
			result += GetFilledDiamondHTML();

		// Label
		CString label;
		CDC* dc = CWnd::GetDesktopWindow()->GetDC();
		CFont font;
		font.CreateFont( -12, 0,0,0,FW_NORMAL,0,0,0,0,0,0,0,0, GetFont() );
		CFont* oldfont = dc->SelectObject( &font );
		CString str = GetTitle();
		int cut = GetMarkerSize().cx / 2;
		if( str.GetLength() )
		{
			CRect rectTemp = GetRect();
			rectTemp.NormalizeRect();
			CRect r( rect.right - cut, rect.top, rect.right - ( rectTemp.Width() + cut ), rect.bottom );
			if( IsHorizontal() )
			{
				CRect r( rect.left, rect.top - ( 14 + cut ), rect.right, rect.bottom );
				r.NormalizeRect();
				label.Format( _T( "<div style='position:absolute;left:%i;top:%i;width:%i;height:%i;font-family:%s;font-size:12;background-color:transparent;text-align:center;'>%s</div>" ), 
					r.left, r.top, r.Width(), r.Height(), GetFont(), str );
				result += label;
			}
			else
			{
				CRect r( rect.right - ( dc->GetTextExtent( str ).cx + cut * 2 ), rect.top, rect.right - cut, rect.bottom );
				r.NormalizeRect();
				r.top += r.Height() / 2 - 7;
				r.bottom = r.top + 14;
				label.Format( _T( "<div style='position:absolute;left:%i;top:%i;width:%i;height:%i;font-family:%s;font-size:12;background-color:transparent;text-align:right;'>%s</div>" ), 
					r.left, r.top, r.Width(), r.Height(), GetFont(), str );
				result += label;
			}

		}

		str = GetStartLabel();
		if( str.GetLength() )
		{
			CRect rectTemp = GetRect();
			if( IsHorizontal() )
			{
				rectTemp.bottom = rectTemp.top - cut;
				rectTemp.top -= 14 + cut;

				if( rectTemp.left < rectTemp.right )
				{
					rectTemp.left += cut;
					rectTemp.right -= cut;
					label.Format( _T( "<div style='position:absolute;left:%i;top:%i;width:%i;height:%i;font-family:%s;font-size:12;background-color:transparent;text-align:right;'>%s</div>" ), 
						rectTemp.left, rectTemp.top, rectTemp.Width(), rectTemp.Height(), GetFont(), str );
					result += label;
				}
				else
				{
					int temp = rectTemp.left;
					rectTemp.left = rectTemp.right + cut;
					rectTemp.right = temp - cut;
					label.Format( _T( "<div style='position:absolute;left:%i;top:%i;width:%i;height:%i;font-family:%s;font-size:12;background-color:transparent;text-align:left;'>%s</div>" ), 
						rectTemp.left, rectTemp.top, rectTemp.Width(), rectTemp.Height(), GetFont(), str );
					result += label;
				}

			}
			else
			{
				rectTemp.left -= dc->GetTextExtent( str ).cx + 2 * cut;
				rectTemp.right -= cut;
				if( rectTemp.top < rectTemp.bottom )
					rectTemp.bottom = rectTemp.top + 14;
				else
				{
					rectTemp.top -= 14;
					rectTemp.bottom = rectTemp.top + 14;
				}

				label.Format( _T( "<div style='position:absolute;left:%i;top:%i;width:%i;height:%i;font-family:%s;font-size:12;background-color:transparent;text-align:right;'>%s</div>" ), 
					rectTemp.left, rectTemp.top, rectTemp.Width(), rectTemp.Height(), GetFont(), str );
				result += label;
			}

		}

		str = GetSecondaryStartLabel();
		if( str.GetLength() )
		{
			CRect rectTemp = GetRect();
			if( IsHorizontal() )
			{
				rectTemp.bottom += 14 + cut;
				rectTemp.top += cut;

				if( rectTemp.left < rectTemp.right )
				{
					rectTemp.left += cut;
					rectTemp.right -= cut;
					label.Format( _T( "<div style='position:absolute;left:%i;top:%i;width:%i;height:%i;font-family:%s;font-size:12;background-color:transparent;text-align:right;'>%s</div>" ), 
						rectTemp.left, rectTemp.top, rectTemp.Width(), rectTemp.Height(), GetFont(), str );
					result += label;
				}
				else
				{
					int temp = rectTemp.left;
					rectTemp.left = rectTemp.right + cut;
					rectTemp.right = temp - cut;
					label.Format( _T( "<div style='position:absolute;left:%i;top:%i;width:%i;height:%i;font-family:%s;font-size:12;background-color:transparent;text-align:left;'>%s</div>" ), 
						rectTemp.left, rectTemp.top, rectTemp.Width(), rectTemp.Height(), GetFont(), str );
					result += label;
				}

			}
			else
			{
				rectTemp.right += dc->GetTextExtent( str ).cx + 2 * cut;
				rectTemp.left += cut;

				if( rectTemp.top < rectTemp.bottom )
					rectTemp.bottom = rectTemp.top + 14;
				else
				{
					rectTemp.top -= 14;
					rectTemp.bottom = rectTemp.top + 14;
				}

				label.Format( _T( "<div style='position:absolute;left:%i;top:%i;width:%i;height:%i;font-family:%s;font-size:12;background-color:transparent;text-align:right;'>%s</div>" ), 
					rectTemp.left, rectTemp.top, rectTemp.Width(), rectTemp.Height(), GetFont(), str );
				result += label;
			}

		}

		str = GetEndLabel();
		if( str.GetLength() )
		{
			CRect rectTemp = GetRect();
			if( IsHorizontal() )
			{
				rectTemp.bottom = rectTemp.top - cut;
				rectTemp.top -= 14 + cut;

				if( rectTemp.left < rectTemp.right )
				{
					rectTemp.left += cut;
					rectTemp.right -= cut;
					label.Format( _T( "<div style='position:absolute;left:%i;top:%i;width:%i;height:%i;font-family:%s;font-size:12;background-color:transparent;text-align:left;'>%s</div>" ), 
						rectTemp.left, rectTemp.top, rectTemp.Width(), rectTemp.Height(), GetFont(), str );
					result += label;
				}
				else
				{
					int temp = rectTemp.left;
					rectTemp.left = rectTemp.right + cut;
					rectTemp.right = temp - cut;
					label.Format( _T( "<div style='position:absolute;left:%i;top:%i;width:%i;height:%i;font-family:%s;font-size:12;background-color:transparent;text-align:right;'>%s</div>" ), 
						rectTemp.left, rectTemp.top, rectTemp.Width(), rectTemp.Height(), GetFont(), str );
					result += label;
				}

			}
			else
			{
				rectTemp.left -= dc->GetTextExtent( str ).cx + 2 * cut;
				rectTemp.right -= cut;
				if( rectTemp.top < rectTemp.bottom )
					rectTemp.top = rectTemp.bottom - 14;
				else
				{
					rectTemp.top = rectTemp.bottom;
					rectTemp.bottom = rectTemp.top + 14;
				}
				label.Format( _T( "<div style='position:absolute;left:%i;top:%i;width:%i;height:%i;font-family:%s;font-size:12;background-color:transparent;text-align:right;'>%s</div>" ), 
					rectTemp.left, rectTemp.top, rectTemp.Width(), rectTemp.Height(), GetFont(), str );
				result += label;
			}
		}

		str = GetSecondaryEndLabel();
		if( str.GetLength() )
		{
			CRect rectTemp = GetRect();
			if( IsHorizontal() )
			{
				rectTemp.bottom += 14 + cut;
				rectTemp.top += cut;

				if( rectTemp.left < rectTemp.right )
				{
					rectTemp.left += cut;
					rectTemp.right -= cut;
					label.Format( _T( "<div style='position:absolute;left:%i;top:%i;width:%i;height:%i;font-family:%s;font-size:12;background-color:transparent;text-align:left;'>%s</div>" ), 
						rectTemp.left, rectTemp.top, rectTemp.Width(), rectTemp.Height(), GetFont(), str );
					result += label;
				}
				else
				{
					int temp = rectTemp.left;
					rectTemp.left = rectTemp.right + cut;
					rectTemp.right = temp - cut;
					label.Format( _T( "<div style='position:absolute;left:%i;top:%i;width:%i;height:%i;font-family:%s;font-size:12;background-color:transparent;text-align:right;'>%s</div>" ), 
						rectTemp.left, rectTemp.top, rectTemp.Width(), rectTemp.Height(), GetFont(), str );
					result += label;
				}

			}
			else
			{
				rectTemp.right += dc->GetTextExtent( str ).cx + 2 * cut;
				rectTemp.left += cut;
				if( rectTemp.top < rectTemp.bottom )
					rectTemp.top = rectTemp.bottom - 14;
				else
				{
					rectTemp.top = rectTemp.bottom;
					rectTemp.bottom = rectTemp.top + 14;
				}
				label.Format( _T( "<div style='position:absolute;left:%i;top:%i;width:%i;height:%i;font-family:%s;font-size:12;background-color:transparent;text-align:right;'>%s</div>" ), 
					rectTemp.left, rectTemp.top, rectTemp.Width(), rectTemp.Height(), GetFont(), str );
				result += label;
			}
		}

		dc->SelectObject( oldfont );
		CWnd::GetDesktopWindow()->ReleaseDC( dc );
	}

	return result;

}

CString CUMLLineSegment::GetArrowHeadHTML() const
/* ============================================================
	Function :		CUMLLineSegment::GetArrowHeadHTML
	Description :	Gets a string representing the inheritance
					arrow picture for HTML-exports.
	Access :		Private
					
	Return :		CString	-	The string in HTML-format.
	Parameters :	none

	Usage :			Called as a part of the export mechanism.

   ============================================================*/
{

	BOOL invert = TRUE;
	if( !( GetLinkType( LINK_END ) & LINK_ALL ) && 
		( GetLinkType( LINK_START ) & LINK_ALL ) )
		invert = FALSE;

	CString img;
	CRect marker = GetRect();

	int width = 0;
	int height = 0;

	CSize size( 17, 9 );
	if( IsHorizontal() )
	{
		width = size.cx;
		height = size.cy;
		if( !invert )
		{
			if( marker.right > marker.left )
				img = _T( "images/larrow.gif" );
			else
				img = _T( "images/rarrow.gif" );
		}
		else
		{
			if( marker.right < marker.left )
				img = _T( "images/larrow.gif" );
			else
				img = _T( "images/rarrow.gif" );
		}
	}
	else
	{
		width = size.cy;
		height = size.cx;
		if( !invert )
		{
			if( marker.bottom > marker.top )
				img = _T( "images/uarrow.gif" );
			else
				img = _T( "images/darrow.gif" );
		}
		else
		{
			if( marker.bottom < marker.top )
				img = _T( "images/uarrow.gif" );
			else
				img = _T( "images/darrow.gif" );
		}
	}

	CPoint pos = GetStyleMarkerRect( LINK_END, size );

	CString arrow;
	arrow.Format( _T( "<div style='position:absolute;left:%i;top:%i;width:%i;height:%i;background-image:url(\"%s\");background-repeat:no-repeat;'>&nbsp;</div>" ),
		pos.x, pos.y, width, height, img );

	return arrow;

}

CString CUMLLineSegment::GetFilledArrowHeadHTML() const
/* ============================================================
	Function :		CUMLLineSegment::GetFilledArrowHeadHTML
	Description :	Gets a string representing the single-direction  
					arrow picture for HTML-exports.
	Access :		Private
					
	Return :		CString	-	The string in HTML-format.
	Parameters :	none

	Usage :			Called as a part of the export mechanism.

   ============================================================*/
{

	BOOL invert = FALSE;
	if( !( GetLinkType( LINK_START ) & LINK_ALL ) && 
		( GetLinkType( LINK_END ) & LINK_ALL ) )
		invert = TRUE;

	CString img;
	CRect marker = GetRect();

	int width = 0;
	int height = 0;

	CSize size( 17, 9 );
	if( IsHorizontal() )
	{
		width = size.cx;
		height = size.cy;
		if( !invert )
		{
			if( marker.right < marker.left )
				img = _T( "images/rfarrow.gif" );
			else
				img = _T( "images/lfarrow.gif" );
		}
		else
		{
			if( marker.right > marker.left )
				img = _T( "images/rfarrow.gif" );
			else
				img = _T( "images/lfarrow.gif" );
		}
	}
	else
	{
		width = size.cy;
		height = size.cx;
		if( !invert )
		{
			if( marker.bottom < marker.top )
				img = _T( "images/dfarrow.gif" );
			else
				img = _T( "images/ufarrow.gif" );
		}
		else
		{
			if( marker.bottom > marker.top )
				img = _T( "images/dfarrow.gif" );
			else
				img = _T( "images/ufarrow.gif" );
		}
	}

	CPoint pos = GetStyleMarkerRect( LINK_START, size );

	CString arrow;
	arrow.Format( _T( "<div style='position:absolute;left:%i;top:%i;width:%i;height:%i;background-image:url(\"%s\");background-repeat:no-repeat;'>&nbsp;</div>" ),
		pos.x, pos.y, width, height, img );

	return arrow;

}

CString CUMLLineSegment::GetFilledDiamondHTML() const
/* ============================================================
	Function :		CUMLLineSegment::GetFilledDiamondHTML
	Description :	Gets a string representing the containment 
					picture for HTML-exports.
	Access :		Private
					
	Return :		CString	-	The string in HTML-format.
	Parameters :	none

	Usage :			Called as a part of the export mechanism.

   ============================================================*/
{

	CString img;

	int width = 0;
	int height = 0;

	CSize size( 16, 9 );
	if( IsHorizontal() )
	{
		width = size.cx;
		height = size.cy;
		img = _T( "images/lrdiamond.gif" );
	}
	else
	{
		width = size.cy;
		height = size.cx;
		img = _T( "images/uddiamond.gif" );
	}

	CPoint pos = GetStyleMarkerRect( LINK_END, size );

	CString arrow;
	arrow.Format( _T( "<div style='position:absolute;left:%i;top:%i;width:%i;height:%i;background-image:url(\"%s\");background-repeat:no-repeat;'>&nbsp;</div>" ),
		pos.x, pos.y, width, height, img );

	return arrow;

}

CString CUMLLineSegment::GetCircleCrossHTML() const
/* ============================================================
	Function :		CUMLLineSegment::GetCircleCrossHTML
	Description :	Gets a string representing the part-of 
					picture for HTML-exports.
	Access :		Private
					
	Return :		CString	-	The string in HTML-format.
	Parameters :	none

	Usage :			Called as a part of the export mechanism.

   ============================================================*/
{

	CString img( _T( "images/partof.gif" ) );

	CSize marker( 16, 16 );
	CPoint pos = GetStyleMarkerRect( LINK_END, marker );

	CString arrow;
	arrow.Format( _T( "<div style='position:absolute;left:%i;top:%i;width:%i;height:%i;background-image:url(\"%s\");background-repeat:no-repeat;'>&nbsp;</div>" ),
		pos.x, pos.y, marker.cx, marker.cy, img );

	return arrow;

}

CPoint CUMLLineSegment::GetStyleMarkerRect( int node, const CSize& size ) const
/* ============================================================
	Function :		CUMLLineSegment::GetStyleMarkerRect
	Description :	The function returns a the start point for 
					a rectangle with the size "size" for either 
					the start or end of a line.
	Access :		Private
					
	Return :		CPoint				-	The start point
	Parameters :	int node			-	Either "LINK_START" 
											or "LINK_END" for the 
											start or end of the 
											line.
					const CSize& size	-	The size of the 
											rectangle.
					
	Usage :			Call to get the position of a rectangle to 
					draw markers at either the start or the end 
					of a line segment.

   ============================================================*/
{

	CPoint result( 0, 0 );

	int invers = LINK_START;
	if( node == LINK_START )
		invers = LINK_END;

	// We try to find what end of the segment is 
	// really the start/end, that is the end where 
	// a non-line object is attached.
	BOOL invert = FALSE;
	if( !( GetLinkType( node ) & LINK_ALL ) && 
		( GetLinkType( invers ) & LINK_ALL ) )
		invert = TRUE;

	CRect marker = GetRect();

	int width = 0;
	int height = 0;
	int left = 0;
	int top = 0;

	if( node == LINK_END )
	{
		if( IsHorizontal() )
		{
			width = size.cx;
			height = size.cy;
			top = marker.top - height / 2;
			if( !invert )
			{
				if( marker.right < marker.left )
					left = marker.right;
				else
					left = marker.right - width;
			}
			else
			{
				if( marker.right > marker.left )
					left = marker.left;
				else
					left = marker.left - width;
			}
		}
		else
		{
			width = size.cy;
			height = size.cx;
			left = marker.left - width / 2;
			if( !invert )
			{
				if( marker.bottom < marker.top )
					top = marker.bottom;
				else
					top = marker.bottom - height;
			}
			else
			{
				if( marker.bottom > marker.top )
					top = marker.top;
				else
					top = marker.top - height;
			}
		}

		result.x = left;
		result.y = top;

	}
	else
	{
		if( IsHorizontal() )
		{
			width = size.cx;
			height = size.cy;
			top = marker.top - height / 2;
			if( !invert )
			{
				if( marker.right < marker.left )
					left = marker.left - width;
				else
					left = marker.left;
			}
			else
			{
				if( marker.right > marker.left )
					left = marker.right - width;
				else
					left = marker.right;
			}
		}
		else
		{
			width = size.cy;
			height = size.cx;
			left = marker.left - width / 2;
			if( !invert )
			{
				if( marker.bottom < marker.top )
					top = marker.top - height;
				else
					top = marker.top;
			}
			else
			{
				if( marker.bottom > marker.top )
					top = marker.bottom - height;
				else
					top = marker.bottom;
			}
		}

		result.x = left;
		result.y = top;

	}

	return result;

}

BOOL CUMLLineSegment::DoMessage( UINT msg, CDiagramEntity* sender, CWnd* from )
/* ============================================================
	Function :		CUMLLineSegment::DoMessage
	Description :	Receives a message from the editor.
	Access :		Public
					
	Return :		BOOL					-	"TRUE" to stop 
												processing.
	Parameters :	UINT msg				-	App or editor 
												defined message.
					CDiagramEntity* sender	-	The objects sending 
												this message, if any.
					CWnd* from				-	The editor window.
					
	Usage :			Overridden to handle of the Flip-link 
					command.

   ============================================================*/
{

	BOOL result = TRUE;

	if( msg == CMD_FLIP )
		GetUMLContainer()->FlipLink();
	else
		result = CUMLEntity::DoMessage( msg, sender, from );

	return result;

}

void CUMLLineSegment::ShowPopup( CPoint point, CWnd* parent )
/* ============================================================
	Function :		CUMLLineSegment::ShowPopup
	Description :	Displays an object-specific popup menu.
	Access :		Public
					
	Return :		void
	Parameters :	CPoint point	-	position of popup
					CWnd* parent	-	parent window of popup
					
	Usage :			Called from the editor when the line is 
					right-clicked.

   ============================================================*/
{

	CMenu menu;
	menu.LoadMenu( IDR_UML_MENU_LINK );
	menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, parent );

}
