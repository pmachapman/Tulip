#ifndef _FLOWCHARTLINK_H_
#define _FLOWCHARTLINK_H_

// Link types
#define LINK_LEFT	1
#define LINK_RIGHT	2
#define LINK_TOP	4
#define LINK_BOTTOM 8

#define LINK_ALL	15

#define LINK_START	16
#define LINK_END	32

class CFlowchartLink : public CObject
{

public:
	// Creation/destruction
	CFlowchartLink();
	virtual ~CFlowchartLink();

	static int ReverseLink(int link);

	// Implementation
	virtual CString GetString() const;
	virtual CFlowchartLink* Clone() const;
	virtual BOOL FromString(const CString& str);

	// Data
	int		fromtype;	// Type of the from-part of the link
	int		totype;	// Type of the to-part.
	CString title;		// Title of the link, if any.
	CString	from;		// Name of the attached from-object.
	CString	to;			// Name of the attached to-object, if any.

};

#endif // _FLOWCHARTLINK_H_
