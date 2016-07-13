Option Explicit

' フォルダの数を引数の数に収めるように削除する
'
' コールバック関数を指定すると、情報をログに残すことができます。
' コールバックが不要な時は Null を指定します。
' (処理内容に干渉することはできません)
' action =-1 が未来日なので削除、=0 残す =1 削除
' 例)
' Sub CallbackFunc(action, path, mtime) ... End Sub
' Call KeepFolders("C:\temp", 10, GetRef("CallbackFunc"))
'
Sub KeepFolders(path, count, callback)
	Const adDBTimeStamp = 135
	Const adVarChar = 200
	
	Dim fso: Set fso = WScript.CreateObject("Scripting.FileSystemObject")
	Dim rs: Set rs = WScript.CreateObject("ADODB.Recordset")
	rs.Fields.Append "F_path", adVarChar, 512
	rs.Fields.Append "F_mtime", adDBTimeStamp
	rs.Open
	
	Dim obj
	For Each obj In fso.GetFolder(path).SubFolders
		rs.AddNew
		rs.Fields(0) = obj.Path
		rs.Fields(1) = obj.DateLastModified
		rs.Update
	Next
	
	rs.Sort = "F_mtime DESC"
	rs.MoveFirst
	
	Dim keepCount: keepCount = 0
	Do Until rs.EOF
		Dim action: action = 0
		If (rs(1) - Now) > 1 Then
			If Not IsNull(callback) Then Call callback(-1, rs(0), rs(1))
			fso.DeleteFolder rs(0), True
		ElseIf keepCount < count Then
			If Not IsNull(callback) Then Call callback(0, rs(0), rs(1))
			keepCount = keepCount + 1
		Else
			If Not IsNull(callback) Then Call callback(1, rs(0), rs(1))
			fso.DeleteFolder rs(0), True
		End If
		rs.MoveNext
	Loop
	
	rs.Close
	Set rs = Nothing
End Sub



Sub KeepFoldersCallback(action, path, mtime)
	Select Case action
		Case -1
			WScript.Echo "未来", path, mtime
		Case 0
			WScript.Echo "残す", path, mtime
		Case 1
			WScript.Echo "削除", path, mtime
	End Select
End Sub

KeepFolders "L:\rsbackup_ng\rs\RS1", 20, GetRef("KeepFoldersCallback")
