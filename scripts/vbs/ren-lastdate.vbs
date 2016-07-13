'----------------------------------------------------------------------
' 指定フォルダのファイル名を更新日時にリネームする
'----------------------------------------------------------------------
Option Explicit


Call Main	'プログラムスタート


'----------------------------------------------------------------------
' すべてのファイルを列挙する(再帰的ではない)
'----------------------------------------------------------------------
Sub RenAll(strSrc, strDst)
	Dim objFs, objCurDir, objSubDir, objFile, baseName, objDstDir
	
	Set objFs = WScript.CreateObject("Scripting.FileSystemObject")
	Set objCurDir = objFs.GetFolder(strSrc)
	Set objDstDir = objFs.GetFolder(strDst)

	' フォルダ内のファイルを列挙する
	For Each objFile In objCurDir.Files
		baseName = Year(objFile.DateLastModified) & Right("0" & Month(objFile.DateLastModified), 2) & Right("0" & Day(objFile.DateLastModified), 2) & "_" & Right("0" & Hour(objFile.DateLastModified), 2) & Right("0" & Minute(objFile.DateLastModified), 2) & Right("0" & Second(objFile.DateLastModified), 2)

		WScript.Echo objFile.Path & " → " & objDstDir.Path & "\" & baseName & "." & objFs.GetExtensionName(objFile.Name)
		Call objFile.Copy(objDstDir.Path & "\" & baseName & "." & objFs.GetExtensionName(objFile.Name), False)
	Next
	
	Set objCurDir = Nothing
	Set objFs = Nothing
End Sub


'----------------------------------------------------------------------
' メインルーチン
'----------------------------------------------------------------------
Sub Main
	Dim i
	
	Call RenAll(WScript.Arguments(0), WScript.Arguments(1))
End Sub
