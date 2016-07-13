Option Explicit

Dim intType
Dim objSystemSet
Dim objSystem
Dim objArgs

intType = 8

On Error Resume Next
Set objArgs = WScript.Arguments
If CInt(objArgs.Count) <> 0 Then
  intType = CInt(objArgs.Item(0))
End If

Set objSystemSet = GetObject("winmgmts:{impersonationLevel=impersonate,(Shutdown)}").InstancesOf("Win32_OperatingSystem")
For Each objSystem In objSystemSet
  objSystem.Win32Shutdown intType
Next
On Error Goto 0
