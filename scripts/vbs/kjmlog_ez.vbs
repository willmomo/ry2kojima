Option Explicit


WScript.Quit(TestMain)
Function TestMain()
	LogWrite "hoge"
End Function

'==============================================================================

'コードをコピペして、とにかく簡単にログを残すための関数群
'2015/10/07 new r.kojima

'------------------------------------------------------------------------------
' ログを書く
'------------------------------------------------------------------------------
Sub LogWrite(msg)
	Const ForAppending = 8
	Const vbLongDate = 1
	Const vbLongTime = 3
	Const TemporaryFolder = 2
	Dim fso: Set fso = WScript.CreateObject("Scripting.FileSystemObject")
	Dim log: Set log = fso.OpenTextFile(fso.Buildpath(fso.GetSpecialFolder(TemporaryFolder), fso.GetBaseName(WScript.ScriptFullName) & ".log"), ForAppending, True)
	Dim dt: dt = Now
	log.WriteLine FormatDateTime(dt, vbLongDate) & " "& FormatDateTime(dt, vbLongTime) & " " & msg
	log.Close
	WScript.Echo FormatDateTime(dt, vbLongDate) & " "& FormatDateTime(dt, vbLongTime) & " " & msg
	LogRotate
End Sub

'------------------------------------------------------------------------------
' ログをシフトする
'------------------------------------------------------------------------------
Sub LogRotate()
	Const TemporaryFolder = 2
	Dim fso: Set fso = WScript.CreateObject("Scripting.FileSystemObject")
	Dim fname: fname = fso.Buildpath(fso.GetSpecialFolder(TemporaryFolder), fso.GetBaseName(WScript.ScriptFullName) & ".log")
	If fso.FileExists(fname) Then
		If fso.GetFile(fname).Size > (1 * 1024 * 1024) Then
			Dim n
			For n = 8 To 1 Step -1
				Dim fname1: fname1 = fso.Buildpath(fso.GetSpecialFolder(TemporaryFolder), fso.GetBaseName(WScript.ScriptFullName) & "_" & n & ".log")
				Dim fname2: fname2 = fso.Buildpath(fso.GetSpecialFolder(TemporaryFolder), fso.GetBaseName(WScript.ScriptFullName) & "_" & (n + 1) & ".log")
				If fso.FileExists(fname1) Then
					If fso.FileExists(fname2) Then
						fso.DeleteFile fname2, True
					End If
					fso.MoveFile fname1, fname2
				End If
			Next
			fso.MoveFile fname, fso.Buildpath(fso.GetSpecialFolder(TemporaryFolder), fso.GetBaseName(WScript.ScriptFullName) & "_1.log")
		End If
	End If
End Sub
