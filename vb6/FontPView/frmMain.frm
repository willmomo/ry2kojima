VERSION 5.00
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "comdlg32.ocx"
Begin VB.Form frmMain 
   BorderStyle     =   1  '固定(実線)
   Caption         =   "パネルフォント プレビュー"
   ClientHeight    =   5130
   ClientLeft      =   45
   ClientTop       =   435
   ClientWidth     =   7275
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   5130
   ScaleWidth      =   7275
   StartUpPosition =   3  'Windows の既定値
   Begin VB.CheckBox chkStrikeOut 
      Caption         =   "StrikeOut"
      Height          =   195
      Left            =   2580
      TabIndex        =   27
      Top             =   2580
      Width           =   1095
   End
   Begin VB.CheckBox chkUnderline 
      Caption         =   "Underline"
      Height          =   195
      Left            =   1380
      TabIndex        =   26
      Top             =   2580
      Width           =   1095
   End
   Begin VB.CheckBox chkItalic 
      Caption         =   "Italic"
      Height          =   195
      Left            =   180
      TabIndex        =   25
      Top             =   2580
      Width           =   1095
   End
   Begin VB.CommandButton cmdFont 
      Caption         =   "フォント選択(&F)"
      Height          =   375
      Left            =   2340
      TabIndex        =   24
      Top             =   180
      Width           =   1575
   End
   Begin MSComDlg.CommonDialog dlgFont 
      Left            =   3660
      Top             =   720
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
   End
   Begin VB.TextBox txtFaceName 
      Height          =   315
      Left            =   1500
      TabIndex        =   22
      Top             =   4680
      Width           =   1935
   End
   Begin VB.TextBox txtPitchAndFamily 
      Height          =   315
      Left            =   1500
      TabIndex        =   20
      Top             =   4320
      Width           =   1935
   End
   Begin VB.TextBox txtQuality 
      Height          =   315
      Left            =   1500
      TabIndex        =   18
      Top             =   3960
      Width           =   1935
   End
   Begin VB.TextBox txtClipPrecision 
      Height          =   315
      Left            =   1500
      TabIndex        =   16
      Top             =   3600
      Width           =   1935
   End
   Begin VB.TextBox txtOutPrecision 
      Height          =   315
      Left            =   1500
      TabIndex        =   14
      Top             =   3240
      Width           =   1935
   End
   Begin VB.TextBox txtCharSet 
      Height          =   315
      Left            =   1500
      TabIndex        =   12
      Top             =   2880
      Width           =   1935
   End
   Begin VB.TextBox txtWeight 
      Height          =   315
      Left            =   1500
      TabIndex        =   9
      Top             =   2160
      Width           =   1935
   End
   Begin VB.TextBox txtOrientation 
      Height          =   315
      Left            =   1500
      TabIndex        =   8
      Top             =   1800
      Width           =   1935
   End
   Begin VB.TextBox txtEscapement 
      Height          =   315
      Left            =   1500
      TabIndex        =   7
      Top             =   1440
      Width           =   1935
   End
   Begin VB.TextBox txtWidth 
      Height          =   315
      Left            =   1500
      TabIndex        =   5
      Top             =   1080
      Width           =   1935
   End
   Begin VB.TextBox txtHeight 
      Height          =   315
      Left            =   1500
      TabIndex        =   3
      Top             =   720
      Width           =   1935
   End
   Begin VB.CommandButton cmdLoadDefault 
      Caption         =   "デフォルトフォント(&D)"
      Height          =   375
      Left            =   180
      TabIndex        =   1
      Top             =   180
      Width           =   1995
   End
   Begin VB.PictureBox picSample 
      Height          =   4275
      Left            =   4080
      ScaleHeight     =   4215
      ScaleWidth      =   3015
      TabIndex        =   0
      Top             =   720
      Width           =   3075
   End
   Begin VB.Label lblPanelSample 
      Caption         =   "Panel Sample:"
      Height          =   195
      Left            =   4080
      TabIndex        =   28
      Top             =   480
      Width           =   1215
   End
   Begin VB.Label lblFaceName 
      Caption         =   "FaceName:"
      Height          =   195
      Left            =   180
      TabIndex        =   23
      Top             =   4740
      Width           =   1215
   End
   Begin VB.Label lblPitchAndFamily 
      Caption         =   "PitchAndFamily:"
      Height          =   195
      Left            =   180
      TabIndex        =   21
      Top             =   4380
      Width           =   1215
   End
   Begin VB.Label lblQuality 
      Caption         =   "Quality:"
      Height          =   195
      Left            =   180
      TabIndex        =   19
      Top             =   4020
      Width           =   1215
   End
   Begin VB.Label lblClipPrecision 
      Caption         =   "ClipPrecision:"
      Height          =   195
      Left            =   180
      TabIndex        =   17
      Top             =   3660
      Width           =   1215
   End
   Begin VB.Label lblOutPrecision 
      Caption         =   "OutPrecision:"
      Height          =   195
      Left            =   180
      TabIndex        =   15
      Top             =   3300
      Width           =   1215
   End
   Begin VB.Label lblCharSet 
      Caption         =   "CharSet:"
      Height          =   195
      Left            =   180
      TabIndex        =   13
      Top             =   2940
      Width           =   1215
   End
   Begin VB.Label lblWeight 
      Caption         =   "Weight:"
      Height          =   195
      Left            =   180
      TabIndex        =   11
      Top             =   2220
      Width           =   1215
   End
   Begin VB.Label lblOrientation 
      Caption         =   "Orientation:"
      Height          =   195
      Left            =   180
      TabIndex        =   10
      Top             =   1860
      Width           =   1215
   End
   Begin VB.Label lblEscapement 
      Caption         =   "Escapement:"
      Height          =   195
      Left            =   180
      TabIndex        =   6
      Top             =   1500
      Width           =   1215
   End
   Begin VB.Label lblWidth 
      Caption         =   "Width:"
      Height          =   195
      Left            =   180
      TabIndex        =   4
      Top             =   1140
      Width           =   1215
   End
   Begin VB.Label lblHeight 
      Caption         =   "Height:"
      Height          =   195
      Left            =   180
      TabIndex        =   2
      Top             =   780
      Width           =   1215
   End
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Const def_PanelBlockWidth As Long = 48
Private Const def_PanelBlockHeight As Long = 17

