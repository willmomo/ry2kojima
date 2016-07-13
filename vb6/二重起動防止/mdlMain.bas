Attribute VB_Name = "mdlMain"
Option Explicit

'------------------------------------------------------------------------------
' プログラムスタート
'------------------------------------------------------------------------------
Public Sub Main()
    Dim dupGuard As New clsMutex
    
    If Not dupGuard.create("二重起動.mutex") Then
        MsgBox "二重起動防止用 Mutex が作成できません。"
        Exit Sub
    End If
    
    If dupGuard.AlreadyExists Then
        MsgBox "二重起動なので終了します。"
        Exit Sub
    End If
    
    Form1.Show vbModal
    
    '二重起動防止Mutexは、自動破棄されます。
    
End Sub
