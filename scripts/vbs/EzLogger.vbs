'------------------------------------------------------------------------------
' �R�s�y���Ďg�����ȒP���O����
'------------------------------------------------------------------------------
'------------------------------------------------------------------------------
' ���O������
' Log_GetPath LogWrite �̃R���r
'------------------------------------------------------------------------------
Function Log_GetPath
	Dim fso: Set fso = WScript.CreateObject("Scripting.FileSystemObject")

	' %TEMP% �Ƀ��O�����Ƃ��́A������̃R�[�h��L���ɂ���
	'Const TemporaryFolder = 2
	'Log_GetPath = fso.GetSpecialFolder(TemporaryFolder)

	' �X�N���v�g�̏ꏊ\Log\YYYYMMDD �̒��Ƀ��O�����Ƃ��́A������̃R�[�h��L���ɂ���
	Log_GetPath = fso.BuildPath(fso.GetParentFolderName(WScript.ScriptFullName), "Log")
	If Not fso.FolderExists(Log_GetPath) Then fso.CreateFolder(Log_GetPath)
	Log_GetPath = fso.BuildPath(Log_GetPath, Year(Now) & Right("00" & Month(Now), 2) & Right("00" & Day(Now), 2))
	If Not fso.FolderExists(Log_GetPath) Then fso.CreateFolder(Log_GetPath)
End Function

Sub LogWrite(msg)
	On Error Resume Next
	Dim fso: Set fso = WScript.CreateObject("Scripting.FileSystemObject")
	Const ForApending = 8
	Const RotateSize = 1048576	'1M=1048576 128K=131072 4K=4096
	Const LogRotate = 9			'�c�����㐔(.???)�ɂȂ�
	Dim fname, tfile, i, fnameSrc, fnameDst, retry
	fname = fso.BuildPath(Log_GetPath, fso.GetBaseName(WScript.ScriptName) & ".log")
	For retry = 0 To 9
		Err.Clear
		Set tfile = fso.OpenTextFile(fname, ForApending, True)
		If Err.Number = 0 Then Exit For
		WScript.Sleep(50)
	Next
	If Not IsEmpty(tfile) Then
		tfile.WriteLine(Now & " " & msg)
		tfile.Close
		WScript.StdOut.WriteLine msg
		If fso.GetFile(fname).Size > RotateSize Then
			fnameDst = fso.BuildPath(fso.GetParentFolderName(fname), fso.GetBaseName(fname) & ".log." & LogRotate)
			If fso.FileExists(fnameDst) Then fso.DeleteFile fnameDst
			For i = (LogRotate - 1) to 1 Step -1
				fnameSrc = fso.BuildPath(fso.GetParentFolderName(fname), fso.GetBaseName(fname) & ".log." & i)
				fnameDst = fso.BuildPath(fso.GetParentFolderName(fname), fso.GetBaseName(fname) & ".log." & (i + 1))
				If fso.FileExists(fnameSrc) Then fso.MoveFile fnameSrc, fnameDst
			Next
			fso.MoveFile fname, fso.BuildPath(fso.GetParentFolderName(fname), fso.GetBaseName(fname) & ".log.1")
		End If
	Else
		'���E�܂Ń��g���C������A�ǂ����悤���Ȃ��̂ł�����߂�
	End If
End Sub

'------------------------------------------------------------------------------
Function TestMain
	Dim counter
	For counter = 1 To 100000
		LogWrite "counter: " & counter
		'WScript.Sleep(0)
	Next
	TestMain = 0
End Function

LogWrite "$ ********** [ " & WScript.ScriptFullName & " ] �J�n"

TestMain

LogWrite "$ ********** [ " & WScript.ScriptFullName & " ] �I��"
