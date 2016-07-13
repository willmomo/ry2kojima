Attribute VB_Name = "kjmlib"
'------------------------------------------------------------------------------
' ver.0.2011.6.8
'------------------------------------------------------------------------------
Option Explicit

Private Declare Function GetComputerName Lib "kernel32" Alias "GetComputerNameA" (ByVal lpBuffer As String, nSize As Long) As Long

'------------------------------------------------------------------------------
' VB6 向け GetComputerName の実装
'------------------------------------------------------------------------------
Public Function VBGetComputerName() As String
    Dim strName As String
    Dim nSize As Long

    'バッファを確保
    Do
        nSize = nSize + 16
        strName = String(nSize, Chr(0))
    Loop While GetComputerName(strName, nSize) = 0
    
    VBGetComputerName = Left(strName, nSize)
End Function

