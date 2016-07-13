' Script Skeleton

Option Explicit

'----------------------------------------------------------------------------
' スクリプトの実行をcscriptに制限する
'----------------------------------------------------------------------------
Sub CScriptOnly
	Dim fso: Set fso = WScript.CreateObject("Scripting.FileSystemObject")
	If LCase(fso.GetFileName(WScript.FullName)) <> "cscript.exe" Then
		WScript.Echo "This script only works on CSCRIPT.EXE."
		WScript.Quit 255
	End If
End Sub

'----------------------------------------------------------------------------
' テストファイルの作成
'----------------------------------------------------------------------------
Sub CreateTestFile
	Const ForAppend = 8
	
	Const TempoCode = "007"
	Const OutPath = "C:\ftp\csv2"
	
	Dim fso: Set fso = WScript.CreateObject("Scripting.FileSystemObject")
	Dim dt: dt = Now
	
	Dim fname: fname = TempoCode & Year(dt) & Right("00" & Month(dt), 2) & Right("00" & Day(dt), 2)
	Dim ts
	
	WScript.Echo Now & " Update Test File."
	
	Dim asFile: asFile = fso.BuildPath(OutPath, fname & "AS.csv")
	WScript.Echo Now & " Update [" & asFile & "]"
	Set ts = fso.OpenTextFile(asFile, ForAppend, True)
	ts.WriteLine dt
	ts.Close
	
	Dim zaFile: zaFile = fso.BuildPath(OutPath, fname & "ZA.csv")
	WScript.Echo Now & " Update [" & zaFile & "]"
	Set ts = fso.OpenTextFile(zaFile, ForAppend, True)
	ts.WriteLine dt
	ts.Close
End Sub

'----------------------------------------------------------------------------
' プログラムスタート
'----------------------------------------------------------------------------
Function DoMain
	CScriptOnly

	Do While True
		CreateTestFile
		WScript.Sleep(120 * 1000)
	Loop
	
	DoMain = 0
End Function

'============================================================================
'On Error Resume Next
Dim ret: ret = DoMain
If Err.Number <> 0 Then ret = Err.Number
WScript.Quit ret
