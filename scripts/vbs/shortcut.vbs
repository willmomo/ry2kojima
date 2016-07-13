'------------------------------------------------------------------------------
' �V���[�g�J�b�g���쐬����X�N���v�g
'
' 2011/06/10 r.kojima
' 2012/09/12 r.kojima targetPath��SpecialFolders�W�J�@�\��ǉ��B
'   ?{...}�Ǝw�肷�邱�ƂŁA����t�H���_�̎w�肪�ł���B
' 2015/09/10 r.kojima wscript�œ�������悤�Ƀ��b�Z�[�W��}�~�B
'------------------------------------------------------------------------------
Option Explicit

WScript.Quit(Main)

'------------------------------------------------------------------------------
' ������� main
'------------------------------------------------------------------------------
Function Main
	Dim i, c, o
	Dim objLnk
	Dim linkName	'�쐬���郊���N�t�@�C���̖��O
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
	
	linkName = ReplaceSpecialFolder(linkName)
	targetPath = ReplaceSpecialFolder(targetPath)
	workingDirectory = ReplaceSpecialFolder(workingDirectory)

'�쐬�O�̃��b�Z�[�W��}�~
'	WScript.Echo "'" & linkName & "' ���쐬���܂��B"
'	WScript.Echo _
'		"          �����N��: [" & targetPath & "]" & vbNewLine & _
'		"              ����: [" & arguments & "]" & vbNewLine & _
'		"      ��ƃt�H���_: [" & workingDirectory & "]" & vbNewLine & _
'		"          �R�����g: [" & description & "]" & vbNewLine
	
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
' �g�p�@�̕\��
'------------------------------------------------------------------------------
Sub Usage
	Dim t
	
	t = t & "usage: " & WScript.ScriptName & " <�����N�t�@�C����> -l <�����N��> [-w <��ƃt�H���_>] [-a <����>]... [-d <�R�����g>]" & vbNewLine
	t = t & "" & vbNewLine
	t = t & " �V���[�g�J�b�g�Ɉ����𕡐��w�肷��Ƃ��́A-a �𕡐��L�q���Ă��������B" & vbNewLine
	t = t & " ��) -a -i -a ""C:\Documents and Settings\All Users""" & vbNewLine
	t = t & "" & vbNewLine
	t = t & " �����N�t�@�C�����ɂ́A����t�H���_���ȉ��̗p�Ɏw�肷�邱�Ƃ��ł���B" & vbNewLine
	t = t & " ��) ?{AllUsersStartup}\myprog.lnk" & vbNewLine
	t = t & " ��) ?{Startup}\myprog.lnk" & vbNewLine
	t = t & " �w��\�ȕ�����́AWshShell.SpecialFolders �̃w���v���Q�ƁB" & vbNewLine
	t = t & "" & vbNewLine
	WScript.Echo t
End Sub

'------------------------------------------------------------------------------
' ������̒��Ɋ܂܂�� ?{...} �̌`�Ŏw�肳�ꂽ special folder ��
' ���ۂ̃p�X�Ƀ��v���[�X����B
' OS�Ɉˑ����܂����A�����悻�ȉ��̂��̂��g����F
' AllUsersDesktop, AllUsersStartMenu, AllUsersPrograms, AllUsersStartup, 
' Desktop, Favorites, Fonts, MyDocuments, NetHood, PrintHood, Programs, Recent
' SendTo, StartMenu, Startup, Templates
'------------------------------------------------------------------------------
Function ReplaceSpecialFolder(strOrg)
	Dim strResult, objShell, objRe, objMatches

	strResult = strOrg

	Set objShell = WScript.CreateObject("WScript.Shell")
	Set objRe = New RegExp

	objRe.Pattern = "\?{(.+?)}"
	objRe.IgnoreCase = True
	objRe.Global = False

	Do While True
		Set objMatches = objRe.Execute(strResult)

		If objMatches.Count = 1 Then
			If objMatches(0).SubMatches.Count = 1 Then
				strResult = Replace(strResult, "?{" & objMatches(0).SubMatches(0) & "}", objShell.SpecialFolders(objMatches(0).SubMatches(0)))
			End If
		Else
			Exit Do
		End If
	Loop

	ReplaceSpecialFolder = strResult
End Function
