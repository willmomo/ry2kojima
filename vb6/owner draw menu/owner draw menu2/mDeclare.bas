Attribute VB_Name = "mDeclare"
'
'*************************************************************************
'
'                        ���̑�API���W���[��
'
'*************************************************************************
'
Option Explicit
' �������[�̈���R�s�[����֐�
Declare Sub CopyMemory Lib "kernel32" Alias "RtlMoveMemory" (pDest As Any, pSource As Any, ByVal dwLength As Long)
' �f�o�C�X�R���e�L�X�g�̃n���h�����擾
Declare Function GetDC Lib "user32" (ByVal hwnd As Long) As Long
' �f�o�C�X�R���e�L�X�g�����
Declare Function ReleaseDC Lib "user32" (ByVal hwnd As Long, ByVal hdc As Long) As Long
' �w�i�F��ݒ�
Declare Function SetBkColor Lib "gdi32" (ByVal hdc As Long, ByVal crColor As Long) As Long
' �e�L�X�g�̐F��ݒ�
Declare Function SetTextColor Lib "gdi32" (ByVal hdc As Long, ByVal crColor As Long) As Long
' ��ʂ̃f�U�C���Ɏg���Ă���F���擾
Declare Function GetSysColor Lib "user32" (ByVal nIndex As Long) As Long
    Public Const COLOR_MENU = 4             ' ���j���[�̔w�i�F
    Public Const COLOR_MENUTEXT = 7         ' ���j���[���̃e�L�X�g�̐F
    Public Const COLOR_HIGHLIGHT = 13       ' �I�����ꂽ���ڂ̐F
    Public Const COLOR_HIGHLIGHTTEXT = 14   ' �I�����ꂽ���ڂ̃e�L�X�g�̐F
    Public Const COLOR_GRAYTEXT = 17        ' �W�F��� (�������) �̃e�L�X�g�̐F
' �e�L�X�g��\��
Declare Function DrawText Lib "user32" Alias "DrawTextA" (ByVal hdc As Long, ByVal lpStr As String, ByVal nCount As Long, lpRect As RECT, ByVal wFormat As Long) As Long
    Public Const DT_CALCRECT = &H400    ' �e�L�X�g��\�����邽�߂ɕK�v�ȃT�C�Y�擾
    Public Const DT_LEFT = &H0
    Public Const DT_RIGHT = &H2
    Public Const DT_SINGLELINE = &H20
    Public Const DT_VCENTER = &H4
    Public Const DT_EXPANDTABS = &H40

' �w�i���[�h��ݒ�
Declare Function SetBkMode Lib "gdi32" (ByVal hdc As Long, ByVal nBkMode As Long) As Long
    Public Const TRANSPARENT = 1    ' �w�i�����̂܂܎c��
    Public Const OPAQUE = 2         ' �`�悳���O�ɁA���݂̔w�i�F�Ŕw�i��h��Ԃ�
' �h��Ԃ������`�̕`��
Declare Function FillRect Lib "user32" (ByVal hdc As Long, lpRect As RECT, ByVal hBrush As Long) As Long
' �_���u���V���쐬
Declare Function CreateSolidBrush Lib "gdi32" (ByVal crColor As Long) As Long
' �I�u�W�F�N�g���폜
Declare Function DeleteObject Lib "gdi32" (ByVal hObject As Long) As Long

