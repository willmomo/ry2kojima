'------------------------------------------------------------------------------
' ショートカットを作成するスクリプト
'
' 2011/06/10 r.kojima
'------------------------------------------------------------------------------
Option Explicit

WScript.Quit(Main)

'------------------------------------------------------------------------------
' 事実上の main
'------------------------------------------------------------------------------
Function Main
	Dim i, c, o
	Dim objLnk
	Dim linkName	'作成するリンクファイルの名前
	Dim targetPath
	Dim workingDirectory
	Dim arguments
	Dim description
	Dim argText

	For i = 0 To WScript.Arguments.Count - 1
		c = Left(WScript.Arguments(i), 1)

		If c = "-" Then
			o = Mid(WScript.Arguments(i), 2, 1)
			If o = "l" Then
				targetPath = WScript.Arguments(i + 1)
				i = i + 1
			ElseIf o = "w" Then
				workingDirectory = WScript.Arguments(i + 1)
				i = i + 1
			ElseIf o = "a" Then
				If InStr(WScript.Arguments(i + 1), " ") = 0 Then
					argText = WScript.Arguments(i + 1)
				Else
					argText = """" & WScript.Arguments(i + 1) & """"
				End If
				
				If arguments = "" Then
					arguments = argText
				Else
					arguments = arguments & " " & argText
				End If
				i = i + 1
			ElseIf o = "d" Then
				description = WScript.Arguments(i + 1)
				i = i + 1
			Else
				Usage
				Main = -1
				Exit Function
			End If
		Else
			linkName = WScript.Arguments(i)
		End If
	Next

	If linkName = "" Or targetPath = "" Then
		Usage
		Main = -1
		Exit Function
	End If
	
	WScript.Echo "'" & linkName & "' を作成します。"
	WScript.Echo _
		"          リンク先: [" & targetPath & "]" & vbNewLine & _
		"              引数: [" & arguments & "]" & vbNewLine & _
		"      作業フォルダ: [" & workingDirectory & "]" & vbNewLine & _
		"          コメント: [" & description & "]" & vbNewLine
	
	Set objLnk = WScript.CreateObject("WScript.Shell").CreateShortcut(linkName)
	objLnk.TargetPath = targetPath
	If arguments <> "" Then objLnk.Arguments = arguments
	If workingDirectory <> "" Then objLnk.WorkingDirectory = workingDirectory
	If description <> "" Then objLnk.Description = description
	
	On Error Resume Next
	objLnk.Save
	If Err.Number <> 0 Then
		WScript.Echo "!! ERROR OCCURRED !!"
		WScript.Echo "Source     : " & Err.Source
		WScript.Echo "Number     : $" & Hex(Err.Number)
		WScript.Echo "Description: " & Err.Description
		Main = 1
	End If
End Function

'------------------------------------------------------------------------------
' 使用法の表示
'------------------------------------------------------------------------------
Sub Usage
	Dim t
	
	t = t & "usage: ShortcutCreate.vbs <リンクファイル名> -l <リンク先> [-w <作業フォルダ>] [-a <引数>]... [-d <コメント>]" & vbNewLine
	t = t & "" & vbNewLine
	t = t & " ショートカットに引数を複数指定するときは、-a を複数記述してください。" & vbNewLine
	t = t & " 例) -a -i -a ""C:\Documents and Settings\All Users""" & vbNewLine
	
	WScript.Echo t
End Sub
