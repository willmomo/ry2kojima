
'Const vbSunday = 1		'“ú—j
'Const vbMonday = 2		'Œ—j
'Const vbTuesday = 3		'‰Î—j
'Const vbWednesday = 4	'…—j
'Const vbThursday = 5	'–Ø—j
'Const vbFriday = 6		'‹à—j
'Const vbSaturday = 7	'“y—j

WScript.Echo Now
WScript.Echo Hour(Now)
WScript.Echo (vbMonday <= Weekday(Now) And Weekday(Now) <= vbFriday)
WScript.Echo (8 <= Hour(Now) And Hour(Now) < 18)

	'(Œ`‹à and 8:00`17:59)
WScript.Echo ((vbMonday <= Weekday(Now) And Weekday(Now) <= vbFriday) And (8 <= Hour(Now) And Hour(Now) < 18))


'(Œ`‹à and 8:00`16:59)‚ÍAÀs‚ğ•Û—¯‚·‚éB
fSilent = False
Do While ((vbMonday <= Weekday(Now) And Weekday(Now) <= vbFriday) And (8 <= Hour(Now) And Hour(Now) < 17))
	If Not fSilent Then
		WScript.Echo "(Às•Û—¯)"
		fSilent = True
	End If
	WScript.Sleep(5 * 1000)
Loop
