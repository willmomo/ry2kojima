VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsMonitor"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
'
' モニタの情報を取得するクラス
'
' 参考: http://www.vbaccelerator.com/home/vb/Tips/Working_with_Multiple_Monitors/article.asp
'

Option Explicit

Private Type RECT
    Left As Long
    Top As Long
    Right As Long
    Bottom As Long
End Type

Private Const CCHDEVICENAME = 32

Private Type MONITORINFOEXA
    cbSize As Long
    rcMonitor As RECT
    rcWork As RECT
    dwFlags As Long
    b(0 To CCHDEVICENAME - 1) As Byte
End Type

Private Type MONITORINFOEXW
    cbSize As Long
    rcMonitor As RECT
    rcWork As RECT
    dwFlags As Long
    b(0 To CCHDEVICENAME * 2 - 1) As Byte
End Type

Private Declare Function GetMonitorInfoA Lib "user32" (ByVal hMonitor As Long, lpmi As MONITORINFOEXA) As Long
Private Declare Function GetMonitorInfoW Lib "user32" (ByVal hMonitor As Long, lpmi As MONITORINFOEXW) As Long

Private Const MONITORINFOF_PRIMARY = &H1

Private m_hMonitor As Long
Private m_sName As String
Private m_rcMonitor As RECT
Private m_rcWork As RECT
Private m_bIsPrimary As Boolean

Public Property Get IsPrimary()
    IsPrimary = m_bIsPrimary
End Property

Public Property Get MonitorName()
    MonitorName = m_sName
End Property

Public Property Get MonitorLeft()
    MonitorLeft = m_rcMonitor.Left
End Property

Public Property Get MonitorTop()
    MonitorTop = m_rcMonitor.Top
End Property

Public Property Get MonitorRight()
    MonitorRight = m_rcMonitor.Right
End Property

Public Property Get MonitorBottom()
    MonitorBottom = m_rcMonitor.Bottom
End Property

Public Property Get WorkLeft()
    WorkLeft = m_rcWork.Left
End Property

Public Property Get WorkTop()
    WorkTop = m_rcWork.Top
End Property

Public Property Get WorkRight()
    WorkRight = m_rcWork.Right
End Property

Public Property Get WorkBottom()
    WorkBottom = m_rcWork.Bottom
End Property

Public Sub GetMonitorInfo(hMonitor As Long)

    Dim iPos As Long
    Dim sName As String
    sName = String(CCHDEVICENAME * 2, vbNullChar)
    
    If (True) Then
        Dim tMIW As MONITORINFOEXW
        tMIW.cbSize = Len(tMIW)
        GetMonitorInfoW hMonitor, tMIW
        With tMIW
            LSet m_rcMonitor = .rcMonitor
            LSet m_rcWork = .rcWork
            m_bIsPrimary = ((.dwFlags And MONITORINFOF_PRIMARY) = MONITORINFOF_PRIMARY)
            sName = .b
            iPos = InStr(sName, vbNullChar)
        End With
    Else
        Dim tMIA As MONITORINFOEXA
        tMIA.cbSize = Len(tMIA)
        GetMonitorInfoA hMonitor, tMIA
        With tMIA
            LSet m_rcMonitor = .rcMonitor
            LSet m_rcWork = .rcWork
            m_bIsPrimary = ((.dwFlags And MONITORINFOF_PRIMARY) = MONITORINFOF_PRIMARY)
            sName = StrConv(.b, vbUnicode)
        End With
    End If
    
    iPos = InStr(sName, vbNullChar)
    If (iPos > 0) Then
        m_sName = Left(sName, iPos - 1)
    Else
        m_sName = sName
    End If

End Sub

