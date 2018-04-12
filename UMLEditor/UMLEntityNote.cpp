/* ==========================================================================
	Class :			CUMLEntityNote

	Author :		Johan Rosengren, Abstrakt Mekanik AB

	Date :			2004-06-21

	Purpose :		"CUMLEntityNote" is derived from "CUMLEntity" and 
					represents an UML note object.

	Description :	The "CUMLEntity"-derived objects are the drawing objects 
					of the UML-editor. The class represents an UNL note entity,
					and can be linked to other objects from all sides. The links 
					will automatically be set as a weak dependency (dashed line) 
					by the editor.

	Usage :			When loading, create with "CUMLControlFactory::CreateFromString". 
					Otherwise, create instances in the application view or dialog and 
					add them to the editor with "StartDrawingObject". The 
					editor takes responsibility for the object.

   ========================================================================*/
#include "stdafx.h"
#include "UMLEntityNote.h"
#include "DiagramEditor/Tokenizer.h"
#include "UMLEntityContainer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CUMLEntityNote::CUMLEntityNote()
/* ============================================================
	Function :		CUMLEntityNote::CUMLEntityNote
	Description :	Constructor
	Access :		Public

	Return :		void
	Parameters :	none

	Usage :			Create either through "CUMLControlFactory" or
					by calling "new" and adding the pointer to the
					editor via "StartDrawingObject"

   ============================================================*/
{

	SetDefaultSize( CSize( 96, 64 ) );

	CString title;
	title.LoadString( IDS_UML_NOTE );
	SetTitle( title );

	SetType( _T( "uml_note" ) );
	SetConstraints( GetDefaultSize(), CSize( -1, -1 ) );

	SetBkColor( RGB( 223,255,223 ) );

	SetPropertyDialog( &m_dlg, CUMLNotePropertyDialog::IDD );

}

CUMLEntityNote::~CUMLEntityNote()
/* ============================================================
	Function :		CUMLEntityNote::~CUMLEntityNote
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

CDiagramEntity* CUMLEntityNote::Clone()
/* ============================================================
	Function :		CUMLEntityNote::Clone
	Description :	Creates a new object of this type, copies 
					the data from this object, and returns the 
					new one.
	Access :		Public

	Return :		CDiagramEntity*	-	New object
	Parameters :	none

	Usage :			Call to clone this object

   ============================================================*/
{

	CUMLEntityNote* obj = new CUMLEntityNote;
	obj->Copy( this );
	return obj;

}

CDiagramEntity* CUMLEntityNote::CreateFromString( const CString& str )
/* ============================================================
	Function :		CUMLEntityNote::CreateFromString
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

	CUMLEntityNote* obj = new CUMLEntityNote;
	if(!obj->FromString( str ) )
	{
		delete obj;
		obj = NULL;
	}

	return obj;

}

void CUMLEntityNote::Draw( CDC* dc, CRect rect )
/* ============================================================
	Function :		CUMLEntityNote::Draw
	Description :	Draws this object.
	Access :		Public

	Return :		void
	Parameters :	CDC* dc		-	"CDC" to draw to
					CRect rect	-	Rectangle to draw to
					
	Usage :			Call to draw the object.

   ============================================================*/
{

	int cutoff = round( static_cast< double >( GetMarkerSize().cx ) * GetZoom() );

	dc->SelectStockObject( BLACK_PEN );
	CBrush bk;
	bk.CreateSolidBrush( GetBkColor() );
	dc->SelectObject( &bk );

	POINT pts[ 5 ];

	pts[ 0 ].x = rect.left;
	pts[ 0 ].y = rect.top;

	pts[ 1 ].x = rect.right - cutoff;
	pts[ 1 ].y = rect.top;

	pts[ 2 ].x = rect.right;
	pts[ 2 ].y = rect.top + cutoff;

	pts[ 3 ].x = rect.right;
	pts[ 3 ].y = rect.bottom;

	pts[ 4 ].x = rect.left;
	pts[ 4 ].y = rect.bottom;

	dc->Polygon( pts, 5 );

	dc->MoveTo( rect.right - cutoff, rect.top );
	dc->LineTo( rect.right - cutoff, rect.top + cutoff );
	dc->LineTo( rect.right, rect.top + cutoff );

	if( GetTitle().GetLength() )
	{
		CRect textRect = GetTextRect( rect );

		CFont font;
		dc->SetBkMode( OPAQUE );
		dc->SetBkColor( GetBkColor() );
		font.CreateFont( -round( 12.0 * GetZoom() ), 0,0,0,FW_NORMAL,0,0,0,0,0,0,0,0, GetFont() );
		CFont* oldfont = dc->SelectObject( &font );
		dc->DrawText( GetTitle(), textRect, DT_WORDBREAK );
		dc->SelectObject( oldfont );
	}

	dc->SelectStockObject( WHITE_BRUSH );

}

void CUMLEntityNote::SetTitle( CString title )
/* ============================================================
	Function :		CUMLEntityNote::SetTitle
	Description :	Sets the title of the object
	Access :		Public

	Return :		void
	Parameters :	CString title	-	New title
					
	Usage :			The function also recalculates the minimum 
					restraints based on the note contents.

   ============================================================*/
{

	CDiagramEntity::SetTitle( title );
	CalcRestraints();

}

