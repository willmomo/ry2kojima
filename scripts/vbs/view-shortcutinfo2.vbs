Option Explicit

'----------------------------------------------------------------------
' �悭�g���I�u�W�F�N�g�̐���
'----------------------------------------------------------------------
Dim g_objFS, g_objShell
Set g_objFS = WScript.CreateObject("Scripting.FileSystemObject")
Set g_objShell = WScript.CreateObject("WScript.Shell")


Call Main	'���̊֐����N�_�ɏ������L�q���Ă���


'----------------------------------------------------------------------
' ���C�����[�`��
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
			WScript.Echo "�t�@�C����  : " & strLine
			WScript.Echo "�����N��    : " & objLnk.TargetPath
			WScript.Echo "��ƃt�H���_: " & objLnk.WorkingDirectory
			''WScript.Echo "       DriveName: " & g_objFS.GetDriveName(objLnk.TargetPath)
		End If
		
		Set objLnk = Nothing

	Loop
End Sub

