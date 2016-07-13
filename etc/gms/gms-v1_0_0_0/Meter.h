// Meter.h: rkMeter クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_METER_H__A7F3EA77_5E4E_442B_B6CB_C089EBA320D0__INCLUDED_)
#define AFX_METER_H__A7F3EA77_5E4E_442B_B6CB_C089EBA320D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>

class rkMeter  
{
private:
	RECT m_rc[20];
	HBRUSH m_hbr[3][2];
	HPEN m_hpen;

public:
	rkMeter();
	virtual ~rkMeter();

	int MeterLeft()
	{
		return m_rc[0].left;
	};
	int MeterTop()
	{
		return m_rc[0].top;
	};
	int MeterRight()
	{
		return m_rc[19].right;
	};
	int MeterBottom()
	{
		return m_rc[0].bottom;
	};
	int MeterHeight()
	{
		return m_rc[0].bottom - m_rc[0].top;
	};
	void Offset(int x, int y);
	BOOL DrawRect(HDC hdc, int n);
	BOOL DrawMeter(HDC hdc, int per);
};

#endif // !defined(AFX_METER_H__A7F3EA77_5E4E_442B_B6CB_C089EBA320D0__INCLUDED_)
