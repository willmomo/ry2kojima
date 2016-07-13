'----------------------------------------------------------------------
' Windows Installer のバージョンをチェックするスクリプト
'----------------------------------------------------------------------
Option Explicit

Dim result

If GetOSVersion > 5.0 Then
	On Error Resume Next
	result = CheckWindowsInstallerVersion
	If Err.Number = 0 Then
		'正常処理
		If result Then
			WScript.Echo "Windows Installer 3.1 以上がインストール済み。"
		Else
			WScript.Echo "Windows Installer 3.1 以上をインストールしてください。"
		End If
	Else
		'異常処理
		If Err.Number > 0 Then
			WScript.Echo "Windows Installer のバージョン情報が取得できません。" & vbNewLine & _
				Err.Number & " : " & Err.Description
		Else
			WScript.Echo "Windows Installer のバージョン情報が取得できません。" & vbNewLine & _
				Hex(Err.Number) & " : [" & Err.Description & "]"
		End If
	End If
	On Error Goto 0
Else
	WScript.Echo "Windows XP 未満なのでチェックしません。"
End If

'----------------------------------------------------------------------
' Windows Installer のバージョンが3.1以上かどうか調べる
' 固定的に C:\windows\system32\msi.dll のバージョンを調べているので
' Windows XP 32bit のみでの使用を禁止
'----------------------------------------------------------------------
Function CheckWindowsInstallerVersion()
	CheckWindowsInstallerVersion = False
	Dim fso, fname, version, vers
	Set fso = WScript.CreateObject("Scripting.FileSystemObject")
	fname = "C:\windows\system32\msi.dll"
	version = fso.GetFileVersion(fname)

	WScript.Echo "Version: " & version

	vers = Split(version, ".")
	If (vers(0) > 3) Or (vers(0) = 3 And vers(1) >= 1) Then
		CheckWindowsInstallerVersion = True
	End If
End Function

'----------------------------------------------------------------------
' OSのバージョンを取得
' 5.0 = Windows 2000
' 5.1 = Windows XP
' 6.1 = Windows 7
'----------------------------------------------------------------------
Function GetOSVersion()
	Dim wmi, target, row, version, vers, computer
	computer = "."
	Set wmi = GetObject("winmgmts:{impersonationLevel=impersonate}!\\" & computer & "\root\cimv2")
	Set target = wmi.ExecQuery("select Version from Win32_OperatingSystem")
	For Each row In target
		version = row.Version
	Next
	vers = Split(version, ".")
	GetOSVersion = CDbl(vers(0) & "." & vers(1))
	
	'WScript.Echo "OS: " & GetOSVersion
End Function
