VERSION 5.00
Object = "{0A45DB48-BD0D-11D2-8D14-00104B9E072A}#2.0#0"; "Sstabs2.ocx"
Object = "{86CF1D34-0C5F-11D2-A9FC-0000F8754DA1}#2.0#0"; "MSCOMCT2.OCX"
Begin VB.Form dlgShapeProperty 
   BorderStyle     =   3  '固定ﾀﾞｲｱﾛｸﾞ
   Caption         =   "ﾀﾞｲｱﾛｸﾞ ｷｬﾌﾟｼｮﾝ"
   ClientHeight    =   6045
   ClientLeft      =   2760
   ClientTop       =   3750
   ClientWidth     =   7485
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   6045
   ScaleWidth      =   7485
   ShowInTaskbar   =   0   'False
   StartUpPosition =   1  'ｵｰﾅｰ ﾌｫｰﾑの中央
   Begin ActiveTabs.SSActiveTabs SSActiveTabs1 
      Height          =   5355
      Left            =   60
      TabIndex        =   4
      Top             =   60
      Width           =   7335
      _ExtentX        =   12938
      _ExtentY        =   9446
      _Version        =   131082
      TabCount        =   1
      Tabs            =   "dlgShapeProperty.frx":0000
      Begin ActiveTabs.SSActiveTabPanel SSActiveTabPanel1 
         Height          =   4965
         Left            =   30
         TabIndex        =   5
         Top             =   360
         Width           =   7275
         _ExtentX        =   12832
         _ExtentY        =   8758
         _Version        =   131082
         TabGuid         =   "dlgShapeProperty.frx":0043
         Begin MSComCtl2.UpDown UpDown1 
            Height          =   375
            Left            =   2041
            TabIndex        =   8
            Top             =   960
            Width           =   255
            _ExtentX        =   450
            _ExtentY        =   661
            _Version        =   393216
            AutoBuddy       =   -1  'True
            BuddyControl    =   "Text2"
            BuddyDispid     =   196616
            OrigLeft        =   2400
            OrigTop         =   960
            OrigRight       =   2655
            OrigBottom      =   1455
            Max             =   180
            Min             =   -180
            SyncBuddy       =   -1  'True
            BuddyProperty   =   65547
            Enabled         =   -1  'True
         End
         Begin VB.TextBox Text2 
            Height          =   375
            Left            =   1200
            TabIndex        =   7
            Text            =   "0"
            Top             =   960
            Width           =   840
         End
         Begin VB.TextBox Text1 
            Height          =   375
            Left            =   1200
            TabIndex        =   1
            Top             =   240
            Width           =   5655
         End
         Begin VB.Label Label2 
            Caption         =   "回転"
            Height          =   255
            Left            =   240
            TabIndex        =   6
            Top             =   960
            Width           =   855
         End
         Begin VB.Label Label1 
            Caption         =   "テキスト"
            Height          =   255
            Left            =   240
            TabIndex        =   0
            Top             =   360
            Width           =   735
         End
      End
   End
   Begin VB.CommandButton CancelButton 
      Cancel          =   -1  'True
      Caption         =   "ｷｬﾝｾﾙ"
      Height          =   375
      Left            =   6120
      TabIndex        =   3
      Top             =   5520
      Width           =   1215
   End
   Begin VB.CommandButton OKButton 
      Caption         =   "OK"
      Default         =   -1  'True
      Height          =   375
      Left            =   4740
      TabIndex        =   2
      Top             =   5520
      Width           =   1215
   End
End
Attribute VB_Name = "dlgShapeProperty"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False

Option Explicit

Private m_targetShape As clsAnyShape
Private m_result As VBA.VbMsgBoxResult

Public Property Get Result() As VBA.VbMsgBoxResult
    Result = m_result
End Property

Public Property Set TargetShape(Value)
    Set m_targetShape = Value
End Property

Private Sub InitializeComponent()
    Text1.Text = m_targetShape.ShapeText
    
    Text2.Text = m_targetShape.Rotate
End Sub

Private Sub CancelButton_Click()
    m_result = vbCancel
    
    Unload Me
End Sub

Private Sub Form_Load()
    m_result = vbCancel
    
    InitializeComponent
End Sub

Private Sub Form_Unload(Cancel As Integer)

    Set m_targetShape = Nothing
    
End Sub

Private Sub OKButton_Click()
    m_targetShape.ShapeText = Text1.Text
    m_targetShape.Rotate = CLng(Text2.Text)
    
    m_result = vbOK
    Unload Me
End Sub

Private Sub Text1_GotFocus()
    With Text1
        .SelStart = 0
        .SelLength = Len(.Text)
    End With
End Sub
