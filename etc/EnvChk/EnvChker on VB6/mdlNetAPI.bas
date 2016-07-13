Attribute VB_Name = "mdlNetAPI"
Option Explicit

Public Declare Function NetShareGetInfo Lib "netapi32.dll" ( _
    lpServer As Any, _
    NetName As Byte, _
    ByVal Level As Long, _
    lpBuffer As Long _
) As Long

Public Declare Function NetUserGetInfo Lib "netapi32.dll" ( _
    lpServer As Any, _
    UserName As Byte, _
    ByVal Level As Long, _
    lpBuffer As Long _
) As Long

Public Declare Function NetApiBufferFree Lib "netapi32.dll" (ByVal lpBuffer As Long) As Long

Public Declare Sub CopyMemory Lib "kernel32" Alias "RtlMoveMemory" ( _
    Destination As Any, _
    Source As Any, _
    ByVal Length As Long)

Public Declare Function lstrcpy Lib "kernel32" Alias "lstrcpyA" ( _
    ByVal lpString1 As String, _
    ByVal lpString2 As String _
) As Long

Public Declare Function lstrcpyW Lib "kernel32" ( _
    lpString1 As Byte, _
    ByVal lpString2 As Long _
) As Long

Public Declare Function lstrlen Lib "kernel32" Alias "lstrlenA" ( _
    ByVal lpString As String _
) As Long

Public Declare Function lstrlenW Lib "kernel32" ( _
    ByVal lpString As Long _
) As Long

Public Type SHARE_INFO_2
    ' Level 0 starts here
    NetName As Long
    ' Level 1 starts here
    Type As Long
    Remark As Long
    ' Level 2 starts here
    Permissions As Long
    MaxUses As Long
    CurrentUses As Long
    Path As Long
    Passwd As Long
End Type

'VB用にLPWSTRなLongをStringに変換済みの型
'
Public Type SHARE_INFO_2_VB
    ' Level 0 starts here
    NetName As String
    ' Level 1 starts here
    Type As Long
    Remark As String
    ' Level 2 starts here
    Permissions As Long
    MaxUses As Long
    CurrentUses As Long
    Path As String
    Passwd As String
End Type

Public Type USER_INFO_3
    ' Level 0 starts here
    Name As Long
    ' Level 1 starts here
    Password As Long
    PasswordAge As Long
    Privilege As Long
    HomeDir As Long
    Comment As Long
    Flags As Long
    ScriptPath As Long
    ' Level 2 starts here
    AuthFlags As Long
    FullName As Long
    UserComment As Long
    Parms As Long
    Workstations As Long
    LastLogon As Long
    LastLogoff As Long
    AcctExpires As Long
    MaxStorage As Long
    UnitsPerWeek As Long
    LogonHours As Long
    BadPwCount As Long
    NumLogons As Long
    LogonServer As Long
    CountryCode As Long
    CodePage As Long
    ' Level 3 starts here
    UserID As Long
    PrimaryGroupID As Long
    Profile As Long
    HomeDirDrive As Long
    PasswordExpired As Long
End Type

'VB用にLPWSTRなLongをStringに変換済みの型
'
Public Type USER_INFO_3_VB
    ' Level 0 starts here
    Name As String
    ' Level 1 starts here
    Password As String
    PasswordAge As Long
    Privilege As Long
    HomeDir As String
    Comment As String
    Flags As Long
    ScriptPath As String
    ' Level 2 starts here
    AuthFlags As Long
    FullName As String
    UserComment As String
    Parms As String
    Workstations As String
    LastLogon As Long
    LastLogoff As Long
    AcctExpires As Long
    MaxStorage As Long
    UnitsPerWeek As Long
    LogonHours(21 - 1) As Byte
    BadPwCount As Long
    NumLogons As Long
    LogonServer As String
    CountryCode As Long
    CodePage As Long
    ' Level 3 starts here
    UserID As Long
    PrimaryGroupID As Long
    Profile As String
    HomeDirDrive As String
    PasswordExpired As Long
End Type

Public Const ERROR_ACCESS_DENIED = 5&

Public Const NERR_BASE = 2100
Public Const NERR_Success As Long = 0&
Public Const NERR_UserNotFound As Long = (NERR_BASE + 121)
Public Const NERR_InvalidComputer = (NERR_BASE + 251)

Public Const TIMEQ_FOREVER = -1&             '((unsigned long) -1L)

