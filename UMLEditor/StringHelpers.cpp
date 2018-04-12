/* ==========================================================================
	Author :		Johan Rosengren, Abstrakt Mekanik AB

	Date :			2004-06-21

	Purpose :		Misc string-related functions

	Description :	Contains several string-related function.

	Usage :			Call as needed.

   ========================================================================*/
#include "stdafx.h"
#include "StringHelpers.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void MakeSaveString( CString& str )
{

	// This function replaces some non-alphanumeric 
	// character with tag-codes, as the characters 
	// are used for delimiting different kind of 
	// substrings.

	str.Replace( _T( ":" ), _T( "\\colon" ) );
	str.Replace( _T( ";" ), _T( "\\semicolon" ) );
	str.Replace( _T( "," ), _T( "\\comma" ) );
	str.Replace( _T( "|" ), _T( "\\bar" ) );
	str.Replace( _T( "#" ), _T( "\\hash" ) );
	str.Replace( _T( "\r\n" ), _T( "\\newline" ) );

}

void UnmakeSaveString( CString& str )
{

	// The function replaces some tag-strings 
	// with the corresponding characters after 
	// loading the string from file.

	str.Replace( _T( "\\colon" ), _T( ":" ) );
	str.Replace( _T( "\\semicolon" ) , _T( ";" ) );
	str.Replace( _T( "\\comma" ), _T( "," ) );
	str.Replace( _T( "\\bar" ), _T( "|" ) );
	str.Replace( _T( "\\hash" ), _T( "#" ) );
	str.Replace( _T( "\\newline" ), _T( "\r\n" ) );

}

CString ColorrefToString( COLORREF col )
{

	// The function converts a COLORREF to
	// a DHTML-color string.

	CString result;
	result.Format( _T( "%x%x%x" ), GetRValue( col ), GetGValue( col ), GetBValue( col ) );
	return result;

}

void AddString( const CString& str, CStringArray& stra )
{

	// Adds "str" to "stra" if it 
	// doesn't already exist there
	BOOL found = FALSE;
	int max = stra.GetSize();
	for( int t = 0 ; t < max ; t++ )
	{
		CString cls = stra[ t ];
		if( str == cls || 
			CString( _T( "#" ) + str ) == cls || 
			str == CString( _T( "#" ) + cls ) )
			found = TRUE;
	}

	if( !found )
		stra.Add( str );

}

CString GetApplicationDirectory()
{

	// Returns the folder where the application
	// is installed

	TCHAR buffer[ _MAX_PATH + 1 ];
	::GetModuleFileName( NULL, buffer, _MAX_PATH );
	CString path( buffer );

	int find = path.ReverseFind( _TCHAR( '\\' ) );
	if( find != -1 )
		path = path.Left( find );

	path += _TCHAR( '\\' );

	return path;

}

