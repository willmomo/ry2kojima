'----------------------------------------------------------------------
' Windows Installer �̃o�[�W�������`�F�b�N����X�N���v�g
'----------------------------------------------------------------------
Option Explicit

Dim result

If GetOSVersion > 5.0 Then
	On Error Resume Next
	result = CheckWindowsInstallerVersion
	If Err.Number = 0 Then
		'���폈��
		If result Then
			WScript.Echo "Windows Installer 3.1 �ȏオ�C���X�g�[���ς݁B"
		Else
			WScript.Echo "Windows Installer 3.1 �ȏ���C���X�g�[�����Ă��������B"
		End If
	Else
		'�ُ폈��
		If Err.Number > 0 Then
			WScript.Echo "Windows Installer �̃o�[�W������񂪎擾�ł��܂���B" & vbNewLine & _
				Err.Number & " : " & Err.Description
		Else
			WScript.Echo "Windows Installer �̃o�[�W������񂪎擾�ł��܂���B" & vbNewLine & _
				Hex(Err.Number) & " : [" & Err.Description & "]"
		End If
	End If
	On Error Goto 0
Else
	WScript.Echo "Windows XP �����Ȃ̂Ń`�F�b�N���܂���B"
End If

'----------------------------------------------------------------------
' Windows Installer �̃o�[�W������3.1�ȏォ�ǂ������ׂ�
' �Œ�I�� C:\windows\system32\msi.dll �̃o�[�W�����𒲂ׂĂ���̂�
' Windows XP 32bit �݂̂ł̎g�p���֎~
'----------------------------------------------------------------------
Function CheckWindowsInstallerVersion()
	CheckWindowsInstallerVersion = False
	Dim fso, fname, version, vers
	Set fso = WScript.CreateObject("Scripting.FileSystemObject")
	fname = "C:\windows\system32\msi.dll"
	version = fso.GetFileVersion(fname)

	WScript.Echo "Version: " & version

	vers = Split(version, ".")
	If (vers(0) > 3) Or (vers(0) = 3 And vers(1) >= 1) Then
		CheckWindowsInstallerVersion = True
	End If
End Function

'----------------------------------------------------------------------
' OS�̃o�[�W�������擾
' 5.0 = Windows 2000
' 5.1 = Windows XP
' 6.1 = Windows 7
'----------------------------------------------------------------------
Function GetOSVersion()
	Dim wmi, target, row, version, vers, computer
	computer = "."
	Set wmi = GetObject("winmgmts:{impersonationLevel=impersonate}!\\" & computer & "\root\cimv2")
	Set target = wmi.ExecQuery("select Version from Win32_OperatingSystem")
	For Each row In target
		version = row.Version
	Next
	vers = Split(version, ".")
	GetOSVersion = CDbl(vers(0) & "." & vers(1))
	
	'WScript.Echo "OS: " & GetOSVersion
End Function
