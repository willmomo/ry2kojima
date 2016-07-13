'----------------------------------------------------------------------
' 登録情報表示.vbs
'
' 登録情報を表示するスクリプト
'----------------------------------------------------------------------

Set objShell = WScript.CreateObject("WScript.Shell")

strOwner = objShell.RegRead("HKLM\Software\Microsoft\Windows NT\CurrentVersion\RegisteredOwner")
strOrganization = objShell.RegRead("HKLM\Software\Microsoft\Windows NT\CurrentVersion\RegisteredOrganization")

WScript.Echo "ユーザー名: " & strOwner & vbNewLine & "会社名: " & strOrganization

