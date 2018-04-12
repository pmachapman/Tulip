/* ==========================================================================
	Class :			CUMLEntity

	Author :		Johan Rosengren, Abstrakt Mekanik AB

	Date :			2004-04-29

	Purpose :		"CUMLEntity" is a base class for linked objects used 
					by a "CDiagramEditor".	

	Description :	The class contains a few virtual functions that derived 
					classes can implement:
					"GetLinkPosition" will return the position of a link type. 
					The coordinates are not zoomed, so if they should be 
					used for drawing, they must be multiplied with the 
					current zoom factor of the editor.

	Usage :			Use this class as a base class for linked objects.

   ========================================================================*/
#include "stdafx.h"
#include "UMLEntity.h"
#include "LinkFactory.h"
#include "DiagramEditor/Tokenizer.h"
#include "StringHelpers.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CUMLEntity::CUMLEntity()
/* ============================================================
	Function :		CUMLEntity::CUMLEntity
	Description :	Constructor
	Access :		Public
					
	Return :		void
	Parameters :	none

	Usage :			Create either through "CUMLControlFactory" or
					by calling "new" and adding the pointer to the
					editor via "StartDrawingObject"

   ============================================================*/
{

	// Setting fixed size
	SetDefaultSize( CSize( 0, 0 ) );
	SetConstraints( CSize( 0, 0 ), CSize( 0, 0 ) );
	SetType( _T( "uml_entity" ) );

	SetName( CLinkFactory::GetID() );

	SetRect( 0, 0, 0, 0 );

	SetFont( _T( "Arial" ) );
	SetBkColor( RGB( 255, 255, 255 ) );
	SetZoom( 1.0 );

	SetPackage( _T( "" ) );

	SetDisplayOptions( DISPLAY_ALL );

}

CUMLEntity::~CUMLEntity()
/* ============================================================
	Function :		CUMLEntity::~CUMLEntity
	Description :	Destructor
	Access :		Public
					
	Return :		void
	Parameters :	none

	Usage :			Note that objects will normally be deleted by
					the container.

   ============================================================*/
{
}

CDiagramEntity* CUMLEntity::Clone()
/* ============================================================
	Function :		CUMLEntity::Clone
	Description :	Clone this object to a new object.
	Access :		Public
					
	Return :		CDiagramEntity*	-	The new object.
	Parameters :	none

	Usage :			Call to create a clone of the object. The 
					caller will have to delete the object.

   ============================================================*/
{

	CUMLEntity* obj = new CUMLEntity;
	obj->Copy( this );
	return obj;

}

