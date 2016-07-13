VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsVirtualMonitor"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
'
' 仮想モニタ(マルチモニタ)の情報を取得するクラス
'
' 参考: http://www.vbaccelerator.com/home/vb/Tips/Working_with_Multiple_Monitors/article.asp
'

Option Explicit

Private Const SM_CXVIRTUALSCREEN = 78
Private Const SM_CYVIRTUALSCREEN = 79
Private Const SM_CMONITORS = 80
Private Const SM_SAMEDISPLAYFORMAT = 81

Private Declare Function GetSystemMetrics Lib "user32" (ByVal nIndex As Long) As Long

' 実モニタのコレクション
Private m_monitors As Collection

Public Property Get Monitors() As Collection
    Set Monitors = m_monitors
End Property

Public Property Get VirtualScreenLeft() As Long
    VirtualScreenLeft = 0
    
    Dim mon As clsMonitor
    For Each mon In m_monitors
        If mon.MonitorLeft < VirtualScreenLeft Then
            VirtualScreenLeft = mon.MonitorLeft
        End If
    Next
End Property

Public Property Get VirtualScreenTop() As Long
    VirtualScreenTop = 0
    
    Dim mon As clsMonitor
    For Each mon In m_monitors
        If mon.MonitorTop < VirtualScreenTop Then
            VirtualScreenTop = mon.MonitorTop
        End If
    Next
End Property

Public Property Get VirtualScreenWidth() As Long
   VirtualScreenWidth = GetSystemMetrics(SM_CXVIRTUALSCREEN)
End Property

Public Property Get VirtualScreenHeight() As Long
   VirtualScreenHeight = GetSystemMetrics(SM_CYVIRTUALSCREEN)
End Property

Public Property Get DisplayMonitorCount() As Long
   DisplayMonitorCount = GetSystemMetrics(SM_CMONITORS)
End Property

Public Property Get AllMonitorsSame() As Long
   AllMonitorsSame = GetSystemMetrics(SM_SAMEDISPLAYFORMAT)
End Property

Public Function MonitorEnumProc(ByVal hMonitor As Long, ByVal hDCMonitor As Long, ByVal lprcMonitor As Long, ByVal dwData As Long) As Long
   
    Dim mon As clsMonitor
    Set mon = New clsMonitor
    
    mon.GetMonitorInfo hMonitor
    m_monitors.Add mon, mon.MonitorName
   
    MonitorEnumProc = 1
   
End Function

Private Sub Class_Initialize()

    Set m_monitors = New Collection
    
    EnumMonitors Me

End Sub
