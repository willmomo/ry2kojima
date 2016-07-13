Attribute VB_Name = "formatKm"
Private Sub last_dateToStr()
    Range("N2").Select
    Do While ActiveCell <> ""
        val_date = ActiveCell And &H1F
        val_mon = Int(ActiveCell / 32) And &HF
        val_year = (Int(ActiveCell / 512) And &H7F) + 1910
        ActiveCell = val_year & "/" & val_mon & "/" & val_date
        
        ActiveCell.Offset(1, 0).Activate
    Loop
    
End Sub

Private Sub dateToStr()
    Range("D2").Select
    Do While ActiveCell <> ""
        val_date = ActiveCell And &H1F
        val_mon = Int(ActiveCell / 32) And &HF
        val_year = (Int(ActiveCell / 512) And &H7F) + 1910
        ActiveCell = val_year & "/" & val_mon & "/" & val_date
        
        ActiveCell.Offset(1, 0).Activate
    Loop
    
End Sub

Public Sub formatKm()
    
    Call last_dateToStr
    Call dateToStr

    Rows("1:1").Select
    Selection.Orientation = -90
    Cells.Select
    Selection.Columns.AutoFit
    
    Range("F:H,L:L,R:S,AA:AP,AS:AW,BK:BK").Select
    Selection.EntireColumn.Hidden = True
    
    Range("A2").Select
    ActiveWindow.FreezePanes = True
    
    Range("A1").Select

    Selection.AutoFilter

End Sub
