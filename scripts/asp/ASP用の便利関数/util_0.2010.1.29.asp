<%
'----------------------------------------------------------------------
' ASP�Ŏg�p�ł���A���[�e�B���e�B�֐�(ver.0.2010.01.29)
'
' [�g����]
' �g�������t�@�C���ŁA
'
' <!-- #include file="util.asp" -->
'
' �̂悤�ɁA�C���N���[�h���Ďg�p����B
'----------------------------------------------------------------------

'----------------------------------------------------------------------
' HTMLEncode + (���s -> <br>) + (�󕶎��� -> &nbsp;)�ϊ�
'----------------------------------------------------------------------
Function HTMLEncode3(expr)
	HTMLEncode3 = Replace(Server.HTMLEncode("" & expr), vbNewLine, "<br>")
	If HTMLEncode3 = "" Then HTMLEncode3 = "&nbsp;"
End Function

'----------------------------------------------------------------------
' HTMLEncode + (���s -> <br>) �ϊ�
'----------------------------------------------------------------------
Function HTMLEncode2(expr)
	HTMLEncode2 = Replace(Server.HTMLEncode("" & expr), vbNewLine, "<br>")
End Function


'----------------------------------------------------------------------
' IIf(Immediate-If)�֐�
'
' [����]
' �Ȃ����AVBScript�Ɏ�������Ă��Ȃ��AVB6�ǂ��g���֐��B
'----------------------------------------------------------------------
Function IIf(expr, truepart, falsepart)
	If expr Then IIf = truepart Else IIf = falsepart
End Function


'----------------------------------------------------------------------
' ���ˑ������𓯓������ɒu������֐�
'
' result �����́A�͂����茾���ė]���Ȃ̂ŁA�����폜���܂��傤�B
'----------------------------------------------------------------------
Function ReplaceDependenceChar(expr, result)
	result = ""
	
	' �� �� �H �ɒu��
	If InStr(expr, "��") > 0 Then
		result = "���ˑ��������y�H�z�ɕϊ����܂����B"
	End If
	ReplaceDependenceChar = Replace(expr, "��", "�H")
End Function
%>
