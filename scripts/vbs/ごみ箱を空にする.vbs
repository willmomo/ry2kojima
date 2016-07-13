Option Explicit

Call EmptyTrushBox2

WScript.Quit
'---- ここまで ----------------------------------------------------------------

'------------------------------------------------------------------------------
' 一行で実行できるが、確認ダイアログが出る方法
'------------------------------------------------------------------------------
Sub EmptyTrushBox1()
	WScript.CreateObject("Shell.Application").NameSpace(10).Self.InvokeVerb "ごみ箱を空にする(&B)"
End Sub


'------------------------------------------------------------------------------
' 確認ダイアログなしで、ごみ箱を空にする方法
'------------------------------------------------------------------------------
Sub EmptyTrushBox2()
	Const ssfBITBUCKET = 10

	Dim fso
	Dim Shell
	Dim Folder
	Dim FolderItem
	Dim sFolder

	Set fso = WScript.CreateObject("Scripting.FileSystemObject")
	Set Shell = WScript.CreateObject("Shell.Application")
	Set Folder = Shell.NameSpace(ssfBITBUCKET)

	For Each FolderItem In Folder.Items()
		If fso.FileExists(FolderItem.Path) Then
			fso.DeleteFile FolderItem.Path,True
		ElseIf fso.FolderExists(FolderItem.Path) Then
			fso.DeleteFolder FolderItem.Path,True
		End If
	Next

	If Folder.Items().Count = 0 Then
		Set sFolder = Shell.NameSpace(WScript.ScriptFullName & "\..")

		Folder.MoveHere sFolder.Items().Item(WScript.ScriptName)
		Do While Not sFolder.Items().Item(WScript.ScriptName) Is Nothing
			WScript.Sleep 100
		Loop

		sFolder.MoveHere Folder.Items().Item(0)
	End If
End Sub
