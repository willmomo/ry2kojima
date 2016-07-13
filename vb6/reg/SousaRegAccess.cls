VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "SousaRegAccess"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

Private Const HKEY_CURRENT_USER = &H80000001
Private Const HKEY_LOCAL_MACHINE = &H80000002

Private Const STANDARD_RIGHTS_ALL = &H1F0000
Private Const SYNCHRONIZE = &H100000
Private Const KEY_QUERY_VALUE = &H1
Private Const KEY_SET_VALUE = &H2
Private Const KEY_CREATE_SUB_KEY = &H4
Private Const KEY_ENUMERATE_SUB_KEYS = &H8
Private Const KEY_NOTIFY = &H10
Private Const KEY_CREATE_LINK = &H20

Private Const KEY_ALL_ACCESS = ((STANDARD_RIGHTS_ALL Or KEY_QUERY_VALUE Or KEY_SET_VALUE Or KEY_CREATE_SUB_KEY Or KEY_ENUMERATE_SUB_KEYS Or KEY_NOTIFY Or KEY_CREATE_LINK) And (Not SYNCHRONIZE))

Private Const REG_SZ = 1                         ' Unicode nul terminated string
Private Const REG_DWORD = 4                      ' 32-bit number

Private Type SECURITY_ATTRIBUTES
    nLength As Long
    lpSecurityDescriptor As Long
    bInheritHandle As Long
End Type

Private Declare Function RegOpenKeyEx Lib "advapi32.dll" Alias "RegOpenKeyExA" ( _
    ByVal hKey As Long, _
    ByVal lpSubKey As String, _
    ByVal ulOptions As Long, _
    ByVal samDesired As Long, _
    phkResult As Long) As Long

Private Declare Function RegCloseKey Lib "advapi32.dll" (ByVal hKey As Long) As Long

Private Declare Function RegQueryValueEx Lib "advapi32.dll" Alias "RegQueryValueExA" ( _
    ByVal hKey As Long, _
    ByVal lpValueName As String, _
    ByVal lpReserved As Long, _
    lpType As Long, _
    lpData As Any, _
    lpcbData As Long) As Long         ' Note that if you declare the lpData parameter as String, you must pass it By Value.

Private Declare Function RegSetValueEx Lib "advapi32.dll" Alias "RegSetValueExA" ( _
    ByVal hKey As Long, _
    ByVal lpValueName As String, _
    ByVal Reserved As Long, _
    ByVal dwType As Long, _
    lpData As Any, _
    ByVal cbData As Long) As Long         ' Note that if you declare the lpData parameter as String, you must pass it By Value.

Private Declare Function RegQueryValueExString Lib "advapi32.dll" Alias "RegQueryValueExA" ( _
    ByVal hKey As Long, _
    ByVal lpValueName As String, _
    ByVal lpReserved As Long, _
    lpType As Long, _
    ByVal lpData As String, _
    lpcbData As Long) As Long         ' Note that if you declare the lpData parameter as String, you must pass it By Value.

Private Declare Function RegSetValueExString Lib "advapi32.dll" Alias "RegSetValueExA" ( _
    ByVal hKey As Long, _
    ByVal lpValueName As String, _
    ByVal Reserved As Long, _
    ByVal dwType As Long, _
    ByVal lpData As String, _
    ByVal cbData As Long) As Long         ' Note that if you declare the lpData parameter as String, you must pass it By Value.

Private Declare Function GetLastError Lib "kernel32" () As Long

Public Property Get NoDesktop() As Long

    Dim hKey As Long
    Dim ret As Long
    Dim data As Long
    Dim cbData As Long
    Dim winErr As Long
    
    ret = RegOpenKeyEx( _
        HKEY_CURRENT_USER, _
        "Software\Microsoft\Windows\CurrentVersion\Policies\Explorer", _
        0, _
        KEY_ALL_ACCESS, _
        hKey)

    If ret = 0 Then
        cbData = Len(data)
        
        ret = RegQueryValueEx( _
            hKey, _
            "NoDesktop", _
            0, _
            ByVal 0, _
            data, _
            cbData)
        winErr = Err.LastDllError
        
        If ret <> 0 Then
            MsgBox "RegQueryValueEx error: " & winErr
        End If
        
        RegCloseKey hKey
    Else
        MsgBox "RegOpenKeyEx error: " & Err.LastDllError
    End If

    NoDesktop = data
End Property

Public Property Let NoDesktop(ByVal newData As Long)
    Dim hKey As Long
    Dim ret As Long
    Dim cbData As Long
    Dim winErr As Long
    
    ret = RegOpenKeyEx( _
        HKEY_CURRENT_USER, _
        "Software\Microsoft\Windows\CurrentVersion\Policies\Explorer", _
        0, _
        KEY_ALL_ACCESS, _
        hKey)

    If ret = 0 Then
        cbData = Len(newData)
        
        ret = RegSetValueEx( _
            hKey, _
            "NoDesktop", _
            0, _
            REG_DWORD, _
            newData, _
            cbData)
        winErr = Err.LastDllError
        
        If ret <> 0 Then
            MsgBox "RegSetValueEx error: " & winErr
        End If
        
        RegCloseKey hKey
    Else
        MsgBox "RegOpenKeyEx error: " & Err.LastDllError
    End If
