Attribute VB_Name = "Module1"
Option Explicit

'----------------------------------------------------------------------
'[�T�v]
'   ���l���w�肳�ꂽ�����Ő؂�̂Ă܂��B
'
'[����]
'   expression          �؂�̂Ă̑ΏۂƂȂ�����l���w�肵�܂��B
'   numdecimalplaces    expression��؂�̂Ă����ʂ̌������w�肵�܂��B
'
'[����]
'   Excel��ROUNDDOWN�֐��Ɠ����̓��������܂��B
'   numdecimalplaces�ɐ��̐����w�肷��ƁA���l �͏����_�̉E(�����_�ȉ�)
'   �̎w�肵�����Ő؂�̂Ă��܂��B
'   numdecimalplaces��0(�[��)���w�肷�邩�A�܂��͏ȗ�����ƁAexpression
'   �͍ł��߂������ɐ؂�̂Ă��܂��B
'   numdecimalplaces�ɕ��̐����w�肷��ƁAexpression�͏����_�̍�(������
'   ��)�̎w�肵�����Ő؂�̂Ă��܂��B

Public Function RoundDown(expression, Optional numdecimalplaces = 0)
    Dim lngInflate As Long
    Dim i As Long
    
    '�����ł̐؂�̂ď����̂��߉��ʂ�p��
    lngInflate = 1
    For i = 1 To Abs(numdecimalplaces)
        lngInflate = lngInflate * 10
    Next i
    
    '��{��Fix�֐����g���Đ؂�̂�
    If numdecimalplaces > 0 Then
        RoundDown = Fix(expression * lngInflate) / lngInflate
    Else
        RoundDown = Fix(expression / lngInflate) * lngInflate
    End If
End Function

