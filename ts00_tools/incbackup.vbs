Option Explicit

Dim g_verbose: g_verbose = False

Dim g_log
Set g_log = New FileLog

g_log.SetLogPathToScriptFolder "log"
g_log.LogIdent = "INC"
g_log.LogWrite "---------- " & WScript.ScriptName & " start ----------"
g_log.LogWrite "job:incbackup"

Dim ret: ret = Main
''Dim ret: ret = TestMain

g_log.LogWrite "---------- " & WScript.ScriptName & " end ----------"

'�I�����[���𑗐M
SendJobMail "incbackup", IIf(ret = 0, "����I��", "�ُ�I��")

WScript.Quit


'----------------------------------------------------------------------
' �������t�H�[�}�b�g���Ė߂�
'----------------------------------------------------------------------
Function FormattedNow
	Dim dt

	dt = Now
	FormattedNow = Right("0000" & Year(dt), 4) & "/"
	FormattedNow = FormattedNow & Right("00" & Month(dt), 2) & "/"
	FormattedNow = FormattedNow & Right("00" & Day(dt), 2) & " "
	FormattedNow = FormattedNow & Right("00" & Hour(dt), 2) & ":"
	FormattedNow = FormattedNow & Right("00" & Minute(dt), 2) & ":"
	FormattedNow = FormattedNow & Right("00" & Second(dt), 2)
End Function

'----------------------------------------------------------------------
' ���t�� YYYYMMDD �̌`�Ńt�H�[�}�b�g����
'----------------------------------------------------------------------
Function ToYYYYMMDD(expr)
	ToYYYYMMDD = Right("0000" & Year(expr), 4)
	ToYYYYMMDD = ToYYYYMMDD & Right("00" & Month(expr), 2)
	ToYYYYMMDD = ToYYYYMMDD & Right("00" & Day(expr), 2)
End Function

'----------------------------------------------------------------------
' ���t�� YYYYMMDD_HHMM �̌`�Ńt�H�[�}�b�g����
'----------------------------------------------------------------------
Function ToYYYYMMDD_HHMM(expr)
	ToYYYYMMDD_HHMM = ToYYYYMMDD(expr) & "_"
	ToYYYYMMDD_HHMM = ToYYYYMMDD_HHMM & Right("00" & Hour(expr), 2)
	ToYYYYMMDD_HHMM = ToYYYYMMDD_HHMM & Right("00" & Minute(expr), 2)
End Function

'----------------------------------------------------------------------
' ���t�� YYYYMMDDHHMMSS �̌`�Ńt�H�[�}�b�g����
'----------------------------------------------------------------------
Function ToYYYYMMDDHHMMSS(expr)
	ToYYYYMMDDHHMMSS = Right("0000" & Year(expr), 4)
	ToYYYYMMDDHHMMSS = ToYYYYMMDDHHMMSS & Right("00" & Month(expr), 2)
	ToYYYYMMDDHHMMSS = ToYYYYMMDDHHMMSS & Right("00" & Day(expr), 2)
	ToYYYYMMDDHHMMSS = ToYYYYMMDDHHMMSS & "_"
	ToYYYYMMDDHHMMSS = ToYYYYMMDDHHMMSS & Right("00" & Hour(expr), 2)
	ToYYYYMMDDHHMMSS = ToYYYYMMDDHHMMSS & Right("00" & Minute(expr), 2)
	ToYYYYMMDDHHMMSS = ToYYYYMMDDHHMMSS & Right("00" & Second(expr), 2)
End Function

'----------------------------------------------------------------------
' �f�B���N�g���̏����t�@�C���ɏ����o��
'----------------------------------------------------------------------
Function DirToFile2(expr, stream)
	Dim fso, folder, file, subFolder
	Set fso = WScript.CreateObject("Scripting.FileSystemObject")
	Set folder = fso.GetFolder(expr)
	
	stream.WriteLine folder.Name
	
	'�t�@�C�����
	For Each file In folder.Files
		stream.WriteLine file.Name & "," & file.Size & "," & file.DateLastModified
	Next
	
	'�t�H���_���
	For Each subFolder In folder.SubFolders
		DirToFile2 subFolder, stream
	Next
