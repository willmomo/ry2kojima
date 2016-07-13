VERSION 5.00
Object = "{0A45DB48-BD0D-11D2-8D14-00104B9E072A}#2.0#0"; "Sstabs2.ocx"
Object = "{F13C99C0-4D73-11D2-B8B2-0000C00A958C}#7.0#0"; "fpSpr70.ocx"
Object = "{065E6FD1-1BF9-11D2-BAE8-00104B9E0792}#3.0#0"; "ssa3d30.ocx"
Begin VB.Form dlgDaiProperty 
   BorderStyle     =   3  'ŒÅ’èÀÞ²±Û¸Þ
   Caption         =   "ÀÞ²±Û¸Þ ·¬Ìß¼®Ý"
   ClientHeight    =   7005
   ClientLeft      =   2760
   ClientTop       =   3750
   ClientWidth     =   6150
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   7005
   ScaleWidth      =   6150
   ShowInTaskbar   =   0   'False
   StartUpPosition =   1  'µ°Å° Ì«°Ñ‚Ì’†‰›
   Begin ActiveTabs.SSActiveTabs SSActiveTabs1 
      Height          =   6315
      Left            =   60
      TabIndex        =   12
      Top             =   60
      Width           =   5895
      _ExtentX        =   10398
      _ExtentY        =   11139
      _Version        =   131082
      TabCount        =   1
      Tabs            =   "dlgDaiProperty.frx":0000
      Begin ActiveTabs.SSActiveTabPanel SSActiveTabPanel1 
         Height          =   5925
         Left            =   30
         TabIndex        =   13
         Top             =   360
         Width           =   5835
         _ExtentX        =   10292
         _ExtentY        =   10451
         _Version        =   131082
         TabGuid         =   "dlgDaiProperty.frx":0049
         Begin Threed.SSCommand SSCommand2 
            Height          =   435
            Left            =   120
            TabIndex        =   4
            Top             =   5280
            Width           =   2595
            _ExtentX        =   4577
            _ExtentY        =   767
            _Version        =   196610
            Caption         =   "49”²‚«‚ÅŽ©“®“ü—Í"
         End
         Begin Threed.SSCommand SSCommand1 
            Height          =   435
            Left            =   120
            TabIndex        =   3
            Top             =   4800
            Width           =   2595
            _ExtentX        =   4577
            _ExtentY        =   767
            _Version        =   196610
            Caption         =   "˜A”Ô‚ÅŽ©“®“ü—Í"
         End
         Begin Threed.SSFrame SSFrame1 
            Height          =   2415
            Left            =   3000
            TabIndex        =   5
            Top             =   240
            Width           =   2595
            _ExtentX        =   4577
            _ExtentY        =   4260
            _Version        =   196610
            Caption         =   "•À‚Ñ"
            Begin Threed.SSOption SSOption1 
               Height          =   315
               Index           =   0
               Left            =   240
               TabIndex        =   6
               Top             =   480
               Width           =   2055
               _ExtentX        =   3625
               _ExtentY        =   556
               _Version        =   196610
               Caption         =   "ã‚©‚ç‰º [«]"
            End
            Begin Threed.SSOption SSOption1 
               Height          =   315
               Index           =   1
               Left            =   240
               TabIndex        =   7
               Top             =   900
               Width           =   2055
               _ExtentX        =   3625
               _ExtentY        =   556
               _Version        =   196610
               Caption         =   "‰º‚©‚çã [ª]"
            End
            Begin Threed.SSOption SSOption1 
               Height          =   315
               Index           =   2
               Left            =   240
               TabIndex        =   8
               Top             =   1380
               Width           =   2055
               _ExtentX        =   3625
               _ExtentY        =   556
               _Version        =   196610
               Caption         =   "¶‚©‚ç‰E [¨]"
            End
            Begin Threed.SSOption SSOption1 
               Height          =   315
               Index           =   3
               Left            =   240
               TabIndex        =   9
               Top             =   1860
               Width           =   2055
               _ExtentX        =   3625
               _ExtentY        =   556
               _Version        =   196610
               Caption         =   "‰E‚©‚ç¶ [©]"
            End
         End
         Begin VB.TextBox Text1 
            Height          =   270
            Left            =   840
            TabIndex        =   1
            Top             =   240
            Width           =   1875
         End
         Begin FPSpreadADO.fpSpread fpSpread1 
            Height          =   4095
            Left            =   120
            TabIndex        =   2
            Top             =   600
            Width           =   2655
            _Version        =   458752
            _ExtentX        =   4683
            _ExtentY        =   7223
            _StockProps     =   64
            MaxCols         =   1
            MaxRows         =   0
            SpreadDesigner  =   "dlgDaiProperty.frx":0071
         End
         Begin VB.Label Label1 
            Caption         =   "‘ä”(&N)"
            Height          =   255
            Left            =   180
            TabIndex        =   0
            Top             =   240
            Width           =   735
         End
      End
   End
   Begin VB.CommandButton CancelButton 
      Cancel          =   -1  'True
      Caption         =   "·¬Ý¾Ù"
      Height          =   375
      Left            =   4500
      TabIndex        =   11
      Top             =   6480
      Width           =   1215
   End
   Begin VB.CommandButton OKButton 
      Caption         =   "OK"
      Default         =   -1  'True
      Height          =   375
      Left            =   3180
      TabIndex        =   10
      Top             =   6480
      Width           =   1215
   End
