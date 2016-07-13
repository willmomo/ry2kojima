Attribute VB_Name = "Module1"
Option Explicit

Private Sub ConvInfoBoard()
    Dim result As VbMsgBoxResult
    result = MsgBox("データベースは、v4.2以降に変換されている必要があります。" & vbNewLine & _
                     "一行ごと伝言板にデータが入っている場合は、削除されます。" & vbNewLine & _
                     vbNewLine & _
                     "伝言板データを移行しますか?", _
                     vbExclamation Or vbYesNo, "警告: 伝言板データ移行")
    
    If result <> vbYes Then
        Exit Sub
    End If
    
    ' 旧伝言板版データにアクセスするオブジェクトを作成
    Dim infoBoard As clsInfoboard
    Set infoBoard = New clsInfoboard
    
    ' 新伝言板データにアクセするオブジェクトを作成
    Dim information As clsInformation
    Set information = New clsInformation
    
    ' 旧伝言板を読み込み、新伝言板に変換
    infoBoard.DBOpen "localhost"
    information.UpdateInfoBoard infoBoard
    
    ' 新伝言板に書き込む
    information.UpdateDatabase "localhost"
    
    ' 完了メッセージの表示
    MsgBox "伝言板データを移行しました。", vbInformation Or vbOKOnly, "情報: 伝言板データ移行"
    
    ' 後始末
    Set infoBoard = Nothing
    Set information = Nothing
End Sub

Public Sub Main()

    If Command = "40_42" Then
    
        ConvInfoBoard
        
    ElseIf Command = "43_45" Then
    
        ConvInformation
        Conv
        
    End If
    
End Sub
