VERSION 5.00
Begin VB.MDIForm mdiMain 
   BackColor       =   &H8000000C&
   ClientHeight    =   3090
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   4680
   LinkTopic       =   "MDIForm1"
   StartUpPosition =   3  'Windows �̊���l
End
Attribute VB_Name = "mdiMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Sub MDIForm_Load()

    Dim n As Long
    
    '�L���v�V�����ƃV�X�e�����j���[������
    hs_SetCaption Me.hwnd, False
    hs_SetSysMenu Me.hwnd, False
    
    '�^�X�N�o�[�ɕ\�������Ȃ�
    hs_SetExAppWindow Me.hwnd, False
    
End Sub
