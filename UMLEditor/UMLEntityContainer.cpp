/* ==========================================================================
	Class :			CUMLEntityContainer

	Author :		Johan Rosengren, Abstrakt Mekanik AB

	Date :			2004-04-29

	Purpose :		"CUMLEntityContainer" is a "CDiagramEntityContainer"-
					derived class, holding the data for a "CUMLEditor". 
					In addition to "CDiagramEntityContainer", this class keeps 
					and manages links. This includes copy/paste and 
					undo-handling.

	Description :	The class uses a derived "CDiagramClipboardHandler".

	Usage :			Use as a normal "CDiagramEntityContainer" class.

   ========================================================================
	Changes :		27/6 2004	Added Load function.
					8/7 2004	Changed base class handling in c++-export 
								from "CString" to "CStringArray" to support 
								multiple inheritance
					8/7 2004	Corrected bug in path name creation (double 
								slashes) in the cpp/h-export.
   ========================================================================
					5/8 2004	Added support for private and protected 
								base classes through line stereotypes.
   ========================================================================*/

#include "stdafx.h"
#include "UMLEntityContainer.h"
#include "UMLControlFactory.h"
#include "DiskObject/DiskObject.h"
#include "TextFile/TextFile.h"
#include "UMLEntityInterface.h"

#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma warning( disable : 4706 )

CUMLEntityContainer::CUMLEntityContainer()
/* ============================================================
	Function :		CUMLEntityContainer::CUMLEntityContainer
	Description :	Constructor
	Access :		Public
					
	Return :		void
	Parameters :	none

	Usage :			Create and attach to a "CUMLEditor".

   ============================================================*/
{

	m_displayOptions = 0;
	m_color = RGB( 0, 0, 0 );

	SetUndoStackSize( 10 );

}

CUMLEntityContainer::~CUMLEntityContainer()
/* ============================================================
	Function :		CUMLEntityContainer::~CUMLEntityContainer
	Description :	Destructor
	Access :		Public
					
	Return :		void
	Parameters :	none

	Usage :			

   ============================================================*/
{

	ClearUndo();

}


void CUMLEntityContainer::RemoveAt( int index )
/* ============================================================
	Function :		CUMLEntityContainer::RemoveAt
	Description :	Removes the object at "index". Will also 
					remove all linked lines refering to this 
					object if it is a segment itself.
	Access :		Public

	Return :		void
	Parameters :	int index	-	Index for object to remove.

	Usage :			Overridden to remove linked segments as well.

   ============================================================*/
{

	CUMLEntity* obj = GetObjectAt( index );
	if( obj )
	{

		CUMLLineSegment* line = dynamic_cast< CUMLLineSegment* >( obj );

		if( line )
		{
			DeleteLine( line );
		}
		else
		{
			CString link;

			link = obj->GetName();
			for( int t = GetSize() - 1 ; t >= 0 ; t-- )
			{
				CUMLLineSegment* del = dynamic_cast< CUMLLineSegment* >( GetAt( t ) );
				if( del )
				{
					if( del->GetLink( LINK_START ) == link || 
						del->GetLink( LINK_END ) == link )
					{
						Remove( del );
						t = GetSize() - 1;
					}
				}
			}

			index = Find( obj );
			if( index != -1 )
			{
				GetData()->RemoveAt( index );
				delete obj;
			}

		}

	}

}


CUMLLineSegment* CUMLEntityContainer::GetNextSegment( CUMLLineSegment* from ) const
/* ============================================================
	Function :		CUMLEntityContainer::GetNextSegment
	Description :	Gets the next segment in the line from 
					the "from" line segment.
	Access :		Public

	Return :		CUMLLineSegment*	-	The next segment, or "NULL"
	Parameters :	CUMLLineSegment* from	-	The segment to start from.
					
	Usage :			Call to get the next segment in a segmented 
					line.

   ============================================================*/
{
	CUMLLineSegment* result = NULL;
	CString name = from->GetLink( LINK_END );
	if( name.GetLength() )
		result = dynamic_cast< CUMLLineSegment* >( GetNamedObject( name ) );
	else
	{
		name = from->GetName();
		CUMLLineSegment* obj = NULL;
		int max = GetSize();
		for( int t = 0 ; t < max && result == NULL ; t++ )
		{
			obj = dynamic_cast< CUMLLineSegment* >( GetAt( t ) );
			if( obj && obj->GetLink( LINK_START) == name )
				return obj;
		}
	}

	return result;
}

CUMLLineSegment* CUMLEntityContainer::GetPrevSegment( CUMLLineSegment* from ) const
/* ============================================================
	Function :		CUMLEntityContainer::GetPrevSegment
	Description :	Gets the previous segment in the line from 
					the "from" line segment.

	Access :		Public

	Return :		CUMLLineSegment*		-	Previsous segment or "NULL"
	Parameters :	CUMLLineSegment* from	-	Segment to start from
					
	Usage :			Call to get the previous segment in a 
					segmented line.

   ============================================================*/
{
	CUMLLineSegment* result = NULL;
	CString name = from->GetLink( LINK_START );
	if( name.GetLength() )
		result = dynamic_cast< CUMLLineSegment* >( GetNamedObject( name ) );
	else
	{
		name = from->GetName();
		CUMLLineSegment* obj = NULL;
		int max = GetSize();
		for( int t = 0 ; t < max && result == NULL ; t++ )
		{
			obj = dynamic_cast< CUMLLineSegment* >( GetAt( t ) );
			if( obj && obj->GetLink( LINK_END ) == name )
				return obj;
		}
	}

	return result;
}

CUMLLineSegment* CUMLEntityContainer::GetStartSegment( CUMLLineSegment* from ) const
/* ============================================================
	Function :		CUMLEntityContainer::GetStartSegment
	Description :	Gets the start segment in the line 
					containing the line segment "from".
	Access :		Public

	Return :		CUMLLineSegment*	-	Start segment. Might be identical to "from"
	Parameters :	CUMLLineSegment* from	-	Segment in the line to check.
					
	Usage :			Call to get the starting segment in a line 
					containing the segment "from".

   ============================================================*/
{

	CUMLLineSegment* result = from;
	CUMLLineSegment* test = GetPrevSegment( from );

	while( test )
	{
		result = test;
		test = GetPrevSegment( test );
	}

	return result;

}

CUMLLineSegment* CUMLEntityContainer::GetEndSegment( CUMLLineSegment* from ) const
/* ============================================================
	Function :		CUMLEntityContainer::GetEndSegment
	Description :	Gets the end segment in the line 
					containing the segment "from".
	Access :		Public

	Return :		CUMLLineSegment*	-	End segment. Might be identical to "from"
	Parameters :	CUMLLineSegment* from	-	Segment in the line to check.
					
	Usage :			Call to get the ending segment in a line 
					containing the segment "from".

   ============================================================*/
{

	CUMLLineSegment* result = from;
	CUMLLineSegment* test = GetNextSegment( from );

	while( test )
	{
		result = test;
		test = GetNextSegment( test );
	}

	return result;

}

void CUMLEntityContainer::DeleteLine( CUMLLineSegment* from )
/* ============================================================
	Function :		CUMLEntityContainer::DeleteLine
	Description :	Deletes all segments in the line containing 
					the line segment "from".
	Access :		Private

	Return :		void
	Parameters :	CUMLLineSegment* from	-	Segment in the line to delete
					
	Usage :			Call to delete a complete line. Will 
					recalculate the restraints for attached 
					objects.

   ============================================================*/
{

	CUMLEntity* start = GetStartNode( from );
	CUMLEntity* end = GetEndNode( from );
	CUMLLineSegment* obj = GetStartSegment( from );
	CUMLLineSegment* next = NULL;
	while( obj )
	{
		int index = Find( obj );
		next = GetNextSegment( obj );
		GetData()->RemoveAt( index );
		delete obj;
		obj = next;
	}

	if( start )
		start->CalcRestraints();
	if( end )
		end->CalcRestraints();

}

CUMLEntity* CUMLEntityContainer::GetNamedObject( const CString& name ) const
/* ============================================================
	Function :		CUMLEntityContainer::GetNamedObject
	Description :	Gets the object with the name "name" from 
					the data container.
	Access :		Public

	Return :		CUMLEntity*			-	Result, "NULL" if not found.
	Parameters :	const CString& name	-	Name to search for.
					
	Usage :			Call to get the pointer to an object given 
					its name.

   ============================================================*/
{

	CUMLEntity* test;
	int max = GetSize();
	for( int t = 0 ; t < max ; t++ )
	{
		test = GetObjectAt( t );
		if( test->GetName() == name )
			return test;
	}

	return NULL;

}

CUMLEntity* CUMLEntityContainer::GetTitledObject( const CString& name ) const
/* ============================================================
	Function :		CUMLEntityContainer::GetTitledObject
	Description :	Gets the object with the title "name" from 
					the data container.
	Access :		Public

	Return :		CUMLEntity*			-	Result, "NULL" if not found.
	Parameters :	const CString& name	-	Title to search for.
					
	Usage :			Call to get the pointer to an object given 
					its title.

   ============================================================*/
{

	CUMLEntity* test;
	int max = GetSize();
	for( int t = 0 ; t < max ; t++ )
	{

		test = GetObjectAt( t );
		if( test->GetTitle() == name )
			return test;

	}

	return NULL;

}

void CUMLEntityContainer::RemoveAllSelected()
/* ============================================================
	Function :		CUMLEntityContainer::RemoveAllSelected
	Description :	Removes all selected objects in the 
					container.
	Access :		Public

	Return :		void
	Parameters :	none

	Usage :			Call to delete all selected objects in the 
					container. The function will also remove 
					attached links.

   ============================================================*/
{

	for( int t = GetSize() - 1; t >= 0 ; t-- )
	{
		if( GetAt( t )->IsSelected() )
		{
			RemoveAt( t );
			t = GetSize();
		}
	}

}

CUMLEntity* CUMLEntityContainer::GetStartNode( CUMLLineSegment* from ) const
/* ============================================================
	Function :		CUMLEntityContainer::GetStartNode
	Description :	Gets the object attached to the start 
					segment of the line containing the 
					segment "from"
	Access :		Public

	Return :		CUMLEntity*	-	
	Parameters :	CUMLLineSegment* from	-	Segment in the line
					
	Usage :			Call to get the starting object of the line.

   ============================================================*/
{

	CUMLEntity* result = NULL;
	CUMLLineSegment* line = GetStartSegment( from );
	if( line )
	{
		CString start = line->GetLink( LINK_START );
		result = GetNamedObject( start );
	}
	return result;

}

