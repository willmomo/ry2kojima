
'Const vbSunday = 1		'���j
'Const vbMonday = 2		'���j
'Const vbTuesday = 3		'�Ηj
'Const vbWednesday = 4	'���j
'Const vbThursday = 5	'�ؗj
'Const vbFriday = 6		'���j
'Const vbSaturday = 7	'�y�j

WScript.Echo Now
WScript.Echo Hour(Now)
WScript.Echo (vbMonday <= Weekday(Now) And Weekday(Now) <= vbFriday)
WScript.Echo (8 <= Hour(Now) And Hour(Now) < 18)

	'(���`�� and 8:00�`17:59)
WScript.Echo ((vbMonday <= Weekday(Now) And Weekday(Now) <= vbFriday) And (8 <= Hour(Now) And Hour(Now) < 18))


'(���`�� and 8:00�`16:59)�́A���s��ۗ�����B
fSilent = False
Do While ((vbMonday <= Weekday(Now) And Weekday(Now) <= vbFriday) And (8 <= Hour(Now) And Hour(Now) < 17))
	If Not fSilent Then
		WScript.Echo "(���s�ۗ�)"
		fSilent = True
	End If
	WScript.Sleep(5 * 1000)
Loop
