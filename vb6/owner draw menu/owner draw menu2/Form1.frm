VERSION 5.00
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "MSCOMCTL.OCX"
Begin VB.Form Form1 
   Caption         =   "Owner Draw - [ Menu ]"
   ClientHeight    =   3495
   ClientLeft      =   165
   ClientTop       =   735
   ClientWidth     =   4680
   LinkTopic       =   "Form1"
   ScaleHeight     =   233
   ScaleMode       =   3  'ﾋﾟｸｾﾙ
   ScaleWidth      =   312
   StartUpPosition =   3  'Windows の既定値
   Begin MSComctlLib.ImageList ImageList1 
      Left            =   1920
      Top             =   1920
      _ExtentX        =   1005
      _ExtentY        =   1005
      BackColor       =   -2147483643
      ImageWidth      =   16
      ImageHeight     =   16
      MaskColor       =   12632256
      _Version        =   393216
      BeginProperty Images {2C247F25-8591-11D1-B16A-00C0F0283628} 
         NumListImages   =   17
         BeginProperty ListImage1 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "Form1.frx":0000
            Key             =   "ファイル(&F)"
         EndProperty
         BeginProperty ListImage2 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "Form1.frx":0454
            Key             =   "表示(&V)"
         EndProperty
         BeginProperty ListImage3 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "Form1.frx":0568
            Key             =   "書式(&O)"
         EndProperty
         BeginProperty ListImage4 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "Form1.frx":067C
            Key             =   "新規作成(&C)"
         EndProperty
         BeginProperty ListImage5 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "Form1.frx":0790
            Key             =   "開く(&O)"
         EndProperty
         BeginProperty ListImage6 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "Form1.frx":08A4
            Key             =   "保存(&S)"
         EndProperty
         BeginProperty ListImage7 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "Form1.frx":09B8
            Key             =   "印刷(&P)"
         EndProperty
         BeginProperty ListImage8 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "Form1.frx":0ACC
            Key             =   "プレビュー(&V)"
         EndProperty
         BeginProperty ListImage9 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "Form1.frx":1010
            Key             =   "大きいアイコン(&G)"
         EndProperty
         BeginProperty ListImage10 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "Form1.frx":1124
            Key             =   "小さいアイコン(&M)"
         EndProperty
         BeginProperty ListImage11 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "Form1.frx":1238
            Key             =   "一覧(&L)"
         EndProperty
         BeginProperty ListImage12 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "Form1.frx":134C
            Key             =   "詳細(&V)"
         EndProperty
         BeginProperty ListImage13 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "Form1.frx":1460
            Key             =   "左(&L)"
         EndProperty
         BeginProperty ListImage14 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "Form1.frx":1574
            Key             =   "中央(&C)"
         EndProperty
         BeginProperty ListImage15 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "Form1.frx":1688
            Key             =   "右(&R)"
         EndProperty
         BeginProperty ListImage16 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "Form1.frx":179C
            Key             =   "最前面に移動(&B)"
         EndProperty
         BeginProperty ListImage17 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "Form1.frx":18B0
            Key             =   "最背面に移動(&S)"
         EndProperty
      EndProperty
   End
   Begin VB.Menu mnuFile 
      Caption         =   "ファイル(&F)"
      Begin VB.Menu mnuFile_Create 
         Caption         =   "新規作成(&C)"
      End
      Begin VB.Menu mnuFile_Open 
         Caption         =   "開く(&O)"
         Shortcut        =   ^O
      End
      Begin VB.Menu mnuFile_Save 
         Caption         =   "保存(&S)"
         Shortcut        =   ^S
      End
      Begin VB.Menu mnuFile_Line1 
         Caption         =   "-"
      End
      Begin VB.Menu mnuFile_Print 
         Caption         =   "印刷(&P)"
         Shortcut        =   ^L
      End
      Begin VB.Menu mnuFile_Preview 
         Caption         =   "プレビュー(&V)"
      End
      Begin VB.Menu mnuFile_Line2 
         Caption         =   "-"
      End
      Begin VB.Menu mnuFile_End 
         Caption         =   "終了(&X)"
      End
   End
   Begin VB.Menu mnuDisp 
      Caption         =   "表示(&V)"
      Begin VB.Menu mnuDisp_View 
         Caption         =   "大きいアイコン(&G)"
         Checked         =   -1  'True
         Index           =   0
      End
      Begin VB.Menu mnuDisp_View 
         Caption         =   "小さいアイコン(&M)"
         Index           =   1
      End
      Begin VB.Menu mnuDisp_View 
         Caption         =   "一覧(&L)"
         Index           =   2
      End
      Begin VB.Menu mnuDisp_View 
         Caption         =   "詳細(&V)"
         Index           =   3
      End
   End
   Begin VB.Menu mnuFormat 
      Caption         =   "書式(&O)"
      Begin VB.Menu mnuFormat_Stand 
         Caption         =   "整列(&A)"
         Begin VB.Menu mnuFormat_Stand_Left 
            Caption         =   "左(&L)"
         End
         Begin VB.Menu mnuFormat_Stand_Center 
            Caption         =   "中央(&C)"
         End
         Begin VB.Menu mnuFormat_Stand_Right 
            Caption         =   "右(&R)"
         End
      End
      Begin VB.Menu mnuFormat_Order 
         Caption         =   "順序(&O)"
         Begin VB.Menu mnuFormat_Order_Befor 
            Caption         =   "最前面に移動(&B)"
            Shortcut        =   ^J
         End
         Begin VB.Menu mnuFormat_Order_After 
            Caption         =   "最背面に移動(&S)"
            Shortcut        =   ^K
         End
      End
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Sub Form_Load()
    ' メニューのオーナードロー設定
    Call SetMenuOwnerDraw(GetMenu(Me.hwnd))
    ' サブクラス化の開始
    Call SubClass(Me.hwnd)
End Sub

Private Sub Form_QueryUnload(Cancel As Integer, UnloadMode As Integer)
    ' サブクラス化の終了
    Call UnSubClass
End Sub

Private Sub mnuDisp_View_Click(Index As Integer)
    mnuDisp_View(0).Checked = False
    mnuDisp_View(1).Checked = False
    mnuDisp_View(2).Checked = False
    mnuDisp_View(3).Checked = False
    mnuDisp_View(Index).Checked = True
End Sub

