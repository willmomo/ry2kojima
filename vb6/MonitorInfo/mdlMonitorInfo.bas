Attribute VB_Name = "mdlMonitorInfo"
'
' clsVirtualMonitorクラスで使用するモジュール
'

Option Explicit

Private Declare Function EnumDisplayMonitors Lib "user32" (ByVal hDC As Long, lprcClip As Any, ByVal lpfnEnum As Long, ByVal dwData As Long) As Long

Private activeObject As clsVirtualMonitor

Private Function MonitorEnumProc(ByVal hMonitor As Long, ByVal hDCMonitor As Long, ByVal lprcMonitor As Long, ByVal dwData As Long) As Long
   
    MonitorEnumProc = activeObject.MonitorEnumProc(hMonitor, hDCMonitor, lprcMonitor, dwData)
   
End Function

Public Sub EnumMonitors(cM As clsVirtualMonitor)

    Set activeObject = cM
    
    EnumDisplayMonitors 0, ByVal 0&, AddressOf MonitorEnumProc, 0
    
    Set activeObject = Nothing
    
End Sub