CDiagramEntity* CUMLEntity::CreateFromString( const CString& str )
/* ============================================================
	Function :		CUMLEntity::CreateFromString
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

	CUMLEntity* obj = new CUMLEntity;
	if(!obj->FromString( str ) )
	{
		delete obj;
		obj = NULL;
	}

	return obj;

}

int CUMLEntity::GetLinkCode( CPoint point ) const
/* ============================================================
	Function :		CUMLEntity::GetLinkCode
	Description :	Gets the link code for the position "point".
	Access :		Public

	Return :		int				-	The link code.
	Parameters :	CPoint point	-	Position to thest
					
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

	int cut = 1;
	int result = LINK_NONE;
	CRect rect = GetRect();
	if( ( point.x >= rect.left && point.x <= rect.right ) && ( point.y >= rect.top - cut && point.y <= rect.top + cut ) )
		result = LINK_TOP;
	if( ( point.x >= rect.left && point.x <= rect.right ) && ( point.y >= rect.bottom - cut && point.y <= rect.bottom + cut ) )
		result = LINK_BOTTOM;
	if( ( point.y >= rect.top && point.y <= rect.bottom ) && ( point.x >= rect.left - cut && point.x <= rect.left + cut ) )
		result = LINK_LEFT;
	if( ( point.y >= rect.top && point.y <= rect.bottom ) && ( point.x >= rect.right - cut && point.x <= rect.right + cut ) )
		result = LINK_RIGHT;

	return result;

}

CPoint CUMLEntity::GetLinkPosition( int type ) const
/* ============================================================
	Function :		CUMLEntity::GetLinkPosition
	Description :	Returns the position of a link.
	Access :		Public
					
	Return :		CPoint		-	The position of the link,
									-1, -1 if the link is not
									allowed.
	Parameters :	int type	-	The type of the link.
					
	Usage :			The possible link types are:
						"LINK_TOP" Links are allowed to the top of the object.
						"LINK_BOTTOM" Links are allowed to the bottom.
						"LINK_LEFT" Links are allowed to the left.
						"LINK_RIGHT" Links are allowed to the right.
						"LINK_START" Links are allowed to the start of a line (normally the top-left corner of the non-normalized bounding rect).
						"LINK_END" Links are allowed to the end of a line (normally the bottom-right corner of the non-normalized bounding rect).

   ============================================================*/
{

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
			point.x = rect.left + round( static_cast< double >( rect.Width() ) / 2 );
			point.y = rect.top;
			break;
		case LINK_BOTTOM:
			point.x = rect.left + round( static_cast< double >( rect.Width() ) / 2 );
			point.y = rect.bottom;
			break;
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

void CUMLEntity::Copy( CDiagramEntity* obj )
/* ============================================================
	Function :		CUMLEntity::Copy
	Description :	Copies the information from "obj" to 
					this object.
	Access :		Public

	Return :		void
	Parameters :	CDiagramEntity* obj	-	Object to copy from
					
	Usage :			Call to copy information from "obj" to 
					this object.

   ============================================================*/
{

	CDiagramEntity::Copy( obj );

	SetName( CLinkFactory::GetID() );
	CUMLEntity* uml = dynamic_cast< CUMLEntity* >( obj );
	if( uml )
	{

		SetPackage( uml->GetPackage() );
		SetBkColor( uml->GetBkColor() );
		SetFont( uml->GetFont() );
		SetOldId( uml->GetName() );
		SetDisplayOptions( uml->GetDisplayOptions() );

	}

}

void CUMLEntity::SetPackage( const CString& package )
/* ============================================================
	Function :		CUMLEntity::SetPackage
	Description :	Sets the package this object belongs to.
	Access :		Public

	Return :		void
	Parameters :	const CString& package	-	Name of package
					
	Usage :			Call to set the package this object belongs 
					to. The package is the "m_name" of the package 
					containing this object.

   ============================================================*/
{

	m_package = package;

}

CString CUMLEntity::GetPackage() const
/* ============================================================
	Function :		CUMLEntity::GetPackage
	Description :	Gets the package this object belongs to.
	Access :		Public

	Return :		CString	-	Name of owning package
	Parameters :	none

	Usage :			Call to get the package this object belongs 
					to. The package is the "m_name" of the package 
					containing this object.

   ============================================================*/
{

	return m_package;

}

COLORREF CUMLEntity::GetBkColor() const
/* ============================================================
	Function :		CUMLEntity::GetBkColor
	Description :	Gets the background color of this object.
	Access :		Public

	Return :		COLORREF	-	Background color.
	Parameters :	none

	Usage :			Call to get the background color of this 
					object.

   ============================================================*/
{

	return m_bkColor;

}

void CUMLEntity::SetBkColor( COLORREF bkColor )
/* ============================================================
	Function :		CUMLEntity::SetBkColor
	Description :	Sets the background color of this object.
	Access :		Public

	Return :		void
	Parameters :	COLORREF bkColor	-	Background color.
					
	Usage :			Call to set the background color of this 
					object.

   ============================================================*/
{

	m_bkColor = bkColor;

}

CString CUMLEntity::GetFont() const
/* ============================================================
	Function :		CUMLEntity::GetFont
	Description :	Gets the font for this object.
	Access :		Public

	Return :		CString	-	Font name
	Parameters :	none

	Usage :			Call to get the font for this object.

   ============================================================*/
{

	return m_fontName;

}

void CUMLEntity::SetFont( const CString& fontName )
/* ============================================================
	Function :		CUMLEntity::SetFont
	Description :	Sets the font for this object.
	Access :		Public

	Return :		void
	Parameters :	const CString& fontName	-	New font name
					
	Usage :			Call to set the font for this object.

   ============================================================*/
{

	m_fontName = fontName;

}

CRect CUMLEntity::GetLinkMarkerRect( int type ) const
/* ============================================================
	Function :		CUMLEntity::GetLinkMarkerRect
	Description :	Returns the rectangle of the link marker 
					for this object.
	Access :		Public

	Return :		CRect		-	Resulting marker rectangle.
	Parameters :	int type	-	Type of link to get 
									rectangle for.
					
	Usage :			Call to get the rectangle of a specific 
					link marker.The possible link types are:
						"LINK_TOP" Link at the top
						"LINK_BOTTOM" Link at the bottom
						"LINK_LEFT" Link at the left
						"LINK_RIGHT" Link at the right
						"LINK_START" Link at the start point of a line
						"LINK_END" Link at the end point of a line.

   ============================================================*/
{

	int halfside = this->GetMarkerSize().cx / 2;
	CRect rect( -1,- 1, -1, -1 );
	CPoint point = GetLinkPosition( type );

	rect.left	= point.x - halfside;
	rect.right	= point.x + halfside;
	rect.top	= point.y - halfside;
	rect.bottom	= point.y + halfside;

	return rect;

}

void CUMLEntity::SetDefaultSize( CSize sz )
/* ============================================================
	Function :		CUMLEntity::SetDefaultSize
	Description :	Sets the default size of the object.
	Access :		Public

	Return :		void
	Parameters :	CSize sz	-	Default size
					
	Usage :			Call to set the default size of the object.
					An object can never be smaller than the 
					default size.

   ============================================================*/
{

	m_defaultSize = sz;

}

CSize CUMLEntity::GetDefaultSize() const
/* ============================================================
	Function :		CUMLEntity::GetDefaultSize
	Description :	Gets the default size of the object.
	Access :		Public

	Return :		CSize	-	Default size
	Parameters :	none

	Usage :			Call to get the default size of the object.
					An object can never be smaller than the 
					default size.

   ============================================================*/
{

	return m_defaultSize;

}

void CUMLEntity::SetRect( double left, double top, double right, double bottom )
/* ============================================================
	Function :		CUMLEntity::SetRect
	Description :	Sets the rectangle of the object.
	Access :		Public

	Return :		void
	Parameters :	double left		-	New left position
					double top		-	New top position
					double right	-	New right position
					double bottom	-	New bottom position
					
	Usage :			Call to set the position of the object.

   ============================================================*/
{

	SetLeft( left );
	SetTop( top );
	SetRight( right );
	SetBottom( bottom );

	if( GetMinimumSize().cx != -1 )
		if( GetRect().Width() < GetMinimumSize().cx )
			SetRight( GetLeft() + GetMinimumSize().cx );

	if( GetMinimumSize().cy != -1 )
		if( GetRect().Height() < GetMinimumSize().cy )
			SetBottom( GetTop() + GetMinimumSize().cy );

	if( GetMaximumSize().cx != -1 )
		if( GetRect().Width() > GetMaximumSize().cx )
			SetRight( GetLeft() + GetMaximumSize().cx );

	if( GetMaximumSize().cy != -1 )
		if( GetRect().Height() > GetMaximumSize().cy )
			SetBottom( GetTop() + GetMaximumSize().cy );

}

void CUMLEntity::SetDisplayOptions( int displayOptions )
/* ============================================================
	Function :		CUMLEntity::SetDisplayOptions
	Description :	Sets the display options for the object.
	Access :		Public

	Return :		void
	Parameters :	int displayOptions	-	Options to set
					
	Usage :			
					The display options can be a combination of the following:
						"DISPLAY_ALL" Show everything
						"DISPLAY_ONLY_PUBLIC" Show only public attributes and operations.
						"DISPLAY_NO_MARKERS" Don't show access markers
						"DISPLAY_NO_ATTRIBUTES" Don't show attributes
						"DISPLAY_NO_OPERATIONS" Don't show operations.
						"DISPLAY_NO_OPERATION_ATTRIBUTE_NAMES" Don't show names for operation parameters

   ============================================================*/
{

	m_displayOptions = displayOptions;
	CalcRestraints();

}

int CUMLEntity::GetDisplayOptions() const
/* ============================================================
	Function :		CUMLEntity::GetDisplayOptions
	Description :	Gets the display options of the object.
	Access :		Public

	Return :		int	-	Current options
	Parameters :	none

	Usage :			
					The display options can be a combination of 
					the following:
						"DISPLAY_ALL" Show everything
						"DISPLAY_ONLY_PUBLIC" Show only public attributes and operations.
						"DISPLAY_NO_MARKERS" Don't show access markers
						"DISPLAY_NO_ATTRIBUTES" Don't show attributes
						"DISPLAY_NO_OPERATIONS" Don't show operations.
						"DISPLAY_NO_OPERATION_ATTRIBUTE_NAMES" Don't show names for operation parameters

   ============================================================*/
{

	return m_displayOptions;

}

void CUMLEntity::SetOldId( const CString& oldid )
/* ============================================================
	Function :		CUMLEntity::SetOldId
	Description :	Sets the old id of the object.
	Access :		Public

	Return :		void
	Parameters :	const CString& oldid	-	New old id
					
	Usage :			Call to set the old id of the object. The 
					old id is the original id when an object 
					is copied, and used to fix links after - 
					for example - a copy-operation.

   ============================================================*/
{

	m_oldid = oldid;

}

CString CUMLEntity::GetOldId() const
/* ============================================================
	Function :		CUMLEntity::GetOldId
	Description :	Gets the old id of the object.
	Access :		Public

	Return :		void
	Parameters :	const CString& oldid	-	Old id
					
	Usage :			Call to get the old id of the object. The 
					old id is the original id when an object 
					is copied, and used to fix links after - 
					for example - a copy-operation.

   ============================================================*/
{

	return m_oldid;

}

void CUMLEntity::CalcRestraints()
/* ============================================================
	Function :		CUMLEntity::CalcRestraints
	Description :	Calculates, depending on the contents, the 
					minimum size for this object.
	Access :		Public

	Return :		void
	Parameters :	none

	Usage :			Call when the contents change.

   ============================================================*/
{
	// No specific default implementation
}

CUMLEntityContainer* CUMLEntity::GetUMLContainer() const 
/* ============================================================
	Function :		CUMLEntity::GetUMLContainer
	Description :	Gets a "CUMLEntityContainer"-pointer to
					the parent of this object.
	Access :		Private

	Return :		CUMLEntityContainer*	-	Pointer to the 
												parent.
	Parameters :	none

	Usage :			Convenience function.

   ============================================================*/
{

	return reinterpret_cast< CUMLEntityContainer* >( GetParent() );

}

void CUMLEntity::SetStereotype( const CString& value )
/* ============================================================
	Function :		CUMLEntity::SetStereotype
	Description :	Sets the stereotype of this object.
	Access :		Public

	Return :		void
	Parameters :	const CString& value	-	New stereotype
					
	Usage :			Call to set the stereotype.

   ============================================================*/
{

	m_stereotype = value;

}

CString CUMLEntity::GetStereotype() const
/* ============================================================
	Function :		CUMLEntity::GetStereotype
	Description :	Gets the stereotype of this object.
	Access :		Public

	Return :		CString	-	The stereotype
	Parameters :	none

	Usage :			Call to get the stereotype for this object.

   ============================================================*/
{

	return m_stereotype;

}

BOOL CUMLEntity::FromString( const CString& str )
/* ============================================================
	Function :		CUMLEntity::FromString
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

		CString package;
		CString fontName;
		int		bkColor;
		int count = 0;

		tok.GetAt( count++, package );
		tok.GetAt( count++, fontName );
		tok.GetAt( count++, bkColor );

		UnmakeSaveString( package );

		SetPackage( package );
		SetFont( fontName );
		SetBkColor( static_cast< COLORREF >( bkColor ) );

		CalcRestraints();
		result = TRUE;
	}

	return result;

}

CString CUMLEntity::GetString() const
/* ============================================================
	Function :		CUMLEntity::GetString
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

	str.Format( _T( ",%s,%s,%i;" ), 
			package,
			GetFont(),
			static_cast< int >( GetBkColor() )
		);

	str = GetDefaultGetString() + str;
	return str;

}

