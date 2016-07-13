Attribute VB_Name = "CheckOver255"
Option Explicit

Sub CheckOver255()

'
' 255�������I�[�o�[�����Z����ŁX�����F�ɕύX���� Macro
' �}�N���L�^�� : 2006/3/24  ���[�U�[�� : r.kojima
'

'
    Range("A1").Select
    Range(Selection, ActiveCell.SpecialCells(xlLastCell)).Select
        
    Dim oneCell As Range
    
    For Each oneCell In ActiveWindow.RangeSelection
    
        '�P�ƃZ��(��������Ă��Ȃ�)�A�܂��́A
        '�����Z���̍���̃Z���̂Ƃ��A�������s��
        '(�P�ƃZ����MergeArea�́A�������g�Ɠ����ɂȂ�)
        
        If oneCell.MergeArea.Column = oneCell.Column And oneCell.MergeArea.Row = oneCell.Row Then
        
            If Len(oneCell.Value) >= 255 Then
                oneCell.Interior.ColorIndex = 54
                oneCell.Interior.Pattern = xlSolid
            End If
            
        End If
        
    Next
End Sub
