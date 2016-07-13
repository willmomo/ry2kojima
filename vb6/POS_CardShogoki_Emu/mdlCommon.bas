Attribute VB_Name = "mdlCommon"
Option Explicit

'ＩＮＩファイルアクセスのための宣言
Private Declare Function GetPrivateProfileString Lib "kernel32" Alias "GetPrivateProfileStringA" _
        (ByVal lpApplicationName As String, ByVal lpKeyName As Any, ByVal lpDefault As String, ByVal lpReturnedString As String, ByVal nSize As Long, ByVal lpFileName As String) As Long
Private Declare Function WritePrivateProfileString Lib "kernel32" Alias "WritePrivateProfileStringA" _
        (ByVal lpApplicationName As String, ByVal lpKeyName As Any, ByVal lpString As Any, ByVal lpFileName As String) As Long

Public Const SCR0_VERSION As Byte = 0
Public Const SCR1_JYUNBI As Byte = 1
Public Const SCR2_CARD_IN As Byte = 2
Public Const SCR3_CARD_NINSYO As Byte = 3
Public Const SCR4_CARD_NG As Byte = 4
Public Const SCR5_ANSYO_NYURYOKU As Byte = 5
Public Const SCR6_ANSYO_TOROKU As Byte = 6
Public Const SCR7_TOROKU_NINSYO As Byte = 7
Public Const SCR8_TOROKU_NG As Byte = 8
Public Const SCR9_ANSYO_END As Byte = 9
Public Const SCR10_CARD_OUT As Byte = 10

Public Const BUTTON_TEISEI As Byte = 10
Public Const BUTTON_KAKUTEI As Byte = 11

Public Function GetINIValue(KEY As String, Section As String, ININame As String) As String
    Dim Value As String * 255
    Call GetPrivateProfileString(Section, KEY, "", Value, Len(Value), ININame)
    GetINIValue = Left$(Value, InStr(1, Value, vbNullChar) - 1)
End Function

Public Function SetINIValue(Value As String, KEY As String, Section As String, ININame As String) As Boolean
    Dim Ret As Long

    Ret = WritePrivateProfileString(Section, KEY, Value, ININame)
    SetINIValue = CBool(Ret)
End Function

