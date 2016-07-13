Option Explicit

'------------------------------------------------------------------------------
' WScript.Shell.Run
'
' last modified: 2011/07/25
'------------------------------------------------------------------------------
Public Function kjmShellRun(strCommand, intWindowStyle, bWaitOnReturn)
	Dim obj
	
	Set obj = WScript.CreateObject("WScript.Shell")
	kjmShellRun = obj.Run(strCommand, intWindowStyle, bWaitOnReturn)
End Function

' intWinodwStyle 用定数
Public Const SW_HIDE = 0	' ウィンドウを非表示にし、別のウィンドウをアクティブにします。

'------------------------------------------------------------------------------
' Windows 2000 : 5
' Windows XP : 5.1
'------------------------------------------------------------------------------
Function GetOSVersion()
	Dim colTarget,objRow,str,aData,nData, I, nTarget

	Set colTarget = GetObject("winmgmts:\\.\root\cimv2").ExecQuery("select * from Win32_OperatingSystem")
	For Each objRow in colTarget
		str = objRow.Version
	Next
	
	aData = Split( str, "." )
	For I = 0 to Ubound( aData )
		if I > 1 then
			Exit For
		end if
		if I > 0 then
			nTarget = nTarget & "."
		end if
		nTarget = nTarget & aData(I)
	Next
	GetOSVersion = CDbl(nTarget)
	
	GetOSVersion = 5.0
End Function

'------------------------------------------------------------------------------
' [タスク]にスケジュールを追加する。
'------------------------------------------------------------------------------
Private Sub TaskCreateOnWindows2000()
	Dim result, objFs, sFileName, strLine, objTs, alreadyExists
	
	alreadyExists = False
	
	Set objFs = WScript.CreateObject("Scripting.FileSystemObject")

	' リダイレクト先のファイル名を生成します。  
	Do: sFileName = objFs.BuildPath(objFs.GetSpecialFolder(2), objFs.GetTempName)
	Loop While objFs.FileExists(sFileName)
	
	Call kjmShellRun("%ComSpec% /c at > " & sFileName, SW_HIDE, True)

	If objFs.FileExists(sFileName) Then
		Set objTs = objFs.OpenTextFile(sFileName)
		Do Until objTs.AtEndOfStream = true
			strLine = objTs.ReadLine
			If InStr(LCase(strLine), "kishu.exe") > 0 Then
				alreadyExists = True
				Exit Do
			End If
		Loop
		objTs.Close
		objFs.DeleteFile sFileName
	End If

	If alreadyExists = False Then
		Call kjmShellRun("at 6:00 /every:M,T,W,Th,F,S,Su ""C:\mpc3\tools\kisyu_tool\KisyuAutoAdd.exe -Auto""", SW_HIDE, True)
	End If
End Sub

'------------------------------------------------------------------------------
' [タスク]にスケジュールを追加する。
'
' Windows 2000のときは、atコマンドを使用。
' Windows 2000以外のときは、schtasksコマンドを使用。
'------------------------------------------------------------------------------
Private Sub TaskCreate()
	dim result
	
	If GetOSVersion() = 5 Then
		TaskCreateOnWindows2000
	else
		result = kjmShellRun("schtasks /Create /TN 自動割付設定 /RU kojima /RP Kojima /SC DAILY /ST 06:00:00 /TR ""C:\mpc3\tools\kisyu_tool\KisyuAutoAdd.exe -Auto""", SW_HIDE, True)
	end if
End Sub

'------------------------------------------------------------------------------
' [タスク]からスケジュールを削除する。
'
' Windows 2000は、atコマンドidを取得してからでないと、削除すべきIDが分からない
' ため、少々複雑な処理が必要。
'------------------------------------------------------------------------------
Private Sub TaskDeleteOnWindows2000()
	Dim result, objFs, sFileName, strLine, objTs
	
	Set objFs = WScript.CreateObject("Scripting.FileSystemObject")

	' リダイレクト先のファイル名を生成します。  
	Do: sFileName = objFs.BuildPath(objFs.GetSpecialFolder(2), objFs.GetTempName)
	Loop While objFs.FileExists(sFileName)
	
	Call kjmShellRun("%ComSpec% /c at > " & sFileName, SW_HIDE, True)

	If objFs.FileExists(sFileName) Then
		Set objTs = objFs.OpenTextFile(sFileName)
		Do Until objTs.AtEndOfStream = true
			strLine = objTs.ReadLine
			If InStr(LCase(strLine), "kishu.exe") > 0 Then
				Call kjmShellRun("at " & CLng(Left(strLine, 9)) & " /delete", SW_HIDE, True)
			End If
		Loop
		objTs.Close
		objFs.DeleteFile sFileName
	End If
End Sub

'------------------------------------------------------------------------------
' [タスク]からスケジュールを削除する。
'
' Windows 2000のときは、atコマンドを使用。
' Windows 2000以外のときは、schtasksコマンドを使用。
'------------------------------------------------------------------------------
private sub TaskDelete()
	Dim result

	if GetOSVersion() = 5 then
		TaskDeleteOnWindows2000
	else
		result = kjmShellRun("schtasks /Delete /TN 自動割付設定 /F", SW_HIDE, True)
	End If
end sub


Call TaskCreate
''Call TaskDelete

