VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsMonitors"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
'
' マルチモニタの情報を取得するクラス
'
' 参考: http://www.vbaccelerator.com/home/vb/Tips/Working_with_Multiple_Monitors/article.asp
'

Option Explicit

Private Const SM_CXVIRTUALSCREEN = 78
Private Const SM_CYVIRTUALSCREEN = 79
Private Const SM_CMONITORS = 80
Private Const SM_SAMEDISPLAYFORMAT = 81

Private Declare Function GetSystemMetrics Lib "user32" ( _
   ByVal nIndex As Long) As Long

Private m_monitors() As clsMonitor

Public Property Get Monitor(Index) As clsMonitor
    Set Monitor = m_monitors(Index)
End Property

Public Property Get VirtualScreenLeft() As Long
    VirtualScreenLeft = 0
    
    Dim i As Long
    For i = 0 To UBound(m_monitors)
        If m_monitors(i).MonitorLeft < VirtualScreenLeft Then
            VirtualScreenLeft = m_monitors(i).MonitorLeft
        End If
    Next
End Property

Public Property Get VirtualScreenTop() As Long
    VirtualScreenTop = 0
    
    Dim i As Long
    For i = 0 To UBound(m_monitors)
        If m_monitors(i).MonitorTop < VirtualScreenTop Then
            VirtualScreenTop = m_monitors(i).MonitorTop
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

Public Function MonitorEnumProc( _
      ByVal hMonitor As Long, _
      ByVal hDCMonitor As Long, _
      ByVal lprcMonitor As Long, _
      ByVal dwData As Long _
   ) As Long
   
    Dim ub As Long
    
    On Error Resume Next
    ub = UBound(m_monitors)
    If Err = 0 Then
        ub = ub + 1
    Else
        ub = 0
    End If
    ReDim Preserve m_monitors(ub)
    Set m_monitors(ub) = New clsMonitor
   
   m_monitors(ub).GetMonitorInfo hMonitor
   
   MonitorEnumProc = 1
   
End Function

Private Sub Class_Initialize()

    EnumMonitors Me

End Sub
