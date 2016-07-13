'----------------------------------------------------------------------
' job完了メール送信
'----------------------------------------------------------------------
Function SendJobMail(subject, state)
	Dim shell: Set shell = WScript.CreateObject("WScript.Shell")
	
	Dim cmd
	cmd = "c:\tools\smail.exe"
	cmd = cmd & " -hsec.sanko-net.co.jp"					'メールサーバー指定
	cmd = cmd & " -fry2kojima@sanko-net.co.jp"				'送信者指定
	cmd = cmd & " -s""" & subject & " - " & state & """"	'件名指定
	cmd = cmd & " -T""" & subject & " - " & state & """"	'本文指定
	cmd = cmd & " -aC:\tools\Log\serverjob.log"				'ログファイルを添付
	cmd = cmd & " ry2kojima@sanko-net.co.jp"				'送信先指定
	
	SendJobEndMail = shell.Run(cmd, 0, True)
End Function
