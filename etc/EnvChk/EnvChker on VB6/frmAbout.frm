VERSION 5.00
Begin VB.Form frmAbout 
   BorderStyle     =   3  '�Œ��޲�۸�
   Caption         =   "�o�[�W�������"
   ClientHeight    =   3555
   ClientLeft      =   2340
   ClientTop       =   1935
   ClientWidth     =   5730
   ClipControls    =   0   'False
   LinkTopic       =   "Form2"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   2453.724
   ScaleMode       =   0  'հ�ް
   ScaleWidth      =   5380.766
   ShowInTaskbar   =   0   'False
   StartUpPosition =   1  '��Ű ̫�т̒���
   Begin VB.PictureBox picIcon 
      AutoSize        =   -1  'True
      ClipControls    =   0   'False
      Height          =   540
      Left            =   240
      Picture         =   "frmAbout.frx":0000
      ScaleHeight     =   337.12
      ScaleMode       =   0  'հ�ް
      ScaleWidth      =   337.12
      TabIndex        =   1
      Top             =   240
      Width           =   540
   End
   Begin VB.CommandButton cmdOK 
      Cancel          =   -1  'True
      Caption         =   "OK"
      Default         =   -1  'True
      Height          =   345
      Left            =   4245
      TabIndex        =   0
      Top             =   2625
      Width           =   1380
   End
   Begin VB.CommandButton cmdSysInfo 
      Caption         =   "���я��(&S)..."
      Height          =   345
      Left            =   4260
      TabIndex        =   2
      Top             =   3075
      Width           =   1380
   End
   Begin VB.Line Line1 
      BorderColor     =   &H00808080&
      BorderStyle     =   6  '���� (�ӂ��ǂ�)
      Index           =   1
      X1              =   84.515
      X2              =   5309.398
      Y1              =   1687.583
      Y2              =   1687.583
   End
   Begin VB.Label lblDescription 
      Caption         =   "�A�v���P�[�V�����̓�������`�F�b�N���܂��B"
      ForeColor       =   &H00000000&
      Height          =   1170
      Left            =   1050
      TabIndex        =   3
      Top             =   1125
      Width           =   3885
   End
   Begin VB.Label lblTitle 
      Caption         =   "���`�F�b�J"
      ForeColor       =   &H00000000&
      Height          =   480
      Left            =   1050
      TabIndex        =   5
      Top             =   240
      Width           =   3885
   End
   Begin VB.Line Line1 
      BorderColor     =   &H00FFFFFF&
      BorderWidth     =   2
      Index           =   0
      X1              =   98.6
      X2              =   5309.398
      Y1              =   1697.936
      Y2              =   1697.936
   End
   Begin VB.Label lblVersion 
      Caption         =   "1.0.0.0"
      Height          =   225
      Left            =   1050
      TabIndex        =   6
      Top             =   780
      Width           =   3885
   End
   Begin VB.Label lblDisclaimer 
      ForeColor       =   &H00000000&
      Height          =   825
      Left            =   255
      TabIndex        =   4
      Top             =   2625
      Width           =   3870
   End
End
Attribute VB_Name = "frmAbout"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

' ڼ޽�� �� ����è ��߼��...
Const READ_CONTROL = &H20000
Const KEY_QUERY_VALUE = &H1
Const KEY_SET_VALUE = &H2
Const KEY_CREATE_SUB_KEY = &H4
Const KEY_ENUMERATE_SUB_KEYS = &H8
Const KEY_NOTIFY = &H10
Const KEY_CREATE_LINK = &H20
Const KEY_ALL_ACCESS = KEY_QUERY_VALUE + KEY_SET_VALUE + _
                       KEY_CREATE_SUB_KEY + KEY_ENUMERATE_SUB_KEYS + _
                       KEY_NOTIFY + KEY_CREATE_LINK + READ_CONTROL
                     
' ڼ޽�� �� ROOT �^...
Const HKEY_LOCAL_MACHINE = &H80000002
Const ERROR_SUCCESS = 0
Const REG_SZ = 1                         ' Null �����ŏI��� Unicode ������

Const REG_DWORD = 4                      ' 32 �ޯĐ��l

Const gREGKEYSYSINFOLOC = "SOFTWARE\Microsoft\Shared Tools Location"
Const gREGVALSYSINFOLOC = "MSINFO"
Const gREGKEYSYSINFO = "SOFTWARE\Microsoft\Shared Tools\MSINFO"
Const gREGVALSYSINFO = "PATH"

Private Declare Function RegOpenKeyEx Lib "advapi32" Alias "RegOpenKeyExA" (ByVal hKey As Long, ByVal lpSubKey As String, ByVal ulOptions As Long, ByVal samDesired As Long, ByRef phkResult As Long) As Long
Private Declare Function RegQueryValueEx Lib "advapi32" Alias "RegQueryValueExA" (ByVal hKey As Long, ByVal lpValueName As String, ByVal lpReserved As Long, ByRef lpType As Long, ByVal lpData As String, ByRef lpcbData As Long) As Long
Private Declare Function RegCloseKey Lib "advapi32" (ByVal hKey As Long) As Long


Private Sub cmdSysInfo_Click()
  Call StartSysInfo
End Sub

Private Sub cmdOK_Click()
  Unload Me
