Option Explicit

'----------------------------------------------------------------------
' よく使うオブジェクトの生成
'----------------------------------------------------------------------
Dim g_objFS, g_objShell
Set g_objFS = WScript.CreateObject("Scripting.FileSystemObject")
Set g_objShell = WScript.CreateObject("WScript.Shell")


Call Main	'この関数を起点に処理を記述していく


'----------------------------------------------------------------------
' メインルーチン
'----------------------------------------------------------------------
Sub Main
	Dim objTsIn
	Dim strLine
	
	Set objTsIn = WScript.StdIn
	
	Do Until objTsIn.AtEndOfStream
		strLine = objTsIn.ReadLine

		Dim objLnk
		Set objLnk = g_objShell.CreateShortcut(strLine)

		If UCase(g_objFS.GetDriveName(objLnk.TargetPath)) = "\\ts00\share" Then
			WScript.Echo String(64, "-")
			WScript.Echo "ファイル名  : " & strLine
			WScript.Echo "リンク先    : " & objLnk.TargetPath
			WScript.Echo "作業フォルダ: " & objLnk.WorkingDirectory
			''WScript.Echo "       DriveName: " & g_objFS.GetDriveName(objLnk.TargetPath)
		End If
		
		Set objLnk = Nothing

	Loop
End Sub

