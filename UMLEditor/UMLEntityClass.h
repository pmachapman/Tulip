#if !defined(AFX_UMLENTITYCLASS_H__8BBDE424_2729_426C_89EC_B3E3E1021F92__INCLUDED_)
#define AFX_UMLENTITYCLASS_H__8BBDE424_2729_426C_89EC_B3E3E1021F92__INCLUDED_

#include "UMLEntity.h"
#include "Attribute.h"
#include "Operation.h"

#include "UMLClassPropertyDialog.h"
#include "DiagramEditor/Tokenizer.h"
#include "PropertyContainer.h"
#include "OperationContainer.h"
#include "AttributeContainer.h"

class CUMLEntityClass : public CUMLEntity
{

public:
// Construction/initialization/destruction
	CUMLEntityClass();
	virtual ~CUMLEntityClass();
	virtual CDiagramEntity* Clone();
	static	CDiagramEntity* CreateFromString( const CString& str );

// Overrides
	virtual void Draw( CDC* dc, CRect rect );

	virtual CString GetString() const;
	virtual BOOL	FromString( const CString& str );
	virtual void	Copy( CDiagramEntity* obj );
	virtual CString	Export( UINT format = 0 ) const;
	virtual BOOL	DoMessage( UINT msg, CDiagramEntity* sender, CWnd* from = NULL );
	virtual void	ShowPopup( CPoint point, CWnd* parent );
	virtual void	SetTitle( CString title );

// Implementation
	int	GetAttributes() const;
	int GetOperations() const;
	CAttribute* GetAttribute( int index ) const;
	COperation* GetOperation( int index ) const;

	void ClearAttributes();
	void ClearOperations();
	void ClearProperties();

	void AddAttribute( CAttribute* obj );
	void AddOperation( COperation* obj );

	virtual void CalcRestraints();

	void		SetBaseClassArray( const CStringArray& baseClassArray, const CStringArray& baseClassAccessArray  );
	void		GetBaseClassArray( CStringArray& array, CStringArray& arrayAccess ) const;
	BOOL		InBaseClassArray( const CString& title ) const;
	CPropertyContainer* GetProperties();
	void		SetFilename( const CString& value );
	CString		GetFilename() const;
	void		SetBaseClassFilenameArray( const CStringArray& value );
	void		GetBaseClassFilenameArray( CStringArray& array ) const;

protected:
	virtual CString ExportHTML() const;
	virtual CString ExportCPP() const;
	virtual CString ExportH() const;
	virtual BOOL	ImportH( const CString& filename );

	virtual CString GetOperationList( int format ) const;
	virtual CString GetAttributeList( int format ) const;
	virtual CString	GetAttributeInitializationList() const;
	virtual CString GetIncludeList() const;
	virtual CString GetDependencyList() const;

	COperationContainer*	GetOperationsContainer();
	CAttributeContainer*	GetAttributesContainer();

	virtual CString GetHeaderTemplateFilename() const;

private:

	CUMLClassPropertyDialog	m_dlg;

	int		GetVisibleAttributes() const;
	int		GetVisibleOperations() const;
	BOOL	IsAbstract() const;

	CStringArray m_baseClassArray;
	CStringArray m_baseClassAccessArray;

	CString m_filename;
	CStringArray m_baseClassFilenameArray;

	CPropertyContainer	m_properties;
	COperationContainer	m_operations;
	CAttributeContainer	m_attributes;

};

#endif // !defined(AFX_UMLENTITYCLASS_H__8BBDE424_2729_426C_89EC_B3E3E1021F92__INCLUDED_)
