'----------------------------------------------------------------------
' Windows Installer �̃o�[�W������3.1�ȏォ�ǂ������ׂ�
' �Œ�I�� C:\windows\system32\msi.dll �̃o�[�W�����𒲂ׂĂ���̂�
' Windows XP 32bit �ł̂ݓ��삷��
'----------------------------------------------------------------------
Function CheckWindowsInstallerVersion()
	CheckWindowsInstallerVersion = False
	Dim fso, fname, version, vers
	Set fso = WScript.CreateObject("Scripting.FileSystemObject")
	'''fname = "C:\windows\system32\msi.dll"
	fname = "C:\Users\kojima\Documents\temp\51Lf_20091120\msi.dll"
	version = fso.GetFileVersion(fname)

	'MsgBox "[" & fname & "] file version: " & version

	If version = "3.1.4001.5512" Then
		msg = "SP3��Windows Installer�ł��B"
	ElseIf version = "3.1.4000.2435" Then
		msg = "Windows Installer 3.1 ��Installer�ł��B"
	ElseIf version = "3.0.3790.2180" Then
		msg = "SP2�܂ł� Windows Installer �ł��B"
	ElseIf version = "4.5.6001.22159" Then
		msg = "Windows Installer 4.5 �ł��B"
	Else
		msg = version & " �́A�m��Ȃ��o�[�W�����ł��B"
	End If
	
	MsgBox fname & " �̃o�[�W����" & vbNewLine & vbNewLine & "file version: " & version & vbNewLine & vbNewLine & msg,, fname & " �̃o�[�W����"
		
	vers = Split(version, ".")
	If (vers(0) > 3) Or (vers(0) = 3 And vers(1) >= 1) Then
		CheckWindowsInstallerVersion = True
	End If
End Function


CheckWindowsInstallerVersion

