'----------------------------------------------------------------------
'----------------------------------------------------------------------
Option Explicit

GetFileLog.SetLogPathToScriptFolder "log"
GetFileLog.LogIdent = "DFR"
GetFileLog.LogWrite "---------- " & WScript.ScriptName & " start ----------"

Dim ret: ret = DoMain

GetFileLog.LogWrite "---------- " & WScript.ScriptName & " end ----------"

'終了メールを送信
'SendJobMail "デフラグ", "正常終了"

WScript.Quit ret

'======================================================================

'----------------------------------------------------------------------
'----------------------------------------------------------------------
Function LogDefragError(cmdLine, errorLevel)
	Dim shell: Set shell = WScript.CreateObject("WScript.Shell")
	
	Select Case errorLevel
	Case 0
		shell.LogEvent 0, cmdLine & " : " & errorLevel & " : " & "完了、エラーなし"
	Case 1
		shell.LogEvent 1, cmdLine & " : " & errorLevel & " : " & "ユーザーが中止、キャンセル"
	Case 2
		shell.LogEvent 1, cmdLine & " : " & errorLevel & " : " & "不良なパラメータ、構文エラー"
	Case 3
		shell.LogEvent 1, cmdLine & " : " & errorLevel & " : " & "予期しない、不明なエラー"
	Case 4
		shell.LogEvent 1, cmdLine & " : " & errorLevel & " : " & "メモリ不足状態 (RAM)"
	Case 5
		shell.LogEvent 1, cmdLine & " : " & errorLevel & " : " & "未使用 (未定義) の、一般的エラー"
	Case 6
		shell.LogEvent 1, cmdLine & " : " & errorLevel & " : " & "システム、ACL、ファイルの不一致、システム全体のエラー"
	Case 6
		shell.LogEvent 1, cmdLine & " : " & errorLevel & " : " & "ディスク容量不足、現在のボリュームの空き容量が 15% 未満"
	Case Else
		shell.LogEvent 1, cmdLine & " : " & errorLevel & " : " & "未知のエラー"
	End Select
End Function

'----------------------------------------------------------------------
' デフラグ実行
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
' プログラム スタート
'----------------------------------------------------------------------
Function DoMain
	GetFileLog.LogWrite "job:defragall"

	' ドライブを列挙
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