End Property

Public Property Get NoSaveSettings() As Long

    Dim hKey As Long
    Dim ret As Long
    Dim lData As Long
    Dim cbData As Long
    Dim winErr As Long
    Dim lType As Long
    
    ret = RegOpenKeyEx( _
        HKEY_CURRENT_USER, _
        "Software\Microsoft\Windows\CurrentVersion\Policies\Explorer", _
        0, _
        KEY_ALL_ACCESS, _
        hKey)

    If ret = 0 Then
        cbData = Len(lData)
        
        ret = RegQueryValueEx( _
            hKey, _
            "NoSaveSettings", _
            0, _
            ByVal 0, _
            lData, _
            cbData)
        winErr = Err.LastDllError
        
        If ret <> 0 Then
            MsgBox "RegQueryValueEx error: " & winErr
        End If
        
        RegCloseKey hKey
    Else
        MsgBox "RegOpenKeyEx error: " & Err.LastDllError
    End If

    NoSaveSettings = lData
End Property

Public Property Let NoSaveSettings(ByVal newData As Long)
    Dim hKey As Long
    Dim ret As Long
    Dim cbData As Long
    Dim winErr As Long
    
    ret = RegOpenKeyEx( _
        HKEY_CURRENT_USER, _
        "Software\Microsoft\Windows\CurrentVersion\Policies\Explorer", _
        0, _
        KEY_ALL_ACCESS, _
        hKey)

    If ret = 0 Then
        cbData = Len(newData)
        
        ret = RegSetValueEx( _
            hKey, _
            "NoSaveSettings", _
            0, _
            REG_DWORD, _
            newData, _
            cbData)
        winErr = Err.LastDllError
        
        If ret <> 0 Then
            MsgBox "RegSetValueEx error: " & winErr
        End If
        
        RegCloseKey hKey
    Else
        MsgBox "RegOpenKeyEx error: " & Err.LastDllError
    End If
End Property

Public Property Get PersistBrowsers() As Long

    Dim hKey As Long
    Dim ret As Long
    Dim lData As Long
    Dim cbData As Long
    Dim winErr As Long
    Dim lType As Long
    
    ret = RegOpenKeyEx( _
        HKEY_CURRENT_USER, _
        "Software\Microsoft\Windows\CurrentVersion\Explorer\Advanced", _
        0, _
        KEY_ALL_ACCESS, _
        hKey)

    If ret = 0 Then
        cbData = Len(lData)
        
        ret = RegQueryValueEx( _
            hKey, _
            "PersistBrowsers", _
            0, _
            ByVal 0, _
            lData, _
            cbData)
        winErr = Err.LastDllError
        
        If ret <> 0 Then
            MsgBox "RegQueryValueEx error: " & winErr
        End If
        
        RegCloseKey hKey
    Else
        MsgBox "RegOpenKeyEx error: " & Err.LastDllError
    End If

    PersistBrowsers = lData
End Property

Public Property Let PersistBrowsers(ByVal newData As Long)
    Dim hKey As Long
    Dim ret As Long
    Dim cbData As Long
    Dim winErr As Long
    
    ret = RegOpenKeyEx( _
        HKEY_CURRENT_USER, _
        "Software\Microsoft\Windows\CurrentVersion\Explorer\Advanced", _
        0, _
        KEY_ALL_ACCESS, _
        hKey)

    If ret = 0 Then
        cbData = Len(newData)
        
        ret = RegSetValueEx( _
            hKey, _
            "PersistBrowsers", _
            0, _
            REG_DWORD, _
            newData, _
            cbData)
        winErr = Err.LastDllError
        
        If ret <> 0 Then
            MsgBox "RegSetValueEx error: " & winErr
        End If
        
        RegCloseKey hKey
    Else
        MsgBox "RegOpenKeyEx error: " & Err.LastDllError
    End If
End Property

Public Property Get AutoEndTasks() As String

    Dim hKey As Long
    Dim ret As Long
    Dim sData As String
    Dim cbData As Long
    Dim winErr As Long
    Dim lType As Long
    
    ret = RegOpenKeyEx( _
        HKEY_CURRENT_USER, _
        "Control Panel\Desktop", _
        0, _
        KEY_ALL_ACCESS, _
        hKey)

    If ret = 0 Then
        sData = Space(256)
        cbData = Len(sData)
        
        ret = RegQueryValueExString( _
            hKey, _
            "AutoEndTasks", _
            0, _
            ByVal 0, _
            sData, _
            cbData)
        winErr = Err.LastDllError
        
        If ret <> 0 Then
            MsgBox "RegQueryValueEx error: " & winErr
        End If
        
        sData = Left(sData, cbData - 1)
        RegCloseKey hKey
    Else
        MsgBox "RegOpenKeyEx error: " & Err.LastDllError
    End If

    AutoEndTasks = sData
