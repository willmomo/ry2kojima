Option Explicit

'------------------------------------------------------------------------------
' WScript.Shell.Run
'
' last modified: 2011/07/25
'------------------------------------------------------------------------------
Public Function kjmShellRun(strCommand, intWindowStyle, bWaitOnReturn)
	Dim obj
	
	Set obj = WScript.CreateObject("WScript.Shell")
	kjmShellRun = obj.Run(strCommand, intWindowStyle, bWaitOnReturn)
End Function

' intWinodwStyle �p�萔
Public Const SW_HIDE = 0	' �E�B���h�E���\���ɂ��A�ʂ̃E�B���h�E���A�N�e�B�u�ɂ��܂��B

'------------------------------------------------------------------------------
' Windows 2000 : 5
' Windows XP : 5.1
'------------------------------------------------------------------------------
Function GetOSVersion()
	Dim colTarget,objRow,str,aData,nData, I, nTarget

	Set colTarget = GetObject("winmgmts:\\.\root\cimv2").ExecQuery("select * from Win32_OperatingSystem")
	For Each objRow in colTarget
		str = objRow.Version
	Next
	
	aData = Split( str, "." )
	For I = 0 to Ubound( aData )
		if I > 1 then
			Exit For
		end if
		if I > 0 then
			nTarget = nTarget & "."
		end if
		nTarget = nTarget & aData(I)
	Next
	GetOSVersion = CDbl(nTarget)
	
	GetOSVersion = 5.0
End Function

'------------------------------------------------------------------------------
' [�^�X�N]�ɃX�P�W���[����ǉ�����B
'------------------------------------------------------------------------------
Private Sub TaskCreateOnWindows2000()
	Dim result, objFs, sFileName, strLine, objTs, alreadyExists
	
	alreadyExists = False
	
	Set objFs = WScript.CreateObject("Scripting.FileSystemObject")

	' ���_�C���N�g��̃t�@�C�����𐶐����܂��B  
	Do: sFileName = objFs.BuildPath(objFs.GetSpecialFolder(2), objFs.GetTempName)
	Loop While objFs.FileExists(sFileName)
	
	Call kjmShellRun("%ComSpec% /c at > " & sFileName, SW_HIDE, True)

	If objFs.FileExists(sFileName) Then
		Set objTs = objFs.OpenTextFile(sFileName)
		Do Until objTs.AtEndOfStream = true
			strLine = objTs.ReadLine
			If InStr(LCase(strLine), "kishu.exe") > 0 Then
				alreadyExists = True
				Exit Do
			End If
		Loop
		objTs.Close
		objFs.DeleteFile sFileName
	End If

	If alreadyExists = False Then
		Call kjmShellRun("at 6:00 /every:M,T,W,Th,F,S,Su ""C:\mpc3\tools\kisyu_tool\KisyuAutoAdd.exe -Auto""", SW_HIDE, True)
	End If
End Sub

'------------------------------------------------------------------------------
' [�^�X�N]�ɃX�P�W���[����ǉ�����B
'
' Windows 2000�̂Ƃ��́Aat�R�}���h���g�p�B
' Windows 2000�ȊO�̂Ƃ��́Aschtasks�R�}���h���g�p�B
'------------------------------------------------------------------------------
Private Sub TaskCreate()
	dim result
	
	If GetOSVersion() = 5 Then
		TaskCreateOnWindows2000
	else
		result = kjmShellRun("schtasks /Create /TN �������t�ݒ� /RU kojima /RP Kojima /SC DAILY /ST 06:00:00 /TR ""C:\mpc3\tools\kisyu_tool\KisyuAutoAdd.exe -Auto""", SW_HIDE, True)
	end if
End Sub

'------------------------------------------------------------------------------
' [�^�X�N]����X�P�W���[�����폜����B
'
' Windows 2000�́Aat�R�}���hid���擾���Ă���łȂ��ƁA�폜���ׂ�ID��������Ȃ�
' ���߁A���X���G�ȏ������K�v�B
'------------------------------------------------------------------------------
Private Sub TaskDeleteOnWindows2000()
	Dim result, objFs, sFileName, strLine, objTs
	
	Set objFs = WScript.CreateObject("Scripting.FileSystemObject")

	' ���_�C���N�g��̃t�@�C�����𐶐����܂��B  
	Do: sFileName = objFs.BuildPath(objFs.GetSpecialFolder(2), objFs.GetTempName)
	Loop While objFs.FileExists(sFileName)
	
	Call kjmShellRun("%ComSpec% /c at > " & sFileName, SW_HIDE, True)

	If objFs.FileExists(sFileName) Then
		Set objTs = objFs.OpenTextFile(sFileName)
		Do Until objTs.AtEndOfStream = true
			strLine = objTs.ReadLine
			If InStr(LCase(strLine), "kishu.exe") > 0 Then
				Call kjmShellRun("at " & CLng(Left(strLine, 9)) & " /delete", SW_HIDE, True)
			End If
		Loop
		objTs.Close
		objFs.DeleteFile sFileName
	End If
End Sub

'------------------------------------------------------------------------------
' [�^�X�N]����X�P�W���[�����폜����B
'
' Windows 2000�̂Ƃ��́Aat�R�}���h���g�p�B
' Windows 2000�ȊO�̂Ƃ��́Aschtasks�R�}���h���g�p�B
'------------------------------------------------------------------------------
private sub TaskDelete()
	Dim result

	if GetOSVersion() = 5 then
		TaskDeleteOnWindows2000
	else
		result = kjmShellRun("schtasks /Delete /TN �������t�ݒ� /F", SW_HIDE, True)
	End If
end sub


Call TaskCreate
''Call TaskDelete

