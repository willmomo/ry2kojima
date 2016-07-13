Option Explicit

'実行サンプル
Dim drv: Set drv = FindDrive("GNAVI_DISK_IMAGES_BACKUP")
If Not IsEmpty(drv) Then WScript.Echo("found on " & drv.DriveLetter & " drive.")

'----------------------------------------------------------------------
' 引数 expr で指定したファイルが見つかった、一番最初のDriveオブジェクトを返す。
' ドライブが見つからないときは、Emptyを返す
'
' 2015/09/11 r.kojima
'----------------------------------------------------------------------
Function FindDrive(expr)
	Dim fso: Set fso = WScript.CreateObject("Scripting.FileSystemObject")
	Dim drv
	For Each drv In fso.Drives
		If fso.FileExists(fso.BuildPath(drv.DriveLetter & ":\", expr)) Then
			Set FindDrive = drv
			Exit For
		End If
	Next
End Function
