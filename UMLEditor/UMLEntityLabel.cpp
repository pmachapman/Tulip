/* ==========================================================================
	Class :			CUMLEntityLabel

	Author :		Johan Rosengren, Abstrakt Mekanik AB

	Date :			2004-06-21

	Purpose :		"CUMLEntityLabel" is derived from "CUMLEntity" and 
					represents an UML label.

	Description :	The "CUMLEntity"-derived objects are the drawing objects 
					of the UML-editor. The label is a transparent text label 
					that can't be linked to, with setable font name, font 
					size and font attributes.

	Usage :			When loading, create with "CUMLControlFactory::CreateFromString". 
					Otherwise, create instances in the application view or 
					dialog and add them to the editor with "StartDrawingObject". 
					The editor takes responsibility for the object.

   ========================================================================*/
#include "stdafx.h"
#include "UMLEntityLabel.h"
#include "DiagramEditor/Tokenizer.h"
#include "UMLEntityContainer.h"
#include "StringHelpers.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CUMLEntityLabel::CUMLEntityLabel()
/* ============================================================
	Function :		CUMLEntityLabel::CUMLEntityLabel
	Description :	Constructor
	Access :		Public

	Return :		void
	Parameters :	none

	Usage :			Create either through "CUMLControlFactory" or
					by calling "new" and adding the pointer to the
					editor via "StartDrawingObject"

   ============================================================*/
{

	SetDefaultSize( CSize( 48, 14 ) );

	CString title;
	title.LoadString( IDS_UML_LABEL );
	SetTitle( title );

	SetType( _T( "uml_label" ) );
	SetConstraints( GetDefaultSize(), CSize( -1, -1 ) );

	SetBkColor( RGB( 0,0,0 ) );

	SetPropertyDialog( &m_dlg, CUMLLabelPropertyDialog::IDD );

	SetPointsize( 12 );
	SetBold( FALSE );
	SetItalic( FALSE );
	SetUnderline( FALSE );

}

CUMLEntityLabel::~CUMLEntityLabel()
/* ============================================================

	Function :		CUMLEntityLabel::~CUMLEntityLabel
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

CDiagramEntity* CUMLEntityLabel::Clone()
/* ============================================================
	Function :		CUMLEntityLabel::Clone
	Description :	Creates a new object of this type, copies 
					the data from this object, and returns the 
					new one.
	Access :		Public

	Return :		CDiagramEntity*	-	New object
	Parameters :	none

	Usage :			Call to clone this object

   ============================================================*/
{

	CUMLEntityLabel* obj = new CUMLEntityLabel;
	obj->Copy( this );
	return obj;

}

BOOL CUMLEntityLabel::FromString( const CString& str )
/* ============================================================
	Function :		CUMLEntityLabel::FromString
	Description :	Sets the data for this object from "str"
	Access :		Public

	Return :		BOOL				-	"TRUE" if "str" was a 
											valid representation of 
											this type.
	Parameters :	const CString& str	-	String representation
					
	Usage :			Use when loading from file.

   ============================================================*/
{

	BOOL result = FALSE;
	CString data( str );

	if( LoadFromString( data ) )
	{

		CTokenizer tok( data );
		CString fontName;
		int		bkColor;

		CString package;
		int		pointsize;
		BOOL	bold;
		BOOL	italic;
		BOOL	underline;

		int count = 0;

		tok.GetAt( count++, package );
		tok.GetAt( count++, fontName );
		tok.GetAt( count++, bkColor );
		tok.GetAt( count++, pointsize );
		tok.GetAt( count++, bold );
		tok.GetAt( count++, italic );
		tok.GetAt( count++, underline );

		UnmakeSaveString( package );

		SetPackage( package );
		SetFont( fontName );
		SetBkColor( static_cast< COLORREF >( bkColor ) );
		SetPointsize( pointsize );
		SetBold( bold );
		SetItalic( italic );
		SetUnderline( underline );

		CalcRestraints();
		result = TRUE;
	}

	return result;

}

CDiagramEntity* CUMLEntityLabel::CreateFromString( const CString& str )
/* ============================================================
	Function :		CUMLEntityLabel::CreateFromString
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

	CUMLEntityLabel* obj = new CUMLEntityLabel;
	if(!obj->FromString( str ) )
	{
		delete obj;
		obj = NULL;
	}

	return obj;

}

void CUMLEntityLabel::Draw( CDC* dc, CRect rect )
/* ============================================================
	Function :		CUMLEntityLabel::Draw
	Description :	Draws this object.
	Access :		Public

	Return :		void
	Parameters :	CDC* dc		-	"CDC" to draw to
					CRect rect	-	Rectangle to draw to
					
	Usage :			Call to draw the object.

   ============================================================*/
{

	if( GetTitle().GetLength() )
	{
	
		CFont font;
		int weight = FW_NORMAL;
		if( GetBold() )
			weight = FW_BOLD;

		font.CreateFont( -round( static_cast< double >( GetPointsize() ) * GetZoom() ), 
			0,0,0,
			weight,
			( BYTE ) GetItalic(),
			( BYTE ) GetUnderline(),0,0,0,0,0,0, 
			GetFont() );

		CFont* oldfont = dc->SelectObject( &font );
		dc->SetBkMode( TRANSPARENT );
		dc->DrawText( GetTitle(), rect, DT_WORDBREAK | DT_NOPREFIX);
		dc->SelectObject( oldfont );

	}

}

