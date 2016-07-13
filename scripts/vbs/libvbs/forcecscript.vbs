Option Explicit

'cscript上で動いているかどうか調べる
Function IsRunningOnCScript()
	If (LCase(Right(WScript.FullName, 11)) = "cscript.exe") Then
		IsRunningOnCScript = True
	Else
		IsRunningOnCScript = False
	End If
End Function

'スクリプトを強制実行する
'
' @param cscriptArgs  cscript.exe に指定する引数
'
Function ForceOnCScript(cscriptArgs)
	ForceOnCScript = 0
	
	'WScript.ScriptFullName
	Dim args: args = ""
	Dim arg
	For Each arg In WScript.Arguments
		If InStr(arg, " ") < 1 Then
			args = args & arg & " "
		Else
			args = args & """" & arg & """ "
		End If
	Next
	Dim cmd
	If cscriptArgs = Null Then
		cmd = "cscript.exe """ & WScript.ScriptFullName & """ " & args
	Else
		cmd = "cscript.exe " & cscriptArgs & " """ & WScript.ScriptFullName & """ " & args
	End If
	ForceOnCScript = WScript.CreateObject("WScript.Shell").Run(cmd, 1, True)
End Function
