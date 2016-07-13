<%
'----------------------------------------------------------------------
' ASPで使用できる、ユーティリティ関数(ver.0.2010.02.25)
'
' [使い方]
' 使いたいファイルで、
'
' <!-- #include file="util.asp" -->
'
' のように、インクルードして使用する。
'----------------------------------------------------------------------

'----------------------------------------------------------------------
' HTMLEncode + (改行 -> <br>) + (空文字列 -> &nbsp;)変換
'----------------------------------------------------------------------
Function HTMLEncode3(expr)
	HTMLEncode3 = Replace(Server.HTMLEncode("" & expr), vbNewLine, "<br>")
	If HTMLEncode3 = "" Then HTMLEncode3 = "&nbsp;"
End Function

'----------------------------------------------------------------------
' HTMLEncode + (改行 -> <br>) 変換
'----------------------------------------------------------------------
Function HTMLEncode2(expr)
	HTMLEncode2 = Replace(Server.HTMLEncode("" & expr), vbNewLine, "<br>")
End Function


'----------------------------------------------------------------------
' IIf(Immediate-If)関数
'
' [説明]
' なぜか、VBScriptに実装されていない、VB6良く使う関数。
'----------------------------------------------------------------------
Function IIf(expr, truepart, falsepart)
	If expr Then IIf = truepart Else IIf = falsepart
End Function


'----------------------------------------------------------------------
' 環境依存文字を同等文字に置換する関数
'
' result 引数は、はっきり言って余分なので、いつか削除しましょう。
'----------------------------------------------------------------------
Function ReplaceDependenceChar(expr, result)
	result = ""
	
	' �� を 羽 に置換
	If InStr(expr, "��") > 0 Then
		result = "環境依存文字を【羽】に変換しました。"
	End If
	ReplaceDependenceChar = Replace(expr, "��", "羽")
End Function
%>