End Function

'----------------------------------------------------------------------
' �f�B���N�g���̏����t�@�C���ɏ����o��
'----------------------------------------------------------------------
Function DirToFile(folderspec, savefile)
	If g_verbose Then g_log.LogWrite "DirToFile(" & folderspec & "," & savefile & ")"
	
	Const ForWriting = 2
	Dim fso: Set fso = WScript.CreateObject("Scripting.FileSystemObject")
	Dim stream: Set stream = fso.OpenTextFile(savefile, ForWriting, True)
	DirToFile2 folderspec, stream
	stream.Close
End Function

'----------------------------------------------------------------------
' �t�@�C���̔�r
' True = ��v , False = �s��v
'----------------------------------------------------------------------
Function FileComp(file1, file2)
	If g_verbose Then g_log.LogWrite "FileComp " & file1 & "," & file2
	
	Const ForReading = 1
	Dim fso: Set fso = WScript.CreateObject("Scripting.FileSystemObject")
	
	If Not fso.FileExists(file1) Then
		FileComp = False
		If g_verbose Then g_log.LogWrite "FileComp result " & FileComp
		Exit Function
	End If

	Dim ts1: Set ts1 = fso.OpenTextFile(file1, ForReading, False)
	Dim ts2: Set ts2 = fso.OpenTextFile(file2, ForReading, False)
	If StrComp(ts1.ReadAll, ts2.ReadAll) = 0 Then
		FileComp = True
	Else
		FileComp = False
	End If
	ts1.Close
	ts2.Close
	
	If g_verbose Then g_log.LogWrite "FileComp result " & FileComp
End Function

'----------------------------------------------------------------------
' ���s�ۗ�
'----------------------------------------------------------------------
Sub Pending()
''	Dim fSilent

	'(���`�� and 7:00�`18:59)�́A���s��ۗ�����B
''	fSilent = False
''	Do While ((vbMonday <= Weekday(Now) And Weekday(Now) <= vbFriday) And (7 <= Hour(Now) And Hour(Now) < 19))
''		If Not fSilent Then
''			g_log.LogWrite "(���s�ۗ�)"
''			fSilent = True
''		End If
''		WScript.Sleep(5 * 1000)
''	Loop
End Sub

