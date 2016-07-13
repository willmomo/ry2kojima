'----------------------------------------------------------------------
'----------------------------------------------------------------------
Option Explicit

Dim g_argument()

WScript.Quit(DoMain)

'----------------------------------------------------------------------
' �z��̗v�f�����擾
'----------------------------------------------------------------------
Function Array_GetCount(array)
	On Error Resume Next
	Array_GetCount = UBound(array) + 1
	If Err.Number = 9 Then Array_GetCount = 0
End Function

'----------------------------------------------------------------------
' �z��̌��ɗv�f��ǉ�
'----------------------------------------------------------------------
Function Array_Append(array, object)
	Dim size
	size = Array_GetCount(array)
	ReDim Preserve array(size)
	array(size) = object
End Function

'----------------------------------------------------------------------
' �����̃`�F�b�N
'----------------------------------------------------------------------
Function ParseCmdLine()
	On Error Resume Next
	
	Dim i
	
	For i = 0 To WScript.Arguments.Count - 1
		Select Case Left(WScript.Arguments.Item(i), 2)
			Case "/h"
				WScript.Echo "/h"
				
			Case Else
				WScript.Echo Array_GetCount(g_argument)
				Array_Append g_argument, WScript.Arguments.Item(i)
				WScript.Echo Array_GetCount(g_argument)
		End Select
	Next
End Function

'----------------------------------------------------------------------
' �v���O�����X�^�[�g
'----------------------------------------------------------------------
Function DoMain()
	Dim objProcs
	Dim objProc		'As Win32_Process
	Dim procName
	Dim strMsg

	ParseCmdLine
	
	procName = WScript.Arguments.Item(0)

	Set objProcs = GetObject("winmgmts:{impersonationLevel=impersonate}").ExecQuery("select * from Win32_Process where name like '%" & procName & "%'")

	For Each objProc In objProcs
		WScript.Echo objProc.Name
		strMsg = strMsg & objProc.ProcessId & " : " & objProc.ExecutablePath & vbNewLine
	Next

	Set objProcs = Nothing

	WScript.Echo strMsg
	
	DoMain = 0
End Function
