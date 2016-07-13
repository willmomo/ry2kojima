'------------------------------------------------------------------------------
' �V���[�g�J�b�g�̏���\������X�N���v�g
'
' 2011/06/10 r.kojima
'------------------------------------------------------------------------------
Option Explicit

Call Main

'------------------------------------------------------------------------------
' ������� main
'------------------------------------------------------------------------------
Sub Main
	Dim i

	For i = 0 To WScript.Arguments.Count - 1
		WScript.Echo "----( " & WScript.Arguments(i) & " )----"
		WScript.Echo CreateMessageText(WScript.Arguments(i))
	Next
End Sub

'------------------------------------------------------------------------------
' �\���p���b�Z�[�W�̍쐬
'------------------------------------------------------------------------------
Function CreateMessageText(fname)
	Dim objLnk

	Set objLnk = WScript.CreateObject("WScript.Shell").CreateShortcut(fname)
	CreateMessageText = CreateMessageText & _
		"'" & WScript.CreateObject("Scripting.FileSystemObject").GetFileName(objLnk.FullName) & "' �̃v���p�e�B" & vbNewLine & _
		"          �����N��: [" & objLnk.TargetPath & "]" & vbNewLine & _
		"              ����: [" & objLnk.Arguments & "]" & vbNewLine & _
		"      ��ƃt�H���_: [" & objLnk.WorkingDirectory & "]" & vbNewLine & _
		"�V���[�g�J�b�g�L�[: [" & HotkeyToText(objLnk.Hotkey) & "]" & vbNewLine & _
		"    ���s���̑傫��: [" & WindowStyleToText(objLnk.WindowStyle) & "]" & vbNewLine & _
		"          �R�����g: [" & objLnk.Description & "]" & vbNewLine & _
		"          �A�C�R��: [" & objLnk.IconLocation & "]"
End Function

'------------------------------------------------------------------------------
' �E�B���h�E�X�^�C���̒l���킩��₷��������ɕϊ�
'------------------------------------------------------------------------------
Function WindowStyleToText(windowStyle)
	Select Case windowStyle
	Case 1
		WindowStyleToText = "�ʏ�̃E�B���h�E"
	Case 3
		WindowStyleToText = "�ő剻"
	Case 7
		WindowStyleToText = "�ŏ���"
	Case Else
		WindowStyleToText = windowStyle
	End Select
End Function

'------------------------------------------------------------------------------
' Hotkey�̒l���킩��₷��������ɕϊ�
'------------------------------------------------------------------------------
Function HotkeyToText(hotkey)
	If hotkey = "" Then
		HotkeyToText = "�Ȃ�"
	Else
		HotkeyToText = hotkey
	End If
End Function
