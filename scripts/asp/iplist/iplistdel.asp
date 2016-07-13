<%

Set db=Server.CreateObject("ADODB.Connection")
db.Open "iplist"

SQL = "delete from T_IP where ip='" & Request.QueryString("ip") & "'"
db.Execute SQL

db.Close
Set db=Nothing

Response.Redirect "iplist.asp"

%>
