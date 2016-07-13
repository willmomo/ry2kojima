Attribute VB_Name = "mdlMain"
Option Explicit

Public g_args() As String

Public Sub Main()

    If Command = "" Then
        '主操作から必要なファイルを読み込む
        If NewDocument Then
            frmMain.Show
        Else
            MsgBox "ファイルの読み込みに失敗しました。"
        End If
    Else
        OpenDocument Command
    
        DataRecover
    End If
    
End Sub
