VERSION 5.00
Object = "{EAB22AC0-30C1-11CF-A7EB-0000C05BAE0B}#1.1#0"; "ieframe.dll"
Begin VB.Form Form1 
   Caption         =   "Form1"
   ClientHeight    =   9000
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   15225
   LinkTopic       =   "Form1"
   ScaleHeight     =   9000
   ScaleWidth      =   15225
   StartUpPosition =   3  'Windows ‚ÌŠù’è’l
   Begin VB.Timer Timer1 
      Interval        =   10000
      Left            =   9240
      Top             =   360
   End
   Begin VB.CommandButton Command1 
      Caption         =   "Command1"
      Height          =   495
      Left            =   7560
      TabIndex        =   1
      Top             =   120
      Width           =   1215
   End
   Begin VB.TextBox Text1 
      Height          =   495
      Left            =   120
      TabIndex        =   0
      Top             =   120
      Width           =   7335
   End
   Begin SHDocVwCtl.WebBrowser WebBrowser1 
      Height          =   8055
      Left            =   120
      TabIndex        =   2
      Top             =   720
      Width           =   8775
      ExtentX         =   15478
      ExtentY         =   14208
      ViewMode        =   0
      Offline         =   0
      Silent          =   0
      RegisterAsBrowser=   0
      RegisterAsDropTarget=   1
      AutoArrange     =   0   'False
      NoClientEdge    =   0   'False
      AlignLeft       =   0   'False
      NoWebView       =   0   'False
      HideFileNames   =   0   'False
      SingleClick     =   0   'False
      SingleSelection =   0   'False
      NoFolders       =   0   'False
      Transparent     =   0   'False
      ViewID          =   "{0057D0E0-3573-11CF-AE69-08002B2E1262}"
      Location        =   "http:///"
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private g_ts As TextStream
Private g_beforeNaviURL  As Variant

Private Sub Command1_Click()
    WebBrowser1.Navigate2 Text1.Text
End Sub

Private Sub Timer1_Timer()
    ''WebBrowser1.Navigate2 "http://172.17.101.21/30/h3062.php?kisyutype=0&rekiLong=1&isslide=true", , "main"
End Sub

Private Sub Form_Load()
    Form2.Show
End Sub

Private Sub Form_Resize()
    WebBrowser1.Width = Me.ScaleWidth - (WebBrowser1.Left * 2)
    WebBrowser1.Height = Me.ScaleHeight - (WebBrowser1.Top + WebBrowser1.Left)

    Form2.Top = Me.Top
    Form2.Left = Me.Left + Me.Width
    'Form2.Width = Me.Width
    Form2.Height = Me.Height
End Sub

Private Sub Form_Unload(Cancel As Integer)
    Unload Form2
End Sub

Private Sub Text1_GotFocus()
    Text1.SelStart = 0
    Text1.SelLength = 9999
End Sub

Private Sub WebBrowser1_BeforeNavigate2(ByVal pDisp As Object, URL As Variant, Flags As Variant, TargetFrameName As Variant, PostData As Variant, Headers As Variant, Cancel As Boolean)
    g_beforeNaviURL = URL
    
    Form2.Text1.Text = Form2.Text1.Text & String(60, "-") & vbNewLine
    Form2.Text1.Text = Form2.Text1.Text & Now & vbTab & "BeforeNavigate2" & vbTab & "URL:=" & URL & vbNewLine
    Form2.Text1.SelStart = Len(Form2.Text1.Text)
End Sub

Private Sub WebBrowser1_CommandStateChange(ByVal Command As Long, ByVal Enable As Boolean)
    ''Form2.Text1.Text = Form2.Text1.Text & Now & vbTab & "CommandStateChange", "Command:=" & Command
End Sub

Private Sub WebBrowser1_DocumentComplete(ByVal pDisp As Object, URL As Variant)
    Form2.Text1.Text = Form2.Text1.Text & Now & vbTab & "DocumentComplete" & vbTab & "URL:=" & URL & vbNewLine
    Form2.Text1.SelStart = Len(Form2.Text1.Text)
End Sub

Private Sub WebBrowser1_DownloadBegin()
    ''Form2.Text1.Text = Form2.Text1.Text & Now & vbTab & "DownloadBegin" & vbNewLine
    ''Form2.Text1.SelStart = Len(Form2.Text1.Text)
End Sub

Private Sub WebBrowser1_DownloadComplete()
    ''Form2.Text1.Text = Form2.Text1.Text & Now & vbTab & "DownloadComplete" & vbNewLine
    ''Form2.Text1.SelStart = Len(Form2.Text1.Text)
End Sub

Private Sub WebBrowser1_FileDownload(ByVal ActiveDocument As Boolean, Cancel As Boolean)
    Form2.Text1.Text = Form2.Text1.Text & Now & vbTab & "FileDownload" & vbNewLine
    Form2.Text1.SelStart = Len(Form2.Text1.Text)
End Sub

Private Sub WebBrowser1_NavigateComplete2(ByVal pDisp As Object, URL As Variant)
    Form2.Text1.Text = Form2.Text1.Text & Now & vbTab & "NavigateComplete2" & vbTab & "URL:=" & URL & vbNewLine
    Form2.Text1.SelStart = Len(Form2.Text1.Text)
End Sub

Private Sub WebBrowser1_NavigateError(ByVal pDisp As Object, URL As Variant, Frame As Variant, StatusCode As Variant, Cancel As Boolean)
    Form2.Text1.Text = Form2.Text1.Text & Now & vbTab & "WebBrowser1_NavigateError" & vbTab & "URL:=" & URL & vbNewLine
    Form2.Text1.SelStart = Len(Form2.Text1.Text)
End Sub

Private Sub WebBrowser1_StatusTextChange(ByVal Text As String)
    ''Form2.Text1.Text = Form2.Text1.Text & Now & vbTab & "StatusTextChange", Text
End Sub
