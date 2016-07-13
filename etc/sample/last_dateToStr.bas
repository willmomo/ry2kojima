Attribute VB_Name = "last_dateToStr"
Function last_dateToStr(last_date)
    val_date = last_date And &H1F
    val_mon = Int(last_date / 32) And &HF
    val_year = (Int(last_date / 512) And &H7F) + 1910
    last_dateToStr = val_year & "/" & val_mon & "/" & val_date
End Function

