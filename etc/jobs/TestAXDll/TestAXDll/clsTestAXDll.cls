VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsTestAXDll"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
Option Explicit

'----------------------------------------------------------------------
Public Function RetDouble_ValLong_ValLong(ByVal value1 As Long, ByVal value2 As Long) As Double

    If value2 <> 0 Then
        RetDouble_ValLong_ValLong = CDbl(value1) / CDbl(value2)
    Else
        RetDouble_ValLong_ValLong = 0#
    End If
    
End Function

Public Function RetString_ValLong_ValLong(ByVal value1 As Long, ByVal value2 As Long) As String

    If value2 <> 0 Then
        RetString_ValLong_ValLong = Format(CDbl(value1) / CDbl(value2), "###0.0")
    Else
        RetString_ValLong_ValLong = "Åá"
    End If
    
End Function

Public Function RetBoolean_ValLong_ValLong_RefDouble(ByVal value1 As Long, ByVal value2 As Long, ByRef value3 As Double) As Boolean

    If value2 <> 0 Then
        value3 = CDbl(value1) / CDbl(value2)
        RetBoolean_ValLong_ValLong_RefDouble = True
    Else
        value3 = 0#
        RetBoolean_ValLong_ValLong_RefDouble = False
    End If
    
End Function

