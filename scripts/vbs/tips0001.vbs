Set d=CreateObject("htmlfile")
d.parentWindow.setTimeout GetRef("proc"),100
MsgBox "���Ԃ̊|���鏈�����ł��B�B�B�Ƃ������b�Z�[�W���o�����ςȂ��ɂ���"

Sub proc
Set wShell=CreateObject("WScript.Shell")
wShell.PopUp "���Ԃ̊|���鏈���̂���B�B�B",5
WScript.Timeout=1
WScript.Sleep 2000
End Sub
