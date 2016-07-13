VERSION 5.00
Begin VB.Form dlgPrinting 
   BorderStyle     =   3  '�Œ��޲�۸�
   Caption         =   "�����"
   ClientHeight    =   1755
   ClientLeft      =   2760
   ClientTop       =   3750
   ClientWidth     =   3915
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   1755
   ScaleWidth      =   3915
   ShowInTaskbar   =   0   'False
   Begin VB.CommandButton cmdAbort 
      Caption         =   "������~"
      Height          =   495
      Left            =   1350
      TabIndex        =   0
      Top             =   1080
      Width           =   1215
   End
   Begin VB.Label lblMsg 
      Alignment       =   2  '��������
      Height          =   255
      Left            =   150
      TabIndex        =   1
      Top             =   480
      Width           =   3615
   End
End
Attribute VB_Name = "dlgPrinting"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'----------------------------------------------------------------------
' ������f�t�H�[�����������
'----------------------------------------------------------------------
Option Explicit

Private m_abort As Boolean

'----------------------------------------------------------------------
' ����\�g��`��
'
' �f�o�b�O�ŕ`��͈͂��m�F���邽�߂̊֐�
'----------------------------------------------------------------------
Private Sub DrawPrintableRect(prn As Printer)
    ''prn.Line (0, 0)-(prn.ScaleWidth - prn.TwipsPerPixelX, prn.ScaleHeight - prn.TwipsPerPixelY), RGB(0, 0, 0), B
End Sub

'----------------------------------------------------------------------
' vbNewLine�ŋ�؂�ꂽ��������v�����^�ɏo�͂���B
'
' @param strText ������镶����(�s��vbNewLine�ŋ�؂�)
'
' @return True  �������������
' @return False ��������f���ꂽ
'----------------------------------------------------------------------
Public Function PrintString(ByVal strText As String, Optional owner As Form = Nothing) As Boolean
    On Error GoTo ErrHandler
    
    m_abort = False
    
    'owner���w�肳��Ă���Ƃ��́A����_�C�A���O�\��
    If Not (owner Is Nothing) Then
        owner.Enabled = False
        Me.Show vbModeless, owner
        DoEvents
    End If
    
    '�������vbNewLine�ŕ�������
    Dim lines() As String
    lines = Split(strText, vbNewLine)
    
    '�ő�y�[�W�����v�Z����
    Dim maxPage As Long
    Dim maxLineInPage As Long    '1�y�[�W�Ɏ��܂�s��
    maxLineInPage = Printer.ScaleHeight \ Printer.TextHeight(lines(0))
    maxPage = (UBound(lines) + 1) \ maxLineInPage + IIf((UBound(lines) + 1) Mod maxLineInPage, 1, 0)
    
    '�s�̐������A������J��Ԃ��B
    Dim l As Long
    For l = LBound(lines) To UBound(lines)
    
        '�o�͍s���y�[�W�Ɏ��܂�Ȃ��Ƃ��́A���y�[�W���s���B
        If Printer.CurrentY + Printer.TextHeight(lines(l)) > Printer.ScaleHeight Then
            DrawPrintableRect Printer
            Printer.NewPage
        End If
        
        '�_�C�A���O�\�����́A��������b�Z�[�W�X�V
        If Not (owner Is Nothing) Then
            lblMsg.Caption = CStr(Printer.Page) & " / " & CStr(maxPage) & " �y�[�W�A������E�E�E"
        End If
        
        '���
        Printer.Print lines(l)
        
        '[������~]�{�^�����Ď�
        '������~�{�^���������ꂽ�Ƃ��́A�v�����^�G���[�𔭐�������B
        '(�v�����^�X�v�[���Œ��f�����Ƃ��̓�������ɂȂ�)
        DoEvents
        If m_abort Then
            Err.Raise 482
        End If
    Next
    
    '�������
    DrawPrintableRect Printer
    Printer.EndDoc
    
    '������_�C�A���O�������B
    If Not (owner Is Nothing) Then
        owner.Enabled = True
        Unload Me
    End If
    
    PrintString = True
    Exit Function
    
ErrHandler:
    '�G���[�������́A����𒆒f�B
    '�v�����^�X�v�[���ň���𒆒f�����Ƃ����A����������B
    Printer.KillDoc
    If Not (owner Is Nothing) Then
        owner.Enabled = True
        Unload Me
    End If
    
    PrintString = False
End Function

'----------------------------------------------------------------------
' ������f�{�^���N���b�N
'----------------------------------------------------------------------
Private Sub cmdAbort_Click()
    m_abort = True
End Sub
