Option Explicit

'----------------------------------------------------------------------
' forfiles 風関数
'
' @path 検索を始めるパス
' @mask 検索対象を見つける正規表現
' @subdir True のときサブディレクトリも検索する
'----------------------------------------------------------------------
Function ForFiles(path, mask, subdir, command)
	ForFiles = 1	'ファイルが見つからない状態で初期化
	
	Dim reg: Set reg = WScript.CreateObject("VBScript.RegExp")
	reg.Pattern = mask
	reg.IgnoreCase = True
	reg.Global = False
	
	Dim fso: Set fso = WScript.CreateObject("Scripting.FileSystemObject")
	Dim folder: Set folder = fso.GetFolder(path)
	
	Dim obj, obj2
	
	For Each obj In folder.Files
		If reg.Test(obj.Name) Then
			ForFiles = 0
			Call command(False, obj.Path)
		End If
	Next
	
	For Each obj In folder.SubFolders
		If ForFiles(obj.Path, mask, subdir, command) = 0 Then
			ForFiles = 0
		End If
		If reg.Test(obj.Name) Then
			Call command(True, obj.Path)
		End If
	Next
End Function
