'----------------------------------------------------------------------
'----------------------------------------------------------------------
Option Explicit

GetFileLog.SetLogPathToScriptFolder "log"
GetFileLog.LogIdent = "DFR"
GetFileLog.LogWrite "---------- " & WScript.ScriptName & " start ----------"

Dim ret: ret = DoMain

GetFileLog.LogWrite "---------- " & WScript.ScriptName & " end ----------"

'�I�����[���𑗐M
'SendJobMail "�f�t���O", "����I��"

WScript.Quit ret

'======================================================================

'----------------------------------------------------------------------
'----------------------------------------------------------------------
Function LogDefragError(cmdLine, errorLevel)
	Dim shell: Set shell = WScript.CreateObject("WScript.Shell")
	
	Select Case errorLevel
	Case 0
		shell.LogEvent 0, cmdLine & " : " & errorLevel & " : " & "�����A�G���[�Ȃ�"
	Case 1
		shell.LogEvent 1, cmdLine & " : " & errorLevel & " : " & "���[�U�[�����~�A�L�����Z��"
	Case 2
		shell.LogEvent 1, cmdLine & " : " & errorLevel & " : " & "�s�ǂȃp�����[�^�A�\���G���["
	Case 3
		shell.LogEvent 1, cmdLine & " : " & errorLevel & " : " & "�\�����Ȃ��A�s���ȃG���["
	Case 4
		shell.LogEvent 1, cmdLine & " : " & errorLevel & " : " & "�������s����� (RAM)"
	Case 5
		shell.LogEvent 1, cmdLine & " : " & errorLevel & " : " & "���g�p (����`) �́A��ʓI�G���["
	Case 6
		shell.LogEvent 1, cmdLine & " : " & errorLevel & " : " & "�V�X�e���AACL�A�t�@�C���̕s��v�A�V�X�e���S�̂̃G���["
	Case 6
		shell.LogEvent 1, cmdLine & " : " & errorLevel & " : " & "�f�B�X�N�e�ʕs���A���݂̃{�����[���̋󂫗e�ʂ� 15% ����"
	Case Else
		shell.LogEvent 1, cmdLine & " : " & errorLevel & " : " & "���m�̃G���["
	End Select
End Function

'----------------------------------------------------------------------
' �f�t���O���s
'----------------------------------------------------------------------
Function Defrag(driveLetter)
	Dim shell: Set shell = WScript.CreateObject("WScript.Shell")
	Dim cmdLine
	Dim ret
	
	cmdLine = "defrag " & driveLetter & ": /U /A"
	GetFileLog.LogWrite "start: " & cmdLine
	ret = shell.Run(cmdLine, 1, True)
	GetFileLog.LogWrite "end: " & ret
	LogDefragError cmdLine, ret
	If ret = 0 Then
		cmdLine = "defrag " & driveLetter & ": /U   "
		GetFileLog.LogWrite "start: " & cmdLine
		ret = shell.Run(cmdLine, 1, True)
		GetFileLog.LogWrite "end: " & ret
		LogDefragError cmdLine, ret
		
		If ret = 0 Then
			cmdLine = "defrag " & driveLetter & ": /U /X"
			GetFileLog.LogWrite "start: " & cmdLine
			ret = shell.Run(cmdLine, 1, True)
			GetFileLog.LogWrite "end: " & ret
			LogDefragError cmdLine, ret
		End If
	End If
	
	Defrag = ret
End Function

'----------------------------------------------------------------------
' �v���O���� �X�^�[�g
'----------------------------------------------------------------------
Function DoMain
	GetFileLog.LogWrite "job:defragall"

	' �h���C�u���
	Const Fixed = 2
	Dim fso: Set fso = WScript.CreateObject("Scripting.FileSystemObject")
	Dim obj
	For Each obj In fso.Drives
		If obj.DriveType = Fixed Then
			Defrag obj.DriveLetter
		End If
	Next
	DoMain = 0
End Function
