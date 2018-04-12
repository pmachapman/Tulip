#ifndef _UMLENTITYCLASSTEMPLATE_H_
#define _UMLENTITYCLASSTEMPLATE_H_

#include "UMLEntityClass.h"
#include "UMLTemplatePropertyDialog.h"

class CUMLEntityClassTemplate : public CUMLEntityClass
{
public:
	CUMLEntityClassTemplate();
	~CUMLEntityClassTemplate();

// Overrides
	virtual CDiagramEntity* Clone();
	static	CDiagramEntity* CreateFromString( const CString& str );

	virtual void Draw( CDC* dc, CRect rect );

	virtual CString GetString() const;
	virtual BOOL	FromString( const CString& str );
	virtual void	Copy( CDiagramEntity* obj );
	virtual CString	Export( UINT format = 0 ) const;
	virtual BOOL	ImportH( const CString& filename );

	// Implementation
	void SetParameterType( const CString& parameterType );
	CString GetParameterType() const;

protected:
	virtual CString GetHeaderTemplateFilename() const;
	virtual CString GetOperationList( int format ) const;

private:
	CUMLTemplatePropertyDialog	m_dlg;

	CString	m_parameterType;

	virtual CString ExportHTML() const;
	virtual CString ExportH() const;

};

#endif //_UMLENTITYCLASSTEMPLATE_H_