CUMLEntity* CUMLEntityContainer::GetEndNode( CUMLLineSegment* from ) const
/* ============================================================
	Function :		CUMLEntityContainer::GetEndNode
	Description :	Gets the object attached to the end 
					segment of the line containing the 
					segment "from"
	Access :		Public

	Return :		CUMLEntity*				-	The object attached 
												to the end of the line.
	Parameters :	CUMLLineSegment* from	-	Segment in the line
					
	Usage :			Call to get the ending object of the line.

   ============================================================*/
{

	CUMLEntity* result = NULL;
	CUMLLineSegment* line = GetEndSegment( from );
	if( line )
	{
		CString end = line->GetLink( LINK_END );
		result = GetNamedObject( end );
	}
	return result;

}

int	CUMLEntityContainer::GetSelectCount() const
/* ============================================================
	Function :		int	CUMLEntityContainer::GetSelectCount
	Description :	Returns the number of currently selected 
					objects.
	Access :		Public
					
	Return :		int		-	The number of selected objects.
	Parameters :	none

	Usage :			Call to see how many objects are selected.

   ============================================================*/
{

	int count = 0;
	int max = GetSize();

	for( int t = 0 ; t < max ; t++ )
		if( GetAt( t )->IsSelected() )
			count++;

	return count;

}

void CUMLEntityContainer::DeleteDanglingLines()
/* ============================================================
	Function :		CUMLEntityContainer::DeleteDanglingLines
	Description :	Deletes all non-attached lines in the 
					container.
	Access :		Public

	Return :		void
	Parameters :	none

	Usage :			Call to clean up all non-attached lines. 
					Lines are not allowed to be unlinked in 
					either end. This function makes sure that 
					no lines are left unattached.

   ============================================================*/
{

	for( int t = 0 ; t < GetSize() ; t++ )
	{
		CUMLLineSegment* obj = dynamic_cast< CUMLLineSegment* >( GetAt( t ) );
		if( obj )
		{
			CUMLEntity* end = GetEndNode( obj );
			CUMLEntity* start = GetStartNode( obj );
			if( !end || !start )
			{
				Remove( obj );
				t = 0;
			}
		}
	}

}

CDiagramEntity* CUMLEntityContainer::GetAt( int index ) const
/* ============================================================
	Function :		CUMLEntityContainer::GetAt
	Description :	Gets the object at "index"
	Access :		Public

	Return :		CDiagramEntity*	-	Object, or "NULL" if out of bounds.
	Parameters :	int index		-	Index to get object from
					
	Usage :			Call to get the object at "index". The 
					function will only return objects in the 
					current package, unless the package name 
					is set to 'all'. 
					If the object at this index resides in 
					another package, a pointer to a 
					"CUMLEntityDummy"-object will be returned.

   ============================================================*/
{

	CUMLEntityContainer* const local = const_cast<CUMLEntityContainer* const>(this);
	CObArray* objs = local->GetData();
	CUMLEntity* result = NULL;
	if( index < objs->GetSize() && index >= 0 )
	{
		result = static_cast< CUMLEntity* >( objs->GetAt( index ) );
		if( GetPackage() != _T( "all" ) && result->GetPackage() != GetPackage() )
			result =  &( local->m_dummy );
	}

	return result;

}

void CUMLEntityContainer::SetPackage( const CString& package )
/* ============================================================
	Function :		CUMLEntityContainer::SetPackage
	Description :	Sets the current package.
	Access :		Public

	Return :		void
	Parameters :	const CString& package	-	New package
					
	Usage :			Call to set the current package. Many 
					operations only work on things in the 
					current package. Note that if the package 
					is set to 'all', all objects are accessible.

   ============================================================*/
{

	m_package = package;

}

CString CUMLEntityContainer::GetPackage() const
/* ============================================================
	Function :		CUMLEntityContainer::GetPackage
	Description :	Gets the current package.
	Access :		Public

	Return :		CString	-	Current package
	Parameters :	none

	Usage :			Call to get the current package. Many 
					operations only work on things in the 
					current package. Note that if the package 
					is set to 'all', all objects are accessible.

   ============================================================*/
{

	return m_package;

}

CUMLEntity* CUMLEntityContainer::GetObjectAt( int index ) const
/* ============================================================
	Function :		CUMLEntityContainer::GetObjectAt
	Description :	Convenience function to get the object at 
					"index", already cast to a "CUMLEntity" 
					pointer.
	Access :		Public

	Return :		CUMLEntity*	-	Object at "index", or "NULL" if out of bounds.
	Parameters :	int index	-	Index to get object from
					
	Usage :			Call to get the object at "index".

   ============================================================*/
{

	CUMLEntity* result = static_cast< CUMLEntity* >( GetAt( index ) );
	return result;

}

void CUMLEntityContainer::AdjustLinkedObjects( CUMLEntity* in, CUMLEntity* filter )
/* ============================================================
	Function :		CUMLEntityContainer::AdjustLinkedObjects
	Description :	Moves all objects in relation to "in", 
					skipping "filter".
	Access :		Public

	Return :		void
	Parameters :	CUMLEntity* in		-	Object to move in relation to
					CUMLEntity* filter	-	Optional object to skip.
					
	Usage :			Call to adjust the positions of objects 
					attached to "in" when it is moved.

   ============================================================*/
{
	CPoint point;
	CPoint objpoint;
	double posdiff = 0.0;
	CString name = in->GetName();
	CString link;
	int linktype;

	CUMLLineSegment* line = dynamic_cast< CUMLLineSegment* >( in );
	if( line )
	{
		link = line->GetLink( LINK_END );
		if( link.GetLength() && link != name )
		{
			CUMLEntity* obj = static_cast< CUMLEntity* >( GetNamedObject( link ) );
			if( obj  && obj != filter && !obj->IsSelected() )
			{
				objpoint = line->GetLinkPosition( LINK_END );
				linktype = line->GetLinkType( LINK_END );
				if( linktype == LINK_TOP )
				{
					CPoint linkpoint = GetLinkPosition( obj, line );
					posdiff = objpoint.x - linkpoint.x;
					if( posdiff )
					{
						obj->MoveRect( posdiff, 0 );
						AdjustLinkedObjects( obj, in );
					}
				}
				if( linktype == LINK_BOTTOM )
				{
					CPoint linkpoint = GetLinkPosition( obj, line );
					posdiff = objpoint.x - linkpoint.x;
					if( posdiff )
					{
						obj->MoveRect( posdiff, 0 );
						AdjustLinkedObjects( obj, in );
					}
				}
				if( linktype == LINK_RIGHT )
				{
					CPoint linkpoint = GetLinkPosition( obj, line );
					posdiff = objpoint.y - linkpoint.y;
					if( posdiff )
					{
						obj->MoveRect( 0, posdiff );
						AdjustLinkedObjects( obj, in );
					}
				}
				if( linktype == LINK_LEFT )
				{
					CPoint linkpoint = GetLinkPosition( obj, line );
					posdiff = objpoint.y - linkpoint.y;
					if( posdiff )
					{
						obj->MoveRect( 0, posdiff );
						AdjustLinkedObjects( obj, in );
					}
				}
				if( linktype == LINK_START )
				{
					CRect first = obj->GetRect();
					obj->SetLeft( objpoint.x );
					obj->SetTop( objpoint.y );
					if( static_cast< CUMLLineSegment* >( obj )->IsHorizontal() )
						obj->SetBottom( obj->GetTop() );
					else
						obj->SetRight( obj->GetLeft() );
					if( first != obj->GetRect() )
						AdjustLinkedObjects( obj, in );
				}
				if( linktype == LINK_END )
				{
					CRect first = obj->GetRect();
					obj->SetRight( objpoint.x );
					obj->SetBottom( objpoint.y );
					if( static_cast< CUMLLineSegment* >( obj )->IsHorizontal() )
						obj->SetTop( obj->GetBottom() );
					else
						obj->SetLeft( obj->GetRight() );
					if( first != obj->GetRect() )
						AdjustLinkedObjects( obj, in );
				}
			}
		}

		link = line->GetLink( LINK_START );
		if( link.GetLength() && link != name )
		{
			CUMLEntity* obj = static_cast< CUMLEntity* >( GetNamedObject( link ) );
			if( obj  && obj != filter && !obj->IsSelected() )
			{
				objpoint = line->GetLinkPosition( LINK_START );
				linktype = line->GetLinkType( LINK_START );
				if( linktype == LINK_TOP )
				{
					CPoint linkpoint = GetLinkPosition( obj, line );
					posdiff = objpoint.x - linkpoint.x;
					if( posdiff )
					{
						obj->MoveRect( posdiff, 0 );
						AdjustLinkedObjects( obj, in );
					}
				}
				if( linktype == LINK_BOTTOM )
				{
					CPoint linkpoint = GetLinkPosition( obj, line );
					posdiff = objpoint.x - linkpoint.x;
					if( posdiff )
					{
						obj->MoveRect( posdiff, 0 );
						AdjustLinkedObjects( obj, in );
					}
				}
				if( linktype == LINK_RIGHT )
				{
					CPoint linkpoint = GetLinkPosition( obj, line );
					posdiff = objpoint.y - linkpoint.y;
					if( posdiff )
					{
						obj->MoveRect( 0, posdiff );
						AdjustLinkedObjects( obj, in );
					}
				}
				if( linktype == LINK_LEFT )
				{
					CPoint linkpoint = GetLinkPosition( obj, line );
					posdiff = objpoint.y - linkpoint.y;
					if( posdiff )
					{
						obj->MoveRect( 0, posdiff );
						AdjustLinkedObjects( obj, in );
					}
				}
				if( linktype == LINK_START )
				{
					CRect first = obj->GetRect();
					obj->SetLeft( objpoint.x );
					obj->SetTop( objpoint.y );
					if( static_cast< CUMLLineSegment* >( obj )->IsHorizontal() )
						obj->SetBottom( obj->GetTop() );
					else
						obj->SetRight( obj->GetLeft() );
					if( first != obj->GetRect() )
						AdjustLinkedObjects( obj, in );
				}
				if( linktype == LINK_END )
				{
					CRect first = obj->GetRect();
					obj->SetRight( objpoint.x );
					obj->SetBottom( objpoint.y );
					if( static_cast< CUMLLineSegment* >( obj )->IsHorizontal() )
						obj->SetTop( obj->GetBottom() );
					else
						obj->SetLeft( obj->GetRight() );
					if( first != obj->GetRect() )
						AdjustLinkedObjects( obj, in );
				}
			}
		}
	}
	else
	{
		CUMLEntity* obj = NULL;
		int count = 0;
		while( ( obj = GetObjectAt( count++ ) ) )
		{
			if( obj->GetPackage() == GetPackage() )
			{
				if( obj != in )
				{
					CUMLLineSegment* line = dynamic_cast< CUMLLineSegment* >( obj );
					if( line )
					{
						link = line->GetLink( LINK_START );
						if( link == name )
						{
							if( line && line != filter && !line->IsSelected() )
							{
								linktype = line->GetLinkType( LINK_START );
								objpoint = GetLinkPosition( in, line );
								CRect first = line->GetRect();
								if( linktype == LINK_RIGHT || linktype == LINK_LEFT )
								{
									line->SetTop( objpoint.y );
									line->SetLeft( objpoint.x );

									line->SetBottom( objpoint.y );
								}
								if( linktype == LINK_TOP || linktype == LINK_BOTTOM )
								{
									line->SetTop( objpoint.y );
									line->SetLeft( objpoint.x );

									line->SetRight( objpoint.x );
								}

								if( linktype == LINK_END )
								{
									line->SetLeft( objpoint.x );
									line->SetTop( objpoint.y );
									if( line->IsHorizontal() )
										line->SetBottom( objpoint.y );
									else
										line->SetRight( objpoint.x );
								}

								if( linktype == LINK_START )
								{
									line->SetRight( objpoint.x );
									line->SetBottom( objpoint.y );
									if( line->IsHorizontal() )
										line->SetTop( objpoint.y );
									else
										line->SetLeft( objpoint.x );
								}

								if( first != line->GetRect() )
									AdjustLinkedObjects( line, in );
							}
						}

						link = line->GetLink( LINK_END );
						if( link == name )
						{
							if( line && line != filter && !line->IsSelected() )
							{
								objpoint = GetLinkPosition( in, line );
								linktype = line->GetLinkType( LINK_END );
								CRect first = line->GetRect();
								if( linktype == LINK_RIGHT || linktype == LINK_LEFT )
								{
									line->SetBottom( objpoint.y );
									line->SetRight( objpoint.x );

									line->SetTop( objpoint.y );
								}
								if( linktype == LINK_TOP || linktype == LINK_BOTTOM )
								{
									line->SetBottom( objpoint.y );
									line->SetRight( objpoint.x );

									line->SetLeft( objpoint.x );
								}

								if( linktype == LINK_END )
								{
									line->SetLeft( objpoint.x );
									line->SetTop( objpoint.y );
									if( line->IsHorizontal() )
										line->SetBottom( objpoint.y );
									else
										line->SetRight( objpoint.x );
								}

								if( linktype == LINK_START )
								{
									line->SetRight( objpoint.x );
									line->SetBottom( objpoint.y );
									if( line->IsHorizontal() )
										line->SetTop( objpoint.y );
									else
										line->SetLeft( objpoint.x );
								}

								if( first != line->GetRect() )
									AdjustLinkedObjects( line, in );
							}
						}
					}
				}
			}
		}
	}

}