End Sub

Private Sub Form_Load()
    Me.Caption = App.Title & "�̃o�[�W�������"
    lblVersion.Caption = "�o�[�W���� " & App.Major & "." & App.Minor & "." & App.Revision
    lblTitle.Caption = App.Title
End Sub

Public Sub StartSysInfo()
    On Error GoTo SysInfoErr
  
    Dim rc As Long
    Dim SysInfoPath As String
    
    ' ڼ޽�؂��缽�я����۸��т��߽\���O���擾���܂�...
    If GetKeyValue(HKEY_LOCAL_MACHINE, gREGKEYSYSINFO, gREGVALSYSINFO, SysInfoPath) Then
    ' ڼ޽�؂��缽�я����۸��т��߽���݂̂��擾���܂�...
    ElseIf GetKeyValue(HKEY_LOCAL_MACHINE, gREGKEYSYSINFOLOC, gREGVALSYSINFOLOC, SysInfoPath) Then
        ' ���ɑ��݂���͂��� 32 �ޯ� �ް�ޮ݂�̧�ق��m�F���܂��B
        If (Dir(SysInfoPath & "\MSINFO32.EXE") <> "") Then
            SysInfoPath = SysInfoPath & "\MSINFO32.EXE"
            
        ' �װ - ̧�ق�������܂���...
        Else
            GoTo SysInfoErr
        End If
    ' �װ - ڼ޽�� ���؂�������܂���...
    Else
        GoTo SysInfoErr
    End If
    
    Call Shell(SysInfoPath, vbNormalFocus)
    
    Exit Sub
SysInfoErr:
    MsgBox "�����_�łͼ��я����g�p�ł��܂���", vbOKOnly
End Sub

Public Function GetKeyValue(KeyRoot As Long, KeyName As String, SubKeyRef As String, ByRef KeyVal As String) As Boolean
    Dim i As Long                                           ' ٰ�� ����
    Dim rc As Long                                          ' �߂�l
    Dim hKey As Long                                        ' ����݂���ڼ޽�� ���������
    Dim hDepth As Long                                      '
    Dim KeyValType As Long                                  ' ڼ޽�� �����ް��^
    Dim tmpVal As String                                    ' ڼ޽�� ���l�̈ꎞ�ۑ��p�ϐ�
    Dim KeyValSize As Long                                  ' ڼ޽�� ���ϐ��̻���
    '------------------------------------------------------------
    ' ٰ� �� {HKEY_LOCAL_MACHINE...} �ɂ���ڼ޽�� �����J���܂��B
    '------------------------------------------------------------
    rc = RegOpenKeyEx(KeyRoot, KeyName, 0, KEY_ALL_ACCESS, hKey) ' ڼ޽�� �����J��
    
    If (rc <> ERROR_SUCCESS) Then GoTo GetKeyError          ' ����� �װ...
    
    tmpVal = String$(1024, 0)                             ' �ϐ��̈�̊��蓖��
    KeyValSize = 1024                                       ' �ϐ��̻��ނ��L��

    '------------------------------------------------------------
    ' ڼ޽�� ���l���擾���܂�...
    '------------------------------------------------------------
    rc = RegQueryValueEx(hKey, SubKeyRef, 0, _
                         KeyValType, tmpVal, KeyValSize)    ' ���l�̎擾/�쐬
                        
    If (rc <> ERROR_SUCCESS) Then GoTo GetKeyError          ' ����� �װ
    
    If (Asc(Mid(tmpVal, KeyValSize, 1)) = 0) Then           ' Win95 �� Null �ŏI��镶�����ǉ����܂�...
        tmpVal = Left(tmpVal, KeyValSize - 1)               ' Null ������������A�����񂩂璊�o���܂��B
    Else                                                    ' WinNT �� Null �ŏI��镶������g�p���܂���...
        tmpVal = Left(tmpVal, KeyValSize)                   ' Null ��������Ȃ�������A������݂̂𒊏o���܂��B
    End If
    '------------------------------------------------------------
    ' �ϊ��̂��߂ɁA���l�̌^�𒲂ׂ܂�...
    '------------------------------------------------------------
    Select Case KeyValType                                  ' �ް��^����...
    Case REG_SZ                                             ' String ڼ޽�� �� �ް��^
        KeyVal = tmpVal                                     ' String �l���߰
    Case REG_DWORD                                          ' Double Word ڼ޽�� �� �ް��^
        For i = Len(tmpVal) To 1 Step -1                    ' �e�ޯĂ̕ϊ�
            KeyVal = KeyVal + Hex(Asc(Mid(tmpVal, i, 1)))   ' Char ���Ƃɒl���쐬
        Next
        KeyVal = Format$("&h" + KeyVal)                     ' Double Word �� String �ɕϊ�
    End Select
    
    GetKeyValue = True                                      ' ����I��
    rc = RegCloseKey(hKey)                                  ' ڼ޽�� ����۰��
    Exit Function                                           ' �I��
    
GetKeyError:      ' �װ������̌�n��...
    KeyVal = ""                                             ' �߂�l�̒l���󕶎���ɐݒ�
    GetKeyValue = False                                     ' �ُ�I��
    rc = RegCloseKey(hKey)                                  ' ڼ޽�� ����۰��
End Function