End Property

Public Property Let AutoEndTasks(ByVal newData As String)
    Dim hKey As Long
    Dim ret As Long
    Dim cbData As Long
    Dim winErr As Long
    
    ret = RegOpenKeyEx( _
        HKEY_CURRENT_USER, _
        "Control Panel\Desktop", _
        0, _
        KEY_ALL_ACCESS, _
        hKey)

    If ret = 0 Then
        cbData = Len(newData)
        
        ret = RegSetValueExString( _
            hKey, _
            "AutoEndTasks", _
            0, _
            REG_SZ, _
            newData, _
            cbData)
        winErr = Err.LastDllError
        
        If ret <> 0 Then
            MsgBox "RegSetValueEx error: " & winErr
        End If
        
        RegCloseKey hKey
    Else
        MsgBox "RegOpenKeyEx error: " & Err.LastDllError
    End If
End Property

Public Property Get LocalNTP() As Long

    Dim hKey As Long
    Dim ret As Long
    Dim lData As Long
    Dim cbData As Long
    Dim winErr As Long
    Dim lType As Long
    
    ret = RegOpenKeyEx( _
        HKEY_LOCAL_MACHINE, _
        "SYSTEM\CurrentControlSet\Services\W32Time\Parameters", _
        0, _
        KEY_ALL_ACCESS, _
        hKey)

    If ret = 0 Then
        cbData = Len(lData)
        
        ret = RegQueryValueEx( _
            hKey, _
            "LocalNTP", _
            0, _
            ByVal 0, _
            lData, _
            cbData)
        
        If ret <> 0 And Err.LastDllError = 0 Then
            LocalNTP = 0
        End If
        
        RegCloseKey hKey
    Else
        MsgBox "RegOpenKeyEx error: " & Err.LastDllError
    End If

    LocalNTP = lData
End Property

Public Property Let LocalNTP(ByVal newData As Long)
    Dim hKey As Long
    Dim ret As Long
    Dim cbData As Long
    Dim winErr As Long
    
    ret = RegOpenKeyEx( _
        HKEY_LOCAL_MACHINE, _
        "SYSTEM\CurrentControlSet\Services\W32Time\Parameters", _
        0, _
        KEY_ALL_ACCESS, _
        hKey)

    If ret = 0 Then
        cbData = Len(newData)
        
        ret = RegSetValueEx( _
            hKey, _
            "LocalNTP", _
            0, _
            REG_DWORD, _
            newData, _
            cbData)
        winErr = Err.LastDllError
        
        If ret <> 0 Then
            MsgBox "RegSetValueEx error: " & winErr
        End If
        
        RegCloseKey hKey
    Else
        MsgBox "RegOpenKeyEx error: " & Err.LastDllError
    End If
End Property

Public Property Get W32TimeStart() As Long

    Dim hKey As Long
    Dim ret As Long
    Dim lData As Long
    Dim cbData As Long
    Dim winErr As Long
    Dim lType As Long
    
    ret = RegOpenKeyEx( _
        HKEY_LOCAL_MACHINE, _
        "SYSTEM\CurrentControlSet\Services\W32Time", _
        0, _
        KEY_ALL_ACCESS, _
        hKey)

    If ret = 0 Then
        cbData = Len(lData)
        
        ret = RegQueryValueEx( _
            hKey, _
            "Start", _
            0, _
            ByVal 0, _
            lData, _
            cbData)
        
        If ret <> 0 And Err.LastDllError = 0 Then
            W32TimeStart = 3
        End If
        
        RegCloseKey hKey
    Else
        MsgBox "RegOpenKeyEx error: " & Err.LastDllError
    End If

    W32TimeStart = lData
End Property

Public Property Let W32TimeStart(ByVal newData As Long)
    Dim hKey As Long
    Dim ret As Long
    Dim cbData As Long
    Dim winErr As Long
    
    ret = RegOpenKeyEx( _
        HKEY_LOCAL_MACHINE, _
        "SYSTEM\CurrentControlSet\Services\W32Time", _
        0, _
        KEY_ALL_ACCESS, _
        hKey)

    If ret = 0 Then
        cbData = Len(newData)
        
        ret = RegSetValueEx( _
            hKey, _
            "Start", _
            0, _
            REG_DWORD, _
            newData, _
            cbData)
        winErr = Err.LastDllError
        
        If ret <> 0 Then
            MsgBox "RegSetValueEx error: " & winErr
        End If
        
        RegCloseKey hKey
    Else
        MsgBox "RegOpenKeyEx error: " & Err.LastDllError
    End If
End Property

