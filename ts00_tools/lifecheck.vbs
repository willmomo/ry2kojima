'----------------------------------------------------------------------
'----------------------------------------------------------------------
Option Explicit

GetFileLog.SetLogPathToScriptFolder "log"
GetFileLog.LogIdent = "LCK"	'LifeChecK
GetFileLog.LogWrite "---------- " & WScript.ScriptName & " start ----------"

Dim ret
ret = DoMain
'' ret = TestMain

GetFileLog.LogWrite "---------- " & WScript.ScriptName & " end ----------"

'終了メールを送信
SendJobMail "生存確認(" & CreateObject("WScript.Network").ComputerName & " lifecheck)", "生存確認(" & CreateObject("WScript.Network").ComputerName & " lifecheck)"

WScript.Quit ret

'======================================================================


'----------------------------------------------------------------------
' プログラム スタート
'----------------------------------------------------------------------
Function DoMain
	GetFileLog.LogWrite "job:lifecheck"
	DoMain = 0
End Function

'----------------------------------------------------------------------
' テスト用のメイン
'----------------------------------------------------------------------
Function TestMain
	GetFileLog.LogWrite "job:lifecheck(TestMain)"
	TestMain = 0
End Function
