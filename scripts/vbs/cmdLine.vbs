Option Explicit

'----------------------------------------------------------------------
' コマンドライン解析クラス
'
' Named プロパティ(Dictionary)にオプションとオプションの引数が入る
' Unnamed プロパティに(Array)にオプション以外の引数が入る
'
' --- 使用例 ---
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
