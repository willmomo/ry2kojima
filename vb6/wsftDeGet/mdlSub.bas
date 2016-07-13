Attribute VB_Name = "mdlSub"
Option Explicit

Public Declare Function wsft_start Lib "wsft.dll" (ByVal hwnd As Long) As Long
Public Declare Function wsft_end Lib "wsft.dll" () As Long
Public Declare Function wsft_getopt Lib "wsft.dll" (ByVal opt As String) As Long
Public Declare Function wsft_setopt Lib "wsft.dll" (ByVal opt As String, ByVal Value As Long) As Long
Public Declare Function wsft_readfile Lib "wsft.dll" (ByVal fid As Integer, ByVal rec As Integer, buf As Any, ByVal bufsize As Long) As Long
Public Declare Function wsft_writefile Lib "wsft.dll" (ByVal fid As Integer, ByVal rec As Integer, buf As Any, ByVal bufsize As Long) As Long
Public Declare Function wsft_dllversion Lib "wsft.dll" () As Long
Public Declare Function wsft_setservstr Lib "wsft.dll" (ByVal servstr As String, ByVal rport As Long, ByVal wport As Long) As Long
Public Declare Function wsft_read_rec Lib "wsft.dll" (ByVal fid As Integer, ByVal starec As Long, ByVal endrec As Long, buf As Any, ByVal recsize As Long) As Long

Public Declare Function GetPrivateProfileString Lib "kernel32" Alias "GetPrivateProfileStringA" _
    (ByVal lpApplicationName As String, ByVal lpKeyName As Any, ByVal lpDefault As String, ByVal lpReturnedString As String, _
    ByVal nSize As Long, ByVal lpFileName As String) As Long


Public Function VBGetPrivateProfileString(sAppName As String, sKeyName As String, sDefault As String, sName As String) As String
    Dim sBuf As String
    Dim nPos As Long
    
    sBuf = String(1024, vbNullChar)
    GetPrivateProfileString sAppName, sKeyName, sDefault, sBuf, 1024, sName
    nPos = InStr(sBuf, vbNullChar)
    If nPos > 0 Then
        VBGetPrivateProfileString = Left(sBuf, nPos - 1)
    Else
        VBGetPrivateProfileString = ""
    End If
End Function

'   wsft�̃G���[������

Public Function wsft_error(ByVal code As Long) As String
    Select Case code
    Case &H1
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":BUSY:���@�킩�珑���ݔr����(�t�@�C�����擾�v����)"
    Case &H2
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":BUSY:���@�킩�珑���ݔr����(�Ǎ��ݗv����)"
    Case &H3
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":BUSY:���@�킩�珑���ݔr����(�����ݏ����v����)"
    Case &H4
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":BUSY:���@�킩�珑���ݔr����(�f�[�^�d�����M��)"
    Case &H5
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":BUSY:���@�킩�珑���ݔr����(�����݊����v����)"
    Case &H1001
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":�o�b�t�@�m�ۃG���["
    Case &H1002
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":�����݃t���[�������G���["
    Case &H1003
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":�����ݎ�M�^�C���A�E�g"
    Case &H1004
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":�t�@�C���ԍ��G���["
    Case &H1005
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":�A�h���X�G���["
    Case &H1006
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":���R�[�h�G���["
    Case &H1007
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":�t���[���G���["
    Case &H1008
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":�R�}���h�G���["
    Case &H1009
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":�o�b�t�@�I�[�o�[�t���[�G���["
    Case &H2001
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":�o�b�t�@�G���["
    Case &H2003
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":��M�T�C�Y�G���["
    Case &H2005
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":�o�b�t�@�T�C�Y�m�F�G���["
    Case &H2010
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":�^�C���A�E�g�G���[(�t�@�C�����擾�v����)"
    Case &H2011
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":�^�C���A�E�g�G���[(�Ǎ��ݗv����)"
    Case &H2012
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":�^�C���A�E�g�G���[(�����ݏ����v����)"
    Case &H2013
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":�^�C���A�E�g�G���[(�f�[�^�d�����M��)"
    Case &H2014
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":�^�C���A�E�g�G���[(�����݊����v����)"
    Case &H2020
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":���M�G���[(�t�@�C�����擾�v����)"
    Case &H2021
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":���M�G���[(�Ǎ��ݗv����)"
    Case &H2022
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":���M�G���[(�����ݏ����v����)"
    Case &H2023
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":���M�G���[(�f�[�^�d�����M��)"
    Case &H2024
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":���M�G���[(�����݊����v����)"
    Case &H2030
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":��M�G���[(�t�@�C�����擾�v����)"
    Case &H2031
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":��M�G���[(�Ǎ��ݗv����)"
    Case &H2032
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":��M�G���[(�����ݏ����v����)"
    Case &H2033
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":��M�G���[(�f�[�^�d�����M��)"
    Case &H2034
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":��M�G���[(�����݊����v����)"
    Case &H2040
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":�������G���[:�T�[�o�[��������Ȃ�"
    Case &H3001
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":�������G���[:winsock�̏������Ɏ��s"
    Case &H3002
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":�������G���[:winsock�̃o�[�W�����G���["
    Case &H3003
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":�������G���[:�\�P�b�g�̍쐬�Ɏ��s"
    Case &H3004
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":�������G���[:�������O�ɒʐM�֐����g�p���ꂽ"
    Case &H3005
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":�������G���[:�������m�ۂɎ��s"
    Case &H3006
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":�������G���[:INI�t�@�C���̐ݒ�G���["
    Case &H3007
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":�������G���[:���O�o�͊J�n�G���["
    Case &H5001
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":�I�v�V�����ݒ�G���["
    Case &H5002
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":�^�C���A�E�g�ݒ�G���["
    Case &H5003
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":�Ǎ��݃|�[�g�ԍ��G���["
    Case &H5004
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":�����݃|�[�g�ԍ��G���["
    Case &H5005
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":���g���C�񐔃G���["
    Case &H5006
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":�z�X�g���G���["
    Case &HF001&
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":�v���g�R���o�[�W����"
    Case Else
        wsft_error = "0x" & Right("0000" & Hex(code), 4) & ":unknown"
    End Select
End Function
