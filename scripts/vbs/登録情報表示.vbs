'----------------------------------------------------------------------
' �o�^���\��.vbs
'
' �o�^����\������X�N���v�g
'----------------------------------------------------------------------

Set objShell = WScript.CreateObject("WScript.Shell")

strOwner = objShell.RegRead("HKLM\Software\Microsoft\Windows NT\CurrentVersion\RegisteredOwner")
strOrganization = objShell.RegRead("HKLM\Software\Microsoft\Windows NT\CurrentVersion\RegisteredOrganization")

WScript.Echo "���[�U�[��: " & strOwner & vbNewLine & "��Ж�: " & strOrganization