void CUMLEntityContainer::ReduceLine( CUMLLineSegment* line )
/* ============================================================
	Function :		CUMLEntityContainer::ReduceLine
	Description :	Removes adjacent line segments with the 
					same orientation.
	Access :		Public

	Return :		void
	Parameters :	CUMLLineSegment* line	-	Line to check
					
	Usage :			Expands segments to cover a complete 
					horizontal or vertical stretch.

   ============================================================*/
{

	SetDefaultLineStyle( line );

	CUMLLineSegment* test = GetPrevSegment( line );
	if( test )
	{
		if( ( line->IsHorizontal() && test->IsHorizontal() ) || 
			( !line->IsHorizontal() && !test->IsHorizontal() ) )
		{
			// We have found two lines with the same orientation 
			// attached to each other. The second line should 
			// swallow the first one

			// Setting start values from test to line
			line->SetLeft( test->GetLeft() );
			line->SetTop( test->GetTop() );

			// Assign the start link from test to line
			line->SetLink( LINK_START, test->GetLink( LINK_START ) );
			line->SetLinkType( LINK_START, test->GetLinkType( LINK_START ) );
			line->SetOffset( LINK_START, test->GetOffset( LINK_START ) );

			// If we have an end link to test, 
			// we must rename it as well
			CUMLLineSegment* prev = GetPrevSegment( test );
			if( prev )
				prev->SetLink( LINK_END, line->GetName() );

			// Remove test
			int index = Find( test );
			GetData()->RemoveAt( index );
			delete test;
		}
	}

}

void CUMLEntityContainer::SetDefaultLineStyle( CUMLLineSegment* line )
/* ============================================================
	Function :		CUMLEntityContainer::SetDefaultLineStyle
	Description :	Sets the default line style of a newly drawn 
					line.
	Access :		Public

	Return :		void
	Parameters :	CUMLLineSegment* line	-	Segment in the line to set style for
					
	Usage :			Call after a line is drawn to set the line style.

   ============================================================*/
{

	CUMLEntity* start = GetStartNode( line );
	CUMLEntity* end = GetEndNode( line );
	BOOL dashed = FALSE;
	BOOL owned = FALSE;

	if( start )
	{
		if( start->GetType() == _T( "uml_note" ) )
			dashed = TRUE;
	}

	if( end )
	{
		if( end->GetType() == _T( "uml_note" ) )
			dashed = TRUE;
		if( end->GetType() == _T( "uml_package" ) && !dashed )
			owned = TRUE;
	}

	if( dashed )
		line->AddLineStyle( STYLE_DASHED );

	if( owned )
		line->SetLineStyle( STYLE_CIRCLECROSS );

}

CPoint CUMLEntityContainer::GetLinkPosition( CUMLEntity* obj, CUMLLineSegment* line ) const
/* ============================================================
	Function :		CUMLEntityContainer::GetLinkPosition
	Description :	Gets the link position for the line "line" 
					on the object "obj".
	Access :		Public

	Return :		CPoint					-	Link position
	Parameters :	CUMLEntity* obj			-	Object to check
					CUMLLineSegment* line	-	Line to check
					
	Usage :			Call to get the position of the link between 
					"line" and "obj".

   ============================================================*/
{

	CPoint result( -1, -1 );
	CPoint	start;
	int linkOffset = 0;
	int type = LINK_NONE;

	if( line->GetLink( LINK_START ) == obj->GetName() )
	{
		type = line->GetLinkType( LINK_START );
		linkOffset = line->GetOffset( LINK_START );
	}
	else if( line->GetLink( LINK_END ) == obj->GetName() )
	{
		type = line->GetLinkType( LINK_END );
		linkOffset = line->GetOffset( LINK_END );
	}

	CRect rect = obj->GetRect();
	start = obj->GetLinkPosition( type );
	switch( type )
	{
		case LINK_LEFT:
		case LINK_RIGHT:
		{
			result.x = start.x;
			result.y = rect.top + linkOffset;
		}
		break;
		case LINK_TOP:
		case LINK_BOTTOM:
		{
			result.y = start.y;
			result.x = rect.left + linkOffset;
		}
		break;
	}

	return result;

}

void CUMLEntityContainer::Save( CArchive& ar )
/* ============================================================
	Function :		CUMLEntityContainer::Save
	Description :	Saves the container data to file.
	Access :		Public

	Return :		void
	Parameters :	CArchive& ar	-	Archive to save to
					
	Usage :			Call to save the container data.

   ============================================================*/
{

	CString package = GetPackage();
	SetPackage( _T( "all" ) );
	ar.WriteString( GetString() + _T( "\r\n" ) );
	int count = 0;
	CDiagramEntity* obj;
	while( ( obj = GetAt( count++ ) ) )
		ar.WriteString( obj->GetString() + _T( "\r\n" ) );

	SetPackage( package );
	SetModified( FALSE );

}

void CUMLEntityContainer::Save( CString& filename )
/* ============================================================
	Function :		CUMLEntityContainer::Save
	Description :	Saves the container data to file.
	Access :		Public

	Return :		void
	Parameters :	CString& filename	-	Filename to save to.
					
	Usage :			Call to save the container data. If 
					"filename" is empty, a standard Windows 
					file dialog will be displayed, and the 
					variable will contain the selected filename 
					on return.

   ============================================================*/
{

	CString package = GetPackage();
	SetPackage( _T( "all" ) );
	CTextFile		file;
	CStringArray	stra;

	stra.Add( GetString() + _T( "\r\n" ) );
	int count = 0;
	CDiagramEntity* obj;
	while( ( obj = GetAt( count++ ) ) )
		stra.Add( obj->GetString() + _T( "\r\n" ) );

	if( file.WriteTextFile( filename, stra ) )
	{
		SetPackage( package );
		SetModified( FALSE );
	}
	else
		AfxMessageBox( file.GetErrorMessage() );

}

void CUMLEntityContainer::SetDisplayOptions( int displayOption )
/* ============================================================
	Function :		CUMLEntityContainer::SetDisplayOptions
	Description :	Sets the display options for the objects 
					in the container.
	Access :		Public

	Return :		void
	Parameters :	int displayOption	-	Display option(s) to set.
					
	Usage :			Call to set new display options.
					It can be a combination of the following:
						"DISPLAY_ALL" Show everything
						"DISPLAY_ONLY_PUBLIC" Show only public attributes and operations.
						"DISPLAY_NO_MARKERS" Don't show access markers
						"DISPLAY_NO_ATTRIBUTES" Don't show attributes
						"DISPLAY_NO_OPERATIONS" Don't show operations.
						"DISPLAY_NO_OPERATION_ATTRIBUTE_NAMES" Don't show names for operation parameters

   ============================================================*/
{

	m_displayOptions = displayOption;
	CUMLEntity* obj;
	int count = 0;
	while( ( obj = GetObjectAt( count++ ) ) )
		obj->SetDisplayOptions( displayOption );

}

int CUMLEntityContainer::GetDisplayOptions() const
/* ============================================================
	Function :		CUMLEntityContainer::GetDisplayOptions
	Description :	Gets the current display option(s)
	Access :		Public

	Return :		int	-	Current display option(s)
	Parameters :	none

	Usage :			Call to get the current display options.
					It can be a combination of the following:
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

void CUMLEntityContainer::SetColor( COLORREF color )
/* ============================================================
	Function :		CUMLEntityContainer::SetColor
	Description :	Sets the color of the container.
	Access :		Public

	Return :		void
	Parameters :	COLORREF color	-	New color
					
	Usage :			The color is used as background color for 
					the editor and will be saved/loaded with 
					the rest of the container data.

   ============================================================*/
{

	m_color = color;

}

COLORREF CUMLEntityContainer::GetColor() const
/* ============================================================
	Function :		CUMLEntityContainer::GetColor
	Description :	Gets the container color
	Access :		Public

	Return :		COLORREF	-	Container color
	Parameters :	none

	Usage :			The color is used as background color for 
					the editor and will be saved/loaded with 
					the rest of the container data.

   ============================================================*/
{

	return m_color;

}

