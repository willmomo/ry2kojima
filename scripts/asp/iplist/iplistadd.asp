<%

Set db=Server.CreateObject("ADODB.Connection")
db.Open "iplist"

on error resume next
SQL = "insert into T_IP(IP,Name,[Note]) values('" & Request.Form("add_ip") & "','" & Request.Form ("add_pc") & "','" & Request.Form("add_note") & "')"
db.Execute SQL
on error goto 0

if db.Errors.count > 0 then
%>
<html>
<head>
</head>
<body>
<h1>Errorが発生しました。</h1>
<table border="1">
<tr>
<th>時間</th><th>発生箇所</th><th>詳細</th><th>SQL</th>
</tr>
<%
for i = 0 to db.Errors.count - 1
	Response.Write "<tr><td nowrap>" & Now & "</td>"
	Response.Write "<td>" & db.Errors.item(i).source & "</td>"
	Response.Write "<td>" & db.Errors.item(i).description & "</td>"
	Response.Write "<td>" & SQL & "</td></tr>"
next
%>
</table>

<p>上記エラーをコピー/ペーストして<a href="mailto:ry2kojima@halsys.co.jp">小島</a>までメールしてください。</p>

<hr>

<p><a href="javascript:history.back()">戻る</a></p>

</body>
</html>
<%
else
	Response.Redirect "iplist.asp"
end if

%>
