'----------------------------------------------------------------------
' Windows Installer のバージョンが3.1以上かどうか調べる
' 固定的に C:\windows\system32\msi.dll のバージョンを調べているので
' Windows XP 32bit でのみ動作する
'----------------------------------------------------------------------
Function CheckWindowsInstallerVersion()
	CheckWindowsInstallerVersion = False
	Dim fso, fname, version, vers
	Set fso = WScript.CreateObject("Scripting.FileSystemObject")
	'''fname = "C:\windows\system32\msi.dll"
	fname = "C:\Users\kojima\Documents\temp\51Lf_20091120\msi.dll"
	version = fso.GetFileVersion(fname)

	'MsgBox "[" & fname & "] file version: " & version

	If version = "3.1.4001.5512" Then
		msg = "SP3のWindows Installerです。"
	ElseIf version = "3.1.4000.2435" Then
		msg = "Windows Installer 3.1 のInstallerです。"
	ElseIf version = "3.0.3790.2180" Then
		msg = "SP2までの Windows Installer です。"
	ElseIf version = "4.5.6001.22159" Then
		msg = "Windows Installer 4.5 です。"
	Else
		msg = version & " は、知らないバージョンです。"
	End If
	
	MsgBox fname & " のバージョン" & vbNewLine & vbNewLine & "file version: " & version & vbNewLine & vbNewLine & msg,, fname & " のバージョン"
		
	vers = Split(version, ".")
	If (vers(0) > 3) Or (vers(0) = 3 And vers(1) >= 1) Then
		CheckWindowsInstallerVersion = True
	End If
End Function


CheckWindowsInstallerVersion

