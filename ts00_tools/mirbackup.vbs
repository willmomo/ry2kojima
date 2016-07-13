Option Explicit

Dim g_log
Set g_log = New FileLog

g_log.SetLogPathToScriptFolder "log"
g_log.LogIdent = "MIR"
g_log.LogWrite "---------- " & WScript.ScriptName & " start ----------"
g_log.LogWrite "job:mirbackup"

Dim ret: ret = Main
''Dim ret: ret = TestMain

g_log.LogWrite "---------- " & WScript.ScriptName & " end ----------"

'終了メールを送信
SendJobMail "mirbackup", IIf(ret = 0, "正常終了", "異常終了")

WScript.Quit

'----------------------------------------------------------------------
' 今日をフォーマットして戻す
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
' 日付を YYYYMMDD の形でフォーマットする
'----------------------------------------------------------------------
Function ToYYYYMMDD(expr)
	ToYYYYMMDD = Right("0000" & Year(expr), 4)
	ToYYYYMMDD = ToYYYYMMDD & Right("00" & Month(expr), 2)
	ToYYYYMMDD = ToYYYYMMDD & Right("00" & Day(expr), 2)
End Function

'----------------------------------------------------------------------
' 日付を YYYYMMDD_HHMM の形でフォーマットする
'----------------------------------------------------------------------
Function ToYYYYMMDD_HHMM(expr)
	ToYYYYMMDD_HHMM = ToYYYYMMDD(expr) & "_"
	ToYYYYMMDD_HHMM = ToYYYYMMDD_HHMM & Right("00" & Hour(expr), 2)
	ToYYYYMMDD_HHMM = ToYYYYMMDD_HHMM & Right("00" & Minute(expr), 2)
End Function

'----------------------------------------------------------------------
' ミラーリング処理
'----------------------------------------------------------------------
Function RunMirBackup(strOrg, dtNow)
	g_log.LogWrite "RunMirBackup enter."
	g_log.LogWrite "RunMirBackup strOrg = " & strOrg
	g_log.LogWrite "RunMirBackup dtNow = " & dtNow

	Dim objFs
	Set objFs = WScript.CreateObject("Scripting.FileSystemObject")
	Dim strLogPath
	Dim strDst
	Dim strCmd
	
	strDst = "\\ts88\" & objFs.GetFileName(strOrg)

	strLogPath = objFs.BuildPath(objFs.GetParentFolderName(WScript.ScriptFullName), "log_mir\" & ToYYYYMMDD_HHMM(dtNow))
	strCmd = "c:\tools\mirror --mode=mirror-backup --logpath=" & strLogPath & " " & strOrg & " " & strDst
	g_log.LogWrite "Running '" & strCmd & "'"
	RunMirBackup = Run(strCmd, 1, True)
	g_log.LogWrite "戻り値: " & RunMirBackup

	g_log.LogWrite "RunMirBackup leave(" & RunMirBackup & ")."
End Function

'----------------------------------------------------------------------
' メイン処理
'----------------------------------------------------------------------
Function Main()
	Dim result

	LogInfoEvent "mirbackup.vbs開始"

	result = 0

	'一般データファイルをミラーリング
	Dim dtNow: dtNow = Now	'ログフォルダ名に使う日時
	result = result + RunMirBackup("E:\fixhome", dtNow)
	result = result + RunMirBackup("E:\hqpc209", dtNow)
	result = result + RunMirBackup("E:\lain2", dtNow)
	result = result + RunMirBackup("E:\nobackup", dtNow)
	result = result + RunMirBackup("E:\share", dtNow)
	
	'' svndataの中は、incbackup で svnsync しています。
	''result = result + RunMirBackup("E:\svndata", dtNow)
	
	result = result + RunMirBackup("G:\fixshare", dtNow)
	result = result + RunMirBackup("G:\home", dtNow)
	result = result + RunMirBackup("G:\img1", dtNow)
	result = result + RunMirBackup("G:\img2", dtNow)
	result = result + RunMirBackup("G:\skshare", dtNow)
	result = result + RunMirBackup("H:\drvimg", dtNow)

	If result = 0 Then
		LogInfoEvent "mirbackup.vbs終了"
	Else
		LogErrorEvent "mirbackup.vbsでエラー発生"
	End If

	Main = result
End Function

'----------------------------------------------------------------------
' メイン処理
'----------------------------------------------------------------------
Function TestMain()
	Dim result

	LogInfoEvent "mirbackup.vbs開始(TestMain)"

	result = 0

	'一般データファイルをミラーリング
	Dim dtNow: dtNow = Now	'ログフォルダ名に使う日時
	result = result + RunMirBackup("E:\lain2", dtNow)

	TestMain = result
End Function
