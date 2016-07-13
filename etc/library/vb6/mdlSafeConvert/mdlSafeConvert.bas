Attribute VB_Name = "mdlSafeConvert"
'----------------------------------------------------------------------
'   [�T�v]
'   ���S�Ɍ^�ϊ�����֐��Q
'
'   [�X�V��]
'   2003/11/01 : �V�K�쐬

Option Explicit

'----------------------------------------------------------------------
'   [�T�v]
'   ���S��CInt
'
'   [����]
'   expression  : �ϊ�����
'   intValue    : �ϊ���l
'
'   [�߂�l]
'   0   : �ϊ��ɐ���
'   !0  : �ϊ��Ɏ��s(�G���[�R�[�h)
'
'   [����]
'   expression���������ϊ��ł����Ƃ��́A0��߂��AintValue�ɕϊ����
'   �l������܂��B
'
'   �������ϊ��ł��Ȃ��Ƃ��́A0�ȊO��߂��AintValue�͕ω����܂���B
'   �G���[�̏ڍׂ́A�߂�l��Error�֐��ɓn�����Ƃł킩��܂��B

Public Function SafeCInt(expression, intValue As Integer) As Long
    On Error GoTo Err_SafeCInt
    
    intValue = CInt(expression)
    SafeCInt = 0
    Exit Function
    
Err_SafeCInt:
    SafeCInt = Err.Number
End Function

'----------------------------------------------------------------------
'   [�T�v]
'   ���S��CLng
'
'   [����]
'   expression  : �ϊ�����
'   lngValue    : �ϊ���l
'
'   [�߂�l]
'   0   : �ϊ��ɐ���
'   !0  : �ϊ��Ɏ��s(�G���[�R�[�h)
'
'   [����]
'   expression���������ϊ��ł����Ƃ��́A0��߂��AlngValue�ɕϊ���̒l��
'   ����܂��B
'
'   �������ϊ��ł��Ȃ��Ƃ��́A0�ȊO��߂��AlngValue�͕ω����܂���B
'   �G���[�̏ڍׂ́A�߂�l��Error�֐��ɓn�����Ƃł킩��܂��B

Public Function SafeCLng(expression, lngValue As Long) As Long
    On Error GoTo Err_SafeCLng
    
    lngValue = CLng(expression)
    SafeCLng = 0
    Exit Function
    
Err_SafeCLng:
    SafeCLng = Err.Number
End Function
