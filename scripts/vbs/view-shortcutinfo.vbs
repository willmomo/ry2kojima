Option Explicit

'----------------------------------------------------------------------
' �悭�g���I�u�W�F�N�g�̐���
'----------------------------------------------------------------------
Dim g_objFS, g_objShell
Set g_objFS = WScript.CreateObject("Scripting.FileSystemObject")
Set g_objShell = WScript.CreateObject("WScript.Shell")


Call Main	'���̊֐����N�_�ɏ������L�q���Ă���


'----------------------------------------------------------------------
' �����I��cscript.exe�œ����������Ƃ��́A���̊֐����Ă�
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
' �����̃`�F�b�N
'----------------------------------------------------------------------
Function CheckArguments
	Dim i
	For i = 0 To WScript.Arguments.Count - 1
		WScript.Echo WScript.Arguments.Item(i)
	Next
	WScript.Quit
	
	If WScript.Arguments.Count = 0 Then
		WScript.Echo "�g�p�@�F" & WScript.ScriptName & " <�����N�t�@�C����>"
		CheckArguments = False
		Exit Function
	End If
	
	CheckArguments = True
End Function


'----------------------------------------------------------------------
' ���C�����[�`��
'----------------------------------------------------------------------
Sub Main
	Dim objShortcut, strMsg

	'����cscript���s
	If ForceRunCScript Then
		WScript.Echo "�I���܂�"
		WScript.Quit
	End If
	
	'�����̊m�F
	If Not CheckArguments Then
		WScript.Quit
	End If
	
	'�V���[�g�J�b�g���쐬
	Set objShortcut = g_objShell.CreateShortcut("D:\home\kojima\work\wod.xls �ւ̃V���[�g�J�b�g.lnk")

	strMsg = objShortcut.FullName
	strMsg = strMsg & vbNewLine & "�A�C�R��(IconLocation): " & objShortcut.IconLocation
	strMsg = strMsg & vbNewLine & "�����N��(TargetPath): " & objShortcut.TargetPath
	strMsg = strMsg & vbNewLine & "(Arguments): " & objShortcut.Arguments
	strMsg = strMsg & vbNewLine & "��ƃt�H���_(WorkingDirectory): " & objShortcut.WorkingDirectory
	strMsg = strMsg & vbNewLine & "�V���[�g�J�b�g�L�[(Hotkey): " & objShortcut.Hotkey
	
	Select Case objShortcut.WindowStyle
	Case 1
		strMsg = strMsg & vbNewLine & "���s���̑傫��(WindowStyle): �ʏ�̃E�B���h�E (" & objShortcut.WindowStyle & ")"
	Case 3
		strMsg = strMsg & vbNewLine & "���s���̑傫��(WindowStyle): �ő剻 (" & objShortcut.WindowStyle & ")"
	Case 7
		strMsg = strMsg & vbNewLine & "���s���̑傫��(WindowStyle): �ŏ��� (" & objShortcut.WindowStyle & ")"
	Case Else
		strMsg = strMsg & vbNewLine & "���s���̑傫��(WindowStyle): ����`�l (" & objShortcut.WindowStyle & ")"
	End Select
	
	strMsg = strMsg & vbNewLine & "�R�����g(Description): " & objShortcut.Description
	
	WScript.Echo strMsg
End Sub

