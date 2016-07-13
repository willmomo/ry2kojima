Option Explicit

'----------------------------------------------------------------------
' fname で指定されたファイルの最終更新日を現在日時にする
'----------------------------------------------------------------------
Function Touch(fname)
	Dim fso: Set fso = WScript.CreateObject("Scripting.FileSystemObject")
	Dim app: Set app = WScript.CreateObject("Shell.Application")
	Dim dir: Set dir = app.NameSpace(fso.GetParentFolderName(fname))
	Dim fil: Set fil = dir.ParseName(fso.GetFileName(fname))
	fil.ModifyDate = Now
End Function
