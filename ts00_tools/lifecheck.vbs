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

'�I�����[���𑗐M
SendJobMail "�����m�F(" & CreateObject("WScript.Network").ComputerName & " lifecheck)", "�����m�F(" & CreateObject("WScript.Network").ComputerName & " lifecheck)"

WScript.Quit ret

'======================================================================


'----------------------------------------------------------------------
' �v���O���� �X�^�[�g
'----------------------------------------------------------------------
Function DoMain
	GetFileLog.LogWrite "job:lifecheck"
	DoMain = 0
End Function

'----------------------------------------------------------------------
' �e�X�g�p�̃��C��
'----------------------------------------------------------------------
Function TestMain
	GetFileLog.LogWrite "job:lifecheck(TestMain)"
	TestMain = 0
End Function
