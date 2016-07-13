'----------------------------------------------------------------------
' #include ìIèàóùÇÇ∑ÇÈÇΩÇﬂÇÃ code snippet
' 2015/04/28 çÏê¨
'----------------------------------------------------------------------
Option Explicit

Include "kjmlog.vbs"

'**********************************************************************
Sub Include(fname)
	Dim fso: Set fso = WScript.CreateObject("Scripting.FileSystemObject")
	Dim ts: Set ts = fso.OpenTextFile(fso.BuildPath(fso.GetParentFolderName(WScript.ScriptFullName), fname), 1)	'1=ForReading
	ExecuteGlobal ts.ReadAll: ts.Close
End Sub
'**********************************************************************

GetKjmLog.SetLogPath_App "log_inc"
GetKjmLog.Info "RotateSize: " & GetKjmLog.RotateSize
GetKjmLog.Info "RotateCount: " & GetKjmLog.RotateCount
