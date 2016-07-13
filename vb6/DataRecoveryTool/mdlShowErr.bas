Attribute VB_Name = "mdlShowErr"
Option Explicit

'
' BASP21Lib.FTP.Connect �̃G���[��\��
'
Public Sub ErrConnect(obj As BASP21Lib.ftp, ByVal rc As Long)
    Dim msg As String
    
    Select Case rc
    Case -1
        msg = "Connect error (" & rc & ") : �\�P�b�g�I�[�v���G���["
    Case -2
        msg = "Connect error (" & rc & ") : �^�C���A�E�g"
    Case 1 To 5
        msg = "Connect error (" & rc & ") : " & obj.GetReply
    Case Is >= 10000
        msg = "Connect error (" & rc & ") : Winsock Error"
    Case Else
        msg = "Connect error (" & rc & ") : Unknown Error"
    End Select
    
    MsgBox msg
End Sub


'
' BASP21Lib.FTP.PutFile �̃G���[��\��
'
Public Sub ErrPutFile(obj As BASP21Lib.ftp, ByVal rc As Long)
    Dim msg As String
    
    Select Case rc
    Case 0
        msg = "PutFile error (" & rc & ") : �Y������t�@�C���Ȃ�"
    Case Else
        msg = "PutFile error (" & rc & ") : " & obj.GetReply
    End Select
    
    MsgBox msg
End Sub

