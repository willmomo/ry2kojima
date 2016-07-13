Attribute VB_Name = "mdlIniFunc"
'------------------------------------------------------------------------------
' INI�t�@�C���ɃA�N�Z�X����֐�
'
' 2012/02/23 r.kojima new
'------------------------------------------------------------------------------
Option Explicit

Private Declare Function GetPrivateProfileSection Lib "kernel32" Alias "GetPrivateProfileSectionA" ( _
    ByVal lpAppName As String, ByVal lpReturnedString As String, ByVal nSize As Long, ByVal lpFileName As String) As Long

' �o�b�t�@���g�����鎞�̃T�C�Y
' INI�t�@�C���֌W�̊֐��́A�o�b�t�@���g�����Ȃ��牽�x���ĂԕK�v������̂ŁA
' ���W���[�����ŁA���ʂɎg�p����B
Private Const BUFF_GROW_SIZE As Long = 512

'------------------------------------------------------------------------------
' GetPrivateProfileSection �� VB�p�Ɏ��������֐�
'
' lpAppName   �Z�N�V������
' lpFileName  INI�t�@�C����
'
' �擾�����Z�N�V������񕶎���BSplit(buffer, vbNulLChar) �ŁA�����\�B
'------------------------------------------------------------------------------
Public Function VBGetPrivateProfileSection(lpAppName As String, lpFileName As String) As String
    Dim buf As String
    Dim bufCnt As Long
    Dim ret As Long
    
    ' GetPrivateProfileString �́A�߂�l���A(�o�b�t�@�T�C�Y - 2)�̊Ԃ́A
    ' �o�b�t�@������Ȃ��\��������̂ŁA�g�����Ȃ���J��Ԃ��Ăяo���B
    Do
        bufCnt = bufCnt + BUFF_GROW_SIZE
        buf = String(bufCnt, vbNullChar)
        ret = GetPrivateProfileSection(lpAppName, buf, bufCnt, lpFileName)
    Loop While ret = bufCnt - 2
    
    ' �擾�������̍Ō�ɂ́A��� NULL ������̂ŁA���̕����ŕ�����Ƃ��Đ؂肾���B
    ' buf �� vbNullChar �Ŗ��߂Ă��邽�߁A�����񂪎擾�ł��Ȃ��Ă��A
    ' �ȉ��̃R�[�h�́A����ɓ��삷��B
    VBGetPrivateProfileSection = Left(buf, InStr(buf, String(2, vbNullChar)) - 1)
End Function
