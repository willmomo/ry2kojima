'----------------------------------------------------------------------
' append_PATH.vbs
'
' ���[�U�[�� PATH �ɒǉ�����
'----------------------------------------------------------------------

Set shell = WScript.CreateObject("WScript.Shell")
Set env = shell.Environment("USER")
WScript.Echo env.Item("path")


'' env.Item("path") = env.Item("path") & ";C:\newPath"
