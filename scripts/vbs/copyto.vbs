Option Explicit

'実行サンプル
Call CopyTo("J:\drvimg", "F:\drvimg")

'----------------------------------------------------------------------
' 引数フォルダ src から dst にファイルを改装を維持しながらコピーする
'
' 2015/09/11 r.kojima
'----------------------------------------------------------------------
Function CopyTo(srcPath, dstPath)
	Dim fso: Set fso = WScript.CreateObject("Scripting.FileSystemObject")
	Dim dir: Set dir = fso.GetFolder(srcPath)
	Dim obj, dst
	'サブフォルダを列挙し先にコピーする
	For Each obj In dir.SubFolders
		Call CopyTo(obj.Path, fso.BuildPath(dstPath, obj.Name))
	Next
	'ファイルを列挙しコピーする
	For Each obj In dir.Files
		dst = fso.BuildPath(dstPath, obj.Name)
		If Not fso.FileExists(dst) Then
			WScript.Echo "コピーしなきゃ"
			WScript.Echo "元: " & obj.Path
			WScript.Echo "先: " & dst
		End If
	Next
End Function