void CUMLEntityLabel::SetTitle( CString title )
/* ============================================================
	Function :		CUMLEntityLabel::SetTitle
	Description :	Sets the title of the object. This is the 
					visible label contents.
	Access :		Public

	Return :		void
	Parameters :	CString title	-	New title
					
	Usage :			This function is overridden to recalculate 
					the object rectangle restraints.

   ============================================================*/
{

	CUMLEntity::SetTitle( title );
	CalcRestraints();

}

CString CUMLEntityLabel::GetString() const
/* ============================================================
	Function :		CUMLEntityLabel::GetString
	Description :	Gets a string representation of this object.
	Access :		Public

	Return :		CString	-	Resulting string
	Parameters :	none

	Usage :			Call to get a string representation for
					saving.

   ============================================================*/
{

	CString str;
	CString package = GetPackage();
	MakeSaveString( package );

	str.Format( _T( ",%s,%s,%i,%i,%i,%i,%i;" ), 
			package,
			GetFont(),
			static_cast< int >( GetBkColor() ),
			GetPointsize(),
			GetBold(),
			GetItalic(),
			GetUnderline()
		);

	str = GetDefaultGetString() + str;
	return str;

}

void CUMLEntityLabel::SetRect( CRect rect )
/* ============================================================
	Function :		CUMLEntityLabel::SetRect
	Description :	Sets the object rectangle
	Access :		Public

	Return :		void
	Parameters :	CRect rect	-	New rectangle
					
	Usage :			This function is overridden to recalculate 
					rectangle restraints.

   ============================================================*/
{

	CDiagramEntity::SetRect( rect );
	CalcRestraints();

}

void CUMLEntityLabel::SetRect( double left, double top, double right, double bottom )
/* ============================================================
	Function :		CUMLEntityLabel::SetRect
	Description :	Sets the object rectangle
	Access :		Public

	Return :		void
	Parameters :	double left		-	New left position
					double top		-	New top position
					double right	-	New right position
					double bottom	-	New bottom position
					
	Usage :			This function is overridden to recalculate 
					rectangle restraints.	

   ============================================================*/
{

	CDiagramEntity::SetRect( left, top, right, bottom );
	CalcRestraints();

}

void CUMLEntityLabel::CalcRestraints()
/* ============================================================
	Function :		CUMLEntityLabel::CalcRestraints
	Description :	Calculates, depending on the contents, the 
					minimum size for this object.
	Access :		Public

	Return :		void
	Parameters :	none

	Usage :			Call when the contents change. The size is 
					calculated by calling "DrawText" on a "CDC", 
					getting back the height from the given 
					width.

   ============================================================*/
{

	CString title = GetTitle();
	if( title.GetLength() )
	{

		CDC* dc = CWnd::GetDesktopWindow()->GetDC();

		CRect rect = GetRect();
		CRect newRect( rect );
		CFont font;
		int weight = FW_NORMAL;
		if( GetBold() )
			weight = FW_BOLD;

		font.CreateFont( -GetPointsize(), 0,0,0,weight, ( BYTE ) GetItalic(), ( BYTE ) GetUnderline(),0,0,0,0,0,0, GetFont() );
		CFont* oldfont = dc->SelectObject( &font );
		dc->DrawText( GetTitle(), newRect, DT_WORDBREAK | DT_CALCRECT );
		dc->SelectObject( oldfont );
		CWnd::GetDesktopWindow()->ReleaseDC( dc );

		int diff = newRect.Height() - rect.Height();
		rect.bottom += diff;
		if( GetBottom() - GetTop() < rect.Height() )
			SetBottom( GetTop() + rect.Height() );

		if( GetDefaultSize().cy < rect.Height() )
		{
			SetMinimumSize( CSize( GetDefaultSize().cx, rect.Height() ) );
			CRect rc = GetRect();
			if( rc.Height() < rect.Height() )
				SetRect( GetLeft(), GetTop(), GetRight(), GetTop() + rect.Height() );
		}
		else
			SetMinimumSize( GetDefaultSize() );

	}

}

int CUMLEntityLabel::GetLinkCode( CPoint /*point*/ ) const
/* ============================================================
	Function :		CUMLEntityLabel::GetLinkCode
	Description :	Returns the link code of this object for 
					the position "point".
	Access :		Public

	Return :		int				-	Always "LINK_NONE"
	Parameters :	CPoint point	-	Position to test
					
	Usage :			Labels can't be linked to.

   ============================================================*/
{

	return LINK_NONE;

}

