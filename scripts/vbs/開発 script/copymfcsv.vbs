Option Explicit

Dim objFS, objFolder, objFile, objRegEx, objMatch, strNewName

'�����`�F�b�N
CheckArg

'FileSystemObject�̍쐬
Set objFS = WScript.CreateObject("Scripting.FileSystemObject")

'�w�肳�ꂽ�t�H���_��Folder�I�u�W�F�N�g���擾
Set objFolder = objFS.GetFolder(WScript.Arguments.Item(0))

'�w�肳�ꂽ�t�H���_������Amf*.�w����t.csv���
'���t������u�������ăR�s�[����B
Set objRegEx = New RegExp
objRegEx.Pattern = WScript.Arguments.Item(1) & "\.csv$"
For Each objFile In objFolder.Files
	If objRegEx.Test(objFile.Name) Then
		strNewName = objRegEx.Replace(objFile.Name, WScript.Arguments.Item(2) & ".csv")
		objFile.Copy objFolder.Path & "\" & strNewName, True
	End If
Next

'�����`�F�b�N
Sub CheckArg()
	If WScript.Arguments.Count <> 3 Then
		WScript.Echo "�g�p�@�Fcopymfcsv.vbs <mf*.YYYYMMDD.csv�����݂���t�H���_��> <�R�s�[�����t������(YYYYMMDD)> <�R�s�[����t������(YYYYMMDD)>"
		WScript.Quit
	End If
End Sub
