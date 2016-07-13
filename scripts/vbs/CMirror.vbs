'----------------------------------------------------------------------
' mirror.exe ラッパークラス
'
' WSF ファイルの中で
' <script language="VBScript" src="CMirror.vbs"></script>
' のようにして使ってください。
'----------------------------------------------------------------------
Class CMirror
	Private m_logPath
	Private m_orgPath
	Private m_mirPath
	Private m_excludeOpt
	Private m_mode
	Private m_test

	Public Property Let LogPath(newValue)
		m_logPath = newValue
	End Property
	
	Public Property Get LogPath()
		LogPath = m_logPath
	End Property

	Public Property Let OrgPath(newValue)
		m_orgPath = newValue
	End Property
	
	Public Property Get OrgPath()
		OrgPath = m_orgPath
	End Property
	
	Public Property Let MirPath(newValue)
		m_mirPath = newValue
	End Property
	
	Public Property Get MirPath()
		MirPath = m_mirPath
	End Property
	
	Public Property Let ExcludeOption(newValue)
		m_excludeOpt = newValue
	End Property

	Public Property Get ExcludeOption()
		ExcludeOption = m_excludeOption
	End Property

	Public Property Let Mode(newValue)
		m_mode = newValue
	End Property

	Public Property Get Mode()
		Mode = m_mode
	End Property

	Public Property Let Test(newValue)
		m_test = newValue
	End Property

	Public Property Get Test()
		Test = m_test
	End Property

	Public Sub Run()
		Dim strCmd, objShell
		
		strCmd = "mirror.exe"
		strCmd = strCmd & " """ & m_orgPath & """"
		strCmd = strCmd & " """ & m_mirPath & """"

		If m_excludeOpt <> "" Then
			strCmd = strCmd & " --exclude=""" & m_excludeOpt & """"
		End If

		If m_logPath <> "" Then
			strCmd = strCmd & " --logpath=""" & m_logPath & """"
		End If

		If m_mode <> "" Then
			strCmd = strCmd & " --mode=" & m_mode
		End If

		If m_test <> False Then
			strCmd = strCmd & " --test"
		End If

		Set objShell = WScript.CreateObject("WScript.Shell")
		Call objShell.Run(strCmd, 1, True)
	End Sub

	Private Sub Class_Initialize()
		m_test = False
	End Sub
End Class
