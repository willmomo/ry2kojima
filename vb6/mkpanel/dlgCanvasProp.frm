VERSION 5.00
Begin VB.Form dlgCanvasProp 
   BorderStyle     =   3  'å≈íË¿ﬁ≤±€∏ﬁ
   Caption         =   "¿ﬁ≤±€∏ﬁ ∑¨ÃﬂºÆ›"
   ClientHeight    =   3195
   ClientLeft      =   2760
   ClientTop       =   3750
   ClientWidth     =   6030
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   3195
   ScaleWidth      =   6030
   ShowInTaskbar   =   0   'False
   StartUpPosition =   1  'µ∞≈∞ Ã´∞—ÇÃíÜâõ
   Begin VB.TextBox Text2 
      Height          =   270
      Left            =   960
      TabIndex        =   3
      Top             =   720
      Width           =   2775
   End
   Begin VB.TextBox Text1 
      Height          =   270
      Left            =   960
      TabIndex        =   1
      Top             =   240
      Width           =   2775
   End
   Begin VB.CommandButton CancelButton 
      Cancel          =   -1  'True
      Caption         =   "∑¨›æŸ"
      Height          =   375
      Left            =   4680
      TabIndex        =   5
      Top             =   2640
      Width           =   1215
   End
   Begin VB.CommandButton OKButton 
      Caption         =   "OK"
      Default         =   -1  'True
      Height          =   375
      Left            =   3240
      TabIndex        =   4
      Top             =   2640
      Width           =   1215
   End
   Begin VB.Label Label2 
      Caption         =   "ê}ñ èc"
      Height          =   255
      Left            =   120
      TabIndex        =   2
      Top             =   720
      Width           =   615
   End
   Begin VB.Label Label1 
      Caption         =   "ê}ñ â°"
      Height          =   255
      Left            =   120
      TabIndex        =   0
      Top             =   240
      Width           =   615
   End
End
Attribute VB_Name = "dlgCanvasProp"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False

Option Explicit

Private m_canvasWidth As Long
Private m_canvasHeight As Long
Private m_result As VBA.VbMsgBoxResult

Private Sub InitializeComponents()
    m_result = vbCancel
    
    Text1.Text = CStr(m_canvasWidth)
    Text2.Text = CStr(m_canvasHeight)
End Sub

Public Property Get Result() As VBA.VbMsgBoxResult
    Result = m_result
End Property

Private Sub CancelButton_Click()
    m_result = vbCancel
    
    Unload Me
End Sub

Private Sub Form_Load()
    InitializeComponents
End Sub

Private Sub OKButton_Click()
    m_canvasWidth = CLng(Text1.Text)
    m_canvasHeight = CLng(Text2.Text)
    
    m_result = vbOK
    
    Unload Me
End Sub

Public Property Get CanvasWidth() As Variant
    CanvasWidth = m_canvasWidth
End Property

Public Property Let CanvasWidth(ByVal vNewValue As Variant)
    m_canvasWidth = vNewValue
End Property

Public Property Get CanvasHeight() As Variant
    CanvasHeight = m_canvasHeight
End Property

Public Property Let CanvasHeight(ByVal vNewValue As Variant)
    m_canvasHeight = vNewValue
End Property

Private Sub Text1_GotFocus()
    With Text1
        .SelStart = 0
        .SelLength = Len(.Text)
    End With
End Sub

Private Sub Text2_GotFocus()
    With Text2
        .SelStart = 0
        .SelLength = Len(.Text)
    End With
End Sub
