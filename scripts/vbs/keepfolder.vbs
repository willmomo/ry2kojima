Option Explicit

' �t�H���_�̐��������̐��Ɏ��߂�悤�ɍ폜����
'
' �R�[���o�b�N�֐����w�肷��ƁA�������O�Ɏc�����Ƃ��ł��܂��B
' �R�[���o�b�N���s�v�Ȏ��� Null ���w�肵�܂��B
' (�������e�Ɋ����邱�Ƃ͂ł��܂���)
' action =-1 ���������Ȃ̂ō폜�A=0 �c�� =1 �폜
' ��)
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
			WScript.Echo "����", path, mtime
		Case 0
			WScript.Echo "�c��", path, mtime
		Case 1
			WScript.Echo "�폜", path, mtime
	End Select
End Sub

KeepFolders "L:\rsbackup_ng\rs\RS1", 20, GetRef("KeepFoldersCallback")
