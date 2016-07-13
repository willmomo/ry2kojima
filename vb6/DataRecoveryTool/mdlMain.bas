Attribute VB_Name = "mdlMain"
Option Explicit

'
' プログラム・スタート
'
Public Sub Main()
    
    If Command = "" Then
    
        '
        ' 引数が無いときは、GUIモード
        '
        If NewDocument Then
            frmMain.Show
        Else
            MsgBox "主操作からファイルの読み込みに失敗しました(MANUAL)。"
        End If
    Else
    
        '
        ' 引数で与えられた代表ファイルを開いてデータ復旧を実行する
        ' データの復旧方法は、常に連動モード
        '
        If NewDocument Then
            If OpenDocument(Command) = False Then
                MsgBox "引数 '" & Command & "' は、開けません。", vbCritical
                Exit Sub
            End If
            
            If DataRecover(True) Then
                frmWait.Show vbModal
                If frmWait.Error = 0 Then
                    NormalWindup
                    ShowResult
                Else
                    AbnormalWindup
                    MsgBox "データ復旧に失敗しました。", vbCritical
                End If
            Else
                AbnormalWindup
                MsgBox "データ復旧に失敗しました。", vbCritical
            End If
        Else
            MsgBox "主操作からのファイルの読み込みに失敗しました(AUTO)。"
        End If
    End If
    
End Sub
