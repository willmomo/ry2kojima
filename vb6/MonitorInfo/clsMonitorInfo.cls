VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsMonitorInfo"
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

