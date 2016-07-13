Attribute VB_Name = "mdlSub"
Option Explicit

Public Function ByteToString(expr() As Byte) As String
    Dim strTemp As String
    
    strTemp = StrConv(expr, vbUnicode)
    ByteToString = Left(strTemp, InStr(strTemp, vbNullChar) - 1)
End Function
