Option Explicit

Dim objFS, objFolder, objFile, objRegEx, objMatch, strNewName

'引数チェック
CheckArg

'FileSystemObjectの作成
Set objFS = WScript.CreateObject("Scripting.FileSystemObject")

'指定されたフォルダのFolderオブジェクトを取得
Set objFolder = objFS.GetFolder(WScript.Arguments.Item(0))

'指定されたフォルダ内から、mf*.指定日付.csvを列挙
'日付部分を置き換えてコピーする。
Set objRegEx = New RegExp
objRegEx.Pattern = WScript.Arguments.Item(1) & "\.csv$"
For Each objFile In objFolder.Files
	If objRegEx.Test(objFile.Name) Then
		strNewName = objRegEx.Replace(objFile.Name, WScript.Arguments.Item(2) & ".csv")
		objFile.Copy objFolder.Path & "\" & strNewName, True
	End If
Next

'引数チェック
Sub CheckArg()
	If WScript.Arguments.Count <> 3 Then
		WScript.Echo "使用法：copymfcsv.vbs <mf*.YYYYMMDD.csvが存在するフォルダ名> <コピー元日付文字列(YYYYMMDD)> <コピー先日付文字列(YYYYMMDD)>"
		WScript.Quit
	End If
End Sub
