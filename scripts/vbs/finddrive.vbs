Option Explicit

'���s�T���v��
Dim drv: Set drv = FindDrive("GNAVI_DISK_IMAGES_BACKUP")
If Not IsEmpty(drv) Then WScript.Echo("found on " & drv.DriveLetter & " drive.")

'----------------------------------------------------------------------
' ���� expr �Ŏw�肵���t�@�C�������������A��ԍŏ���Drive�I�u�W�F�N�g��Ԃ��B
' �h���C�u��������Ȃ��Ƃ��́AEmpty��Ԃ�
'
' 2015/09/11 r.kojima
'----------------------------------------------------------------------
Function FindDrive(expr)
	Dim fso: Set fso = WScript.CreateObject("Scripting.FileSystemObject")
	Dim drv
	For Each drv In fso.Drives
		If fso.FileExists(fso.BuildPath(drv.DriveLetter & ":\", expr)) Then
			Set FindDrive = drv
			Exit For
		End If
	Next
End Function
