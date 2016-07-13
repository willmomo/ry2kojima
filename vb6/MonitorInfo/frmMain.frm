VERSION 5.00
Begin VB.Form frmMain 
   Caption         =   "仮想モニター情報"
   ClientHeight    =   4575
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   7650
   LinkTopic       =   "Form1"
   ScaleHeight     =   4575
   ScaleWidth      =   7650
   StartUpPosition =   3  'Windows の既定値
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private m_virtualMon As New clsVirtualMonitor

Private Sub Form_Paint()

    Me.Cls
    
    '画像の表示比率を求める
    Dim scl1 As Double
    Dim scl2 As Double
    scl1 = (Me.ScaleWidth - Screen.TwipsPerPixelX) / m_virtualMon.VirtualScreenWidth
    scl2 = (Me.ScaleHeight - Screen.TwipsPerPixelY) / m_virtualMon.VirtualScreenHeight
    
    Dim dblScale As Double
    dblScale = IIf(scl1 < scl2, scl1, scl2)
    
    Me.Line (0, 0)-(m_virtualMon.VirtualScreenWidth * dblScale, m_virtualMon.VirtualScreenHeight * dblScale), RGB(64, 64, 64), BF
    Me.Line (0, 0)-(m_virtualMon.VirtualScreenWidth * dblScale, m_virtualMon.VirtualScreenHeight * dblScale), RGB(1, 1, 1), B
    
    Dim i As Long
    Dim ofsX As Long
    Dim ofsY As Long
    
    ofsX = Abs(m_virtualMon.VirtualScreenLeft)
    ofsY = Abs(m_virtualMon.VirtualScreenTop)
    
    Dim mon As clsMonitor
    
    For Each mon In m_virtualMon.Monitors
        
        Me.Line ((mon.MonitorLeft + ofsX) * dblScale, (mon.MonitorTop + ofsY) * dblScale) _
            -((mon.MonitorRight + ofsX) * dblScale, (mon.MonitorBottom + ofsY) * dblScale), RGB(128, 128, 192), BF
        Me.Line ((mon.MonitorLeft + ofsX) * dblScale, (mon.MonitorTop + ofsY) * dblScale) _
            -((mon.MonitorRight + ofsX) * dblScale, (mon.MonitorBottom + ofsY) * dblScale), RGB(64, 64, 255), B
        
        Me.Line ((mon.WorkLeft + ofsX) * dblScale, (mon.WorkTop + ofsY) * dblScale) _
            -((mon.WorkRight + ofsX) * dblScale, (mon.WorkBottom + ofsY) * dblScale), RGB(224, 255, 244), BF
        Me.Line ((mon.WorkLeft + ofsX) * dblScale, (mon.WorkTop + ofsY) * dblScale) _
            -((mon.WorkRight + ofsX) * dblScale, (mon.WorkBottom + ofsY) * dblScale), RGB(128, 244, 128), B
        
        Me.CurrentX = (mon.MonitorLeft + ofsX) * dblScale
        Me.CurrentY = (mon.MonitorTop + ofsY) * dblScale
        
        Me.Print mon.MonitorName;
        
        If mon.IsPrimary Then
            Me.Print "(Primary)"
        End If
    Next
    
    Dim rc As RECT
    GetWindowRect Me.hwnd, rc
    
    Me.Line ((rc.Left + ofsX) * dblScale, (rc.Top + ofsY) * dblScale) _
        -((rc.Right + ofsX) * dblScale, (rc.Bottom + ofsY) * dblScale), RGB(255, 255, 255), BF
    Me.Line ((rc.Left + ofsX) * dblScale, (rc.Top + ofsY) * dblScale) _
        -((rc.Right + ofsX) * dblScale, (rc.Bottom + ofsY) * dblScale), RGB(192, 192, 192), B
        
End Sub

Private Sub Form_Resize()

    Me.Refresh
    
End Sub