BOOL CUMLEntityContainer::PackageExists( const CString& name, CUMLEntity* filter )
/* ============================================================
	Function :		CUMLEntityContainer::PackageExists
	Description :	Checks if a package already exists in the 
					container.
	Access :		Public

	Return :		BOOL				-	"TRUE" if it exists
	Parameters :	const CString& name	-	Name to check
					CUMLEntity* filter	-	Object to skip
					
	Usage :			Call to see if a name already exist in the
					diagram

   ============================================================*/
{

	BOOL result = FALSE;

	CString oldpackage = GetPackage();
	SetPackage( _T( "all" ) );
	CUMLEntity* obj;
	int count = 0;
	while( ( obj = GetObjectAt( count++ ) ) )
		if( obj->GetTitle() == name && obj != filter )
			result = TRUE;

	SetPackage( oldpackage );

	return result;

}

CString CUMLEntityContainer::GetString() const
/* ============================================================
	Function :		CDiagramEntityContainer::GetString
	Description :	Returns a string representation of the 
					virtual paper size and current paper color.
	Access :		Public
					
	Return :		CString	-	Resulting string
	Parameters :	none

	Usage :			Call to get a string representing the paper 
					size of the container. The format is 
					"paper:x,y,color;" where "x" and "y" are the 
					horisontal and vertical sizes, and "color" is 
					the paper color.

   ============================================================*/
{

	CString str;
	str.Format( _T( "paper:%i,%i,%i;" ), GetVirtualSize().cx, GetVirtualSize().cy, static_cast< int >( GetColor() ) );
	return str;

}

BOOL CUMLEntityContainer::FromString( const CString& str )
/* ============================================================
	Function :		CDiagramEntityContainer::FromString
	Description :	Sets the virtual paper size from a string.
	Access :		Public
					
	Return :		BOOL				-	"TRUE" if the string 
											represented a 
											paper.
	Parameters :	const CString& str	-	The string 
											representation.
					
	Usage :			Call to set the paper size of the container 
					from a string. The format is "paper:x,y,color;" 
					where "x" and "y" are the horisontal and 
					vertical sizes, "color" the background color.

   ============================================================*/
{

	BOOL result = FALSE;

	CTokenizer main( str, _T( ":" ) );
	CString header;
	CString data;
	if( main.GetSize() == 2 )
	{
		main.GetAt( 0, header );
		main.GetAt( 1, data );
		header.TrimLeft();
		header.TrimRight();
		data.TrimLeft();
		data.TrimRight();
		if( header == _T( "paper" ) )
		{
			CTokenizer tok( data.Left( data.GetLength() - 1 ) );
			int size = tok.GetSize();
			if( size == 3 )
			{
				int right;
				int bottom;
				int color;

				tok.GetAt(0, right );
				tok.GetAt(1, bottom );
				tok.GetAt(2, color );

				SetVirtualSize( CSize( right, bottom ) );
				SetColor( static_cast< COLORREF >( color ) );
				result = TRUE;
			}
		}
	}

	return result;

}

BOOL CUMLEntityContainer::LineSelected( CUMLLineSegment* line ) const
/* ============================================================
	Function :		CUMLEntityContainer::LineSelected
	Description :	Check if all segments in the line where 
					"line" is a part is selected.
	Access :		Public

	Return :		BOOL							-	"TRUE" if all of the line is selected
	Parameters :	CUMLLineSegment* line	-	Segment to check
					
	Usage :			Call to see if a complete line is selected.

   ============================================================*/
{

	BOOL result = FALSE;
	if( line->IsSelected() )
	{
		CUMLEntity* obj = GetStartNode( line );
		if( obj && obj->IsSelected() )
		{
			obj = GetEndNode( line );
			if( obj && obj->IsSelected() )
			{
				result = TRUE;
				CUMLLineSegment* seg = GetStartSegment( line );
				while( seg )
				{
					if( !seg->IsSelected() )
						result =  FALSE;
					seg = GetNextSegment( seg );
				}
			}
		}
	}

	return result;

}

void CUMLEntityContainer::Undo()
/* ============================================================
	Function :		CUMLEntityContainer::Undo
	Description :	Undos the last operation
	Access :		Public

	Return :		void
	Parameters :	none

	Usage :			We fix links and restore the paper size, 
					color and current package as well.

   ============================================================*/
{

	if( GetUndo()->GetSize() )
	{
		// We remove all current data
		RemoveAll();

		// We get the last entry from the undo-stack
		// and clone it into the container data
		CUMLUndoItem* undo = static_cast< CUMLUndoItem* >( GetUndo()->GetAt( GetUndo()->GetUpperBound() ) );
		int count = ( undo->arr ).GetSize();
		for( int t = 0 ; t < count ; t++ )
		{

			CDiagramEntity* obj = static_cast< CDiagramEntity* >( ( undo->arr ).GetAt( t ) );
			Add( obj->Clone() );

		}

		FixLinks( GetData() );

		// Set the saved virtual size as well
		SetVirtualSize( undo->pt );
		SetColor( undo->col );
		SetPackage( undo->package );

		// We remove the entry from the undo-stack
		delete undo;
		GetUndo()->RemoveAt( GetUndo()->GetUpperBound() );

	}

}

void CUMLEntityContainer::Snapshot()
/* ============================================================
	Function :		CUMLEntityContainer::Snapshot
	Description :	Takes a snapshot of the current data state.
	Access :		Public

	Return :		void
	Parameters :	none

	Usage :			We fix links and get the paper size, color 
					and package as well.

   ============================================================*/
{

	if( GetUndoStackSize() > 0 && GetUndo()->GetSize() == GetUndoStackSize() )
	{
		delete GetUndo()->GetAt( 0 );
		GetUndo()->RemoveAt( 0 );
	}

	CUMLUndoItem* undo = new CUMLUndoItem;

	while( !undo && GetUndo()->GetSize() )
	{

		// We seem - however unlikely -
		// to be out of memory.
		// Remove first element in
		// undo-stack and try again
		delete GetUndo()->GetAt( 0 );
		GetUndo()->RemoveAt( 0 );
		undo = new CUMLUndoItem;

	}

	if( undo )
	{

		// Save current virtual size
		undo->pt = GetVirtualSize();
		undo->col = GetColor();
		undo->package = GetPackage();

		// Save all objects
		int count = GetData()->GetSize();
		for( int t = 0 ; t < count ; t++ )
			( undo->arr ).Add( GetAt( t )->Clone() );

		FixLinks( &( undo->arr ) );

		// Add to undo stack
		GetUndo()->Add( undo );

	}

}

void CUMLEntityContainer::FixLinks( CObArray *arr )
/* ============================================================
	Function :		CUMLEntityContainer::FixLinks
	Description :	Fixes links between the objects in "arr"
	Access :		Public

	Return :		void
	Parameters :	CObArray *arr	-	Container to fix
					
	Usage :			Call to maintain integrity for links even 
					when copied. The links are attached by 
					name, and objects get a new name when 
					copied, therefore we must restore links in 
					the container.

   ============================================================*/
{

	int count = arr->GetSize();

	for( int t = 0 ; t < count ; t++ )
	{
		CUMLEntity* obj = static_cast< CUMLEntity* >( arr->GetAt( t ) );
		if( obj->GetOldId().GetLength() )
		{
			for( int i = 0 ; i < count ; i++ )
			{

				if( t != i )
				{
					CUMLLineSegment* line = dynamic_cast< CUMLLineSegment* >( arr->GetAt( i ) );
					if( line )
					{
						if( line->GetLink( LINK_START ) == obj->GetOldId() )
							line->SetLink( LINK_START, obj->GetName() );
						if( line->GetLink( LINK_END ) == obj->GetOldId() )
							line->SetLink( LINK_END, obj->GetName() );
					}
				}
			}
			obj->SetOldId( _T( "" ) );
		}
	}

}

CSize CUMLEntityContainer::CalcMinimumRestraints( const CString& name )
/* ============================================================
	Function :		CUMLEntityContainer::CalcMinimumRestraints
	Description :	Calculates the minimum size for the object 
					with the name "name" as regards to 
					attached links.
	Access :		Public

	Return :		CSize				-	Minimum size
	Parameters :	const CString& name	-	Name of object to test
					
	Usage :			The objects themselves don't know about the 
					attached links. Therefore, they have to ask 
					the container for the minimum size of the 
					object as far as attached links goes.

   ============================================================*/
{

	CSize result( -1, -1 );
	BOOL horz = FALSE;
	BOOL vert = FALSE;
	if( name.GetLength() )
	{
		int max = GetSize();
		for( int t = 0 ; t < max ; t++ )
		{
			CUMLLineSegment* line = dynamic_cast< CUMLLineSegment* >( GetAt( t ) );
			if( line )
			{
				int offset = -1;
				if( line->GetLink( LINK_START ) == name )
					offset = line->GetOffset( LINK_START );

				if( line->GetLink( LINK_END ) == name )
					offset = line->GetOffset( LINK_END );

				if( offset != -1 )
				{
					if( line->IsHorizontal() )
					{
						if( result.cx == -1 )
							result.cx = offset;
						else if( offset > result.cx )
							result.cx = offset;
						horz = TRUE;
					}
					else
					{
						if( result.cy == -1 )
							result.cy = offset;
						else if( offset > result.cy )
							result.cy = offset;
						vert = TRUE;
					}

					if( horz && vert )
						return result;
				}
			}
		}
	}

	return result;

}

void CUMLEntityContainer::SetProjectName( const CString& value )
/* ============================================================
	Function :		CUMLEntityContainer::SetProjectName
	Description :	Sets the project name
	Access :		Public

	Return :		void
	Parameters :	const CString& value	-	New name
					
	Usage :			Call to set the project name. Used when 
					exporting.

   ============================================================*/
{

	m_project = value;

}

CString CUMLEntityContainer::GetProjectName() const
/* ============================================================
	Function :		CUMLEntityContainer::GetProjectName
	Description :	Gets the project name
	Access :		Public

	Return :		CString	-	Project name
	Parameters :	none

	Usage :			Call to get the project name. Used when 
					exporting.

   ============================================================*/
{

	return m_project;

}

void CUMLEntityContainer::SetProjectLocation( const CString& value )
/* ============================================================
	Function :		CUMLEntityContainer::SetProjectLocation
	Description :	Sets the location (directories) of the 
					project.
	Access :		Public

	Return :		void
	Parameters :	const CString& value	-	New location
					
	Usage :			Call to set the direction of the project.
					This is an informational function that is
					used when exporting.

   ============================================================*/
{

	m_location = value;

}

