Attribute VB_Name = "mdlUtil"
Option Explicit

Public Function ToInteger(expression, intValue As Integer) As Boolean
    If IsNumeric(expression) = True Then
        intValue = CInt(expression)
        ToInteger = True
    Else
        ToInteger = False
    End If
End Function

Public Function ToLong(expression, lngValue As Long) As Boolean
    If IsNumeric(expression) = True Then
        lngValue = CLng(expression)
        ToLong = True
    Else
        ToLong = False
    End If
End Function

Public Sub TBHSelAll(tb As TextBox)
    With tb
        .SelStart = 0
        .SelLength = Len(.Text)
    End With
End Sub

Public Sub CBHAppendItem(cb As ComboBox, item, Optional number = 0)
    With cb
        .AddItem item
        .ItemData(.NewIndex) = number
    End With
End Sub
