Attribute VB_Name = "mdlMain"
Option Explicit

Public g_args() As String

Public Sub Main()

    If Command = "" Then
        '�呀�삩��K�v�ȃt�@�C����ǂݍ���
        If NewDocument Then
            frmMain.Show
        Else
            MsgBox "�t�@�C���̓ǂݍ��݂Ɏ��s���܂����B"
        End If
    Else
        OpenDocument Command
    
        DataRecover
    End If
    
End Sub
