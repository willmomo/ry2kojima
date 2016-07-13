#ifndef __X_H
#define __X_H

/////////////////////////////////////////////////////////////////////////////
//	RECTƒNƒ‰ƒX

class XRect : public RECT
{
public:
	XRect(){SetRectEmpty();};
	XRect(LONG l, LONG t, LONG r, LONG b){SetRect(l, t, r, b);};
	~XRect(){};
	
	LONG Width(){return right - left;};
	LONG Height(){return bottom - top;};

	// Suport rectangle function
	BOOL CopyRect(LPCRECT lprcSrc){return ::CopyRect(this, lprcSrc);};
	BOOL EqualRect(LPCRECT lprc2){return ::EqualRect(this, lprc2);};
	BOOL InflateRect(int dx, int dy){return ::InflateRect(this, dx, dy);};
	BOOL IntersectRect(LPCRECT lprcSrc1, LPCRECT lprcSrc2){
		return ::IntersectRect(this, lprcSrc1, lprcSrc2);
	};
	BOOL IsRectEmpty(){return ::IsRectEmpty(this);};
	BOOL OffsetRect(int dx, int dy){return ::OffsetRect(this, dx, dy);};
	BOOL PtInRect(POINT pt){return ::PtInRect(this, pt);};  
	BOOL SetRect(int xLeft, int yTop, int xRight, int yBottom){
		return ::SetRect(this, xLeft, yTop, xRight, yBottom);
	};
	BOOL SetRectEmpty(){return ::SetRectEmpty(this);};
	BOOL SubtractRect(LPCRECT lprcSrc1, LPCRECT lprcSrc2){
		return ::SubtractRect(this, lprcSrc1, lprcSrc2);
	};
	BOOL UnionRect(LPCRECT lprcSrc1, LPCRECT lprcSrc2){
		return ::UnionRect(this, lprcSrc1, lprcSrc2);
	};

	operator LPRECT(){return (LPRECT)this;};
	operator LPCRECT(){return (LPCRECT)this;};
};

#endif

