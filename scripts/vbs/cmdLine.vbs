Option Explicit

'----------------------------------------------------------------------
' �R�}���h���C����̓N���X
'
' Named �v���p�e�B(Dictionary)�ɃI�v�V�����ƃI�v�V�����̈���������
' Unnamed �v���p�e�B��(Array)�ɃI�v�V�����ȊO�̈���������
'
' --- �g�p�� ---
' Set obj = New CmdLine
' obj.Parse
' If obj.Named.Exists("option") Then
'   If IsEmpty(obj.Named("option")) = False Then
'     otpionArgument = obj.Named("option")
'   End If
' End If
' For i = 0 To UBound(obj.Unnamed)
'   AnyAction obj.Unnamed(i), optionArgument
' Next
'----------------------------------------------------------------------
Class CmdLine
	Public Named
	Public Unnamed
	Public Sub Parse
		Dim reg: Set reg = WScript.CreateObject("VBScript.RegExp")
		reg.Pattern = "^[\-/]{1,2}([^\-/]+?)([:=](.+))?$"
		reg.IgnoreCase = True
		reg.Global = False
		Dim arg
		Dim matches
		For Each arg In WScript.Arguments
			If reg.Test(arg) Then
				Set matches = reg.Execute(arg)
				Named.Add matches(0).SubMatches(0), matches(0).SubMatches(2)
			Else
				ReDim Preserve Unnamed(UBound(Unnamed) + 1)
				Unnamed(UBound(Unnamed)) = arg
			End If
		Next
	End Sub
	Public Sub Class_Initialize
		Set Named = WScript.CreateObject("Scripting.Dictionary")
		Unnamed = Array()
	End Sub
End Class
