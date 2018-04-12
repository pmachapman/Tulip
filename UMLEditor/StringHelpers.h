#ifndef _STRINGHELPERS_H_
#define _STRINGHELPERS_H_

#define STRING_FORMAT_SAVE				0
#define STRING_FORMAT_CPP				1
#define STRING_FORMAT_UML				2
#define STRING_FORMAT_H					3
#define STRING_FORMAT_HTML				4
#define STRING_FORMAT_H_CTOR			5
#define STRING_FORMAT_H_TEMPLATE		6
#define STRING_FORMAT_H_CTOR_TEMPLATE	7

/*

	STRING_FORMAT_SAVE				For saving to file
	STRING_FORMAT_CPP				cpp-file format
	STRING_FORMAT_UML				UML-format
	STRING_FORMAT_H					h-file format
	STRING_FORMAT_HTML				HTML-format
	STRING_FORMAT_H_CTOR			ctor in a header
	STRING_FORMAT_H_TEMPLATE		h-file format for template classes
	STRING_FORMAT_H_CTOR_TEMPLATE	ctor in a header template class

*/

void	MakeSaveString( CString& str );
void	UnmakeSaveString( CString& str );
CString	ColorrefToString( COLORREF col );
void	AddString( const CString& str, CStringArray& stra );
CString	GetApplicationDirectory();

#endif // _STRINGHELPERS_H_