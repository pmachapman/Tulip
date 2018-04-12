#if !defined(AFX_UMLENTITY_H__8BBDE424_2729_426C_89EC_B3E3E1021F92__INCLUDED_)
#define AFX_UMLENTITY_H__8BBDE424_2729_426C_89EC_B3E3E1021F92__INCLUDED_

#include "DiagramEditor\DiagramEntity.h"
#include "UMLPackagePropertyDialog.h"

class CUMLEntityContainer;

// Link types
#define LINK_NONE					0
#define LINK_LEFT					1
#define LINK_RIGHT					2
#define LINK_TOP					4
#define LINK_BOTTOM					8

#define LINK_ALL					15

#define LINK_START					16
#define LINK_END					32

#define CMD_OPEN					199
#define CMD_IMPORT					198
#define CMD_FLIP					197

#define DISPLAY_ALL								0
#define DISPLAY_ONLY_PUBLIC						1
#define DISPLAY_NO_MARKERS						2
#define DISPLAY_NO_ATTRIBUTES					4
#define DISPLAY_NO_OPERATIONS					8
#define DISPLAY_NO_OPERATION_ATTRIBUTE_NAMES	16

#define EXPORT_CPP					0
#define EXPORT_H					1
#define EXPORT_HTML					2

#define ENTITY_TYPE_NONE			0
#define ENTITY_TYPE_STATIC			1
#define ENTITY_TYPE_ABSTRACT		2
#define ENTITY_TYPE_VIRTUAL			4

#define ACCESS_TYPE_PRIVATE			0
#define ACCESS_TYPE_PROTECTED		1
#define ACCESS_TYPE_PUBLIC			2

class CUMLEntity : public CDiagramEntity
{

public:
// Construction/initialization/destruction
	CUMLEntity();
	virtual	~CUMLEntity();
	virtual CDiagramEntity* Clone();
	static	CDiagramEntity* CreateFromString( const CString& str );

	virtual CString GetString() const;
	virtual BOOL	FromString( const CString& str );

// Implementation
	virtual CPoint	GetLinkPosition( int type ) const;
	virtual int		GetLinkCode( CPoint point ) const;
	virtual CRect	GetLinkMarkerRect( int type ) const;

	virtual void	SetRect( double left, double top, double right, double bottom );
	virtual void	Copy( CDiagramEntity* obj );

	void			SetPackage( const CString& package );
	CString			GetPackage() const;

	COLORREF		GetBkColor() const;
	void			SetBkColor( COLORREF bkColor );
	CString			GetFont() const;
	void			SetFont( const CString& fontName );

	void			SetDefaultSize( CSize sz );
	CSize			GetDefaultSize() const;

	void			SetDisplayOptions( int displayOptions );
	int				GetDisplayOptions() const;

	void			SetOldId( const CString& oldid );
	CString			GetOldId() const;

	void			SetStereotype( const CString& value );
	CString			GetStereotype() const;

	virtual void	CalcRestraints();

protected:

	CUMLEntityContainer* GetUMLContainer() const;

private:
// Private data
	CString			m_package;
	CString			m_fontName;
	COLORREF		m_bkColor;
	CSize			m_defaultSize;
	int				m_displayOptions;
	CString			m_stereotype;

	CString			m_oldid;

};

#endif // !defined(AFX_UMLENTITY_H__8BBDE424_2729_426C_89EC_B3E3E1021F92__INCLUDED_)
