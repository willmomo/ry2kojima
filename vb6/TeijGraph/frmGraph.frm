VERSION 5.00
Object = "{92D71E90-25A8-11CF-A640-9986B64D9618}#6.0#0"; "Olch2x32.ocx"
Begin VB.Form frmGraph 
   Caption         =   "Form1"
   ClientHeight    =   6780
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   8835
   LinkTopic       =   "Form1"
   ScaleHeight     =   6780
   ScaleWidth      =   8835
   StartUpPosition =   3  'Windows ‚ÌŠù’è’l
   Begin OlectraChart2D.Chart2D c2d 
      Height          =   5775
      Left            =   360
      TabIndex        =   0
      Top             =   360
      Width           =   7935
      _Version        =   393216
      _Revision       =   1
      _ExtentX        =   13996
      _ExtentY        =   10186
      _StockProps     =   0
      ControlProperties=   "frmGraph.frx":0000
   End
End
Attribute VB_Name = "frmGraph"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Public Sub Update(ByVal n As Long)
    Dim i As Long
    Dim v As Variant
    
    With c2d
        With .ChartGroups(1)
            frmMain.fp.GetText n, 1, v
            .SeriesLabels(n).Text = v
        End With
        
        With .ChartGroups(1).Data
            For i = 1 To 16
                frmMain.fp.GetText n, i + 1, v
                
                If v = 0 Then
                    .Y(n, i) = .HoleValue
                Else
                    .Y(n, i) = v
                End If
            Next
        End With
        
    End With

End Sub

Private Sub Form_Load()
    Dim i As Long
    
    Me.Width = 576 * Screen.TwipsPerPixelX
    Me.Height = 432 * Screen.TwipsPerPixelY
    
    With c2d.ChartGroups(1)
        
        .SeriesLabels.RemoveAll
        .SeriesLabels.Add "----/--/--"
        .SeriesLabels.Add "----/--/--"
        .SeriesLabels.Add "----/--/--"
        
        .Data.NumSeries = 3
        .Data.NumPoints(1) = 16
        
        For i = 1 To .Data.NumPoints(1)
            .Data.Y(1, i) = .Data.HoleValue
            .Data.Y(2, i) = .Data.HoleValue
            .Data.Y(3, i) = .Data.HoleValue
        Next
    End With
    
    c2d.ChartArea.Axes("X").AnnotationMethod = oc2dAnnotateValueLabels
    For i = 1 To c2d.ChartGroups(1).Data.NumPoints(1)
        c2d.ChartArea.Axes("X").ValueLabels.Add i, (i + 8) & "Žž"
    Next
End Sub

Private Sub Form_Resize()
    c2d.Move 0, 0, Me.ScaleWidth, Me.ScaleHeight
End Sub
