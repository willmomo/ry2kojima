'----------------------------------------------------------------------
' code snippet
'----------------------------------------------------------------------
'----------------------------------------------------------------------
' %TEMP%���擾����
'----------------------------------------------------------------------
Function GetTempPath()
	Const TemporaryFolder = 2	'GetSpecialFolder ���\�b�h�A���� option �̂��߂̕ϐ�
	Dim fso: Set fso = WScript.CreateObject("Scripting.FileSystemObject")
	GetTempPath = fso.GetSpecialFolder(TemporaryFolder)
End Function

'----------------------------------------------------------------------
' �t�@�C����������u��������
'----------------------------------------------------------------------
Function ChangeFileName(path, filename)
	Dim fso: Set fso = WScript.CreateObject("Scripting.FileSystemObject")
	ChangeFileName = fso.BuildPath(fso.GetParentFolderName(path), filename)
End Function

'----------------------------------------------------------------------
' ���O�ɏ�������
'----------------------------------------------------------------------
Sub WriteLog(msg)
	Const ForAppending = 8
	Dim fso: Set fso = WScript.CreateObject("Scripting.FileSystemObject")
	Dim path: path = GetTempPath
	Dim basename: basename = fso.GetBaseName(WScript.ScriptFullName)
	Dim ts: Set ts = fso.OpenTextFile(fso.BuildPath(path, basename) & ".log", ForAppending, True)
	ts.WriteLine(Now & " " & msg)
	ts.Close
End Sub

'----------------------------------------------------------------------
' �R�}���h�̎��s
'----------------------------------------------------------------------
Function Exec(strCommand)
	WriteLog(strCommand)
	Dim shl: Set shl = WScript.CreateObject("WScript.Shell")
	On Error Resume Next
	Exec = shl.Run(strCommand, , True)
	If Err.Number <> 0 Then
		Exec = Err.Number
		WriteLog("���s���G���[: " & Exec)
	End If
End Function

