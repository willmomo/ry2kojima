' Script Skeleton

Option Explicit

'----------------------------------------------------------------------------
' �X�N���v�g�̎��s��cscript�ɐ�������
'----------------------------------------------------------------------------
Sub CScriptOnly
	Dim fso: Set fso = WScript.CreateObject("Scripting.FileSystemObject")
	If LCase(fso.GetFileName(WScript.FullName)) <> "cscript.exe" Then
		WScript.Echo "This script only works on CSCRIPT.EXE."
		WScript.Quit 255
	End If
End Sub

'----------------------------------------------------------------------------
' �v���O�����X�^�[�g
'----------------------------------------------------------------------------
Function DoMain
	'''	CScriptOnly	'cscript�������������Ȃ��Ƃ��ɗL���ɂ���

	DoMain = 0
End Function

'============================================================================
On Error Resume Next
Dim ret: ret = DoMain
If Err.Number <> 0 Then ret = Err.Number
WScript.Quit ret
