#ifndef _UMLENTITYCONTAINER_H_
#define _UMLENTITYCONTAINER_H_

#include "DiagramEditor/DiagramEntityContainer.h"
#include "DiagramEditor/DiagramEntity.h"
#include "DiagramEditor/Tokenizer.h"
#include "UMLEntity.h"
#include "UMLLineSegment.h"
#include "UMLEntityClass.h"
#include "UMLEntityDummy.h"
#include "UMLUndoItem.h"

class CUMLEntityContainer : public CDiagramEntityContainer {

public:
// Construction/initialization/destruction
	CUMLEntityContainer();
	virtual ~CUMLEntityContainer();

// Implementation

// Overrides
	virtual void			RemoveAt( int index );
	virtual void			RemoveAllSelected();
	virtual CDiagramEntity* GetAt( int index ) const;
	virtual CString			GetString() const;
	virtual BOOL			FromString( const CString& str );
	virtual void			Undo();
	virtual void			Snapshot();
	virtual void			Export( CStringArray& stra, UINT format = 0 ) const;

	// Objects
	CUMLEntity*		GetNamedObject( const CString& name ) const;
	CUMLEntity*		GetTitledObject( const CString& name ) const;
	CDiagramEntity*	GetSelectedObject() const;
	void			AdjustLinkedObjects( CUMLEntity* in, CUMLEntity* filter = NULL );
	CUMLEntity*		GetObjectAt( int index ) const;

	// Selection
	int				GetSelectCount() const;

	// Segment iterators
	CUMLLineSegment*	GetNextSegment( CUMLLineSegment* from ) const;
	CUMLLineSegment*	GetPrevSegment( CUMLLineSegment* from ) const;
	CUMLLineSegment*	GetStartSegment( CUMLLineSegment* from ) const;
	CUMLLineSegment*	GetEndSegment( CUMLLineSegment* from ) const;
	CUMLEntity*			GetStartNode( CUMLLineSegment* from ) const;
	CUMLEntity*			GetEndNode( CUMLLineSegment* from ) const;

	CUMLLineSegment*	GetLinkBetween( CUMLEntity* start, CUMLEntity* end ) const;

	// Lines
	void		DeleteDanglingLines();
	void		ReduceLine( CUMLLineSegment* line );
	void		SetDefaultLineStyle( CUMLLineSegment* line );
	BOOL		LineSelected( CUMLLineSegment* line ) const;

	// Links
	BOOL		IsLinkSelected() const;
	void		FlipLink();

	// Packages
	void		SetPackage( const CString& package );
	CString		GetPackage() const;
	BOOL		PackageExists( const CString& name, CUMLEntity* filter );

	// Accessors
	void		SetDisplayOptions( int displayOption );
	int			GetDisplayOptions() const;
	void		SetColor( COLORREF color );
	COLORREF	GetColor() const;
	void		SetProjectName( const CString& value );
	CString		GetProjectName() const;
	void		SetProjectLocation( const CString& value );
	CString		GetProjectLocation() const;
	void		SetErrorMessage( const CString& error );
	CString		GetErrorMessage() const;
	void		SetStripLeadingClassCharacter( BOOL stripLeadingClassLetter );
	BOOL		GetStripLeadingClassCharacter() const;

	// I/O
	void		Save( CArchive& ar );
	void		Save( CString& filename );
	void		Load( CArchive& ar );
	void		Load( CString& filename );
	void		Import();

	void		GetIncludeList( CUMLEntityClass* inobj, CStringArray& stringarray ) const;
	void		GetDependencyList( CUMLEntityClass* inobj, CStringArray& stringarray ) const;
	BOOL		VirtualizeClasses();
	BOOL		BaseClassClasses();
	BOOL		AddInterfacesToClasses();

	// Misc
	void		FixLinks( CObArray *arr );
	CSize		CalcMinimumRestraints( const CString& name );
	CString		GetObjectPath( CUMLEntity* inobj ) const;

private:
// Private helpers
	void		DeleteLine( CUMLLineSegment* from );
	CPoint		GetLinkPosition( CUMLEntity* obj, CUMLLineSegment* line ) const;

	CString			m_package; // Current package
	int				m_displayOptions; // Current display options
	COLORREF		m_color; // Current bg color
	CString			m_project; // Current project name
	CString			m_location; // Current pjoject location
	BOOL			m_stripLeadingClassLetter; // TRUE if leading char should be stripped when creating file names
	CUMLEntityDummy	m_dummy; // Dummy object (for objects in other packages)
	CString			m_error; // Current error, if any

	void			ExportHTML( CStringArray& stra ) const;
	void			ExportCPP() const;
	void			ExportH() const;
	void			GetBaseClassArray( CUMLEntityClass* obj, CStringArray& array, CStringArray& arrayAccess ) const;

	BOOL			InClasslist( const CString& name,  const CStringArray& stra ) const;
	void			GetClassList( CStringArray& stra ) const;

	int				GetTotalHeight() const;

	BOOL			IsPrivateLink( CUMLLineSegment* link ) const;
	BOOL			IsProtectedLink( CUMLLineSegment* link ) const;

};

#endif // _UMLENTITYCONTAINER_H_
