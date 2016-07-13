VERSION 5.00
Begin VB.Form frmWindow 
   Caption         =   "Form1"
   ClientHeight    =   3090
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   4680
   LinkTopic       =   "Form1"
   ScaleHeight     =   3090
   ScaleWidth      =   4680
   StartUpPosition =   3  'Windows の既定値
End
Attribute VB_Name = "frmWindow"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private m_MonitorNo As Long

Public Property Let MonitorNo(ByVal newValue As Long)
    m_MonitorNo = newValue
End Property

Private Sub Form_Load()

    '
    ' 指定されたモニタのワークエリアいっぱいにウィンドウを表示
    '
    Me.Move g_mi(m_MonitorNo).rcWork.Left * Screen.TwipsPerPixelX, _
            g_mi(m_MonitorNo).rcWork.Top * Screen.TwipsPerPixelY, _
            GetWidth(g_mi(m_MonitorNo).rcWork) * Screen.TwipsPerPixelX, _
            GetHeight(g_mi(m_MonitorNo).rcWork) * Screen.TwipsPerPixelY
            
End Sub

Private Sub Form_Paint()
    Me.Cls
    Me.Print "No: " & m_MonitorNo
    Me.Print "Flag: " & g_mi(m_MonitorNo).dwFlags
End Sub
