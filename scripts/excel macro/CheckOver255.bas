Attribute VB_Name = "CheckOver255"
Option Explicit

Sub CheckOver255()

'
' 255文字をオーバーしたセルを毒々しい色に変更する Macro
' マクロ記録日 : 2006/3/24  ユーザー名 : r.kojima
'

'
    Range("A1").Select
    Range(Selection, ActiveCell.SpecialCells(xlLastCell)).Select
        
    Dim oneCell As Range
    
    For Each oneCell In ActiveWindow.RangeSelection
    
        '単独セル(結合されていない)、または、
        '結合セルの左上のセルのとき、処理を行う
        '(単独セルのMergeAreaは、自分自身と同じになる)
        
        If oneCell.MergeArea.Column = oneCell.Column And oneCell.MergeArea.Row = oneCell.Row Then
        
            If Len(oneCell.Value) >= 255 Then
                oneCell.Interior.ColorIndex = 54
                oneCell.Interior.Pattern = xlSolid
            End If
            
        End If
        
    Next
End Sub
