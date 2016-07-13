'------------------------------------------------------------------------------
' ショートカットの情報を表示するスクリプト
'
' 2011/06/10 r.kojima
'------------------------------------------------------------------------------
Option Explicit

Call Main

'------------------------------------------------------------------------------
' 事実上の main
'------------------------------------------------------------------------------
Sub Main
	Dim i

	For i = 0 To WScript.Arguments.Count - 1
		WScript.Echo "----( " & WScript.Arguments(i) & " )----"
		WScript.Echo CreateMessageText(WScript.Arguments(i))
	Next
End Sub

'------------------------------------------------------------------------------
' 表示用メッセージの作成
'------------------------------------------------------------------------------
Function CreateMessageText(fname)
	Dim objLnk

	Set objLnk = WScript.CreateObject("WScript.Shell").CreateShortcut(fname)
	CreateMessageText = CreateMessageText & _
		"'" & WScript.CreateObject("Scripting.FileSystemObject").GetFileName(objLnk.FullName) & "' のプロパティ" & vbNewLine & _
		"          リンク先: [" & objLnk.TargetPath & "]" & vbNewLine & _
		"              引数: [" & objLnk.Arguments & "]" & vbNewLine & _
		"      作業フォルダ: [" & objLnk.WorkingDirectory & "]" & vbNewLine & _
		"ショートカットキー: [" & HotkeyToText(objLnk.Hotkey) & "]" & vbNewLine & _
		"    実行時の大きさ: [" & WindowStyleToText(objLnk.WindowStyle) & "]" & vbNewLine & _
		"          コメント: [" & objLnk.Description & "]" & vbNewLine & _
		"          アイコン: [" & objLnk.IconLocation & "]"
End Function

'------------------------------------------------------------------------------
' ウィンドウスタイルの値をわかりやすい文字列に変換
'------------------------------------------------------------------------------
Function WindowStyleToText(windowStyle)
	Select Case windowStyle
	Case 1
		WindowStyleToText = "通常のウィンドウ"
	Case 3
		WindowStyleToText = "最大化"
	Case 7
		WindowStyleToText = "最小化"
	Case Else
		WindowStyleToText = windowStyle
	End Select
End Function

'------------------------------------------------------------------------------
' Hotkeyの値をわかりやすい文字列に変換
'------------------------------------------------------------------------------
Function HotkeyToText(hotkey)
	If hotkey = "" Then
		HotkeyToText = "なし"
	Else
		HotkeyToText = hotkey
	End If
End Function