CString CUMLEntityContainer::GetProjectLocation() const
/* ============================================================
	Function :		CUMLEntityContainer::GetProjectLocation
	Description :	Get the project directory
	Access :		Public

	Return :		CString	-	Result
	Parameters :	none

	Usage :			Call to get the project directory.
					This is an informational function that is
					used when exporting.

   ============================================================*/
{

	return m_location;

}

void CUMLEntityContainer::Export( CStringArray& stra, UINT format ) const
/* ============================================================
	Function :		CDiagramEntityContainer::Export
	Description :	Exports all objects to the format "format".
	Access :		Public
					
	Return :		void
	Parameters :	CStringArray& stra	-	"CStingArray" that 
											will be filled with 
											the exported data on 
											return. 
					UINT format			-	Format to save to.
					
	Usage :			"format" can be one of the following:
						"EXPORT_CPP" Export to cpp-files
						"EXPORT_H" Export to header files
						"EXPORT_HTML" Export to HTML-files

   ============================================================*/
{

	switch( format )
	{
		case EXPORT_HTML:
			ExportHTML( stra );
			break;
		case EXPORT_CPP:
			ExportCPP();
			break;
		case EXPORT_H:
			ExportH();
			break;
	}

}

void CUMLEntityContainer::ExportHTML( CStringArray& stra ) const
/* ============================================================
	Function :		CUMLEntityContainer::ExportHTML
	Description :	Exports the contents of the data array to 
					HTML-format into "stra"
	Access :		Private

	Return :		void
	Parameters :	CStringArray& stra	-	Array to fill with HTML
					
	Usage :			Call to fill "stra" with HTML generated from 
					the current container data.

   ============================================================*/
{

	// Creating headers
	CStringArray	ext;
	CTextFile		file;
	CString			filename;
	filename =		GetApplicationDirectory() + _T( "html_header.txt" );
	if( file.ReadTextFile( filename, ext ) )
	{
		stra.Append( ext );
	}
	else
	{
		CString title;
		title.Format( _T( "<title>%s</title>" ), GetProjectName() );
		stra.Add( _T( "<html>" ) );
		stra.Add( _T( "<head>" ) );
		stra.Add( title );
		stra.Add( _T( "<meta name=\"generator\" content=\"UMLEditorDemo\">" ) );
		stra.Add( _T( "<meta http-equiv=\"content-type\" content=\"text/html; charset=iso-8859-1\">" ) );
		stra.Add( _T( "</head>" ) );
		stra.Add( _T( "<body>" ) );
	}

	CString div;
	div.Format( _T( "<div style='position:relative;height:%i;'>" ), GetTotalHeight() );
	stra.Add( div );

	// Creating the individual objects.
	// Create lines first, as they might
	// be to tall vertically (there is a
	// minimum height of the divs)
	int max = GetSize();
	for( int t = 0 ; t < max ; t++ )
	{
		CUMLLineSegment* obj = dynamic_cast< CUMLLineSegment* >( GetObjectAt( t ) );
		if( obj )
			if( GetPackage() == obj->GetPackage() )
				stra.Add( obj->Export( EXPORT_HTML ) );
	}

	for(int t = 0 ; t < max ; t++ )
	{
		CUMLEntity* obj = GetObjectAt( t );
		CUMLLineSegment* seg = dynamic_cast< CUMLLineSegment* >( GetObjectAt( t ) );
		if( !seg )
			if( GetPackage() == obj->GetPackage() )
				stra.Add( obj->Export( EXPORT_HTML ) );
	}

	// Adding jogs
	// For each line we find in the data, we check against 
	// every other line onwards. If they cross, we add a 
	// div with a jog-picture.

	CString horzTemplate( _T( "<div style='position:absolute;left:%i;top:%i;width:16;height:8;background-image:url(\"images/lrjog.gif\");background-repeat:no-repeat;);'>&nbsp;&nbsp;&nbsp;&nbsp;</div>" ) );
	CString vertTemplate( _T( "<div style='position:absolute;left:%i;top:%i;width:8;height:16;background-image:url(\"images/udjog.gif\");background-repeat:no-repeat;);'>&nbsp;</div>" ) );
	CString result;
	max = GetSize();
	for( int i = 0 ; i < max ; i++ )
	{
		CUMLLineSegment* obj = dynamic_cast< CUMLLineSegment* >( GetObjectAt( i ) );
		if( obj && GetPackage() == obj->GetPackage() )
		{
			BOOL horz = obj->IsHorizontal();
			for( int t = i + 1 ; t < max ; t++ )
			{
				CUMLLineSegment* line = dynamic_cast< CUMLLineSegment* >( GetObjectAt( t ) );
				if( line )
				{
					if( horz && !line->IsHorizontal() )
					{
						if( min( obj->GetLeft(), obj->GetRight() ) < line->GetLeft() &&
							max( obj->GetLeft(), obj->GetRight() ) > line->GetLeft() &&
							min( line->GetTop(), line->GetBottom() ) < obj->GetTop() &&
							max( line->GetTop(), line->GetBottom() ) > obj->GetTop() )
						{
							CRect rect( round( line->GetLeft() - 7 ), round( obj->GetTop() - 7 ), round( line->GetLeft() + 7 ), round( obj->GetTop() + 4 ) );
							result.Format( vertTemplate, rect.left, rect.top );
							stra.Add( result );
						}
					}

					else if( !horz && ( line->IsHorizontal() ) )
					{
						if( min( line->GetLeft(), line->GetRight() ) < obj->GetLeft() &&
							max( line->GetLeft(), line->GetRight() ) > obj->GetLeft() &&
							min( obj->GetTop(), obj->GetBottom() ) < line->GetTop() &&
							max( obj->GetTop(), obj->GetBottom() ) > line->GetTop() )
						{
							CRect rect( round( obj->GetLeft() - 7 ), round( line->GetTop() - 7 ), round( obj->GetLeft() + 4 ), round( line->GetTop() + 7 ) );
							result.Format( horzTemplate, rect.left, rect.top );
							stra.Add( result );
						}
					}
				}
			}
		}
	}

	stra.Add( _T( "</div>" ) );

	// Creating footer
	filename = GetApplicationDirectory() + _T( "html_footer.txt" );
	if( file.ReadTextFile( filename, ext ) )
	{
		stra.Append( ext );
	}
	else
	{
		stra.Add( _T( "</body>" ) );
		stra.Add( _T( "</html>" ) );
	}

}

void CUMLEntityContainer::ExportCPP() const
/* ============================================================
	Function :		CUMLEntityContainer::ExportCPP
	Description :	Export the contents of the container to 
					cpp-files.
	Access :		Private

	Return :		void
	Parameters :	none

	Usage :			Call to generate cpp-files from the data in 
					the container.

   ============================================================*/
{

	CUMLEntityContainer* const local = const_cast<CUMLEntityContainer* const>(this);
	CString current = GetPackage();
	local->SetPackage( _T( "all" ) );

	CString str;
	int max = GetSize();
	CString location = GetProjectLocation() + _T( "\\" ) + GetProjectName();
	CDiskObject cdo;
	local->SetErrorMessage( _T( "" ) );
	if( cdo.CreateDirectory( location ) )
	{
		for( int t = 0 ; t < max ; t++ )
		{
			CUMLEntityClass* obj = dynamic_cast< CUMLEntityClass* >( GetAt( t ) );
			if( obj )
			{
				CString title = obj->GetTitle();
				if( title.GetLength() )
				{
					CStringArray baseClassArray;
					CStringArray baseClassAccessArray;
					GetBaseClassArray( obj, baseClassArray, baseClassAccessArray );
					CStringArray baseClassFilenameArray;
					baseClassFilenameArray.Append( baseClassArray );

					if( GetStripLeadingClassCharacter() )
					{
						title = title.Right( title.GetLength() - 1 );
						int size = baseClassFilenameArray.GetSize();
						for( int i = 0 ; i < size ; i++ )
						{
							CString baseClassFilename = baseClassFilenameArray[ i ].Right( baseClassFilenameArray[ i ].GetLength() - 1 );
							baseClassFilenameArray[ i ] = baseClassFilename;
						}
					}

					obj->SetBaseClassArray( baseClassArray, baseClassAccessArray );

					CString path = GetObjectPath( obj );
					path.Replace( _TCHAR( ':' ), _TCHAR( '\\' ) );
					if( path.GetLength() )
					{
						if( path[0] != _TCHAR( '\\' ) )
							path = _T( "\\" ) + path;
					}

					CString filename = location + path + _T( "\\" ) + title;
					obj->SetFilename( title );
					filename += _T( ".cpp" );
					obj->SetBaseClassFilenameArray( baseClassFilenameArray );
					str = obj->Export( EXPORT_CPP );

					if( str.GetLength() )
					{
						if( cdo.CreateFile( filename ) ) 
						{
							CTextFile file( _T( "" ), _T( "\n" ) );;
							if( !file.WriteTextFile( filename, str ) )
								local->SetErrorMessage( file.GetErrorMessage() );
						}
						else
							local->SetErrorMessage( cdo.GetErrorMessage() );
					}
				}
			}
		}
	}
	else
		local->SetErrorMessage( cdo.GetErrorMessage() );

	local->SetPackage( current );

}

void CUMLEntityContainer::ExportH() const
/* ============================================================
	Function :		CUMLEntityContainer::ExportH
	Description :	Exports the current data in the container 
					to c++ header-files.
	Access :		Private

	Return :		void
	Parameters :	none

	Usage :			Call to export the data in the array to 
					header files.

   ============================================================*/
{

	CUMLEntityContainer* const local = const_cast<CUMLEntityContainer* const>(this);
	CString current = GetPackage();
	local->SetPackage( _T( "all" ) );

	CString str;
	int max = GetSize();
	CString location = GetProjectLocation() + _T( "\\" ) + GetProjectName();
	CDiskObject cdo;
	if( cdo.CreateDirectory( location ) )
	{
		for( int t = 0 ; t < max ; t++ )
		{
			CUMLEntityClass* obj = dynamic_cast< CUMLEntityClass* >( GetAt( t ) );
			if( obj )
			{
				CString title = obj->GetTitle();
				if( title.GetLength() )
				{
					CStringArray baseClassArray;
					CStringArray baseClassAccessArray;
					GetBaseClassArray( obj, baseClassArray, baseClassAccessArray );
					CStringArray baseClassFilenameArray;
					baseClassFilenameArray.Append( baseClassArray );

					if( GetStripLeadingClassCharacter() )
					{
						title = title.Right( title.GetLength() - 1 );
						int size = baseClassFilenameArray.GetSize();
						for( int i = 0 ; i < size ; i++ )
						{
							CString baseClassFilename = baseClassFilenameArray[ i ].Right( baseClassFilenameArray[ i ].GetLength() - 1 );
							baseClassFilenameArray[ i ] = baseClassFilename;
						}
					}

					obj->SetBaseClassArray( baseClassArray, baseClassAccessArray );

					CString path = GetObjectPath( obj );
					path.Replace( _TCHAR( ':' ), _TCHAR( '\\' ) );
					if( path.GetLength() )
					{
						if( path [ 0 ] != _TCHAR( '\\' ) )
							path = _T( "\\" ) + path;
					}

					CString filename = location + path + _T( "\\" ) + title;
					obj->SetFilename( title );
					filename += _T( ".h" );
					obj->SetBaseClassFilenameArray( baseClassFilenameArray );
					str = obj->Export( EXPORT_H );

					if( cdo.CreateFile( filename ) ) 
					{
						CTextFile file( _T( "" ), _T( "\n" ) );;
						if( !file.WriteTextFile( filename, str ) )
							AfxMessageBox( file.GetErrorMessage() );
					}
					else
						local->SetErrorMessage( cdo.GetErrorMessage() );
				}
			}
		}
	}
	else
		AfxMessageBox( cdo.GetErrorMessage() );

	local->SetPackage( current );

}