CRect CUMLEntityNote::GetTextRect(  const CRect& inrect ) const
/* ============================================================
	Function :		CUMLEntityNote::GetTextRect
	Description :	Gets a rectangle deflated for text input.
	Access :		Private

	Return :		CRect				-	Resulting rectangle
	Parameters :	const CRect& inrect	-	Rectangle to put text in
					
	Usage :			Call to get a properly deflated text 
					rectangle.

   ============================================================*/
{

	CRect rect( inrect );

	int cutoff = round( static_cast< double >( GetMarkerSize().cx ) * GetZoom() );
	rect.InflateRect( -( cutoff / 2 ), -( cutoff / 2 ) );
	rect.top += cutoff / 2 + 1;

	return rect;

}

void CUMLEntityNote::SetRect( CRect rect )
/* ============================================================
	Function :		CUMLEntityNote::SetRect
	Description :	Sets the object rectangle
	Access :		Public

	Return :		void
	Parameters :	CRect rect	-	New rectangle
					
	Usage :			Overridden to calculate restraints

   ============================================================*/
{

	CDiagramEntity::SetRect( rect );
	CalcRestraints();

}

void CUMLEntityNote::SetRect( double left, double top, double right, double bottom )
/* ============================================================
	Function :		CUMLEntityNote::SetRect
	Description :	Sets the object rectangle
	Access :		Public

	Return :		void
	Parameters :	double left		-	New left position
					double top		-	New top position
					double right	-	New right position
					double bottom	-	New bottom position
					
	Usage :			Overridden to calculate restraints

   ============================================================*/
{

	CDiagramEntity::SetRect( left, top, right, bottom );
	CalcRestraints();

}

void CUMLEntityNote::CalcRestraints()
/* ============================================================
	Function :		CUMLEntityNote::CalcRestraints
	Description :	Calculates, depending on the contents, the 
					minimum size for this object.
	Access :		Public

	Return :		void
	Parameters :	none

	Usage :			Call when the contents change.

   ============================================================*/
{

	CString title = GetTitle();
	if( title.GetLength() )
	{

		CDC* dc = CWnd::GetDesktopWindow()->GetDC();

		CRect rect = GetRect();
		CRect textRect = GetTextRect( rect );
		CRect newRect( textRect );
		CFont font;
		font.CreateFont( -12, 0,0,0,FW_NORMAL,0,0,0,0,0,0,0,0, GetFont() );
		CFont* oldfont = dc->SelectObject( &font );
		dc->DrawText( GetTitle(), newRect, DT_WORDBREAK | DT_CALCRECT );
		dc->SelectObject( oldfont );
		CWnd::GetDesktopWindow()->ReleaseDC( dc );

		int diff = newRect.Height() - textRect.Height();
		rect.bottom += diff;
		int totalheight = max( GetMinimumSize().cy, rect.Height() );
		int totalwidth = GetMinimumSize().cx;
		CUMLEntityContainer* container = GetUMLContainer();
		if( container )
		{
			CSize sz = container->CalcMinimumRestraints( GetName() );
			if( sz.cx > totalheight )
				totalheight = sz.cx;
			if( sz.cy > totalwidth )
				totalwidth = sz.cy;
		}

		int steps = round( static_cast< double >( totalheight ) / 8 );
		totalheight = steps * 8;
		steps = round( static_cast< double >( totalwidth ) / 8 );
		totalwidth = steps * 8;

		if( GetMinimumSize().cy != totalheight || GetMinimumSize().cx != totalwidth )
		{
			SetMinimumSize( CSize( totalwidth, totalheight ) );
			CRect rc = GetRect();
			if( rc.Height() < totalheight || rc.Width() < totalwidth )
			{
				totalheight = max( totalheight, rc.Height() );
				totalwidth = max( totalwidth, rc.Width() );
				SetRect( GetLeft(), GetTop(), GetLeft() + totalwidth, GetTop() + totalheight );
				if( container )
					container->AdjustLinkedObjects( this );
			}
		}
		else
			SetMinimumSize( GetDefaultSize() );

	}

}

CString CUMLEntityNote::Export( UINT format ) const
/* ============================================================
	Function :		CUMLEntityNote::Export
	Description :	Exports the obect to "format".
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

CString CUMLEntityNote::ExportHTML() const
/* ============================================================
	Function :		CUMLEntityNote::ExportHTML
	Description :	Exports the current object to HTML
	Access :		Private

	Return :		CString	-	Resulting HTML
	Parameters :	none

	Usage :			Call to export this object to HTML

   ============================================================*/
{

	CString result;
	CRect rect = GetRect();

	int font_size = 12;

	CString color = ColorrefToString( GetBkColor() );

	CString title = GetTitle();
	title.Replace( _T( "<" ), _T( "&lt;" ) );
	title.Replace( _T( ">" ), _T( "&gt;" ) );
	title.Replace( _T( "\r\n" ), _T( "<br>" ) );

	result.Format( _T( "<div style='position:absolute;left:%i;top:%i;width:%i;height:%i;border:1px solid black;background-color:#%s;'><div style='position:absolute;left:%i;top:-1;width:9:height:9;background-image:url(\"images/note.gif\");background-repeat:no-repeat;'>&nbsp;&nbsp;&nbsp;&nbsp;</div><div style='position:absolute;left:1;top:10;width:%i;height:%i;font-size:%i;font-family:%s;text-align:left;overflow:hidden;'>%s</div></div>" ),
		rect.left,rect.top,rect.Width(),rect.Height(),color,rect.Width()-10,rect.Width()-2,rect.Height()-11,font_size, GetFont(), title );

	return result;

}