Private m_lf As LOGFONT

Private Sub UpdateControls()
    txtHeight.Text = m_lf.lfHeight
    txtWidth.Text = m_lf.lfWidth
    txtEscapement.Text = m_lf.lfEscapement
    txtOrientation.Text = m_lf.lfOrientation
    txtWeight.Text = m_lf.lfWeight
    chkItalic.Value = m_lf.lfItalic
    chkUnderline.Value = m_lf.lfUnderline
    chkStrikeOut.Value = m_lf.lfStrikeOut
    txtCharSet.Text = m_lf.lfCharSet
    txtOutPrecision.Text = m_lf.lfOutPrecision
    txtClipPrecision.Text = m_lf.lfClipPrecision
    txtQuality.Text = m_lf.lfQuality
    txtPitchAndFamily.Text = m_lf.lfPitchAndFamily
    txtFaceName.Text = m_lf.lfFaceName
End Sub

Private Sub cmdFont_Click()
    On Error GoTo ErrHandler
    
    dlgFont.FontBold = IIf(m_lf.lfWeight = 400, False, True)
    dlgFont.FontItalic = IIf(m_lf.lfItalic = 0, False, True)
    dlgFont.FontName = m_lf.lfFaceName
    dlgFont.FontSize = -m_lf.lfHeight * 72 / GetDeviceCaps(Me.hdc, LOGPIXELSY)
    dlgFont.FontStrikethru = IIf(m_lf.lfStrikeOut = 0, False, True)
    dlgFont.FontUnderline = IIf(m_lf.lfUnderline = 0, False, True)
    
    dlgFont.Flags = cdlCFScreenFonts
    dlgFont.ShowFont
    
    m_lf.lfWeight = IIf(dlgFont.FontBold, 700, 400)
    m_lf.lfItalic = IIf(dlgFont.FontItalic, 1, 0)
    m_lf.lfFaceName = dlgFont.FontName & vbNullChar
    m_lf.lfHeight = -MulDiv(dlgFont.FontSize, GetDeviceCaps(Me.hdc, LOGPIXELSY), 72)
    m_lf.lfStrikeOut = IIf(dlgFont.FontStrikethru, 1, 0)
    m_lf.lfUnderline = IIf(dlgFont.FontUnderline, 1, 0)
    
    UpdateControls
    picSample.Refresh
    
    Exit Sub
ErrHandler:
    'エラーまたは、キャンセルを選択
End Sub

Private Sub cmdLoadDefault_Click()
    GetObject GetStockObject(DEFAULT_GUI_FONT), LenB(m_lf), m_lf
    m_lf.lfWeight = 700
    
    UpdateControls
    picSample.Refresh
End Sub

Private Sub DrawSample(ByVal hdc As Long)
    Dim i As Long
    Dim lfTemp As LOGFONT
    Dim rcTemp As RECT
    Dim hFont As Long, hFontOld As Long
    
    SetRect rcTemp, 0, 0, 640, 480
    FillRect hdc, rcTemp, GetStockObject(WHITE_BRUSH)
    
    For i = 100 To 10 Step -10
        SetRect rcTemp, 0, 0, def_PanelBlockWidth * i / 100, def_PanelBlockHeight * i / 100
        OffsetRect rcTemp, 20, ((100 - i) / 10) * (def_PanelBlockHeight + 10) + 20
        DrawEdge hdc, rcTemp, BDR_RAISED, BF_RECT Or BF_ADJUST
        
        lfTemp = m_lf
        lfTemp.lfHeight = lfTemp.lfHeight * i / 100
        
        SetTextCharacterExtra hdc, 1
        
        hFont = CreateFontIndirect(lfTemp)
        hFontOld = SelectObject(hdc, hFont)
        InflateRect rcTemp, -1, -1
        DrawText hdc, "9999", -1, rcTemp, DT_CENTER Or DT_VCENTER Or DT_SINGLELINE
        SelectObject hdc, GetStockObject(DEFAULT_GUI_FONT)
        DeleteObject hFont
        
        TextOut hdc, 100, rcTemp.Top, i & "%", Len(i & "%")
    Next
End Sub

Private Sub Form_Load()
    Me.cmdLoadDefault.Value = True
End Sub

Private Sub picSample_Paint()
    Dim hBitmap As Long
    Dim hMemDC As Long
    
    hBitmap = CreateCompatibleBitmap(picSample.hdc, 640, 480)
    If hBitmap <> 0 Then
        hMemDC = CreateCompatibleDC(picSample.hdc)
        If hMemDC <> 0 Then
            SelectObject hMemDC, hBitmap
            
            DrawSample hMemDC
            
            BitBlt picSample.hdc, 0, 0, 640, 480, hMemDC, 0, 0, SRCCOPY
            
            DeleteDC hMemDC
        End If
        
        DeleteObject hBitmap
    End If
End Sub