void CUMLEntityContainer::GetBaseClassArray( CUMLEntityClass* obj, CStringArray& array, CStringArray& arrayAccess ) const
/* ============================================================
	Function :		CUMLEntityContainer::GetBaseClass
	Description :	Returns the base classes for "obj"
	Access :		Private

	Return :		CString					-	The base class or empty
	Parameters :	CUMLEntityClass* obj	-	The object to check
					CStringArray& array		-	Array to fill with base classes
					
	Usage :			Call to get the base classes for an object.

   ============================================================*/
{

	CStringArray result;
	CStringArray accessArray;
	CString baseClass = obj->GetProperties()->GetPropertyValue( _T( "baseClass" ) );
	if( baseClass.GetLength() )
	{
		CTokenizer tok( baseClass, _T( " " ) );
		int max = tok.GetSize();
		for( int t = 0 ; t < max ; t++ )
		{
			CString value;
			tok.GetAt( t, value );
			result.Add ( value );
		}
	}
	else
	{
		int max = GetSize();
		for( int t = 0 ; t < max ; t++ )
		{
			CUMLLineSegment* line = dynamic_cast< CUMLLineSegment* >( GetAt( t ) );
			if( line )
			{
				if( line->GetLink( LINK_START ) == obj->GetName() )
				{
					// Check if this is an inherited class
					CUMLLineSegment* seg = GetEndSegment( line );
					if( seg && seg->GetStyle() & STYLE_ARROWHEAD )
					{
						CUMLEntityClass* node = dynamic_cast< CUMLEntityClass* >( GetEndNode( seg ) );
						if( node )
							result.Add( node->GetTitle() );
						else
						{
							// Might be a 
							CUMLEntityInterface* node = dynamic_cast< CUMLEntityInterface* >( GetEndNode( seg ) );
							if( node )
								result.Add( node->GetTitle() );
						}
					}

				}

				// If we have a multi-segment line, it might be flipped
				if( GetNextSegment( line ) || GetPrevSegment( line ) )
				{
					if( line->GetLink( LINK_END ) == obj->GetName() )
					{
						// Check if this is an inherited class
						CUMLLineSegment* seg = GetStartSegment( line );
						if( seg && seg->GetStyle() & STYLE_ARROWHEAD )
						{
							CUMLEntityClass* node = dynamic_cast< CUMLEntityClass* >( GetStartNode( seg ) );
							if( node )
								result.Add( node->GetTitle() );
						}

					}
				}

			}
		}
	}

	int max = result.GetSize();
	for( int t = 0 ; t < max ; t++ )
	{

		CString access( _T( "public" ) );
		CUMLEntity* base = GetTitledObject( result[ t ] );
		if( base )
		{

			// Try to find a link between obj and base
			CUMLLineSegment* seg = GetLinkBetween( obj, base );
			if( seg )
			{

				// Is any segment either private or protected?
				if( IsPrivateLink( seg ) )
					access = _T( "private" );
				if( IsProtectedLink( seg ) )
					access = _T( "protected" );

			}
		}
		accessArray.Add( access );

	}

	array.RemoveAll();
	array.Append( result );

	arrayAccess.RemoveAll();
	arrayAccess.Append( accessArray );

}

void CUMLEntityContainer::SetStripLeadingClassCharacter( BOOL stripLeadingClassLetter )
/* ============================================================
	Function :		CUMLEntityContainer::SetStripLeadingClassCharacter
	Description :	Sets if leading character in the class name 
					should be stripped from the filename when 
					generating to c++.
	Access :		Public

	Return :		void
	Parameters :	BOOL stripLeadingClassLetter	-	"TRUE" if letter should be stripped.
					
	Usage :			Call to set if the first character should 
					be stripped from the class name when 
					creating c++ filenames.

   ============================================================*/
{

	m_stripLeadingClassLetter = stripLeadingClassLetter;

}

BOOL CUMLEntityContainer::GetStripLeadingClassCharacter() const
/* ============================================================
	Function :		CUMLEntityContainer::GetStripLeadingClassCharacter
	Description :	Checks if the leading character in the class name 
					should be stripped from the filename when 
					generating data to c++.
	Access :		Public

	Return :		BOOL	-	"TRUE" if it should be stripped.
	Parameters :	none

	Usage :			The generation mechanism can strip the 
					leading character from a classname when 
					generating cpp/h-files.

   ============================================================*/
{

	return m_stripLeadingClassLetter;

}

BOOL CUMLEntityContainer::IsLinkSelected() const
/* ============================================================
	Function :		CUMLEntityContainer::IsLinkSelected
	Description :	Checks if a single line segment is selected.
	Access :		Public

	Return :		BOOL	-	"TRUE" is only a single segment is selected.
	Parameters :	none

	Usage :			Call in - for example - a command enabler.

   ============================================================*/
{

	if( GetSelectCount() == 1 )
	{
		CUMLLineSegment* line = dynamic_cast< CUMLLineSegment* >( GetSelectedObject() );
		if( line )
			return TRUE;
	}
		
	return FALSE;

}

void CUMLEntityContainer::FlipLink()
/* ============================================================
	Function :		CUMLEntityContainer::FlipLink
	Description :	Flips the currently selected line.
	Access :		Public

	Return :		void
	Parameters :	none

	Usage :			Call to flip the currently selected line 
					(if any). Will only flip if there is one 
					(and only one) link segment selected in the 
					editor.

   ============================================================*/
{

	if( GetSelectCount() == 1 )
	{
		CUMLLineSegment* line = dynamic_cast< CUMLLineSegment* >( GetSelectedObject() );
		if( line )
		{
			if( line->IsSingleLineSegment() )
			{
				line->Flip();
			}
			else
			{
				CUMLLineSegment* start = GetStartSegment( line );
				CUMLLineSegment* end = GetEndSegment( line );
				if( start && end )
				{

					int style = end->GetStyle();
					end->SetStyle( start->GetStyle() );
					start->SetStyle( style );

				}
			}
		}
	}

}

CDiagramEntity* CUMLEntityContainer::GetSelectedObject() const
/* ============================================================
	Function :		CUMLEntityContainer::GetSelectedObject
	Description :	Returns the top selected object in the 
					container.
	Access :		Public
					
	Return :		CDiagramEntity*	-	The top selected object, 
										or "NULL" if none.
	Parameters :	none

	Usage :			Call to get the currently selected object. 
					Note that this function will return a single 
					object (top in the z-order) even if several 
					are selected.

   ============================================================*/
{

	int count = 0;
	CDiagramEntity* obj;

	while( ( obj = GetAt( count++ ) ) )
		if( obj->IsSelected() )
			return obj;

	return NULL;

}

void CUMLEntityContainer::Import()
/* ============================================================
	Function :		CUMLEntityContainer::Import
	Description :	Imports content into the currently selected 
					object from a h-file.
	Access :		Public

	Return :		void
	Parameters :	none

	Usage :			Call to import information from a h-file 
					into an object.

   ============================================================*/
{

	SendMessageToObjects( CMD_IMPORT, TRUE );

}

void CUMLEntityContainer::GetIncludeList( CUMLEntityClass* inobj, CStringArray& stringarray ) const
/* ============================================================
	Function :		CUMLEntityContainer::GetIncludeList
	Description :	Get a list of the "inobj" class inlcudes.
	Access :		Private
					
	Return :		void
	Parameters :	CUMLEntityClass* inobj		-	The object to get
													includes for
					CStringArray& stringarray	-	Results
					
	Usage :			Call to get a list of the classes "inobj" 
					is composed of. This is a list that will be 
					used to generate includes and forward 
					declarations for a h-file. Strings prefixed
					with a hash-mark should be converted to a forward
					declaration.

					The list is created from both connections,
					class members and operation parameters

   ============================================================*/
{

	CStringArray stra;
	int max = GetSize();
	CStringArray classes;
	GetClassList( classes );

	CStringArray baseClassArray;
	CStringArray baseClassAccessArray;
	GetBaseClassArray( inobj, baseClassArray, baseClassAccessArray );
	int size = baseClassArray.GetSize();
	for( int t = 0 ; t < size ; t++ )
	{
		if( InClasslist( baseClassArray[ t ], classes ) )
			stra.Add( baseClassArray[ t ] );
	}

	for( int t = 0 ; t < max ; t++ )
	{
		CUMLLineSegment* line = dynamic_cast< CUMLLineSegment* >( GetAt( t ) );
		if( line )
		{
			if( line->IsSingleLineSegment() )
			{
				if( line->GetLink( LINK_END ) == inobj->GetName() )
				{
					if( line->GetStyle() & STYLE_FILLED_DIAMOND )
					{
						CUMLEntityClass* node = NULL;
						node = dynamic_cast< CUMLEntityClass*>( GetStartNode( line ) );

						if( node && node != inobj )
						{
							CString title = node->GetTitle();
							AddString( title, stra );

						}
					}
				}
			}
			else
			{
				if( line->GetLink( LINK_END ) == inobj->GetName() || line->GetLink( LINK_START ) == inobj->GetName() )
				{
					if( line->GetStyle() & STYLE_FILLED_DIAMOND )
					{
						CUMLEntityClass* node = NULL;
						if( line->GetLink( LINK_END ) == inobj->GetName() )
							node = dynamic_cast< CUMLEntityClass*>( GetStartNode( line ) );
						else
							node = dynamic_cast< CUMLEntityClass*>( GetEndNode( line ) );

						if( node && node != inobj )
						{
							CString title = node->GetTitle();
							AddString( title, stra );

						}
					}
				}
			}
		}
	}

	max = classes.GetSize();
	size = inobj->GetOperations();

	CString cls( inobj->GetTitle() );
	for( int t = 0 ; t < size ; t++ )
	{
		COperation* op = inobj->GetOperation( t );
		int params = op->parameters.GetSize();
		for( int i = 0 ; i < params ; i++ )
		{
			CParameter* param = op->parameters.GetAt( i );
			CString title( param->type );
			title.Remove( _TCHAR( '*' ) );
			title.Remove( _TCHAR( '&' ) );
			int templ = title.Find( _TCHAR( '<' ) );
			if( templ != -1 )
				title = title.Left( templ );

			if( cls != title && InClasslist( title, classes ) )
			{
				if( param->out && ! param->reference )
				{
					if( !inobj->InBaseClassArray( title ) )
					{
						title = _T( "#" ) + title; // prepare for a forward declaration
						AddString( title, stra );
					}
				}
			}
		}
	}

	size = inobj->GetAttributes();
	for( int t = 0 ; t < size ; t++ )
	{
		CAttribute* attr = inobj->GetAttribute( t );
		CString title( attr->type );
		int pointer = title.Remove( _TCHAR( '*' ) );
		int templ = title.Find( _TCHAR( '<' ) );
		if( templ != -1 )
			title = title.Left( templ );

		if( cls != title && InClasslist( title, classes ) )
		{
			if( pointer )
				title = _T( "#" ) + title;
			AddString( title, stra );
		}
	}

	// Put forward declarations at the end of the list
	max = stra.GetSize();
	for( int t = 0 ; t < max ; t++ )
	{
		if( stra[ t ][ 0 ] != _TCHAR( '#' ) )
		{
			CString filename( stra[ t ] );
			if( GetStripLeadingClassCharacter() )
				filename = filename.Right( filename.GetLength() - 1 );
			stringarray.Add( filename );
		}
	}

	for( int t = 0 ; t < max ; t++ )
		if( stra[ t ][ 0 ] == _TCHAR( '#' ) )
			stringarray.Add( stra[ t ] );

}

