Option Explicit

'----------------------------------------------------------------------
' forfiles ���֐�
'
' @path �������n�߂�p�X
' @mask �����Ώۂ������鐳�K�\��
' @subdir True �̂Ƃ��T�u�f�B���N�g������������
'----------------------------------------------------------------------
Function ForFiles(path, mask, subdir, command)
	ForFiles = 1	'�t�@�C����������Ȃ���Ԃŏ�����
	
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
