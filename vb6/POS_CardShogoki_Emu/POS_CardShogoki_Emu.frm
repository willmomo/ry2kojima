VERSION 5.00
Begin VB.Form frmMain 
   Caption         =   "�o�n�r_�J�[�h�ƍ��@�G�~�����[�^"
   ClientHeight    =   7185
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   12135
   ControlBox      =   0   'False
   ScaleHeight     =   7185
   ScaleWidth      =   12135
   StartUpPosition =   3  'Windows �̊���l
   Begin VB.TextBox txtReset 
      Alignment       =   1  '�E����
      BeginProperty Font 
         Name            =   "�l�r �o�S�V�b�N"
         Size            =   18
         Charset         =   128
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   495
      Left            =   7080
      Locked          =   -1  'True
      TabIndex        =   23
      Top             =   6360
      Width           =   1215
   End
   Begin VB.CommandButton cmdClose 
      Caption         =   "����"
      BeginProperty Font 
         Name            =   "�l�r �o�S�V�b�N"
         Size            =   21.75
         Charset         =   128
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   855
      Left            =   9960
      TabIndex        =   22
      Top             =   6000
      Width           =   1695
   End
   Begin VB.TextBox txtAnsyoNum 
      Alignment       =   1  '�E����
      BeginProperty Font 
         Name            =   "�l�r �o�S�V�b�N"
         Size            =   18
         Charset         =   128
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   495
      Left            =   7080
      Locked          =   -1  'True
      TabIndex        =   21
      Top             =   5280
      Width           =   1215
   End
   Begin VB.ComboBox cboScreenNo 
      BeginProperty Font 
         Name            =   "�l�r �o�S�V�b�N"
         Size            =   14.25
         Charset         =   128
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   405
      ItemData        =   "POS_CardShogoki_Emu.frx":0000
      Left            =   7080
      List            =   "POS_CardShogoki_Emu.frx":0025
      Style           =   2  '��ۯ���޳� ؽ�
      TabIndex        =   18
      Top             =   360
      Width           =   3375
   End
   Begin VB.TextBox txtScreen 
      Alignment       =   1  '�E����
      BeginProperty Font 
         Name            =   "�l�r �o�S�V�b�N"
         Size            =   24
         Charset         =   128
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   615
      Index           =   5
      Left            =   3480
      Locked          =   -1  'True
      TabIndex        =   17
      Top             =   2280
      Width           =   3255
   End
   Begin VB.TextBox txtScreen 
      BeginProperty Font 
         Name            =   "�l�r �o�S�V�b�N"
         Size            =   24
         Charset         =   128
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   615
      Index           =   4
      Left            =   240
      Locked          =   -1  'True
      TabIndex        =   16
      Top             =   2280
      Width           =   3255
   End
   Begin VB.TextBox txtScreen 
      Alignment       =   1  '�E����
      BeginProperty Font 
         Name            =   "�l�r �o�S�V�b�N"
         Size            =   24
         Charset         =   128
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   615
      Index           =   3
      Left            =   3480
      Locked          =   -1  'True
      TabIndex        =   15
      Top             =   1680
      Width           =   3255
   End
   Begin VB.TextBox txtScreen 
      BeginProperty Font 
         Name            =   "�l�r �o�S�V�b�N"
         Size            =   24
         Charset         =   128
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   615
      Index           =   2
      Left            =   240
      Locked          =   -1  'True
      TabIndex        =   14
      Top             =   1680
      Width           =   3255
   End
   Begin VB.TextBox txtScreen 
      Alignment       =   1  '�E����
      BeginProperty Font 
         Name            =   "�l�r �o�S�V�b�N"
         Size            =   24
         Charset         =   128
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   615
      Index           =   1
      Left            =   3480
      Locked          =   -1  'True
      TabIndex        =   13
      Top             =   1080
      Width           =   3255
   End
   Begin VB.TextBox txtScreen 
      BeginProperty Font 
         Name            =   "�l�r �o�S�V�b�N"
         Size            =   24
         Charset         =   128
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   615
      Index           =   0
      Left            =   240
      Locked          =   -1  'True
      TabIndex        =   12
      Top             =   1080
      Width           =   3255
   End
   Begin VB.CommandButton cmdTouch 
      Caption         =   "�m��"
      BeginProperty Font 
         Name            =   "�l�r �o�S�V�b�N"
         Size            =   21.75
         Charset         =   128
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   855
      Index           =   11
      Left            =   4320
      TabIndex        =   11
      Top             =   6000
      Width           =   1335
   End
   Begin VB.CommandButton cmdTouch 
      Caption         =   "����"
      BeginProperty Font 
         Name            =   "�l�r �o�S�V�b�N"
         Size            =   21.75
         Charset         =   128
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   855
      Index           =   10
      Left            =   1200
      TabIndex        =   10
      Top             =   6000
      Width           =   1335
   End
   Begin VB.CommandButton cmdTouch 
      Caption         =   "�X"
      BeginProperty Font 
         Name            =   "�l�r �o�S�V�b�N"
         Size            =   21.75
         Charset         =   128
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   855
      Index           =   9
      Left            =   4320
      TabIndex        =   9
      Top             =   5040
      Width           =   1335
   End
   Begin VB.CommandButton cmdTouch 
      Caption         =   "�W"
      BeginProperty Font 
         Name            =   "�l�r �o�S�V�b�N"
         Size            =   21.75
         Charset         =   128
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   855
      Index           =   8
      Left            =   2760
      TabIndex        =   8
      Top             =   5040
      Width           =   1335
   End
   Begin VB.CommandButton cmdTouch 
      Caption         =   "�V"
      BeginProperty Font 
         Name            =   "�l�r �o�S�V�b�N"
         Size            =   21.75
         Charset         =   128
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   855
      Index           =   7
      Left            =   1200
      TabIndex        =   7
      Top             =   5040
      Width           =   1335
   End
   Begin VB.CommandButton cmdTouch 
      Caption         =   "�U"
      BeginProperty Font 
         Name            =   "�l�r �o�S�V�b�N"
         Size            =   21.75
         Charset         =   128
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   855
      Index           =   6
      Left            =   4320
      TabIndex        =   6
      Top             =   4080
      Width           =   1335
   End
   Begin VB.CommandButton cmdTouch 
      Caption         =   "�T"
      BeginProperty Font 
         Name            =   "�l�r �o�S�V�b�N"
         Size            =   21.75
         Charset         =   128
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   855
      Index           =   5
      Left            =   2760
      TabIndex        =   5
      Top             =   4080
      Width           =   1335
   End
   Begin VB.CommandButton cmdTouch 
      Caption         =   "�S"
      BeginProperty Font 
         Name            =   "�l�r �o�S�V�b�N"
         Size            =   21.75
         Charset         =   128
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   855
      Index           =   4
      Left            =   1200
      TabIndex        =   4
      Top             =   4080
      Width           =   1335
   End
   Begin VB.CommandButton cmdTouch 
      Caption         =   "�R"
      BeginProperty Font 
         Name            =   "�l�r �o�S�V�b�N"
         Size            =   21.75
         Charset         =   128
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   855
      Index           =   3
      Left            =   4320
      TabIndex        =   3
      Top             =   3120
      Width           =   1335
   End
   Begin VB.CommandButton cmdTouch 
      Caption         =   "�Q"
      BeginProperty Font 
         Name            =   "�l�r �o�S�V�b�N"
         Size            =   21.75
         Charset         =   128
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   855
      Index           =   2
      Left            =   2760
      TabIndex        =   2
      Top             =   3120
      Width           =   1335
   End
   Begin VB.CommandButton cmdTouch 
      Caption         =   "�P"
      BeginProperty Font 
         Name            =   "�l�r �o�S�V�b�N"
         Size            =   21.75
         Charset         =   128
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   855
      Index           =   1
      Left            =   1200
      TabIndex        =   1
      Top             =   3120
      Width           =   1335
   End
   Begin VB.CommandButton cmdTouch 
      Caption         =   "�O"
      BeginProperty Font 
         Name            =   "�l�r �o�S�V�b�N"
         Size            =   21.75
         Charset         =   128
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   855
      Index           =   0
      Left            =   2760
      TabIndex        =   0
      Top             =   6000
      Width           =   1335
   End
   Begin VB.Label lblReset 
      Caption         =   "�ƍ��@���Z�b�g��"
      BeginProperty Font 
         Name            =   "�l�r �o�S�V�b�N"
         Size            =   12
         Charset         =   128
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   255
      Left            =   7080
      TabIndex        =   24
      Top             =   6000
      Width           =   2415
   End
   Begin VB.Label lblAnsyo 
      Caption         =   "���̃J�[�h�̈Ïؔԍ�"
      BeginProperty Font 
         Name            =   "�l�r �o�S�V�b�N"
         Size            =   12
         Charset         =   128
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   255
      Left            =   7080
      TabIndex        =   20
      Top             =   4920
      Width           =   2415
   End
   Begin VB.Label lblScreen 
      BeginProperty Font 
         Name            =   "�l�r �o�S�V�b�N"
         Size            =   24
         Charset         =   128
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   615
      Left            =   240
      TabIndex        =   19
      Top             =   360
      Width           =   6495
   End
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Const STAGE_1 As Byte = 0
Private Const STAGE_2 As Byte = 1
Private Const STAGE_3 As Byte = 2
Private Const STAGE_4 As Byte = 3
Private Const STAGE_KAKUTEI As Byte = 4

