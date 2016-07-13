Attribute VB_Name = "mdlUtil"
Option Explicit

Private Declare Function GetPrivateProfileString Lib "kernel32" Alias "GetPrivateProfileStringA" ( _
    ByVal lpApplicationName As String, _
    ByVal lpKeyName As Any, _
    ByVal lpDefault As String, _
    ByVal lpReturnedString As String, _
    ByVal nSize As Long, _
    ByVal lpFileName As String _
) As Long


'----------------------------------------------------------------------
' API�֐� GetPrivateProfileString �̃��b�v�֐�
'
' [����]
' strSection    �Z�N�V������
' strKey        �L�[��
' strDefault    �Z�N�V�����܂��́A�L�[���������̖߂�l
' strFileName   INI�t�@�C���̖��O
'
' [�߂�l]
' �w�肵��INI�t�@�C���A�Z�N�V�����A�L�[����擾�ł��镶����B
' strSection��vbNullString���w�肵���Ƃ��́A�S�Z�N�V��������vbNullChar�ŋ�؂���������B
' strKey��vbNullString���w�肵���Ƃ��́A�Z�N�V�������̑S�L�[����vbNullChar�ŋ�؂���������B
'
'----------------------------------------------------------------------
Public Function VBGetPrivateProfileString(strSection As String, strKey As String, strDefault As String, strFileName As String) As String

    Dim lngPos As Long
    Dim lngRet As Long
    Dim lngBufSize As Long
    Dim strRet As String
    
    '�Œ�ł��f�t�H���g������+256�������̃o�b�t�@���m�ۂ��ď������J�n
    lngBufSize = 256 + LenB(strDefault)
    
    'GetPrivateProfileString�̖߂�l���A�o�b�t�@�̃T�C�Y-1 or -2�̎��́A
    '�o�b�t�@�̃T�C�Y������Ȃ��̂ŁA�g�����Ȃ���J��Ԃ�
    Do
        lngBufSize = lngBufSize * 2
        strRet = String(lngBufSize, vbNullChar)
        lngRet = GetPrivateProfileString(strSection, strKey, strDefault, strRet, lngBufSize, strFileName)
    Loop While (lngRet = lngBufSize - 1) Or (lngRet = lngBufSize - 2)
    
    '������I�[��'\0'����菜��
    'strSection, strKey��vbNullString(NULL) or "" ���w�肵���Ƃ��́A���vbNullChar('\0')���I�[�ɂȂ��Ă���
    If (strSection = vbNullString) Or (strKey = vbNullString) Then
        lngPos = InStr(strRet, String(2, vbNullChar))
    Else
        lngPos = InStr(strRet, vbNullChar)
    End If
    If lngPos > 0 Then
        strRet = Left(strRet, lngPos - 1)
    End If
    
    VBGetPrivateProfileString = strRet
End Function
