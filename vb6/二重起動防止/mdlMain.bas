Attribute VB_Name = "mdlMain"
Option Explicit

'------------------------------------------------------------------------------
' �v���O�����X�^�[�g
'------------------------------------------------------------------------------
Public Sub Main()
    Dim dupGuard As New clsMutex
    
    If Not dupGuard.create("��d�N��.mutex") Then
        MsgBox "��d�N���h�~�p Mutex ���쐬�ł��܂���B"
        Exit Sub
    End If
    
    If dupGuard.AlreadyExists Then
        MsgBox "��d�N���Ȃ̂ŏI�����܂��B"
        Exit Sub
    End If
    
    Form1.Show vbModal
    
    '��d�N���h�~Mutex�́A�����j������܂��B
    
End Sub
