<html>
<body>

<h1>IPアドレス一覧</h1>

<hr>

<form method="post" action="iplistadd.asp" id=form1 name=form1>

<table border="1">
<tr>
<th>IPアドレス</th><th>コンピュータ名</th><th>覚え書</th>
</tr>

<%
Set db=Server.CreateObject("ADODB.Connection")
Set rs=Server.CreateObject("ADODB.Recordset")
db.Open "iplist"

'192.168.100.0～9をソートしてリスティング
rs.Open "select * from T_IP where IP like '192.168.100._' order by IP", db
Do Until rs.EOF
	Response.Write "<tr>"
	Response.Write "<td nowrap>" & rs.Fields(0) & "</td>"
	Response.Write "<td nowrap>" & rs.Fields(1) & "</td>"
	Response.Write "<td>" & rs.Fields(2) & "</td>"
	Response.Write "<td nowrap><a href='iplistdel.asp?ip=" & rs.Fields(0) & "'>削除</a></td>"
	Response.Write "</tr>"
	
	rs.MoveNext
Loop
rs.Close

'192.168.100.10～99をソートしてリスティング
rs.Open "select * from T_IP where IP like '192.168.100.__' order by IP", db
Do Until rs.EOF
	Response.Write "<tr>"
	Response.Write "<td nowrap>" & rs.Fields(0) & "</td>"
	Response.Write "<td nowrap>" & rs.Fields(1) & "</td>"
	Response.Write "<td>" & rs.Fields(2) & "</td>"
	Response.Write "<td nowrap><a href='iplistdel.asp?ip=" & rs.Fields(0) & "'>削除</a></td>"
	Response.Write "</tr>"
	
	rs.MoveNext
Loop
rs.Close

'192.168.100.100～255をソートしてリスティング
rs.Open "select * from T_IP where IP like '192.168.100.___' order by IP", db
Do Until rs.EOF
	Response.Write "<tr>"
	Response.Write "<td nowrap>" & rs.Fields(0) & "</td>"
	Response.Write "<td nowrap>" & rs.Fields(1) & "</td>"
	Response.Write "<td>" & rs.Fields(2) & "</td>"
	Response.Write "<td nowrap><a href='iplistdel.asp?ip=" & rs.Fields(0) & "'>削除</a></td>"
	Response.Write "</tr>"
	
	rs.MoveNext
Loop
rs.Close

db.Close
Set rs=Nothing
Set db=Nothing
%>

<tr>
<td><input type="text" name="add_ip" maxlength="15"></td>
<td><input type="text" name="add_pc"></td>
<td><input type="text" name="add_note"></td>
<td><input type="submit" value="追加" id=submit1 name=submit1></td>
</tr>
</table>
</form>

<hr>

<p><a href="javascript:history.back()">戻る</a></p>

</body>
</html>
