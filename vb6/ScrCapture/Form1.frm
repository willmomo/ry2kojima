VERSION 5.00
Begin VB.Form frmMain 
   Caption         =   "ScrCap"
   ClientHeight    =   8175
   ClientLeft      =   165
   ClientTop       =   855
   ClientWidth     =   11580
   LinkTopic       =   "Form1"
   ScaleHeight     =   8175
   ScaleWidth      =   11580
   StartUpPosition =   3  'Windows の既定値
   Visible         =   0   'False
   Begin VB.PictureBox Picture1 
      Appearance      =   0  'ﾌﾗｯﾄ
      AutoRedraw      =   -1  'True
      BackColor       =   &H80000005&
      BorderStyle     =   0  'なし
      ForeColor       =   &H80000008&
      Height          =   7155
      Left            =   180
      ScaleHeight     =   7155
      ScaleWidth      =   11295
      TabIndex        =   1
      Top             =   900
      Width           =   11295
   End
   Begin VB.CommandButton Command1 
      Caption         =   "Command1"
      Height          =   495
      Left            =   240
      TabIndex        =   0
      Top             =   240
      Width           =   1215
   End
   Begin VB.Menu mnuContext 
      Caption         =   "context menu"
      Begin VB.Menu mnuContextExit 
         Caption         =   "終了(&X)"
      End
   End
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private m_nid As NOTIFYICONDATA

Private Sub InitializeEnvironment()
    Dim objFS As FileSystemObject
    Set objFS = New FileSystemObject
    
    If Not objFS.FolderExists(App.Path & "\data") Then
        objFS.CreateFolder App.Path & "\data"
    End If
End Sub

Public Sub SnapShot_A()

    Dim MyFileName As String
    Dim PicData As Picture
    Clipboard.Clear

    '画面全体のスナップショットを取得する(WinXP)
    Call keybd_event(VK_SNAPSHOT, 1, 0, 0)
    ''Call keybd_event(VK_SNAPSHOT, 0, KEYEVENTF_EXTENDEDKEY, 0)
    ''Call keybd_event(VK_SNAPSHOT, 0, KEYEVENTF_EXTENDEDKEY Or KEYEVENTF_KEYUP, 0)

    ''Sleep 500
    DoEvents
    
    'クリップボード内にビットマップ形式のデータがあるか調べる
    If Clipboard.GetFormat(vbCFBitmap) Then
        'ファイル名を自動生成
        MyFileName = App.Path & "\data\" & Format(Now, "yyyymmdd-hhnnss") & ".bmp"
        '表示データーをビットマップ形式のデータで保存
        Set PicData = Clipboard.GetData
        Call SavePicture(PicData, MyFileName)
    Else
        '
    End If
End Sub

Public Sub SnapShot()
    Dim hdcScreen As Long
    
    Picture1.Width = Screen.Width
    Picture1.Height = Screen.Height
    
    hdcScreen = CreateDC("DISPLAY", 0, 0, 0)
    'hdcScreen = GetWindowDC(0)
    BitBlt Picture1.hdc, 0, 0, _
        Screen.Width / Screen.TwipsPerPixelX, _
        Screen.Height / Screen.TwipsPerPixelY, hdcScreen, 0, 0, SRCCOPY Or CAPTUREBLT
    DeleteDC hdcScreen
    'ReleaseDC 0, hdcScreen
    
    SavePicture Picture1.Image, App.Path & "\data\" & Format(Now, "yyyymmdd-hhnnss") & ".bmp"
End Sub

Private Sub Command1_Click()
    SnapShot
End Sub

Private Sub Form_Load()
    InitializeEnvironment
    
    m_nid.cbSize = Len(m_nid)
    m_nid.hwnd = Me.hwnd
    m_nid.uID = 1
    m_nid.uFlags = NIF_ICON Or NIF_MESSAGE
    m_nid.hIcon = LoadIcon(0, IDI_APPLICATION)
    m_nid.uCallbackMessage = WM_MOUSEMOVE
    Shell_NotifyIcon NIM_ADD, m_nid
    
    StartSubClass Me.hwnd
End Sub

Private Sub Form_MouseMove(Button As Integer, Shift As Integer, X As Single, Y As Single)
    Select Case X \ Screen.TwipsPerPixelX
        Case WM_MOUSEMOVE
        
        Case WM_LBUTTONDOWN
        
        Case WM_RBUTTONDOWN
            Me.PopupMenu mnuContext
    End Select
End Sub

Private Sub Form_Unload(Cancel As Integer)
    EndSubClass Me.hwnd
    
    Shell_NotifyIcon NIM_DELETE, m_nid
End Sub

Private Sub mnuContextExit_Click()
    Unload Me
End Sub
