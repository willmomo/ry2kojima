Option Explicit

Const vbHide = 0				'ウィンドウを非表示にします
Const vbNormalFocus = 1			'通常のウィンドウ、かつ最前面のウィンドウにします
Const vbMinimizedFocus = 2		'最小化、かつ最前面のウィンドウにします
Const vbMaximizedFocus = 3		'最大化、かつ最前面のウィンドウにします
Const vbNormalNoFocus = 4		'通常のウィンドウです。ただし、最前面にはなりません
Const vbMinimizedNoFocus = 6	'最小化します。ただし、最前面にはなりません

Function WshShell_Run(strCommand, intWindowStyle, bWaitOnReturn)
	Dim oShell
	Set oShell = WScript.CreateObject("WScript.Shell")
	WshShell_Run = oShell.Run(strCommand, intWindowStyle, bWaitOnReturn)
	Set oShell = Nothing
End Function


