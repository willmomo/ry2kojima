Attribute VB_Name = "mdlMain"
Option Explicit

'
' �v���O�����E�X�^�[�g
'
Public Sub Main()
    
    If Command = "" Then
    
        '
        ' �����������Ƃ��́AGUI���[�h
        '
        If NewDocument Then
            frmMain.Show
        Else
            MsgBox "�呀�삩��t�@�C���̓ǂݍ��݂Ɏ��s���܂���(MANUAL)�B"
        End If
    Else
    
        '
        ' �����ŗ^����ꂽ��\�t�@�C�����J���ăf�[�^���������s����
        ' �f�[�^�̕������@�́A��ɘA�����[�h
        '
        If NewDocument Then
            If OpenDocument(Command) = False Then
                MsgBox "���� '" & Command & "' �́A�J���܂���B", vbCritical
                Exit Sub
            End If
            
            If DataRecover(True) Then
                frmWait.Show vbModal
                If frmWait.Error = 0 Then
                    NormalWindup
                    ShowResult
                Else
                    AbnormalWindup
                    MsgBox "�f�[�^�����Ɏ��s���܂����B", vbCritical
                End If
            Else
                AbnormalWindup
                MsgBox "�f�[�^�����Ɏ��s���܂����B", vbCritical
            End If
        Else
            MsgBox "�呀�삩��̃t�@�C���̓ǂݍ��݂Ɏ��s���܂���(AUTO)�B"
        End If
    End If
    
End Sub