Private StageNo As Byte

Private Sub cboScreenNo_Click()
    Select Case cboScreenNo.ListIndex
        Case SCR0_VERSION
            lblAnsyo.Visible = False
            txtAnsyoNum.Visible = False
            Call KeyMode_Non    '�{�^���͑S�Ė����ɂ���
            lblScreen.Caption = "�r�b�O�F�o�[�W�������"
            txtScreen(0).Text = "Ver"
            txtScreen(1).Text = ""
            txtScreen(2).Text = ""
            txtScreen(3).Text = ""
            txtScreen(4).Text = ""
            txtScreen(5).Text = ""
        Case SCR1_JYUNBI
            lblAnsyo.Visible = False
            txtAnsyoNum.Visible = False
            Call KeyMode_Non    '�{�^���͑S�Ė����ɂ���
            lblScreen.Caption = "�r�b�P�F�������"
            txtScreen(0).Text = "������"
            txtScreen(1).Text = ""
            txtScreen(2).Text = ""
            txtScreen(3).Text = ""
            txtScreen(4).Text = ""
            txtScreen(5).Text = ""
        Case SCR2_CARD_IN
            lblAnsyo.Visible = False
            txtAnsyoNum.Visible = False
            Call KeyMode_Non    '�{�^���͑S�Ė����ɂ���
            lblScreen.Caption = "�r�b�Q�F�J�[�h�h�m�n�j���"
            txtScreen(0).Text = "����J�[�h"
            txtScreen(1).Text = ""
            txtScreen(2).Text = "����Ă�������"
            txtScreen(3).Text = ""
            txtScreen(4).Text = ""
            txtScreen(5).Text = ""
        Case SCR3_CARD_NINSYO
            lblAnsyo.Visible = False
            txtAnsyoNum.Visible = False
            Call KeyMode_Non    '�{�^���͑S�Ė����ɂ���
            lblScreen.Caption = "�r�b�R�F�F�ؒ����"
            txtScreen(0).Text = "����J�[�h"
            txtScreen(1).Text = "�F�ؒ��I"
            txtScreen(2).Text = ""
            txtScreen(3).Text = ""
            txtScreen(4).Text = ""
            txtScreen(5).Text = ""
        Case SCR4_CARD_NG
            lblAnsyo.Visible = False
            txtAnsyoNum.Visible = False
            Call KeyMode_Non    '�{�^���͑S�Ė����ɂ���
            lblScreen.Caption = "�r�b�S�F�J�[�h�m�f���"
            txtScreen(0).Text = "����J�[�h"
            txtScreen(1).Text = ""
            txtScreen(2).Text = "�G���["
            txtScreen(3).Text = ""
            txtScreen(4).Text = ""
            txtScreen(5).Text = ""
        Case SCR5_ANSYO_NYURYOKU
            lblAnsyo.Visible = True
            txtAnsyoNum.Visible = True
            StageNo = STAGE_1   'STAGE_1�ɏ���������
            Call KeyMode_Num1   '�����̃{�^���̂ݗL���ɂ���
            lblScreen.Caption = "�r�b�T�F�Ïؓ��͉��"
            txtScreen(0).Text = "����J�[�h"
            txtScreen(1).Text = "�n�j"
            txtScreen(2).Text = "�Ïؓ���"
            txtScreen(3).Text = ""
            txtScreen(4).Text = ""
            txtScreen(5).Text = ""
        Case SCR6_ANSYO_TOROKU
            lblAnsyo.Visible = False
            txtAnsyoNum.Visible = False
            StageNo = STAGE_1   'STAGE_1�ɏ���������
            Call KeyMode_Num1   '�����̃{�^���̂ݗL���ɂ���
            lblScreen.Caption = "�r�b�U�F�Ïؓo�^���"
            txtScreen(0).Text = "����J�[�h"
            txtScreen(1).Text = "�n�j"
            txtScreen(2).Text = "�Ïؓo�^"
            txtScreen(3).Text = ""
            txtScreen(4).Text = ""
            txtScreen(5).Text = ""
        Case SCR7_TOROKU_NINSYO
            lblAnsyo.Visible = False
            txtAnsyoNum.Visible = False
            Call KeyMode_Non    '�{�^���͑S�Ė����ɂ���
            lblScreen.Caption = "�r�b�V�F�o�^�F�ؒ����"
            txtScreen(0).Text = "����J�[�h"
            txtScreen(1).Text = "�n�j"
            txtScreen(2).Text = "�Ïؓo�^"
            txtScreen(3).Text = "�F�ؒ��I"
            txtScreen(4).Text = ""
            txtScreen(5).Text = ""
        Case SCR8_TOROKU_NG
            lblAnsyo.Visible = False
            txtAnsyoNum.Visible = False
            Call KeyMode_Non    '�{�^���͑S�Ė����ɂ���
            lblScreen.Caption = "�r�b�W�F�o�^���s���"
            txtScreen(0).Text = "����o�^"
            txtScreen(1).Text = ""
            txtScreen(2).Text = "�G���["
            txtScreen(3).Text = ""
            txtScreen(4).Text = ""
            txtScreen(5).Text = ""
        Case SCR9_ANSYO_END
            lblAnsyo.Visible = False
            txtAnsyoNum.Visible = False
            Call KeyMode_Non    '�{�^���͑S�Ė����ɂ���
            lblScreen.Caption = "�r�b�X�F�����������"
            txtScreen(0).Text = "����J�[�h"
            txtScreen(1).Text = "�n�j"
            txtScreen(2).Text = "�Ïؓ���"
            txtScreen(3).Text = "�n�j"
            txtScreen(4).Text = ""
            txtScreen(5).Text = ""
        Case SCR10_CARD_OUT
            lblAnsyo.Visible = False
            txtAnsyoNum.Visible = False
            Call KeyMode_Non    '�{�^���͑S�Ė����ɂ���
            lblScreen.Caption = "�r�b�P�O�F�J�[�h�r�o���"
            txtScreen(0).Text = "����J�[�h"
            txtScreen(1).Text = ""
            txtScreen(2).Text = "����肭������"
            txtScreen(3).Text = ""
            txtScreen(4).Text = ""
            txtScreen(5).Text = ""
    End Select
