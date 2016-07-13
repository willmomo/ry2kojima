Option Explicit

Const TO_ADDR = "ry2kojima@sanko-net.co.jp"
Const SUBJECT = "リモートサポート管理台帳"
Const BODY = "リモートサポート管理台帳"
Const ATTACH_FILE = "\\TS00\share\@開発\@GN店舗接続履歴\リモートサポート管理台帳(SK→GN)..xls"

'----------------------------------------------------------------------
' %TEMP%を取得する
'----------------------------------------------------------------------
Function GetTempPath()
	Const TemporaryFolder = 2	'GetSpecialFolder メソッド、引数 option のための変数
	Dim fso: Set fso = WScript.CreateObject("Scripting.FileSystemObject")
	GetTempPath = fso.GetSpecialFolder(TemporaryFolder)
End Function

'----------------------------------------------------------------------
' ログに書き込む
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
' コマンドの実行
'----------------------------------------------------------------------
Function Exec(strCommand)
	WriteLog(strCommand)
	Dim shl: Set shl = WScript.CreateObject("WScript.Shell")
	On Error Resume Next
	Exec = shl.Run(strCommand, , True)
	If Err.Number <> 0 Then
		Exec = Err.Number
		WriteLog("実行時エラー: " & Exec)
	End If
End Function

'----------------------------------------------------------------------
' 送信者情報(from)の削除
'----------------------------------------------------------------------
Sub RemoveFromAddr()
	Dim fso: Set fso = WScript.CreateObject("Scripting.FileSystemObject")
	Dim basename: basename = fso.GetBaseName(WScript.ScriptFullName)
	Dim fname: fname = fso.BuildPath(GetTempPath, basename & "_from.txt")
	
	If fso.FileExists(fname) Then fso.DeleteFile(fname)
End Sub

'----------------------------------------------------------------------
' 送信者情報(from)を読み込む
'----------------------------------------------------------------------
Function ReadFromAddr()
	Dim fso: Set fso = WScript.CreateObject("Scripting.FileSystemObject")
	Dim basename: basename = fso.GetBaseName(WScript.ScriptFullName)
	Dim fname: fname = fso.BuildPath(GetTempPath, basename & "_from.txt")
	Dim ts
	Dim fromAddr
	
	If Not fso.FileExists(fname) Then
		fromAddr = InputBox("あなたのメールアドレスを入力してください。", basename)
		Set ts = fso.CreateTextFile(fname)
		ts.WriteLine fromAddr
		ts.Close
	End If

	Set ts = fso.OpenTextFile(fname)
	ReadFromAddr = ts.ReadLine
	ts.Close
End Function

'----------------------------------------------------------------------
' 指定ファイルを %TEMP% にコピーする
'----------------------------------------------------------------------
Function CopyToTemp(src)
	Dim fso: Set fso = WScript.CreateObject("Scripting.FileSystemObject")
	Dim dst: dst = fso.BuildPath(GetTempPath, fso.GetFileName(src))
	fso.CopyFile src, dst, True
	
	CopyToTemp = dst
End Function

'----------------------------------------------------------------------
' メールにつけて送る
'----------------------------------------------------------------------
Function SendTo(attachFile)
	Dim fso: Set fso = WScript.CreateObject("Scripting.FileSystemObject")
	Dim basepath: basepath = fso.GetParentFolderName(WScript.ScriptFullName)
	Dim fromAddr: fromAddr = ReadFromAddr
	
	Dim cmd
	cmd = fso.BuildPath(basepath, "smail.exe")
	cmd = cmd & " -hsec.sanko-net.co.jp"					'メールサーバー指定
	cmd = cmd & " -f" & fromAddr				'送信者指定
	cmd = cmd & " -s""" & SUBJECT & """"	'件名指定
	cmd = cmd & " -T""" & Now & " " & SUBJECT & """"	'本文指定
	cmd = cmd & " -a""" & attachFile & """"				'ファイルを添付
	cmd = cmd & " " & fromAddr				'送信先指定
	
	Dim msg
	msg = fromAddr & " に、メール '" & SUBJECT & "' を送ります。" & vbNewLine & "よろしいですか?"
	if MsgBox(msg, vbYesNo Or vbInformation Or vbDefaultButton2, fso.GetBaseName(WScript.ScriptFullName)) = vbYes Then
		SendTo = Exec(cmd)
		If SendTo <> 0 Then
			MsgBox "送信失敗: error " & SendTo, vbOKOnly Or vbError, fso.GetBaseName(WScript.ScriptFullName)
		End If
	End If
End Function

'======================================================================

If WScript.Arguments.Named.Exists("clear") Then
	RemoveFromAddr
	WScript.Echo "送信者情報をクリアしました。"
Else
	Dim attachFile: attachFile = CopyToTemp(ATTACH_FILE)
	SendTo attachFile
End If