CString CUMLEntityLabel::Export( UINT format ) const
/* ============================================================
	Function :		CUMLEntityLabel::Export
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

CString CUMLEntityLabel::ExportHTML() const
/* ============================================================
	Function :		CUMLEntityLabel::ExportHTML
	Description :	Exports the label to HTML
	Access :		Private

	Return :		CString	-	Resulting HTML-code.
	Parameters :	none

	Usage :			Call to export the label to HTML

   ============================================================*/
{

	CString result;
	CRect rect = GetRect();

	int font_size = GetPointsize();
	CString fontweight( _T( "normal" ) );
	if( GetBold() )
		fontweight = _T( "bold" );
	CString fontstyle( _T( "normal" ) );
	if( GetItalic() )
		fontstyle = _T( "italic" );
	CString textdecoration( _T( "none" ) );
	if( GetUnderline() )
		textdecoration = _T( "underline" );

	CString color = ColorrefToString( GetBkColor() );
	CString title = GetTitle();
	title.Replace( _T( "<" ), _T( "&lt;" ) );
	title.Replace( _T( ">" ), _T( "&gt;" ) );
	title.Replace( _T( "\r\n" ), _T( "<br>" ) );

	result.Format( _T( "<div style='position:absolute;left:%i;top:%i;width:%i;height:%i;font-family:%s;font-size:%i;overflow:hidden;font-weight:%s;font-style:%s;text-decoration:%s;'>%s</div>" ),
		rect.left, rect.top, rect.Width(), rect.Height(), GetFont(), font_size, fontweight, fontstyle, textdecoration, title );

	return result;
}

void CUMLEntityLabel::SetPointsize( int pointsize )
/* ============================================================
	Function :		CUMLEntityLabel::SetPointsize
	Description :	Sets the point size (or, more correctly, 
					the pixel size) of the label font.
	Access :		Public

	Return :		void
	Parameters :	int pointsize	-	New size
					
	Usage :			Call to set a new font size.

   ============================================================*/
{

	if( pointsize != m_pointsize )
	{
		m_pointsize = pointsize;
		CalcRestraints();
	}

}

void CUMLEntityLabel::SetBold( BOOL bold )
/* ============================================================
	Function :		CUMLEntityLabel::SetBold
	Description :	Sets the bold-flag of the label font.
	Access :		Public

	Return :		void
	Parameters :	BOOL bold	-	"TRUE" if the font should be bold.
					
	Usage :			Call to set (or reset) a bold font for the 
					label.

   ============================================================*/
{

	m_bold = bold;

}

void CUMLEntityLabel::SetItalic( BOOL italic )
/* ============================================================
	Function :		CUMLEntityLabel::SetItalic
	Description :	Sets the italic flag for the label font.
	Access :		Public

	Return :		void
	Parameters :	BOOL italic	-	"TRUE" if the font should be 
									italic.
					
	Usage :			Call to set (or reset) an italic font for the 
					label.


   ============================================================*/
{

	m_italic = italic;

}

void CUMLEntityLabel::SetUnderline( BOOL underline )
/* ============================================================
	Function :		CUMLEntityLabel::SetUnderline
	Description :	Sets the underline flag for the label font.
	Access :		Public

	Return :		void
	Parameters :	BOOL underline	-	"TRUE" if the font should be 
										underlined.
					
	Usage :			Call to set (or reset) an underlined font 
					for the label.


   ============================================================*/
{

	m_underline = underline;

}

int CUMLEntityLabel::GetPointsize() const
/* ============================================================
	Function :		CUMLEntityLabel::GetPointsize
	Description :	Gets the pixel size for the label font.
	Access :		Public

	Return :		int	-	Current size
	Parameters :	none

	Usage :			Call to get the font size of the label.

   ============================================================*/
{

	return m_pointsize;

}

BOOL CUMLEntityLabel::GetBold() const
/* ============================================================
	Function :		CUMLEntityLabel::GetBold
	Description :	Gets the bold flag from the label
	Access :		Public

	Return :		BOOL	-	"TRUE" if bold
	Parameters :	none

	Usage :			Call to get the bold-flag of the label.

   ============================================================*/
{

	return m_bold;

}

BOOL CUMLEntityLabel::GetItalic() const
/* ============================================================
	Function :		CUMLEntityLabel::GetItalic
	Description :	Gets the italic flag from the label
	Access :		Public

	Return :		BOOL	-	"TRUE" if italic
	Parameters :	none

	Usage :			Call to get the italic-flag of the label.

   ============================================================*/
{

	return m_italic;

}

BOOL CUMLEntityLabel::GetUnderline() const
/* ============================================================
	Function :		CUMLEntityLabel::GetUnderline
	Description :	Gets the underline-flag from the label
	Access :		Public

	Return :		BOOL	-	"TRUE" if underlined
	Parameters :	none

	Usage :			Call to get the underline-flag of the label.

   ============================================================*/
{

	return m_underline;

}
