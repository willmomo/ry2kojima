Attribute VB_Name = "mdlComboBoxHelper"
'----------------------------------------------------------------------
'   [�T�v]
'   ComboBox�R���g���[����֗��Ɉ����֐��Q
'
'   [�X�V��]
'   2003/11/01 : �V�K�쐬

Option Explicit

'----------------------------------------------------------------------
'   [�T�v]
'   ComboBox�ɃA�C�e����ǉ�����Ɠ����Ɋ֘A�f�[�^���ǉ�����B
'
'   [����]
'   cb      : ComboBox�R���g���[��
'   item    : ComboBox�ɒǉ�����A�C�e��
'   number  : �A�C�e���Ɋ֘A�t����f�[�^
'
'   [�߂�l]
'   �Ȃ�
'
'   [����]

Public Sub CBHAppendItem(cb As ComboBox, item, Optional number = 0)
    With cb
        .AddItem item
        .ItemData(.NewIndex) = number
    End With
End Sub

