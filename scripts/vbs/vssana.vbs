
'----------------------------------------------------------------------
' start service
'----------------------------------------------------------------------
Function ServiceStart(strServiceName)

	Set serviceSet = GetObject("winmgmts:").ExecQuery("select * from Win32_Service where Name='" & strServiceName & "'")
	
	For Each service In serviceSet
		service.StartService()
	Next
	
	Set serviceSet = Nothing
	
End Function

'----------------------------------------------------------------------
' stop service
'----------------------------------------------------------------------
Function ServiceStop(strServiceName)

	Set serviceSet = GetObject("winmgmts:").ExecQuery("select * from Win32_Service where Name='" & strServiceName & "'")
	
	For Each service In serviceSet
		ret = service.StopService()
	Next
	
	Set serviceSet = Nothing
	
End Function

'----------------------------------------------------------------------
' �T�[�r�X���ċN������
'----------------------------------------------------------------------
Function ServiceReboot(strServiceName)

	Set serviceSet = GetObject("winmgmts:").ExecQuery("select * from Win32_Service where Name='" & strServiceName & "'")
	
	For Each service In serviceSet
		ret = service.StopService()
		service.StartService()
	Next
	
	Set serviceSet = Nothing
	
End Function


'----------------------------------------------------------------------
' vss��analyze�����s����
'----------------------------------------------------------------------
Sub RunAnalyze()

	Set objShell = CreateObject("WScript.Shell")

	Call objShell.Run("cmd /c del /s /q e:\vssback\*.*", 1, True)

	Call objShell.Run("analyze -i- -c -d -f -v4 -be:\vssback\ms-1000_vss d:\ms-1000_vss\data", 1, True)
	Call objShell.Run("analyze -i- -c -d -f -v4 -be:\vssback\vssas d:\vssas\data", 1, True)
	Call objShell.Run("analyze -i- -c -d -f -v4 -be:\vssback\vssdata d:\vssdata\data", 1, True)
	Call objShell.Run("analyze -i- -c -d -f -v4 -be:\vssback\vssdoc d:\vssdoc\data", 1, True)
	Call objShell.Run("analyze -i- -c -d -f -v4 -be:\vssback\vssppc d:\vssppc\data", 1, True)
	Call objShell.Run("analyze -i- -c -d -f -v4 -be:\vssback\vsslain2web d:\vsslain2web\data", 1, True)

	Set objShell = Nothing
	
End Sub


'----------------------------------------------------------------------
' ���ʂ����[���ő��M
'----------------------------------------------------------------------
Sub ResultMail()

	Set b21 = WScript.CreateObject("basp21")

	strServer = "59.106.72.142"
	strFrom = "r.kojima <ry2kojima@sanko-net.co.jp>"
	strTo = "ry2kojima@sanko-net.co.jp"
	strTo = strTo & vbTab & "ta2ohno@sanko-net.co.jp"
	strSubject = "VSS��Analyze���ʁA�ʒm���[��"
	strBody = "VSS��Analyze���ʁA�ʒm���[���ł��B"

	'�Y�t�t�@�C��
	strFiles = "E:\vssback\ms-1000_vss\analyze.log|ms-1000_vss|text/plain"
	strFiles = strFiles & vbTab & "E:\vssback\vssas\analyze.log|vssas|text/plain"
	strFiles = strFiles & vbTab & "E:\vssback\vssdata\analyze.log|vssdata|text/plain"
	strFiles = strFiles & vbTab & "E:\vssback\vssdoc\analyze.log|vssdoc|text/plain"
	strFiles = strFiles & vbTab & "E:\vssback\vssppc\analyze.log|vssppc|text/plain"
	strFiles = strFiles & vbTab & "E:\vssback\vsslain2web\analyze.log|vsslain2web|text/plain"

	ret = b21.SendMail(strServer, strTo, strFrom , strSubject, strBody, strFiles)

	Set b21 = Nothing

End Sub


'----------------------------------------------------------------------
' �v���O�����X�^�[�g
'----------------------------------------------------------------------
Sub Main()

	Call ServiceStop("sossvrsvc.net")
	Call RunAnalyze
	Call ResultMail
	Call ServiceStart("sossvrsvc.net")
End Sub


'----------------------------------------------------------------------

Set g_objFs = WScript.CreateObject("Scripting.FileSystemObject")
Set g_objTs = g_objFs.OpenTextFile("c:\vssanalog.txt", 8, True)

'���s
g_objTs.WriteLine(Now() & " VSS Analyze script start.")

Call Main

g_objTs.WriteLine(Now() & " VSS Analyze script end.")
g_objTs.WriteLine("")
g_objTs.Close

Set g_objTs = Nothing
Set g_objFs = Nothing
