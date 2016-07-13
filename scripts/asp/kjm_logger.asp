<%
'======================================================================
' kjm_logger.asp ver.0.2015.3.24
'
' �g����
' <!-- #include file="kjm_logger.asp" -->
' Dim log: Set log = Logger_GetInstance
' log.FileName = "my_log_name.log"
' log.Write "message text"
'======================================================================

Dim singleton_Logger__

'----------------------------------------------------------------------
' �V���O���g����Logger�C���X�^���X��Ԃ�
'----------------------------------------------------------------------
Function Logger_GetInstance
	If IsEmpty(singleton_Logger__) Then Set singleton_Logger__ = New Logger
	Set Logger_GetInstance = singleton_Logger__
End Function

'======================================================================
' ���O�o�̓N���X
'======================================================================
Class Logger
	Private m_fso		' as Scripting.FileSystemObject
	Private m_path		' as String
	Private m_fname		' as String
	
	'------------------------------------------------------------------
	' ���O�̃t�@�C�����������擾
	'------------------------------------------------------------------
	Public Property Get FileName
		FileName = m_fname
	End Property
	
	'------------------------------------------------------------------
	' ���O�̃t�@�C����������ݒ�
	'------------------------------------------------------------------
	Public Property Let FileName(Value)
		m_fname = Value
	End Property

	'------------------------------------------------------------------
	' �R���X�g���N�^
	'------------------------------------------------------------------
	Private Sub Class_Initialize()
		Set m_fso = Server.CreateObject("Scripting.FileSystemObject")
		
		Dim path: path = Server.MapPath(Request.ServerVariables("URL"))
		m_path = m_fso.GetParentFolderName(path)
		m_fname = m_fso.GetBaseName(path) & ".log"
	End Sub
	
	'------------------------------------------------------------------
	' �f�X�g���N�^
	'------------------------------------------------------------------
	Private Sub Class_Terminate()
		Set m_fso = Nothing
	End Sub

	'------------------------------------------------------------------
	' ���O�t�@�C���̃t���p�X���擾
	'------------------------------------------------------------------
	Private Function GetFullName
		GetFullName = m_fso.BuildPath(m_path, m_fname)
	End Function
	
	'------------------------------------------------------------------
	' �X�N���v�g�̃x�[�X�t�@�C�������擾
	'------------------------------------------------------------------
	Private Function GetBaseName
		GetBaseName = m_fso.GetBaseName(Server.MapPath(Request.ServerVariables("URL")))
	End Function
	
	'------------------------------------------------------------------
	' ������������擾
	'------------------------------------------------------------------
	Private Function GetDate
		Dim dateTimer: dateTimer = Timer
		GetDate = CStr(Now) & "." & Right("000" & Fix((dateTimer - Fix(dateTimer)) * 1000), 3)
	End Function
	
	'------------------------------------------------------------------
	' �f�B���N�g���̍쐬�i�ċA�����Ή��j
	'------------------------------------------------------------------
	Private Sub mkdir(path)
		If m_fso.FolderExists(path) Then Exit Sub
		mkdir m_fso.GetParentFolderName(path)
		m_fso.CreateFolder(path)
	End Sub

	'------------------------------------------------------------------
	' ping���g���đ҂�����
	'------------------------------------------------------------------
	Private Sub Sleep(msec)
		Dim cmd: cmd = "PING 10." & Hour(Now()) & "." & Minute(Now()) & "." & Second(Now()) & " -n 1 -w " & msec
		CreateObject("WScript.Shell").Run cmd, 0, True
	End Sub

	'------------------------------------------------------------------
	' �傫���Ȃ������O���V�t�g����
	'------------------------------------------------------------------
	Private Sub Shift
		On Error Resume Next
		Dim num, toFname
		If m_fso.GetFile(GetFullName).Size > (2 * 1024 * 1024) Then
			toFname = m_fso.BuildPath(m_path, m_fso.GetBaseName(m_fname) & "_" & Replace(Replace(Replace(CStr(Now), "/", ""), ":", ""), " ", "_") & ".log")
			m_fso.MoveFile GetFullName, toFname
		End If
		On Error Goto 0
	End Sub
	
	'------------------------------------------------------------------
	' ���O�̏o��
	'------------------------------------------------------------------
	Private Sub Write(msg)
		Dim f, retry
		
		mkdir m_path
		
		Const ForAppending = 8
		
		On Error Resume Next
		Shift
		For retry = 0 To 9
			Set f = m_fso.OpenTextFile(GetFullName, ForAppending, True)
			If Err.Number = 0 Then Exit For
			Sleep 200
		Next
		On Error Goto 0

		If Not IsEmpty(f) Then
			f.WriteLine GetDate & vbTab & msg
			f.Close
			Set f = Nothing
		Else
			WScript.Echo "log file cannot open."
		End If
	End Sub
	
	'------------------------------------------------------------------
	' FATAL ERROR���O�̏o��
	'------------------------------------------------------------------
	Public Sub Fatal(msg)
		Write "FATAL " & GetBaseName & " - " & msg
	End Sub
	
	'------------------------------------------------------------------
	' ERROR���O�̏o��
	'------------------------------------------------------------------
	Public Sub Error(msg)
		Write "ERROR " & GetBaseName & " - " & msg
	End Sub
	
	'------------------------------------------------------------------
	' WARN���O�̏o��
	'------------------------------------------------------------------
	Public Sub Warn(msg)
		Write "WARN " & GetBaseName & " - " & msg
	End Sub
	
	'------------------------------------------------------------------
	' INFO���O�̏o��
	'------------------------------------------------------------------
	Public Sub Info(msg)
		Write "INFO " & GetBaseName & " - " & msg
	End Sub
	
	'------------------------------------------------------------------
	' DEBUG���O�̏o��
	'------------------------------------------------------------------
	Public Sub Debug(msg)
		Write "DEBUG " & GetBaseName & " - " & msg
	End Sub
	
	'------------------------------------------------------------------
	' TRACE���O�̏o��
	'------------------------------------------------------------------
	Public Sub Trace(msg)
		Write "TRACE " & GetBaseName & " - " & msg
	End Sub
End Class
%>
