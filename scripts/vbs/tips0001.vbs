Set d=CreateObject("htmlfile")
d.parentWindow.setTimeout GetRef("proc"),100
MsgBox "時間の掛かる処理中です。。。というメッセージを出しっぱなしにして"

Sub proc
Set wShell=CreateObject("WScript.Shell")
wShell.PopUp "時間の掛かる処理のつもり。。。",5
WScript.Timeout=1
WScript.Sleep 2000
End Sub
