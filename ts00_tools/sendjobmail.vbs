'----------------------------------------------------------------------
' job�������[�����M
'----------------------------------------------------------------------
Function SendJobMail(subject, state)
	Dim shell: Set shell = WScript.CreateObject("WScript.Shell")
	
	Dim cmd
	cmd = "c:\tools\smail.exe"
	cmd = cmd & " -hsec.sanko-net.co.jp"					'���[���T�[�o�[�w��
	cmd = cmd & " -fry2kojima@sanko-net.co.jp"				'���M�Ҏw��
	cmd = cmd & " -s""" & subject & " - " & state & """"	'�����w��
	cmd = cmd & " -T""" & subject & " - " & state & """"	'�{���w��
	cmd = cmd & " -aC:\tools\Log\serverjob.log"				'���O�t�@�C����Y�t
	cmd = cmd & " ry2kojima@sanko-net.co.jp"				'���M��w��
	
	SendJobEndMail = shell.Run(cmd, 0, True)
End Function
