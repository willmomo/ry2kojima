Option Explicit

'----------------------------------------------------------------------
' fname �Ŏw�肳�ꂽ�t�@�C���̍ŏI�X�V�������ݓ����ɂ���
'----------------------------------------------------------------------
Function Touch(fname)
	Dim fso: Set fso = WScript.CreateObject("Scripting.FileSystemObject")
	Dim app: Set app = WScript.CreateObject("Shell.Application")
	Dim dir: Set dir = app.NameSpace(fso.GetParentFolderName(fname))
	Dim fil: Set fil = dir.ParseName(fso.GetFileName(fname))
	fil.ModifyDate = Now
End Function
