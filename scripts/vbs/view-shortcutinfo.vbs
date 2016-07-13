Option Explicit

'----------------------------------------------------------------------
' よく使うオブジェクトの生成
'----------------------------------------------------------------------
Dim g_objFS, g_objShell
Set g_objFS = WScript.CreateObject("Scripting.FileSystemObject")
Set g_objShell = WScript.CreateObject("WScript.Shell")


Call Main	'この関数を起点に処理を記述していく


'----------------------------------------------------------------------
' 強制的にcscript.exeで動かしたいときは、この関数を呼ぶ
'----------------------------------------------------------------------
Function ForceRunCScript
	Dim i, cmdLine
	
	If UCase(g_objFS.GetFileName(WScript.FullName)) <> "CSCRIPT.EXE" Then
		cmdLine = "cscript.exe "
		cmdLine = cmdLine & " """ & WScript.ScriptFullName & """"
		For i = 0 To WScript.Arguments.Count - 1
			cmdLine = cmdLine & " """ & WScript.Arguments.Item(i) & """"
		Next
		
		g_objShell.Run cmdLine, , True
		ForceRunCScript = True
	End If
	
	ForceRunCScript = False
End Function


'----------------------------------------------------------------------
' 引数のチェック
'----------------------------------------------------------------------
Function CheckArguments
	Dim i
	For i = 0 To WScript.Arguments.Count - 1
		WScript.Echo WScript.Arguments.Item(i)
	Next
	WScript.Quit
	
	If WScript.Arguments.Count = 0 Then
		WScript.Echo "使用法：" & WScript.ScriptName & " <リンクファイル名>"
		CheckArguments = False
		Exit Function
	End If
	
	CheckArguments = True
End Function


'----------------------------------------------------------------------
' メインルーチン
'----------------------------------------------------------------------
Sub Main
	Dim objShortcut, strMsg

	'強制cscript実行
	If ForceRunCScript Then
		WScript.Echo "終わります"
		WScript.Quit
	End If
	
	'引数の確認
	If Not CheckArguments Then
		WScript.Quit
	End If
	
	'ショートカットを作成
	Set objShortcut = g_objShell.CreateShortcut("D:\home\kojima\work\wod.xls へのショートカット.lnk")

	strMsg = objShortcut.FullName
	strMsg = strMsg & vbNewLine & "アイコン(IconLocation): " & objShortcut.IconLocation
	strMsg = strMsg & vbNewLine & "リンク先(TargetPath): " & objShortcut.TargetPath
	strMsg = strMsg & vbNewLine & "(Arguments): " & objShortcut.Arguments
	strMsg = strMsg & vbNewLine & "作業フォルダ(WorkingDirectory): " & objShortcut.WorkingDirectory
	strMsg = strMsg & vbNewLine & "ショートカットキー(Hotkey): " & objShortcut.Hotkey
	
	Select Case objShortcut.WindowStyle
	Case 1
		strMsg = strMsg & vbNewLine & "実行時の大きさ(WindowStyle): 通常のウィンドウ (" & objShortcut.WindowStyle & ")"
	Case 3
		strMsg = strMsg & vbNewLine & "実行時の大きさ(WindowStyle): 最大化 (" & objShortcut.WindowStyle & ")"
	Case 7
		strMsg = strMsg & vbNewLine & "実行時の大きさ(WindowStyle): 最小化 (" & objShortcut.WindowStyle & ")"
	Case Else
		strMsg = strMsg & vbNewLine & "実行時の大きさ(WindowStyle): 未定義値 (" & objShortcut.WindowStyle & ")"
	End Select
	
	strMsg = strMsg & vbNewLine & "コメント(Description): " & objShortcut.Description
	
	WScript.Echo strMsg
End Sub

