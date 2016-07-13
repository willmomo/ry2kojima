'----------------------------------------------------------------------
' 2015/04/28 作成
'----------------------------------------------------------------------
Class KjmLog
	Private m_fso
	Private m_folderName
	Private m_baseName
	Private m_rotateSize
	Private m_rotateCount
	
	Property Get BaseName
		BaseName = m_baseName
	End Property
	
	Property Get FolderName
		FolderName = m_folderName
	End Property
	
	Property Get FullName
		FullName = m_fso.BuildPath(m_folderName, m_baseName & ".log")
	End Property
	
	Property Get RotateSize
		RotateSize = m_rotateSize
	End Property
	
	Property Let RotateSize(expr)
		m_rotateSize = expr
	End Property
	
	Property Get RotateCount
		RotateCount = m_rotateCount
	End Property
	
	Property Let RotateCount(expr)
		m_rotateCount = expr
	End property
	
	' ログの保存先を %TEMP%\<subdir> にする
	Sub SetLogPath_Temp(subdir)
		Const TemporaryFolder = 2
		m_folderName = m_fso.BuildPath(m_fso.GetSpecialFolder(TemporaryFolder), subdir)
	End Sub
	
	' ログの保存先を スクリプトと同じ場所\<subdir> にする
	Sub SetLogPath_App(subdir)
		m_folderName = m_fso.BuildPath(m_fso.GetParentFolderName(WScript.ScriptFullName), subdir)
	End Sub
	
	Sub CreatePath(expr)
		If Not m_fso.FolderExists(expr) Then
			CreatePath m_fso.GetParentFolderName(expr)
			m_fso.CreateFolder expr
		End If
	End Sub
	
	Sub Rotate
		Dim srcName, dstName, n
		If m_fso.GetFile(FullName).Size > m_rotateSize Then
			For n = m_rotateCount - 1 To 1 Step -1
				srcName = m_fso.BuildPath(FolderName, BaseName & "_" & CStr(n) & ".log")
				dstName = m_fso.BuildPath(FolderName, BaseName & "_" & CStr(n+1) & ".log")
				If m_fso.FileExists(srcName) Then
					If m_fso.FileExists(dstName) Then m_fso.DeleteFile(dstName)
					m_fso.MoveFile srcName, dstName
				End If
			Next
			m_fso.MoveFile FullName, m_fso.BuildPath(FolderName, BaseName & "_1.log")
		End If
	End Sub
	
	Sub Write(msg)
		On Error Resume Next
		CreatePath m_folderName
		msg = Now & " " & msg
		WScript.StdOut.WriteLine msg
		Const ForAppending = 8
		Dim ts, retry
		For retry = 0 To 9
			Set ts = m_fso.OpenTextFile(FullName, ForAppending, True)
			If Not IsEmpty(ts) Then
				ts.WriteLine msg
				ts.Close
				Rotate
				Exit For
			End If
			WScript.Sleep(200)
		Next
	End Sub
	
	Sub Info(msg)
		Write "I/ " & msg
	End Sub
	
	Sub Error(msg)
		Write "E/ " & msg
	End Sub
	
	Sub Class_Initialize
		Set m_fso = WScript.CreateObject("Scripting.FileSystemObject")

		SetLogPath_Temp ""	' %TEMP%フォルダにログを作るのがデフォルト
		m_baseName = m_fso.GetBaseName(WScript.ScriptFullName)
		
		m_rotateSize = 1 * 1024 * 1024
		
		m_rotateCount = 9
	End Sub
	
	Sub Class_Terminate
	End Sub
End Class

Dim KjmLog_singleton_instance__
Function GetKjmLog
	If IsEmpty(KjmLog_singleton_instance__) Then Set KjmLog_singleton_instance__ = New KjmLog
	Set GetKjmLog = KjmLog_singleton_instance__
End Function