Public Const UF_DONT_EXPIRE_PASSWD = &H10000

'WSTRへのポインタが指している文字列をStringにして返す
'
Public Function LPWSTRtoString(ByVal lpWStr As Long) As String
    Dim bWork() As Byte
    
    If lpWStr = 0 Then Exit Function
    
    ReDim bWork((lstrlenW(lpWStr) + 1) * 2 - 1)
    lstrcpyW bWork(0), lpWStr
    
    LPWSTRtoString = Left(CStr(bWork), InStr(CStr(bWork), vbNullChar) - 1)
End Function

'共有フォルダ情報を取得する
'
Public Function VB_NetShareGetInfo(NetName As String, si2 As SHARE_INFO_2_VB) As Long
    Dim ret As Long
    Dim lpBuffer As Long
    Dim si2work As SHARE_INFO_2
    Dim bNetName() As Byte
    
    bNetName = NetName & vbNullChar
    ret = NetShareGetInfo(ByVal 0, bNetName(0), 2, lpBuffer)
    If ret = NERR_Success Then
        CopyMemory si2work, ByVal lpBuffer, Len(si2work)
        
        si2.NetName = LPWSTRtoString(si2work.NetName)
        si2.Type = si2work.Type
        si2.Remark = LPWSTRtoString(si2work.Remark)
        si2.Permissions = si2work.Permissions
        si2.MaxUses = si2work.MaxUses
        si2.CurrentUses = si2work.CurrentUses
        si2.Path = LPWSTRtoString(si2work.Path)
        si2.Passwd = LPWSTRtoString(si2work.Passwd)
        
        NetApiBufferFree lpBuffer
    End If
    
    VB_NetShareGetInfo = ret
End Function

'ユーザー情報を取得する
'
Public Function VB_NetUserGetInfo(UserName As String, ui3 As USER_INFO_3_VB) As Long
    Dim ret As Long
    Dim lpBuffer As Long
    Dim ui3work As USER_INFO_3
    Dim bUserName() As Byte
    
    bUserName = UserName & vbNullChar
    ret = NetUserGetInfo(ByVal 0, bUserName(0), 3, lpBuffer)
    If ret = NERR_Success Then
        Call CopyMemory(ui3work, ByVal lpBuffer, Len(ui3work))
        
        ui3.Name = LPWSTRtoString(ui3work.Name)
        ui3.PasswordAge = ui3work.PasswordAge
        ui3.Privilege = ui3work.Privilege
        ui3.Password = LPWSTRtoString(ui3work.Password)
        ui3.HomeDir = LPWSTRtoString(ui3work.HomeDir)
        ui3.Comment = LPWSTRtoString(ui3work.Comment)
        ui3.Flags = ui3work.Flags
        ui3.ScriptPath = LPWSTRtoString(ui3work.ScriptPath)
        ui3.AuthFlags = ui3work.AuthFlags
        ui3.FullName = LPWSTRtoString(ui3work.FullName)
        ui3.UserComment = LPWSTRtoString(ui3work.UserComment)
        ui3.Parms = LPWSTRtoString(ui3work.Parms)
        ui3.Workstations = LPWSTRtoString(ui3work.Workstations)
        ui3.LastLogon = ui3work.LastLogon
        ui3.LastLogoff = ui3work.LastLogoff
        ui3.AcctExpires = ui3work.AcctExpires
        ui3.MaxStorage = ui3work.MaxStorage
        ui3.UnitsPerWeek = ui3work.UnitsPerWeek
        CopyMemory ui3.LogonHours(0), ByVal ui3work.LogonHours, 21
        ui3.BadPwCount = ui3work.BadPwCount
        ui3.NumLogons = ui3work.NumLogons
        ui3.LogonServer = LPWSTRtoString(ui3work.LogonServer)
        ui3.CountryCode = ui3work.CountryCode
        ui3.CodePage = ui3work.CodePage
        ui3.UserID = ui3work.UserID
        ui3.PrimaryGroupID = ui3work.PrimaryGroupID
        ui3.Profile = LPWSTRtoString(ui3work.Profile)
        ui3.HomeDirDrive = LPWSTRtoString(ui3work.HomeDirDrive)
        ui3.PasswordExpired = ui3work.PasswordExpired
        
        Call NetApiBufferFree(lpBuffer)
    End If
    
    VB_NetUserGetInfo = ret
End Function
