Attribute VB_Name = "Module1"
Option Explicit

Private Sub ConvInfoBoard()
    Dim result As VbMsgBoxResult
    result = MsgBox("�f�[�^�x�[�X�́Av4.2�ȍ~�ɕϊ�����Ă���K�v������܂��B" & vbNewLine & _
                     "��s���Ɠ`���Ƀf�[�^�������Ă���ꍇ�́A�폜����܂��B" & vbNewLine & _
                     vbNewLine & _
                     "�`���f�[�^���ڍs���܂���?", _
                     vbExclamation Or vbYesNo, "�x��: �`���f�[�^�ڍs")
    
    If result <> vbYes Then
        Exit Sub
    End If
    
    ' ���`���Ńf�[�^�ɃA�N�Z�X����I�u�W�F�N�g���쐬
    Dim infoBoard As clsInfoboard
    Set infoBoard = New clsInfoboard
    
    ' �V�`���f�[�^�ɃA�N�Z����I�u�W�F�N�g���쐬
    Dim information As clsInformation
    Set information = New clsInformation
    
    ' ���`����ǂݍ��݁A�V�`���ɕϊ�
    infoBoard.DBOpen "localhost"
    information.UpdateInfoBoard infoBoard
    
    ' �V�`���ɏ�������
    information.UpdateDatabase "localhost"
    
    ' �������b�Z�[�W�̕\��
    MsgBox "�`���f�[�^���ڍs���܂����B", vbInformation Or vbOKOnly, "���: �`���f�[�^�ڍs"
    
    ' ��n��
    Set infoBoard = Nothing
    Set information = Nothing
End Sub

Public Sub Main()

    If Command = "40_42" Then
    
        ConvInfoBoard
        
    ElseIf Command = "43_45" Then
    
        ConvInformation
        Conv
        
    End If
    
End Sub
