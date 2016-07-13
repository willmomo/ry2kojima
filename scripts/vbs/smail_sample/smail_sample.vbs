Option Explicit

Const TO_ADDR = "ry2kojima@sanko-net.co.jp"
Const SUBJECT = "�����[�g�T�|�[�g�Ǘ��䒠"
Const BODY = "�����[�g�T�|�[�g�Ǘ��䒠"
Const ATTACH_FILE = "\\TS00\share\@�J��\@GN�X�ܐڑ�����\�����[�g�T�|�[�g�Ǘ��䒠(SK��GN)..xls"

'----------------------------------------------------------------------
' %TEMP%���擾����
'----------------------------------------------------------------------
Function GetTempPath()
	Const TemporaryFolder = 2	'GetSpecialFolder ���\�b�h�A���� option �̂��߂̕ϐ�
	Dim fso: Set fso = WScript.CreateObject("Scripting.FileSystemObject")
	GetTempPath = fso.GetSpecialFolder(TemporaryFolder)
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

'----------------------------------------------------------------------
' ���M�ҏ��(from)�̍폜
'----------------------------------------------------------------------
Sub RemoveFromAddr()
	Dim fso: Set fso = WScript.CreateObject("Scripting.FileSystemObject")
	Dim basename: basename = fso.GetBaseName(WScript.ScriptFullName)
	Dim fname: fname = fso.BuildPath(GetTempPath, basename & "_from.txt")
	
	If fso.FileExists(fname) Then fso.DeleteFile(fname)
End Sub

'----------------------------------------------------------------------
' ���M�ҏ��(from)��ǂݍ���
'----------------------------------------------------------------------
Function ReadFromAddr()
	Dim fso: Set fso = WScript.CreateObject("Scripting.FileSystemObject")
	Dim basename: basename = fso.GetBaseName(WScript.ScriptFullName)
	Dim fname: fname = fso.BuildPath(GetTempPath, basename & "_from.txt")
	Dim ts
	Dim fromAddr
	
	If Not fso.FileExists(fname) Then
		fromAddr = InputBox("���Ȃ��̃��[���A�h���X����͂��Ă��������B", basename)
		Set ts = fso.CreateTextFile(fname)
		ts.WriteLine fromAddr
		ts.Close
	End If

	Set ts = fso.OpenTextFile(fname)
	ReadFromAddr = ts.ReadLine
	ts.Close
End Function

'----------------------------------------------------------------------
' �w��t�@�C���� %TEMP% �ɃR�s�[����
'----------------------------------------------------------------------
Function CopyToTemp(src)
	Dim fso: Set fso = WScript.CreateObject("Scripting.FileSystemObject")
	Dim dst: dst = fso.BuildPath(GetTempPath, fso.GetFileName(src))
	fso.CopyFile src, dst, True
	
	CopyToTemp = dst
End Function

'----------------------------------------------------------------------
' ���[���ɂ��đ���
'----------------------------------------------------------------------
Function SendTo(attachFile)
	Dim fso: Set fso = WScript.CreateObject("Scripting.FileSystemObject")
	Dim basepath: basepath = fso.GetParentFolderName(WScript.ScriptFullName)
	Dim fromAddr: fromAddr = ReadFromAddr
	
	Dim cmd
	cmd = fso.BuildPath(basepath, "smail.exe")
	cmd = cmd & " -hsec.sanko-net.co.jp"					'���[���T�[�o�[�w��
	cmd = cmd & " -f" & fromAddr				'���M�Ҏw��
	cmd = cmd & " -s""" & SUBJECT & """"	'�����w��
	cmd = cmd & " -T""" & Now & " " & SUBJECT & """"	'�{���w��
	cmd = cmd & " -a""" & attachFile & """"				'�t�@�C����Y�t
	cmd = cmd & " " & fromAddr				'���M��w��
	
	Dim msg
	msg = fromAddr & " �ɁA���[�� '" & SUBJECT & "' �𑗂�܂��B" & vbNewLine & "��낵���ł���?"
	if MsgBox(msg, vbYesNo Or vbInformation Or vbDefaultButton2, fso.GetBaseName(WScript.ScriptFullName)) = vbYes Then
		SendTo = Exec(cmd)
		If SendTo <> 0 Then
			MsgBox "���M���s: error " & SendTo, vbOKOnly Or vbError, fso.GetBaseName(WScript.ScriptFullName)
		End If
	End If
End Function

'======================================================================

If WScript.Arguments.Named.Exists("clear") Then
	RemoveFromAddr
	WScript.Echo "���M�ҏ����N���A���܂����B"
Else
	Dim attachFile: attachFile = CopyToTemp(ATTACH_FILE)
	SendTo attachFile
End If