void CUMLEntityContainer::GetDependencyList( CUMLEntityClass* inobj, CStringArray& stringarray ) const
/* ============================================================
	Function :		CUMLEntityContainer::GetDependencyList
	Description :	Get a list of the "inobj" class dependencies.
	Access :		Private
					
	Return :		void
	Parameters :	CUMLEntityClass* inobj		-	The object to get
													dependencies for
					CStringArray& stringarray	-	Results
					
	Usage :			Call to get a list of the classes "inobj" 
					depends on. This is a list that will be used 
					to generate includes for a cpp-file.

					The list is created from both connections,
					and operation parameters. The list is a
					list of titles for the objects.

   ============================================================*/
{

	CStringArray stra;
	CStringArray included;
	GetIncludeList( inobj, included );

	int max = GetSize();
	for( int t = 0 ; t < max ; t++ )
	{
		CUMLLineSegment* line = dynamic_cast< CUMLLineSegment* >( GetAt( t ) );
		if( line )
		{
			if( line->IsSingleLineSegment() )
			{
				if( line->GetLink( LINK_END ) == inobj->GetName() )
				{
					if( line->GetStyle() & STYLE_FILLED_ARROWHEAD && line->GetStyle() & STYLE_DASHED )
					{
						CUMLEntityClass* node = NULL;
						node = dynamic_cast< CUMLEntityClass* >( GetStartNode( line ) );

						if( node && node != inobj )
						{

							CString title = node->GetTitle();
							AddString( title, stra );

						}
					}
				}
			}
			else
			{
				if( line->GetLink( LINK_START ) == inobj->GetName() || line->GetLink( LINK_END ) == inobj->GetName() )
				{
					if( !( line->GetStyle() & STYLE_FILLED_ARROWHEAD ) && line->GetStyle() & STYLE_DASHED )
					{
						CUMLEntityClass* node = NULL;
						if( line->GetLink( LINK_START ) == inobj->GetName() )
							node = dynamic_cast< CUMLEntityClass*>( GetEndNode( line ) );
						else
							node = dynamic_cast< CUMLEntityClass*>( GetStartNode( line ) );

						if( node && node != inobj )
						{

							CString title = node->GetTitle();
							AddString( title, stra );

						}
					}
				}
			}
		}
	}

	CStringArray classes;
	GetClassList( classes );
	max = classes.GetSize();
	int size = inobj->GetOperations();

	CString cls( inobj->GetTitle() );
	for( int t = 0 ; t < size ; t++ )
	{
		COperation* op = inobj->GetOperation( t );
		int params = op->parameters.GetSize();
		for( int i = 0 ; i < params ; i++ )
		{
			CParameter* param = op->parameters.GetAt( i );
			CString title( param->type );
			int templ = title.Find( _TCHAR( '<' ) );
			if( templ != -1 )
				title = title.Left( templ );
			if( cls != title && InClasslist( title, classes ) )
				AddString( title, stra );
		}
	}

	size = inobj->GetAttributes();
	for( int t = 0 ; t < size ; t++ )
	{
		CAttribute* at = inobj->GetAttribute( t );
		if( at->type[ at->type.GetLength() - 1 ] == _TCHAR( '*' ) )
		{
			CString title( at->type );
			title = title.Left( title.GetLength() - 1 );
			int templ = title.Find( _TCHAR( '<' ) );
			if( templ != -1 )
				title = title.Left( templ );
			if( cls != title && InClasslist( title, classes ) )
				AddString( title, stra );

		}
	}

	// Check against already included
	max = stra.GetSize();
	size = included.GetSize();
	for( int t = max - 1 ; t >= 0 ; t-- )
	{
		for( int i = 0 ; i < size ; i++ )
		{
			if( included[ i ] == stra[ t ] )
			{
				stra.RemoveAt( t );
				i = size;
			}
		}
	}

	max = stra.GetSize();
	for( int t = 0 ; t < max ; t++ )
	{
		CString filename( stra[ t ] );
		if( GetStripLeadingClassCharacter() )
			filename = filename.Right( filename.GetLength() - 1 );
		stringarray.Add( filename );
	}

}

BOOL CUMLEntityContainer::VirtualizeClasses()
/* ============================================================
	Function :		CUMLEntityContainer::VirtualizeClasses
	Description :	Loops all class objects, adding the 
					"{virtual}"-property to all operations that 
					are defined both in a base- and a derived 
					class.
	Access :		Private

	Return :		BOOL	-	"TRUE" if any object was modified.
	Parameters :	none

	Usage :			The "{virtual}"-property is a application 
					specific property to make it possible to 
					add the "virtual" keyword when generating 
					c++-code. This function is automatically 
					called before any export.

   ============================================================*/
{

	BOOL result = FALSE;
	int max = GetSize();
	for( int t = 0 ; t < max ; t++ )
	{
		CUMLEntityClass* obj = dynamic_cast< CUMLEntityClass* >( GetAt( t ) );
		if( obj )
		{
			CStringArray baseClassArray;
			CStringArray baseClassAccessArray;
			GetBaseClassArray( obj, baseClassArray, baseClassAccessArray );
			if( baseClassArray.GetSize() )
			{
				for( int i = 0 ; i < baseClassArray.GetSize() ; i++ )
				{
					CUMLEntityClass* base = dynamic_cast< CUMLEntityClass* >( GetTitledObject( baseClassArray[ i ] ) );
					if( base )
					{
						int operations = obj->GetOperations();
						int baseoperations = base->GetOperations();
						for( int i = 0 ; i < operations ; i++ )
						{
							COperation* operation = obj->GetOperation( i );
							if( operation->name == _T( "~" ) + obj->GetTitle() )
							{
								// Always make dtors virtual
								operation->properties.Add( _T( "virtual" ) );
								for( int x = 0 ; x < baseoperations ; x++ )
								{
									COperation* baseoperation = base->GetOperation( x );
									if( baseoperation->name == _T( "~" ) + base->GetTitle() )
										baseoperation->properties.Add( _T( "virtual" ) );
								}
								result = TRUE;
							}
							else
							{
								for( int x = 0 ; x < baseoperations ; x++ )
								{
									COperation* baseoperation = base->GetOperation( x );
									if( *operation == *baseoperation )
									{
										operation->properties.Add( _T( "virtual" ) );
										baseoperation->properties.Add( _T( "virtual" ) );
										result = TRUE;
									}
								}
							}
						}
					}
					else
					{
						int operations = obj->GetOperations();
						for( int i = 0 ; i < operations ; i++ )
						{
							COperation* operation = obj->GetOperation( i );
							if( operation->name == _T( "~" ) + obj->GetTitle() )
							{
								// Always make dtors virtual
								operation->properties.Add( _T( "virtual" ) );
								result = TRUE;
							}
						}
					}
				}
			}
		}
	}

	return result;

}

BOOL CUMLEntityContainer::BaseClassClasses()
/* ============================================================
	Function :		CUMLEntityContainer::BaseClassClasses
	Description :	Adds the baseClass-property to all classes.
	Access :		

	Return :		BOOL	-	"TRUE" if any class was modified
	Parameters :	none

	Usage :			Call to automatically add the application 
					defined property baseClass.

   ============================================================*/
{

	// Loop all classes, setting the base 
	// class property
	BOOL result = FALSE;
	int max = GetSize();
	for( int t = 0 ; t < max ; t++ )
	{
		CUMLEntityClass* obj = dynamic_cast< CUMLEntityClass* >( GetAt( t ) );
		if( obj )
		{
			CStringArray baseClassArray;
			CStringArray baseClassAccessArray;
			GetBaseClassArray( obj, baseClassArray, baseClassAccessArray );
			if( baseClassArray.GetSize() )
			{
				CString baseClass;
				for( int i = 0 ; i < baseClassArray.GetSize() ; i++ )
				{
					baseClass += baseClassArray[ i ] + _T( " ") ;
				}

				baseClass.TrimLeft();
				baseClass.TrimRight();
				obj->GetProperties()->SetPropertyValue( _T( "baseClass" ), baseClass );
				result = TRUE;
			}
		}
	}

	return result;

}