End Sub

Private Sub cmdClose_Click()
    Call SetINIValue(CStr(Val(txtReset.Text) + 1), "RESET_COUNT", "SETTEI", CurDir & "\" & "emu.ini")
    Unload Me
End Sub

Private Sub cmdTouch_Click(Index As Integer)
    '�Ïؓ���
    If cboScreenNo.ListIndex = SCR5_ANSYO_NYURYOKU Then
        Select Case StageNo
            Case STAGE_1
                txtScreen(3) = txtScreen(3) & CStr(Index)   '�ǉ�����
                Call KeyMode_Num2   '�����{�^���ƒ����{�^����L���ɂ���
                StageNo = STAGE_2
            
            Case STAGE_2
                If Index = BUTTON_TEISEI Then
                    txtScreen(3) = Left(txtScreen(3), Len(txtScreen(3)) - 1)    '��������
                    If txtScreen(3) = "" Then
                        Call KeyMode_Num1   '�����̃{�^���̂ݗL���ɂ���
                        StageNo = STAGE_1
                    End If
                Else
                    txtScreen(3) = txtScreen(3) & CStr(Index)   '�ǉ�����
                    If Len(txtScreen(3)) = 4 Then
                        If txtScreen(3).Text = txtAnsyoNum.Text Then
                            txtScreen(0).Text = "����J�[�h"
                            txtScreen(1).Text = "�n�j"
                            txtScreen(2).Text = "�Ïؓ���"
                            txtScreen(3).Text = "�ʐM���I"
                            txtScreen(4).Text = ""
                            txtScreen(5).Text = ""
                            Call KeyMode_Non    '�{�^���͑S�Ė����ɂ���
                        Else
                            txtScreen(0).Text = "����J�[�h"
                            txtScreen(1).Text = "�n�j"
                            txtScreen(2).Text = "�Ïؓ���"
                            txtScreen(3).Text = "�m�f"
                            txtScreen(4).Text = "�ē���"
                            txtScreen(5).Text = ""
                            Call KeyMode_Num1   '�����̃{�^���̂ݗL���ɂ���
                            StageNo = STAGE_3
                        End If
                    End If
                End If
                
            Case STAGE_3
                txtScreen(5) = txtScreen(5) & CStr(Index)   '�ǉ�����
                Call KeyMode_Num2   '�����{�^���ƒ����{�^����L���ɂ���
                StageNo = STAGE_4
        
            Case STAGE_4
                If Index = BUTTON_TEISEI Then
                    txtScreen(5) = Left(txtScreen(5), Len(txtScreen(5)) - 1)    '��������
                    If txtScreen(5) = "" Then
                        Call KeyMode_Num1   '�����̃{�^���̂ݗL���ɂ���
                        StageNo = STAGE_3
                    End If
                Else
                    txtScreen(5) = txtScreen(5) & CStr(Index)   '�ǉ�����
                    If Len(txtScreen(5)) = 4 Then
                        If txtScreen(5).Text = txtAnsyoNum.Text Then
                            txtScreen(0).Text = "����J�[�h"
                            txtScreen(1).Text = "�n�j"
                            txtScreen(2).Text = "�Ïؓ���"
                            txtScreen(3).Text = "�ʐM���I"
                            txtScreen(4).Text = ""
                            txtScreen(5).Text = ""
                            Call KeyMode_Non    '�{�^���͑S�Ė����ɂ���
                        Else
                            txtScreen(0).Text = "����J�[�h"
                            txtScreen(1).Text = "�n�j"
                            txtScreen(2).Text = "�Ïؓ���"
                            txtScreen(3).Text = "�m�f"
                            txtScreen(4).Text = "�ē���"
                            txtScreen(5).Text = ""
                            Call KeyMode_Num1   '�����̃{�^���̂ݗL���ɂ���
                            StageNo = STAGE_3
                        End If
                    End If
                End If
        End Select
    End If


    '�Ïؓo�^
    If cboScreenNo.ListIndex = SCR6_ANSYO_TOROKU Then
        Select Case StageNo
            Case STAGE_1
                txtScreen(3) = txtScreen(3) & CStr(Index)   '�ǉ�����
                Call KeyMode_Num2   '�����{�^���ƒ����{�^����L���ɂ���
                StageNo = STAGE_2
                
            Case STAGE_2
                If Index = BUTTON_TEISEI Then
                    txtScreen(3) = Left(txtScreen(3), Len(txtScreen(3)) - 1)    '��������
                    If txtScreen(3) = "" Then
                        Call KeyMode_Num1   '�����̃{�^���̂ݗL���ɂ���
                        StageNo = STAGE_1
                    End If
                Else
                    txtScreen(3) = txtScreen(3) & CStr(Index)   '�ǉ�����
                    If Len(txtScreen(3)) = 4 Then
                        If txtScreen(3).Text = "0000" Then
                            txtScreen(0).Text = "�Ïؔԍ�"
                            txtScreen(1).Text = ""
                            txtScreen(2).Text = "�G���["
                            txtScreen(3).Text = ""
                            txtScreen(4).Text = ""
                            txtScreen(5).Text = ""
                            Call KeyMode_Kakutei    '�m��{�^���̂ݗL���ɂ���
                            StageNo = STAGE_KAKUTEI
                        Else
                            txtScreen(4).Text = "�Ï؊m�F"
                            Call KeyMode_Num1   '�����̃{�^���̂ݗL���ɂ���
                            StageNo = STAGE_3
                        End If
                    End If
                End If
            
            Case STAGE_3
                txtScreen(5) = txtScreen(5) & CStr(Index)   '�ǉ�����
                Call KeyMode_Num2   '�����{�^���ƒ����{�^����L���ɂ���
                StageNo = STAGE_4
        
            Case STAGE_4
                If Index = BUTTON_TEISEI Then
                    txtScreen(5) = Left(txtScreen(5), Len(txtScreen(5)) - 1)    '��������
                    If txtScreen(5) = "" Then
                        Call KeyMode_Num1   '�����̃{�^���̂ݗL���ɂ���
                        StageNo = STAGE_3
                    End If
                Else
                    txtScreen(5) = txtScreen(5) & CStr(Index)   '�ǉ�����
                    If Len(txtScreen(5)) = 4 Then
                        If txtScreen(3).Text = txtScreen(5).Text Then
                            txtScreen(0).Text = "����J�[�h"
                            txtScreen(1).Text = "�n�j"
                            txtScreen(2).Text = "�Ïؓo�^"
                            txtScreen(3).Text = "�ʐM���I"
                            txtScreen(4).Text = ""
                            txtScreen(5).Text = ""
                            Call KeyMode_Non    '�{�^���͑S�Ė����ɂ���
                        Else
                            txtScreen(0).Text = "�Ïؔԍ�"
                            txtScreen(1).Text = ""
                            txtScreen(2).Text = "�G���["
                            txtScreen(3).Text = ""
                            txtScreen(4).Text = ""
                            txtScreen(5).Text = ""
                            Call KeyMode_Kakutei    '�m��{�^���̂ݗL���ɂ���
                            StageNo = STAGE_KAKUTEI
                        End If
                    End If
                End If
        
            Case STAGE_KAKUTEI
                StageNo = STAGE_1   'STAGE_1�ɏ���������
                Call KeyMode_Num1   '�����̃{�^���̂ݗL���ɂ���
                lblScreen.Caption = "�r�b�U�F�Ïؓo�^���"
                txtScreen(0).Text = "����J�[�h"
                txtScreen(1).Text = "�n�j"
                txtScreen(2).Text = "�Ïؓo�^"
                txtScreen(3).Text = ""
                txtScreen(4).Text = ""
                txtScreen(5).Text = ""
        End Select
    End If
End Sub

Private Sub Form_Load()
    cboScreenNo.ListIndex = SCR0_VERSION
    txtReset.Text = Val(GetINIValue("RESET_COUNT", "SETTEI", CurDir & "\" & "emu.ini"))
End Sub

Private Function KeyMode_Non()
    cmdTouch(0).Enabled = False
    cmdTouch(1).Enabled = False
    cmdTouch(2).Enabled = False
    cmdTouch(3).Enabled = False
    cmdTouch(4).Enabled = False
    cmdTouch(5).Enabled = False
    cmdTouch(6).Enabled = False
    cmdTouch(7).Enabled = False
    cmdTouch(8).Enabled = False
    cmdTouch(9).Enabled = False
    cmdTouch(BUTTON_TEISEI).Enabled = False
    cmdTouch(BUTTON_KAKUTEI).Enabled = False
End Function

Private Function KeyMode_Num1()
    cmdTouch(0).Enabled = True
    cmdTouch(1).Enabled = True
    cmdTouch(2).Enabled = True
    cmdTouch(3).Enabled = True
    cmdTouch(4).Enabled = True
    cmdTouch(5).Enabled = True
    cmdTouch(6).Enabled = True
    cmdTouch(7).Enabled = True
    cmdTouch(8).Enabled = True
    cmdTouch(9).Enabled = True
    cmdTouch(BUTTON_TEISEI).Enabled = False
    cmdTouch(BUTTON_KAKUTEI).Enabled = False
End Function

Private Function KeyMode_Num2()
    cmdTouch(0).Enabled = True
    cmdTouch(1).Enabled = True
    cmdTouch(2).Enabled = True
    cmdTouch(3).Enabled = True
    cmdTouch(4).Enabled = True
    cmdTouch(5).Enabled = True
    cmdTouch(6).Enabled = True
    cmdTouch(7).Enabled = True
    cmdTouch(8).Enabled = True
    cmdTouch(9).Enabled = True
    cmdTouch(BUTTON_TEISEI).Enabled = True
    cmdTouch(BUTTON_KAKUTEI).Enabled = False
End Function

Private Function KeyMode_Kakutei()
    cmdTouch(0).Enabled = False
    cmdTouch(1).Enabled = False
    cmdTouch(2).Enabled = False
    cmdTouch(3).Enabled = False
    cmdTouch(4).Enabled = False
    cmdTouch(5).Enabled = False
    cmdTouch(6).Enabled = False
    cmdTouch(7).Enabled = False
    cmdTouch(8).Enabled = False
    cmdTouch(9).Enabled = False
    cmdTouch(BUTTON_TEISEI).Enabled = False
    cmdTouch(BUTTON_KAKUTEI).Enabled = True
End Function


