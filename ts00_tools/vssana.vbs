Option Explicit

'----------------------------------------------------------------------
' write log
'----------------------------------------------------------------------
Sub LogWrite(msg)
	Const TemporaryFolder = 2
	Const ForApending = 8
	Dim fso, fname, tfile
	
	Set fso = WScript.CreateObject("Scripting.FileSystemObject")
	fname = "c:\tools\vssanalog.txt"
	Set tfile = fso.OpenTextFile(fname, ForApending, True)
	tfile.WriteLine(Now & " " & msg)
	tfile.Close
	
	Set tfile = Nothing
	Set fso = Nothing
End Sub

'----------------------------------------------------------------------
' vss��analyze�����s����
'----------------------------------------------------------------------
Sub RunAnalyze()
	Dim objShell
	Dim objFso
	Dim analyze
	Dim s

	Set objFso = WScript.CreateObject("Scripting.FileSystemObject")
	Set objShell = CreateObject("WScript.Shell")

	analyze = """C:\Program Files\Microsoft Visual SourceSafe\analyze.exe"""

	objFso.DeleteFolder "O:\vssback", True
	objFso.CreateFolder "O:\vssback"
	For Each s In Array("O:\vssback\vssas", "O:\vssback\vsschecker", "O:\vssback\vssdata2", "O:\vssback\vsskspc", "O:\vssback\vsslain2web", "O:\vssback\vsslinux", "O:\vssback\vsspsv", "O:\vssback\vsspsv2")
		objFso.CreateFolder s
	Next

	Call objShell.Run(analyze & " -i- -c -d -f -v3 -bO:\vssback\vssas O:\vssas\data", 1, True)
	Call objShell.Run(analyze & " -i- -c -d -f -v3 -bO:\vssback\vsschecker O:\vsschecker\data", 1, True)
	Call objShell.Run(analyze & " -i- -c -d -f -v3 -bO:\vssback\vssdata2 O:\vssdata2\data", 1, True)
	Call objShell.Run(analyze & " -i- -c -d -f -v3 -bO:\vssback\vsskspc O:\vsskspc\data", 1, True)
	Call objShell.Run(analyze & " -i- -c -d -f -v3 -bO:\vssback\vsslain2web O:\vsslain2web\data", 1, True)
	Call objShell.Run(analyze & " -i- -c -d -f -v3 -bO:\vssback\vsslinux O:\vsslinux\data", 1, True)
	Call objShell.Run(analyze & " -i- -c -d -f -v3 -bO:\vssback\vsspsv O:\vsspsv\data", 1, True)
	Call objShell.Run(analyze & " -i- -c -d -f -v3 -bO:\vssback\vsspsv2 O:\vsspsv2\data", 1, True)
''	Call objShell.Run(analyze & " -x -v3 -bO:\vssback\vsspsv2 O:\vsspsv2\data", 1, True)

	Set objShell = Nothing
End Sub


'----------------------------------------------------------------------
' ���ʂ����[���ő��M
'----------------------------------------------------------------------
Sub ResultMail()
	Dim b21
	Dim ret
	Dim strServer, strFrom, strTo, strSubject, strBody, strFiles

	Set b21 = WScript.CreateObject("basp21")

	strServer = "59.106.72.142"
	strFrom = "r.kojima <ry2kojima@sanko-net.co.jp>"
	strTo = "ry2kojima@sanko-net.co.jp"
	strTo = strTo & vbTab & "ta2ohno@sanko-net.co.jp"
	strSubject = "VSS��Analyze���ʁA�ʒm���[�� on TS00"
	strBody = "VSS��Analyze���ʁA�ʒm���[�� on TS00 �ł��B"

	'�Y�t�t�@�C��
	strFiles = "O:\vssback\vssas\analyze.log|vssas|text/plain"
	strFiles = strFiles & vbTab & "O:\vssback\vsschecker\analyze.log|vsschecker|text/plain"
	strFiles = strFiles & vbTab & "O:\vssback\vssdata2\analyze.log|vssdata2|text/plain"
	strFiles = strFiles & vbTab & "O:\vssback\vsskspc\analyze.log|vsskspc|text/plain"
	strFiles = strFiles & vbTab & "O:\vssback\vsslain2web\analyze.log|vsslain2web|text/plain"
	strFiles = strFiles & vbTab & "O:\vssback\vsslinux\analyze.log|vsslinux|text/plain"
	strFiles = strFiles & vbTab & "O:\vssback\vsspsv\analyze.log|vsspsv|text/plain"
	strFiles = strFiles & vbTab & "O:\vssback\vsspsv2\analyze.log|vsspsv2|text/plain"

	ret = b21.SendMail(strServer, strTo, strFrom , strSubject, strBody, strFiles)

	Set b21 = Nothing

End Sub


'----------------------------------------------------------------------
' �v���O�����X�^�[�g
'----------------------------------------------------------------------
Sub Main()
	Call RunAnalyze
	Call ResultMail
End Sub


'----------------------------------------------------------------------

'���s
LogWrite "VSS Analyze script start."

Call Main

LogWrite "VSS Analyze script end."
LogWrite ""