BOOL CUMLEntityContainer::AddInterfacesToClasses()
/* ============================================================
	Function :		CUMLEntityContainer::AddInterfacesToClasses
	Description :	Adds implementations of all pure virtual 
					functions to derived classes.
	Access :		

	Return :		BOOL	-	"TRUE" if any class was 
								modified.
	Parameters :	none

	Usage :			Call to automatically implement interfaces.

   ============================================================*/
{

	// Loop all classes, getting interfaces 
	// and adding operations
	BOOL result = FALSE;
	int max = GetSize();
	for( int t = 0 ; t < max ; t++ )
	{
		CUMLEntityClass* obj = dynamic_cast< CUMLEntityClass* >( GetAt( t ) );
		if( obj )
		{
			CStringArray baseClassArray;
			CStringArray baseClassAccessArray;
			GetBaseClassArray( obj, baseClassArray, baseClassAccessArray );
			if( baseClassArray.GetSize() )
			{
				for( int i = 0 ; i < baseClassArray.GetSize() ; i++ )
				{
					CUMLEntityClass* base = dynamic_cast< CUMLEntityClass* >( GetTitledObject( baseClassArray[ i ] ) );
					if( base )
					{
						int operations = obj->GetOperations();
						int baseoperations = base->GetOperations();
						// Loop the base operations, trying to find pure virtual
						// operations. If a pure virtual function is found,
						// it is added to the operation container of the derived class.
						for( int x = 0 ; x < baseoperations ; x++ )
						{
							COperation* baseoperation = base->GetOperation( x );
							if( baseoperation )
							{
								if( baseoperation->maintype & ENTITY_TYPE_ABSTRACT )
								{
									BOOL found = FALSE;
									for( int y = 0 ; y < operations ; y++ )
									{
										COperation* operation = obj->GetOperation( y );
										if( operation )
										{
											if( *operation == *baseoperation )
											{
												found = TRUE;
											}
										}
									}
									if( !found )
									{
										// Add the pure virtual operation 
										// to the derived class
										COperation* newoperation = baseoperation->Clone();
										newoperation->maintype = 0;
										obj->AddOperation( newoperation );
										obj->CalcRestraints();
										result = TRUE;
									}
								}
							}
						}
					}
				}
			}
		}
	}

	return result;

}

void CUMLEntityContainer::SetErrorMessage( const CString& error )
/* ============================================================
	Function :		CUMLEntityContainer::SetErrorMessage
	Description :	Sets the internal error message
	Access :		Public

	Return :		void
	Parameters :	const CString& error	-	New error
					
	Usage :			Call to set the internal error message

   ============================================================*/
{

	m_error = error;

}

CString CUMLEntityContainer::GetErrorMessage() const
/* ============================================================
	Function :		CUMLEntityContainer::GetErrorMessage
	Description :	Gets the internal error message
	Access :		Public

	Return :		CString	-	Error message
	Parameters :	none

	Usage :			Call to get error messages after an export

   ============================================================*/
{

	return m_error;

}

void CUMLEntityContainer::GetClassList( CStringArray& stra ) const
/* ============================================================
	Function :		CUMLEntityContainer::GetClassList
	Description :	Gets a list of all defined classes in the 
					diagram.
	Access :		Private
					
	Return :		void
	Parameters :	CStringArray& stra	-	Array to be filled
											with the classes
					
	Usage :			Call to get a list of all classes defined 
					in the diagram.

   ============================================================*/
{

	int max = GetSize();
	for( int t = 0 ; t < max ; t++ )
	{

		CUMLEntityClass* obj = dynamic_cast< CUMLEntityClass* >( GetAt( t ) );
		if( obj )
		{
			CString name = obj->GetTitle();
			stra.Add( name );
		}

	}

}

BOOL CUMLEntityContainer::InClasslist( const CString& name,  const CStringArray& stra ) const
/* ============================================================
	Function :		CUMLEntityContainer::InClasslist
	Description :	Checks if "name" is in the "CStringArray" 
					"stra".
	Access :		Private
					
	Return :		BOOL						-	"TRUE" if it exists
	Parameters :	const CString& name			-	Class to check
					const CStringArray& stra	-	List of classes
					
	Usage :			Call to see if a type is one of the types 
					created in the diagram.

   ============================================================*/
{

	BOOL result = FALSE;

	int max = stra.GetSize();
	for( int t = 0 ; t < max ; t++ )
	{
		CString cls = stra[ t ];
		if( cls == name )
			result = TRUE;
	}

	return result;

}

CString CUMLEntityContainer::GetObjectPath( CUMLEntity* inobj ) const
/* ============================================================
	Function :		CUMLEntityContainer::GetObjectPath
	Description :	Returns the path of packages as a human 
					readable string for "inobj".
	Access :		Public

	Return :		CString				-	Result
	Parameters :	CUMLEntity* inobj	-	Object to get string for
					
	Usage :			Call to get the path of packages as a string
					for an object. The object path is formatted as
					"package1:package2:package3", constisting of 
					zero or more package names.

   ============================================================*/
{

	CUMLEntityContainer* const local = const_cast<CUMLEntityContainer* const>(this);
	CString current = GetPackage();
	local->SetPackage( _T( "all" ) );
	CString package = inobj->GetPackage();
	CString level( package );
	CString path;
	while( level.GetLength() )
	{
		CUMLEntity* obj = GetNamedObject( level );
		if( obj )
		{
			path = obj->GetTitle() + _T( ":" ) + path;
			level = obj->GetPackage();
		}
		else
			level = _T( "" );
	}

	if( path.GetLength() )
		path = path.Left( path.GetLength() - 1 );

	local->SetPackage( current );

	return path;

}

int CUMLEntityContainer::GetTotalHeight() const
/* ============================================================
	Function :		CUMLEntityContainer::GetTotalHeight
	Description :	Gets the total height of the diagram, with 
					as much space added to the bottom as there 
					is empty space at the top.
	Access :		Private

	Return :		int	-	Total height of diagram
	Parameters :	none

	Usage :			Call to get the total height of the diagram.
					Used by the HTML-export function.

   ============================================================*/
{

	int bottom = 0;
	int top = 100;
	int max = GetSize();
	for( int t = 0 ; t < max ; t++ )
	{

		CDiagramEntity* obj = GetAt( t );
		bottom = max( bottom, round( obj->GetTop() ) );
		bottom = max( bottom, round( obj->GetBottom() ) );

		// We want to leave as much space at 
		// the bottom as at the top.
		top = min( top, round( obj->GetTop() ) );
		top = min( top, round( obj->GetBottom() ) );

	}

	return bottom + top;

}

void CUMLEntityContainer::Load( CArchive& ar )
/* ============================================================
	Function :		CUMLEntityContainer::Load
	Description :	Loads a diagram from a "CArchive".
	Access :		Public

	Return :		void
	Parameters :	CArchive& ar	-	Archive to load from
					
	Usage :			Call to load a diagram from a "CArchive".

   ============================================================*/
{

	Clear();
	CString str;
	while(ar.ReadString( str ) )
	{

		if( !FromString( str ) )
		{
			CDiagramEntity* obj = CUMLControlFactory::CreateFromString( str );
			if( obj )
				Add( obj );
		}
	}

	SetModified( FALSE );

}

void CUMLEntityContainer::Load( CString& filename )
/* ============================================================
	Function :		CUMLEntityContainer::Load
	Description :	Loads a diagram from a "CArchive".
	Access :		Public

	Return :		void
	Parameters :	CString& filename	-	Filename to load from
					
	Usage :			Call to load a diagram from a file. If 
					"filename" is empty, a standard Windows 
					file dialog will be displayed, and the 
					variable will contain the selected filename 
					on return.

   ============================================================*/
{

	CTextFile		file;
	CStringArray	stra;
	if( file.ReadTextFile( filename, stra ) )
	{
		Clear();
		int max = stra.GetSize();
		for( int t = 0 ; t < max ; t++ )
		{
			if( !FromString( stra[ t ] ) )
			{
				CDiagramEntity* obj = CUMLControlFactory::CreateFromString( stra[ t ] );
				if( obj )
					Add( obj );
			}
		}
	}
	else
		AfxMessageBox( file.GetErrorMessage() );

}

CUMLLineSegment* CUMLEntityContainer::GetLinkBetween( CUMLEntity* start, CUMLEntity* end ) const
/* ============================================================
	Function :		CUMLEntityContainer::GetLinkBetween
	Description :	Gets a link - if any - in the line between
					"start" and "end"
	Access :		

	Return :		CUMLLineSegment*	-	A segment in the line
	Parameters :	CUMLEntity* start	-	First object
					CUMLEntity* end		-	Last object
					
	Usage :			Call to get a line segment for the line 
					between "start" and "end". "NULL" is 
					returned if there is no line.

   ============================================================*/
{

	CUMLLineSegment* result = NULL;
	CUMLEntity* startnode;
	CUMLEntity* endnode;
	int max = GetSize();
	for( int t = 0; t < max ; t++ )
	{
		CUMLLineSegment* link = dynamic_cast< CUMLLineSegment* >( GetAt( t ) );
		if( link )
		{
			startnode = GetStartNode( link );
			endnode = GetEndNode( link );
			if( ( startnode == start && endnode == end ) ||
				( startnode == end && endnode == start ) )
			{
				result = link;
				t = max;
			}
		}
	}

	return result;

}

BOOL CUMLEntityContainer::IsPrivateLink( CUMLLineSegment* link ) const
/* ============================================================
	Function :		CUMLEntityContainer::IsPrivateLink
	Description :	Checks if any of the link segments in the 
					line containing "link" has a private 
					stereotype.
	Access :		

	Return :		BOOL					-	"TRUE" if any link 
												has a private 
												stereotype.
	Parameters :	CUMLLineSegment* link	-	Segment in line to test
					
	Usage :			Call to see if a base class is private.

   ============================================================*/
{

	CString val( _T( "« private »" ) );
	BOOL result = FALSE;
	CUMLLineSegment* segment = GetStartSegment( link );
	while( segment )
	{
		if( segment->GetTitle() == val )
		{
			result = TRUE;
			segment = NULL;
		}
		else
			segment = GetNextSegment( segment );
	}

	return result;

}

BOOL CUMLEntityContainer::IsProtectedLink( CUMLLineSegment* link ) const
/* ============================================================
	Function :		CUMLEntityContainer::IsProtectedLink
	Description :	Checks if any of the link segments in the 
					line containing "link" has a protected 
					stereotype.
	Access :		

	Return :		BOOL					-	"TRUE" if any link 
												has a protected 
												stereotype.
	Parameters :	CUMLLineSegment* link	-	Segment in line to test
					
	Usage :			Call to see if a base class is protected.

   ============================================================*/
{

	CString val( _T( "« protected »" ) );
	BOOL result = FALSE;
	CUMLLineSegment* segment = GetStartSegment( link );
	while( segment )
	{
		if( segment->GetTitle() == val )
		{
			result = TRUE;
			segment = NULL;
		}
		else
			segment = GetNextSegment( segment );
	}

	return result;

}

#pragma warning( default : 4706 )
