VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsCSVFile"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

Private m_fno As Integer

Public Property Get IsEOF() As Boolean
    IsEOF = EOF(m_fno)
End Property

Public Function OpenReadFile(strFileName As String) As Boolean
    m_fno = FreeFile
    Open strFileName For Input As m_fno
End Function

Public Function OpenWriteFile(strFileName As String) As Boolean
    m_fno = FreeFile
    Open strFileName For Output As m_fno
End Function

Public Sub CloseFile()
    If m_fno <> 0 Then
        Close m_fno
        m_fno = 0
    End If
End Sub

Public Function ReadCSV() As Variant
    Dim lineBuffer As String
    
    Line Input #m_fno, lineBuffer
    ReadCSV = Split(lineBuffer, ",")
End Function

Public Sub WriteCSV(csvBuffer As Variant)
    Print #m_fno, Join(csvBuffer, ",")
End Sub
