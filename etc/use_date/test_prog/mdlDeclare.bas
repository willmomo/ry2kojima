Attribute VB_Name = "mdlDeclare"
Option Explicit

Declare Function DosDate2VbDate _
    Lib ".\use_date.dll" _
        (ByVal dosDate As Long) As Date

Declare Function VbDate2DosDate _
    Lib ".\use_date.dll" _
        (ByVal vbDate As Date) As Long

Declare Function VbDate2String _
    Lib ".\use_date.dll" _
        (ByVal vbDate As Date) As String

Declare Function VbDate2String2 _
    Lib ".\use_date.dll" _
        (ByVal vbDate As Date, _
         strYMD As String, strHMS As String) As Long

