<%
'----------------------------------------------------------------------
' ASPで使用できる、ユーティリティ関数(ver.0.2010.01.15)
'
' [使い方]
' 使いたいファイルで、
'
' <!-- #include file="util.asp" -->
'
' のように、インクルードして使用する。
'----------------------------------------------------------------------

'----------------------------------------------------------------------
' IIf(Immediate-If)関数
'
' [説明]
' なぜか、VBScriptに実装されていない、VB6良く使う関数。
'----------------------------------------------------------------------
Function IIf(expr, truepart, falsepart)
	If expr Then IIf = truepart Else IIf = falsepart
End Function
%>
