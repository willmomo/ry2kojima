Attribute VB_Name = "mdlTextBoxHelper"
'----------------------------------------------------------------------
'   [�T�v]
'   TextBox�R���g���[����֗��Ɉ����֐��Q
'
'   [�X�V��]
'   2003/11/01 : �V�K�쐬

Option Explicit

'----------------------------------------------------------------------
'   [�T�v]
'   �e�L�X�g��S�I������
'
'   [����]
'   tb  : TextBox�R���g���[��
'
'   [�߂�l]
'   �Ȃ�
'
'   [����]
'   TextBox�Ƀt�H�[�J�X���Ȃ��ƑI������Č����Ȃ����Ƃɒ��ӁB

Public Sub TBHSelAll(tb As TextBox)
    With tb
        .SelStart = 0
        .SelLength = Len(.Text)
    End With
End Sub
