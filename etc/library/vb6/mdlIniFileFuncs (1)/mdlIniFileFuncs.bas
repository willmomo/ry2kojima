Attribute VB_Name = "mdlIniFileFuncs"
'----------------------------------------------------------------------
'   [�T�v]
'   INI�t�@�C���ɃA�N�Z�X����֐��Q
'
'   [�X�V��]
'   2003/11/01 : �V�K�쐬

Option Explicit

'----------------------------------------------------------------------
'   �O���v���V�[�W���̎Q�Ɛ錾

Private Declare Function GetPrivateProfileString Lib "kernel32" Alias "GetPrivateProfileStringA" (ByVal lpApplicationName As String, ByVal lpKeyName As Any, ByVal lpDefault As String, ByVal lpReturnedString As String, ByVal nSize As Long, ByVal lpFileName As String) As Long

'----------------------------------------------------------------------
'   [�T�v]
'   ������I�[��NULL���J�b�g����

Private Sub RemoveNulls(sValue As String)
    Dim lngPos As Long
    lngPos = InStr(sValue, vbNullChar)
    If lngPos > 0 Then
        sValue = Left(sValue, lngPos - 1)
    End If
End Sub

'----------------------------------------------------------------------
'   [�T�v]
'   INI�t�@�C�����當������擾
'
'   [����]
'   sSection    : �Z�N�V������
'   sKey        : �L�[��
'   sDefault    : �L�[�������Ƃ��̃f�t�H���g�l
'   sValue      : �l
'   sFile       : INI�t�@�C����
'
'   [�߂�l]
'   0   : ����I��
'   1   : ����I��(�f�t�H���g�l���g�p)
'
'   [���l]
'   �֐��͕K���������܂����A�L�[��ǂݍ��񂾏ꍇ�ƁA�f�t�H���g�l���g��
'   ���ꍇ�Ŗ߂�l���Ⴂ�܂��B
'
'   sSection, sKey��vbNullString���w�肵���ꍇ�̓���͕s��ł��B

Public Function IFFGetPrivateProfileString(sSection As String, sKey As String, sDefault As String, sValue As String, sFile As String) As Long
    Dim rc As Long
    Dim bufSize As Long
    Dim sTemp As String
    
    '�\���ɒ����l���l�����ēǂݍ���
    Do
        bufSize = bufSize + 256
        sValue = String(bufSize, vbNullChar)
        rc = GetPrivateProfileString(sSection, sKey, sDefault, sValue, bufSize, sFile)
    Loop While rc = bufSize - 1
    
    RemoveNulls sValue  '�s����NULL�������폜
    
    '�ǂݍ���(�Ƃ����)�����񂪃f�t�H���g�l�Ɠ����Ƃ���
    '�f�t�H���g�l��ς��ēǂݍ���ł݂�B
    If sValue = sDefault Then
        sTemp = String(rc + 2, vbNullChar)
        rc = GetPrivateProfileString(sSection, sKey, "!" & sDefault, sTemp, rc + 2, sFile)
    
        RemoveNulls sTemp
        
        '1��ڂ�2��ڂ̒l���Ⴄ�Ƃ��́A�L�[�����݂��Ȃ��̂Ńf�t�H���g�l��ǂݍ���ł���͂��B
        If sValue <> sTemp Then
            IFFGetPrivateProfileString = 1
        End If
    End If
End Function
