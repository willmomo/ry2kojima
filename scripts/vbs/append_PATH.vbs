'----------------------------------------------------------------------
' append_PATH.vbs
'
' ÉÜÅ[ÉUÅ[ÇÃ PATH Ç…í«â¡Ç∑ÇÈ
'----------------------------------------------------------------------

Set shell = WScript.CreateObject("WScript.Shell")
Set env = shell.Environment("USER")
WScript.Echo env.Item("path")


'' env.Item("path") = env.Item("path") & ";C:\newPath"
