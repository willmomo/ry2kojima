VERSION 5.00
Object = "{648A5603-2C6E-101B-82B6-000000000014}#1.1#0"; "MSCOMM32.OCX"
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "COMDLG32.OCX"
Begin VB.Form Form1 
   Caption         =   "Form1"
   ClientHeight    =   5880
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   10560
   LinkTopic       =   "Form1"
   ScaleHeight     =   5880
   ScaleWidth      =   10560
   StartUpPosition =   3  'Windows �̊���l
   Begin VB.CommandButton Command3 
      Caption         =   "�Q��(&B)..."
      Height          =   495
      Left            =   9120
      TabIndex        =   5
      Top             =   360
      Width           =   1215
   End
   Begin MSComDlg.CommonDialog openDlg 
      Left            =   3960
      Top             =   1440
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
   End
   Begin VB.TextBox Text1 
      Height          =   375
      Left            =   120
      TabIndex        =   4
      Text            =   "Text1"
      Top             =   360
      Width           =   8775
   End
   Begin VB.ListBox List1 
      Height          =   3480
      Left            =   120
      TabIndex        =   2
      Top             =   2160
      Width           =   7095
   End
   Begin VB.CommandButton Command2 
      Caption         =   "Command2"
      Height          =   495
      Left            =   1800
      TabIndex        =   1
      Top             =   1440
      Width           =   1215
   End
   Begin VB.CommandButton Command1 
      Caption         =   "Command1"
      Height          =   495
      Left            =   240
      TabIndex        =   0
      Top             =   1440
      Width           =   1215
   End
   Begin MSCommLib.MSComm MSComm1 
      Left            =   3240
      Top             =   1440
      _ExtentX        =   1005
      _ExtentY        =   1005
      _Version        =   393216
      DTREnable       =   -1  'True
   End
   Begin VB.Label Label1 
      Caption         =   "�p�^�[���t�@�C��"
      Height          =   255
      Left            =   120
      TabIndex        =   3
      Top             =   120
      Width           =   1335
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

'----------------------------------------------------------------------
' ��M�f�[�^�ɉ����āA�������s���B
'----------------------------------------------------------------------
Private Sub ProcRecv(recvData() As Byte)
    Dim sendBuf() As Byte
    
    If recvData(0) = &H31 And recvData(1) = &H5 Then
        ' "1\x05" ����M�����Ƃ��̏���
        ReDim sendBuf(0)
        sendBuf(0) = &H6
        
        ' ���M��̎�M�f�[�^ "\x02C\x03@" ����M���邽�߂ɁA4�ɐݒ�
        MSComm1.RThreshold = 4
        
        MSComm1.SThreshold = 1
        MSComm1.Output = sendBuf
    Else
        ' "1\x05" ����M�o����悤�ɁA2�ɐݒ�
        MSComm1.RThreshold = 2
    End If
End Sub

'----------------------------------------------------------------------
' �ʐM�|�[�g�̏�����
'----------------------------------------------------------------------
Private Sub InitMSComm()
    With MSComm1
        'COM5���g�p
        .CommPort = 5
        
        '�{�[���[�g�Ȃ�
        .Settings = "9600,E,8,2"
        
        '�o�C�i�����[�h�ɐݒ�
        .InputMode = comInputModeBinary
    End With
End Sub

'----------------------------------------------------------------------
' UI��Ԃ�ω�������
'----------------------------------------------------------------------
Private Sub UpdateUI()
    Command1.Caption = IIf(MSComm1.PortOpen, "����", "�J��")
    Command2.Enabled = MSComm1.PortOpen
End Sub

'----------------------------------------------------------------------
' �ʐM�|�[�gOpen/Close
'----------------------------------------------------------------------
Private Sub Command1_Click()
    MSComm1.PortOpen = Not MSComm1.PortOpen
    
    ' "1\x05" ����M���邽�߂ɁA2��ݒ�
    MSComm1.RThreshold = 2
    
    'UI��Ԃ�ω�������
    Call UpdateUI
End Sub

'----------------------------------------------------------------------
' �p�^�[�������X�^�[�g
'----------------------------------------------------------------------
Private Sub Command2_Click()
    'pattern_open(text1.text)
    
    'pattern_read_line(c1, c2, cdata)
    
    'if c1 = 'R' then
    '
    'elseif c1 = 'S' then
    '
    'end if
End Sub

'----------------------------------------------------------------------
' �Q�ƃ_�C�A���O�̕\��
'----------------------------------------------------------------------
Private Sub Command3_Click()
    On Error GoTo ErrHandler
    
    openDlg.CancelError = True
    openDlg.ShowOpen
    Text1.Text = openDlg.FileName
    
    Exit Sub
ErrHandler:

End Sub

'----------------------------------------------------------------------
' Load�C�x���g
'----------------------------------------------------------------------
Private Sub Form_Load()
    '�ʐM�f�o�C�X�̏�����
    Call InitMSComm
    
    'UI��Ԃ̏�����
    Call UpdateUI
End Sub

'----------------------------------------------------------------------
' �ʐM�C�x���g
'----------------------------------------------------------------------
Private Sub MSComm1_OnComm()
    Dim recvBuf() As Byte
    Dim i As Long
    Dim recvStr As String
    
    Select Case MSComm1.CommEvent
    Case comEvReceive
        '��M����
        List1.AddItem Now & vbTab & "�f�[�^��" & MSComm1.InBufferCount & "�o�C�g��M���܂����B"
        List1.ListIndex = List1.ListCount - 1
        
        recvBuf = MSComm1.Input
        Debug.Print Now & vbTab;
        For i = 0 To UBound(recvBuf)
            Debug.Print recvBuf(i);
        Next
        Debug.Print
        
        ProcRecv recvBuf
    Case comEvSend
        '���M����
        List1.AddItem Now & vbTab & "�f�[�^�𑗐M���܂����B"
        List1.ListIndex = List1.ListCount - 1
    Case comEvCTS
        List1.AddItem Now & vbTab & "CTS�̏�Ԃ��ω����܂����B"
        List1.ListIndex = List1.ListCount - 1
    Case Else
        List1.AddItem Now & vbTab & MSComm1.CommEvent
        List1.ListIndex = List1.ListCount - 1
    End Select
End Sub