End
Attribute VB_Name = "dlgDaiProperty"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False

Option Explicit

Private m_result As VBA.VbMsgBoxResult
Private m_targetShape As clsAnyShape

Public Property Get Result() As VBA.VbMsgBoxResult
    Result = m_result
End Property

Private Function NumRenToNum49Nuki(ByVal numRen As Long) As Long

    Dim strOct As String
    strOct = Oct(numRen)
    
    Dim i As Long
    Dim c As String
    Dim strTemp As String
    For i = 0 To Len(strOct) - 1
        c = Mid(strOct, i + 1, 1)
        If CLng(c) >= 4 Then
            strTemp = strTemp & CStr(CLng(c) + 1)
        Else
            strTemp = strTemp & c
        End If
    Next
    
    NumRenToNum49Nuki = CLng(strTemp)
End Function

Private Function Num49NukiToNumRen(ByVal num49Nuki As Long) As Long

    Dim str49Nuki As String
    str49Nuki = CStr(num49Nuki)
    
    Dim i As Long
    Dim c As String
    Dim strTemp As String
    For i = 0 To Len(str49Nuki) - 1
        c = Mid(str49Nuki, i + 1, 1)
        If CLng(c) > 3 Then
            strTemp = strTemp & CStr(CLng(c) - 1)
        Else
            strTemp = strTemp & c
        End If
    Next
    
    Num49NukiToNumRen = CLng("&O" & strTemp)
End Function

Public Property Set TargetShape(Value)
    Set m_targetShape = Value
End Property

Private Sub InitializeComponent()
    m_result = vbCancel
    
    Text1.Text = m_targetShape.DaiCount
    
    Dim i As Long
    For i = 0 To m_targetShape.DaiCount - 1
        fpSpread1.SetText 1, i + 1, m_targetShape.DaiNumber(i)
    Next
    
    fpSpread1.EditModeReplace = True
    
    SSOption1(m_targetShape.DaiDirection).Value = True
        
End Sub

Private Sub CancelButton_Click()
    m_result = vbCancel
    
    Unload Me
End Sub

Private Sub Form_Load()
    
    InitializeComponent
    
End Sub

Private Sub Form_Unload(Cancel As Integer)

    Set m_targetShape = Nothing
    
End Sub

Private Sub OKButton_Click()

    m_targetShape.DaiCount = CLng(Text1.Text)
    
    Dim i As Long
    For i = 0 To m_targetShape.DaiCount - 1
        Dim tmp As Variant
        fpSpread1.GetText 1, i + 1, tmp
        If IsNumeric(tmp) Then
            m_targetShape.DaiNumber(i) = CLng(tmp)
        End If
    Next
    
    For i = 0 To SSOption1.UBound
        If SSOption1(i).Value Then
            m_targetShape.DaiDirection = i
            Exit For
        End If
    Next
    
    m_result = vbOK
    
    Unload Me
    
End Sub

Private Sub SSCommand1_Click()

    Dim topValue
    fpSpread1.GetText 1, 1, topValue
    
    Dim i As Long
    For i = 2 To fpSpread1.MaxRows
        fpSpread1.SetText 1, i, topValue + (i - 1)
    Next
    
End Sub

Private Sub SSCommand2_Click()

    Dim topValue
    fpSpread1.GetText 1, 1, topValue
    
    Dim i As Long
    For i = 2 To fpSpread1.MaxRows
        fpSpread1.SetText 1, i, NumRenToNum49Nuki(Num49NukiToNumRen(topValue) + (i - 1))
    Next
End Sub

Private Sub Text1_Change()

    If IsNumeric(Text1.Text) Then
        Dim numDai As Long
        
        numDai = CLng(Text1.Text)
        
        If numDai > 0 Then
            fpSpread1.MaxRows = numDai
        End If
        
    End If
    
End Sub

Private Sub Text1_GotFocus()
    With Text1
        .SelStart = 0
        .SelLength = Len(.Text)
    End With
End Sub