'----------------------------------------------------------------------
' �����o�b�N�A�b�v����
'----------------------------------------------------------------------
Function RunIncBackup(strOrg, dtNow)
	Dim objFs
	Dim strLogPath
	Dim strDst
	Dim strCmd

	g_log.LogWrite "RunIncBackup enter."
	g_log.LogWrite "RunIncBackup strOrg = " & strOrg
	g_log.LogWrite "RunIncBackup dtNow = " & dtNow

	Pending

	Set objFs = WScript.CreateObject("Scripting.FileSystemObject")

	strDst = objFs.BuildPath(objFs.BuildPath("\\ts02\pcbackup\ts00", ToYYYYMMDD(Now)), objFs.GetFileName(strOrg))

	strLogPath = objFs.BuildPath(objFs.GetParentFolderName(WScript.ScriptFullName), "log_inc\" & ToYYYYMMDD_HHMM(dtNow))
	strCmd = "c:\tools\mirror --archive --no-empty-folder --logpath=" & strLogPath & " --exclude=C:\tools\incbackup-exl.txt " & strOrg & " " & strDst
	g_log.LogWrite "Running '" & strCmd & "'"
	RunIncBackup = Run(strCmd, 1, True)
	g_log.LogWrite "�߂�l: " & RunIncBackup

	g_log.LogWrite "RunIncBackup leave(" & RunIncBackup & ")."
End Function

'----------------------------------------------------------------------
' mysqldump�Ńo�b�N�A�b�v
'----------------------------------------------------------------------
Function RunMysqldump()
	Dim cmdLine, strNow, retCode

	RunMysqlDump = 0
	
	strNow = NowYYYYMMDD
	g_log.LogWrite "T2DB�̃o�b�N�A�b�v (" & strNow & ")"

	cmdLine = "%comspec% /c mysqldump --single-transaction --user=root --password=root --default-character-set=utf8 t2>E:\lain2\share\@�J��\t2dbm\backup\t2_" & strNow & "_utf8.sql"
	g_log.LogWrite " " & cmdLine
	retCode = Run(cmdLine, 0, True)
	If retCode <> 0 Then
		g_log.LogWrite " error return: " & retCode
		RunMysqlDump = retCode
	End If

	'ts88�Ƀf�[�^�x�[�X���C���|�[�g
	cmdLine = "%comspec% /c mysql --host=ts88 --user=root --password=root --default-character-set=utf8 t2<E:\lain2\share\@�J��\t2dbm\backup\t2_" & strNow & "_utf8.sql"
	g_log.LogWrite " " & cmdLine
	retCode = Run(cmdLine, 0, True)
	If retCode <> 0 Then
		g_log.LogWrite " error return: " & retCode
		RunMysqlDump = retCode
	End If

	g_log.LogWrite "HINKAIDB�̃o�b�N�A�b�v (" & strNow & ")"

	cmdLine = "%comspec% /c mysqldump --single-transaction --user=root --password=root --default-character-set=utf8 hinkai>E:\lain2\share\@�J��\hinkaidbm\backup\hinkai_" & strNow & "_utf8.sql"
	g_log.LogWrite " " & cmdLine
	retCode = Run(cmdLine, 0, True)
	If retCode <> 0 Then
		g_log.LogWrite " error return: " & retCode
		RunMysqlDump = retCode
	End If

	'ts88�Ƀf�[�^�x�[�X���C���|�[�g
	cmdLine = "%comspec% /c mysql --host=ts88 --user=root --password=root --default-character-set=utf8 hinkai<E:\lain2\share\@�J��\hinkaidbm\backup\hinkai_" & strNow & "_utf8.sql"
	g_log.LogWrite " " & cmdLine
	retCode = Run(cmdLine, 0, True)
	If retCode <> 0 Then
		g_log.LogWrite " error return: " & retCode
		RunMysqlDump = retCode
	End If

	g_log.LogWrite "IPDB�̃o�b�N�A�b�v (" & strNow & ")"

	cmdLine = "%comspec% /c mysqldump --single-transaction --user=root --password=root --default-character-set=utf8 ip>E:\lain2\share\@�J��\ip\backup\ip_" & strNow & "_utf8.sql"
	g_log.LogWrite " " & cmdLine
	retCode = Run(cmdLine, 0, True)
	If retCode <> 0 Then
		g_log.LogWrite " error return: " & retCode
		RunMysqlDump = retCode
	End If

	'ts88�Ƀf�[�^�x�[�X���C���|�[�g
	cmdLine = "%comspec% /c mysql --host=ts88 --user=root --password=root --default-character-set=utf8 ip<E:\lain2\share\@�J��\ip\backup\ip_" & strNow & "_utf8.sql"
	g_log.LogWrite " " & cmdLine
	retCode = Run(cmdLine, 0, True)
	If retCode <> 0 Then
		g_log.LogWrite " error return: " & retCode
		RunMysqlDump = retCode
	End If
End Function

'----------------------------------------------------------------------
' svnadmin dump
'----------------------------------------------------------------------
Function RunSvnadminDump()
	Dim cmdLine, objFs, prefix, obj, retCode
	Dim toPath
	Dim toBase
	Dim toFullPath
	Dim toTempPath
	Dim toTempFullPath
	Dim dirFile1, dirFile2

	g_log.LogWrite "SVN�̃o�b�N�A�b�v"

	Set objFs = WScript.CreateObject("Scripting.FileSystemObject")

	prefix = ToYYYYMMDDHHMMSS(Now)
	'toBase = "\\Ts02\pcbackup\svndump"
	'toPath = objFs.BuildPath(toBase, prefix)
	toPath = "G:\home\_svndump"
	'toTempPath = objFs.GetParentFolderName(WScript.ScriptFullName)
	toTempPath = "G:\home\_svndump"

	For Each obj In objFs.GetFolder("E:\svndata").SubFolders
		If objFs.FileExists(objFs.BuildPath(obj.Path, "svn.ico")) Then

			dirFile1 = objFs.BuildPath(toPath, obj.Name & ".txt")
			dirFile2 = dirFile1 & "_"
			DirToFile obj.Path, dirFile2

			' sync �́A�K���s��(����ȊO�͈�u�ŏI���)
			cmdLine = "svnsync --non-interactive --quiet sync svn://ts88/" & obj.Name
			g_log.LogWrite " " & cmdLine
			retCode = Run(cmdLine, 1, True)
			If retCode <> 0 Then
				g_log.LogWrite " error return: " & retCode
				RunSvnadminDump = retCode
			End If
			
			If FileComp(dirFile1, dirFile2) = False Then
			
				'toTempFullPath = objFs.BuildPath(toTempPath, "svn_" & obj.Name & ".dump")
				'toFullPath = objFs.BuildPath(toPath, "svn_" & obj.Name & ".dump")
				toTempFullPath = objFs.BuildPath(toTempPath, obj.Name & ".dump")
				toFullPath = objFs.BuildPath(toPath, obj.Name & ".dump")
				
				cmdLine = "%comspec% /c svnadmin dump " & obj.Path & " > " & toTempFullPath
				g_log.LogWrite " " & cmdLine
				retCode = Run(cmdLine, 1, True)
				If retCode <> 0 Then
					g_log.LogWrite " error return: " & retCode
					RunSvnadminDump = retCode
				End If
				
				cmdLine = "7za a -tzip " & toFullPath & ".zip " & toTempFullPath
				g_log.LogWrite " " & cmdLine
				retCode = Run(cmdLine, 1, True)
				If retCode <> 0 Then
					g_log.LogWrite " error return: " & retCode
					RunSvnadminDump = retCode
				End If

				g_log.LogWrite " Delete '" & toTempFullPath & "'"
				objFs.DeleteFile(toTempFullPath)
	
				If objFs.FileExists(dirFile1) Then
					objFs.DeleteFile dirFile1
				End If
				
				objFs.MoveFile dirFile2, dirFile1
			Else
				objFs.DeleteFile dirFile2
			End If
		End If
	Next
End Function

'----------------------------------------------------------------------
' dokuwiki backup
'----------------------------------------------------------------------
Function RunDokuwikiBackup()
	g_log.LogWrite "Dokuwiki�̃o�b�N�A�b�v"
	RunDokuwikiBackup = 0
	
	Dim fso: Set fso = WScript.CreateObject("Scripting.FileSystemObject")

	Dim zipfile
	Dim target
	Dim ret
	Dim cmdLine
	Dim log
	
	zipfile = fso.BuildPath("E:\lain2\share\_dokuwiki_backup", "doku_" & ToYYYYMMDDHHMMSS(Now) & ".zip")
	target = "E:\lain2\share\doku"
	cmdLine = "7za a -tzip " & zipfile & " " & target
	g_log.LogWrite " " & cmdLine
	ret = Run(cmdLine, 0, True)
	g_log.LogWrite " ret = " & ret
	
	zipfile = fso.BuildPath("E:\lain2\share\_dokuwiki_backup", "dokuibox_" & ToYYYYMMDDHHMMSS(Now) & ".zip")
	target = "E:\lain2\share\dokuibox"
	cmdLine = "7za a -tzip " & zipfile & " " & target
	g_log.LogWrite " " & cmdLine
	ret = Run(cmdLine, 0, True)
	g_log.LogWrite " ret = " & ret
	
	zipfile = fso.BuildPath("E:\lain2\share\_dokuwiki_backup", "dokutest_" & ToYYYYMMDDHHMMSS(Now) & ".zip")
	target = "E:\lain2\share\dokutest"
	cmdLine = "7za a -tzip " & zipfile & " " & target
	g_log.LogWrite " " & cmdLine
	ret = Run(cmdLine, 0, True)
	g_log.LogWrite " ret = " & ret

	' �Ō�Ƀ~���[�����O���Ē��߂�����
	log = fso.BuildPath(fso.GetParentFolderName(WScript.ScriptFullName), "log")
	cmdLine = "mirror --mode=backup-mirror e:\lain2\share\doku \\ts88\lain2\share\doku --logpath=" & log
	g_log.LogWrite " " & cmdLine
	ret = Run(cmdLine, 0, True)
	g_log.LogWrite " ret = " & ret

	cmdLine = "mirror --mode=backup-mirror e:\lain2\share\dokuibox \\ts88\lain2\share\dokuibox --logpath=" & log
	g_log.LogWrite " " & cmdLine
	ret = Run(cmdLine, 0, True)
	g_log.LogWrite " ret = " & ret

	cmdLine = "mirror --mode=backup-mirror e:\lain2\share\dokutest \\ts88\lain2\share\dokutest --logpath=" & log
	g_log.LogWrite " " & cmdLine
	ret = Run(cmdLine, 0, True)
	g_log.LogWrite " ret = " & ret
	
	' ���łɁA�A�C�f�A�ڈ��� ���~���[�����O
	log = fso.BuildPath(fso.GetParentFolderName(WScript.ScriptFullName), "log")
	cmdLine = "mirror --mode=backup-mirror e:\lain2\share\@�J��\bbs \\ts88\lain2\share\@�J��\bbs --logpath=" & log
	g_log.LogWrite " " & cmdLine
	ret = Run(cmdLine, 0, True)
	g_log.LogWrite " ret = " & ret

	g_log.LogWrite "Dokuwiki�̃o�b�N�A�b�v�I��(return " & RunDokuwikiBackup & ")"
End Function

'----------------------------------------------------------------------
' Traclight backup
'----------------------------------------------------------------------
Function RunTraclightBackup()
	g_log.LogWrite "Traclight�̃o�b�N�A�b�v"
	RunTraclightBackup = 0
	
	Dim fso: Set fso = WScript.CreateObject("Scripting.FileSystemObject")

	Dim zipfile
	Dim target
	Dim ret
	Dim cmdLine
	
	zipfile = fso.BuildPath("E:\lain2\share\_dokuwiki_backup", "trac_" & ToYYYYMMDDHHMMSS(Now) & ".zip")
	target = "E:\lain2\share\Traclight"
	cmdLine = "7za a -tzip " & zipfile & " " & target
	g_log.LogWrite " " & cmdLine
	ret = Run(cmdLine, 0, True)
	g_log.LogWrite " ret = " & ret

	g_log.LogWrite "Traclight�̃o�b�N�A�b�v�I��(return " & RunTraclightBackup & ")"
End Function

'----------------------------------------------------------------------
' �T�C�{�E�Y �I�t�B�X�A�f�W�G�̃o�b�N�A�b�v
'----------------------------------------------------------------------
Function RunCybozuBackup()
	g_log.LogWrite "�T�C�{�E�Y �I�t�B�X�A�f�W�G�̃o�b�N�A�b�v"
	RunCybozuBackup = 0
	
	Dim fso: Set fso = WScript.CreateObject("Scripting.FileSystemObject")

	Dim zipfile
	Dim target
	Dim ret
	Dim cmdLine
	Dim log
	
	zipfile = fso.BuildPath("E:\lain2\share\_dokuwiki_backup", "cybozu_" & ToYYYYMMDDHHMMSS(Now) & ".zip")
	target = "G:\cybozu C:\inetpub\wwwroot\cb65 C:\inetpub\wwwroot\cbdb C:\inetpub\wwwroot\cbdb8"
	cmdLine = "7za a -tzip " & zipfile & " " & target
	g_log.LogWrite " " & cmdLine
	ret = Run(cmdLine, 0, True)
	g_log.LogWrite " ret = " & ret

	' �Ō�Ƀ~���[�����O���Ē��߂�����
	log = fso.BuildPath(fso.GetParentFolderName(WScript.ScriptFullName), "log")
	cmdLine = "mirror --mode=backup-mirror G:\cybozu \\ts88\H$\cybozu --logpath=" & log
	g_log.LogWrite " " & cmdLine
	ret = Run(cmdLine, 0, True)
	g_log.LogWrite " ret = " & ret

	g_log.LogWrite "�T�C�{�E�Y �I�t�B�X�A�f�W�G�̃o�b�N�A�b�v�I��(return " & RunCybozuBackup & ")"
End Function

'----------------------------------------------------------------------
' ���C������
'----------------------------------------------------------------------
Function Main()
	Dim result: result = 0

	LogInfoEvent "incbackup.vbs�J�n"

	'mysql database(t2data)���o�b�N�A�b�v
	result = result + RunMysqldump
	
	'subversion�̃f�[�^���o�b�N�A�b�v
	result = result + RunSvnadminDump

	'dokuwiki�̃f�[�^��zip���k(�����o�b�N�A�b�v�ł͈Ӗ����Ȃ��̂�)
	result = result + RunDokuwikiBackup
	
	'Traclight�̃f�[�^��zip���k(�����̃t�@�C�������������ăo�b�N�A�b�v�ł��Ȃ��̂�)
	result = result + RunTraclightBackup
	
	'�T�C�{�E�Y,�f�W�G��zip���k(�����o�b�N�A�b�v�ł͈Ӗ����Ȃ��̂�)
	result = result + RunCybozuBackup
	
	'��ʃf�[�^�t�@�C���𑝕��o�b�N�A�b�v
	Dim dtNow: dtNow = Now	'���O�t�H���_���Ɏg������
	result = result + RunIncBackup("e:\fixhome", dtNow)
	result = result + RunIncBackup("e:\hqpc209", dtNow)
	result = result + RunIncBackup("e:\lain2", dtNow)
	result = result + RunIncBackup("e:\share", dtNow)
	result = result + RunIncBackup("g:\fixshare", dtNow)
	result = result + RunIncBackup("g:\home", dtNow)
	result = result + RunIncBackup("g:\img1", dtNow)
	result = result + RunIncBackup("g:\img2", dtNow)
	result = result + RunIncBackup("g:\skshare", dtNow)
	result = result + RunIncBackup("h:\drvimg", dtNow)

	Call g_log.LogWrite(" result = " & result)
	If result = 0 Then
		LogInfoEvent "incbackup.vbs�I��"
	Else
		LogErrorEvent "incbackup.vbs�ŃG���[����"
	End If

	Main = result
End Function

'----------------------------------------------------------------------
' �e�X�g�p���C������
'----------------------------------------------------------------------
Function TestMain()
	Dim result: result = 0

	LogInfoEvent "incbackup.vbs�J�n - TestMain"

	'mysql database(t2data)���o�b�N�A�b�v
	result = result + RunMysqldump

	Call g_log.LogWrite(" result = " & result)
	If result = 0 Then
		LogInfoEvent "incbackup.vbs�I�� - TestMain"
	Else
		LogErrorEvent "incbackup.vbs�ŃG���[���� - TestMain"
	End If

	TestMain = result
End Function
