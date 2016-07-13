Attribute VB_Name = "mdlHaraiDasiDll"
Option Explicit

Public Declare Function scriptOpen Lib "HaraiDasiDll.dll" (ByVal fname As String) As Long
Public Declare Function scriptClose Lib "HaraiDasiDll.dll" () As Long
Public Declare Function scriptGetLine Lib "HaraiDasiDll.dll" (ByRef cmd As Byte, ByRef param As Byte, ByRef comData As Byte) As Long
Public Declare Function scriptCompare Lib "HaraiDasiDll.dll" (ByRef cmd As Byte, ByRef param As Byte, ByRef comData As Byte) As Long

