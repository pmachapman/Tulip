#ifndef _DISKOBJECT_H_
#define _DISKOBJECT_H_

#define EF_ONLY_FILENAMES	0
#define EF_FULLY_QUALIFIED	1

class CDiskObject 
{
public:
	// ctor(s)
	CDiskObject();
	CDiskObject( CWnd* feedbackWindow );

	// dtor
	~CDiskObject();

	// - - -
	// Operations

	// Files

	BOOL FileExists( const CString& file );
	BOOL CreateFile( const CString& file );
	BOOL CopyFile( const CString& sourceFile, const CString& destDirectory );
	BOOL RemoveFile( const CString& sourceFile );

	BOOL CopyFiles( const CString& sourceDirectory, const CString& destDirectory );
	BOOL CopyFiles( CStringArray& files, const CString& destDirectory );

	// Directories

	BOOL CreateDirectory( const CString& directory );
	BOOL CopyDirectory( const CString& sourceDirectory, const CString& destDirectory );
	BOOL EmptyDirectory( const CString& directory );
	BOOL RemoveDirectory( const CString& directory );

	BOOL CopyDirectories( const CString& sourceDirectory, const CString& destDirectory );
	BOOL EmptyDirectories( const CString& directory );
	BOOL RemoveDirectories( const CString& directory );

	BOOL EnumFilesInDirectoryWithFilter( const CString& filter, const CString& sourceDirectory, CStringArray& files, int mode = EF_ONLY_FILENAMES );
	BOOL EnumAllDirectories( const CString& sourceDirectory, CStringArray& directories );
	BOOL EnumDirectories( const CString& sourceDirectory, CStringArray& directories );
	BOOL EnumFilesInDirectory( const CString& sourceDirectory, CStringArray& files, int mode = EF_ONLY_FILENAMES );
	BOOL EnumAllFiles( const CString& sourceDirectory, CStringArray& files );
	BOOL EnumAllFilesWithFilter( const CString& filter, const CString& sourceDirectory, CStringArray& files );

	// Error handling
	CString GetErrorMessage();	

private:
	// Error handling
	CString m_errorMessage;

	void	ClearError();
	void	SetInternalErrorMessage();
	void	SetSystemErrorMessage( int error, const CString& elements = _T( "" ) );

	// Helpers

	CWnd*	m_feedbackWindow;
	void	QualifyPath( CString& str );
	void	QualifyFile( CString& str );
	CString GetFileName( const CString& str );
	CString GetDirectoryName( const CString& str );

};

#endif // _DISKOBJECT_H_
