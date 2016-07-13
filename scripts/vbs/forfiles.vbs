'
' forfiles �R�}���h���ۂ��N���X
'
Class ForFiles
	Public m_Path
	Public m_Mask
	Public m_Subdir
	Public m_CallbackFunc

	Sub Class_Initialize
		Dim objShell: Set objShell = WScript.CreateObject("WScript.Shell")

		m_Path = objShell.CurrentDirectory
		m_Mask = "^.+$"
		m_Subdir = False
		Set m_CallbackFunc = GetRef("ForFiles_DefaultCallback")
	End Sub

	Function ForEach
		Dim objFs: Set objFs = WScript.CreateObject("Scripting.FileSystemObject")
		Dim objFolder: Set objFolder = objFs.GetFolder(m_Path)

		Dim oneObj

		For Each oneObj In objFolder.Files
			m_CallbackFunc(oneObj)
		Next

		Dim objFF: Set objFF = New ForFiles
		For Each oneObj In objFolder.SubFolders
			If m_Subdir Then
				objFF.m_Path = oneObj.Path
				objFF.m_Mask = m_Mask
				objFF.m_Subdir = m_Subdir
				Set objFF.m_CallbackFunc = m_CallbackFunc
				objFF.ForEach
			End If
			m_CallbackFunc(oneObj)
		Next
	End Function
End Class

'
' �f�t�H���g�̃R�[���o�b�N�֐��Ƃ��ď���
'
Function ForFiles_DefaultCallback(obj)
	WScript.Echo obj.Name
End Function

'==== TEST CODE ====

'
' �A�[�J�C�u������ ON �̃t�@�C�������X�g�t�@�C���ɒǋL����
'
Function CreateList(obj)
	Const Archive = 32

	If (TypeName(obj) = "File") And ((obj.Attributes And Archive) = Archive) Then
		WScript.Echo obj.Attributes, obj
	End If
End Function

Dim obj: Set obj = New ForFiles
obj.m_Subdir = True
Set obj.m_CallbackFunc = GetRef("CreateList")
obj.ForEach
