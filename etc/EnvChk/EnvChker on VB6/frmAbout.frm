VERSION 5.00
Begin VB.Form frmAbout 
   BorderStyle     =   3  '固定ﾀﾞｲｱﾛｸﾞ
   Caption         =   "バージョン情報"
   ClientHeight    =   3555
   ClientLeft      =   2340
   ClientTop       =   1935
   ClientWidth     =   5730
   ClipControls    =   0   'False
   LinkTopic       =   "Form2"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   2453.724
   ScaleMode       =   0  'ﾕｰｻﾞｰ
   ScaleWidth      =   5380.766
   ShowInTaskbar   =   0   'False
   StartUpPosition =   1  'ｵｰﾅｰ ﾌｫｰﾑの中央
   Begin VB.PictureBox picIcon 
      AutoSize        =   -1  'True
      ClipControls    =   0   'False
      Height          =   540
      Left            =   240
      Picture         =   "frmAbout.frx":0000
      ScaleHeight     =   337.12
      ScaleMode       =   0  'ﾕｰｻﾞｰ
      ScaleWidth      =   337.12
      TabIndex        =   1
      Top             =   240
      Width           =   540
   End
   Begin VB.CommandButton cmdOK 
      Cancel          =   -1  'True
      Caption         =   "OK"
      Default         =   -1  'True
      Height          =   345
      Left            =   4245
      TabIndex        =   0
      Top             =   2625
      Width           =   1380
   End
   Begin VB.CommandButton cmdSysInfo 
      Caption         =   "ｼｽﾃﾑ情報(&S)..."
      Height          =   345
      Left            =   4260
      TabIndex        =   2
      Top             =   3075
      Width           =   1380
   End
   Begin VB.Line Line1 
      BorderColor     =   &H00808080&
      BorderStyle     =   6  '実線 (ふちどり)
      Index           =   1
      X1              =   84.515
      X2              =   5309.398
      Y1              =   1687.583
      Y2              =   1687.583
   End
   Begin VB.Label lblDescription 
      Caption         =   "アプリケーションの動作環境をチェックします。"
      ForeColor       =   &H00000000&
      Height          =   1170
      Left            =   1050
      TabIndex        =   3
      Top             =   1125
      Width           =   3885
   End
   Begin VB.Label lblTitle 
      Caption         =   "環境チェッカ"
      ForeColor       =   &H00000000&
      Height          =   480
      Left            =   1050
      TabIndex        =   5
      Top             =   240
      Width           =   3885
   End
   Begin VB.Line Line1 
      BorderColor     =   &H00FFFFFF&
      BorderWidth     =   2
      Index           =   0
      X1              =   98.6
      X2              =   5309.398
      Y1              =   1697.936
      Y2              =   1697.936
   End
   Begin VB.Label lblVersion 
      Caption         =   "1.0.0.0"
      Height          =   225
      Left            =   1050
      TabIndex        =   6
      Top             =   780
      Width           =   3885
   End
   Begin VB.Label lblDisclaimer 
      ForeColor       =   &H00000000&
      Height          =   825
      Left            =   255
      TabIndex        =   4
      Top             =   2625
      Width           =   3870
   End
End
Attribute VB_Name = "frmAbout"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

' ﾚｼﾞｽﾄﾘ ｷｰ ｾｷｭﾘﾃｨ ｵﾌﾟｼｮﾝ...
Const READ_CONTROL = &H20000
Const KEY_QUERY_VALUE = &H1
Const KEY_SET_VALUE = &H2
Const KEY_CREATE_SUB_KEY = &H4
Const KEY_ENUMERATE_SUB_KEYS = &H8
Const KEY_NOTIFY = &H10
Const KEY_CREATE_LINK = &H20
Const KEY_ALL_ACCESS = KEY_QUERY_VALUE + KEY_SET_VALUE + _
                       KEY_CREATE_SUB_KEY + KEY_ENUMERATE_SUB_KEYS + _
                       KEY_NOTIFY + KEY_CREATE_LINK + READ_CONTROL
                     
' ﾚｼﾞｽﾄﾘ ｷｰ ROOT 型...
Const HKEY_LOCAL_MACHINE = &H80000002
Const ERROR_SUCCESS = 0
Const REG_SZ = 1                         ' Null 文字で終わる Unicode 文字列

Const REG_DWORD = 4                      ' 32 ﾋﾞｯﾄ数値

Const gREGKEYSYSINFOLOC = "SOFTWARE\Microsoft\Shared Tools Location"
Const gREGVALSYSINFOLOC = "MSINFO"
Const gREGKEYSYSINFO = "SOFTWARE\Microsoft\Shared Tools\MSINFO"
Const gREGVALSYSINFO = "PATH"

Private Declare Function RegOpenKeyEx Lib "advapi32" Alias "RegOpenKeyExA" (ByVal hKey As Long, ByVal lpSubKey As String, ByVal ulOptions As Long, ByVal samDesired As Long, ByRef phkResult As Long) As Long
Private Declare Function RegQueryValueEx Lib "advapi32" Alias "RegQueryValueExA" (ByVal hKey As Long, ByVal lpValueName As String, ByVal lpReserved As Long, ByRef lpType As Long, ByVal lpData As String, ByRef lpcbData As Long) As Long
Private Declare Function RegCloseKey Lib "advapi32" (ByVal hKey As Long) As Long


Private Sub cmdSysInfo_Click()
  Call StartSysInfo
End Sub

Private Sub cmdOK_Click()
  Unload Me
End Sub

Private Sub Form_Load()
    Me.Caption = App.Title & "のバージョン情報"
    lblVersion.Caption = "バージョン " & App.Major & "." & App.Minor & "." & App.Revision
    lblTitle.Caption = App.Title
End Sub

Public Sub StartSysInfo()
    On Error GoTo SysInfoErr
  
    Dim rc As Long
    Dim SysInfoPath As String
    
    ' ﾚｼﾞｽﾄﾘからｼｽﾃﾑ情報ﾌﾟﾛｸﾞﾗﾑのﾊﾟｽ\名前を取得します...
    If GetKeyValue(HKEY_LOCAL_MACHINE, gREGKEYSYSINFO, gREGVALSYSINFO, SysInfoPath) Then
    ' ﾚｼﾞｽﾄﾘからｼｽﾃﾑ情報ﾌﾟﾛｸﾞﾗﾑのﾊﾟｽ名のみを取得します...
    ElseIf GetKeyValue(HKEY_LOCAL_MACHINE, gREGKEYSYSINFOLOC, gREGVALSYSINFOLOC, SysInfoPath) Then
        ' 既に存在するはずの 32 ﾋﾞｯﾄ ﾊﾞｰｼﾞｮﾝのﾌｧｲﾙを確認します。
        If (Dir(SysInfoPath & "\MSINFO32.EXE") <> "") Then
            SysInfoPath = SysInfoPath & "\MSINFO32.EXE"
            
        ' ｴﾗｰ - ﾌｧｲﾙが見つかりません...
        Else
            GoTo SysInfoErr
        End If
    ' ｴﾗｰ - ﾚｼﾞｽﾄﾘ ｴﾝﾄﾘが見つかりません...
    Else
        GoTo SysInfoErr
    End If
    
    Call Shell(SysInfoPath, vbNormalFocus)
    
    Exit Sub
SysInfoErr:
    MsgBox "現時点ではｼｽﾃﾑ情報を使用できません", vbOKOnly
End Sub

Public Function GetKeyValue(KeyRoot As Long, KeyName As String, SubKeyRef As String, ByRef KeyVal As String) As Boolean
    Dim i As Long                                           ' ﾙｰﾌﾟ ｶｳﾝﾀ
    Dim rc As Long                                          ' 戻り値
    Dim hKey As Long                                        ' ｵｰﾌﾟﾝしたﾚｼﾞｽﾄﾘ ｷｰのﾊﾝﾄﾞﾙ
    Dim hDepth As Long                                      '
    Dim KeyValType As Long                                  ' ﾚｼﾞｽﾄﾘ ｷｰのﾃﾞｰﾀ型
    Dim tmpVal As String                                    ' ﾚｼﾞｽﾄﾘ ｷｰ値の一時保存用変数
    Dim KeyValSize As Long                                  ' ﾚｼﾞｽﾄﾘ ｷｰ変数のｻｲｽﾞ
    '------------------------------------------------------------
    ' ﾙｰﾄ ｷｰ {HKEY_LOCAL_MACHINE...} にあるﾚｼﾞｽﾄﾘ ｷｰを開きます。
    '------------------------------------------------------------
    rc = RegOpenKeyEx(KeyRoot, KeyName, 0, KEY_ALL_ACCESS, hKey) ' ﾚｼﾞｽﾄﾘ ｷｰを開く
    
    If (rc <> ERROR_SUCCESS) Then GoTo GetKeyError          ' ﾊﾝﾄﾞﾙ ｴﾗｰ...
    
    tmpVal = String$(1024, 0)                             ' 変数領域の割り当て
    KeyValSize = 1024                                       ' 変数のｻｲｽﾞを記憶

    '------------------------------------------------------------
    ' ﾚｼﾞｽﾄﾘ ｷｰ値を取得します...
    '------------------------------------------------------------
    rc = RegQueryValueEx(hKey, SubKeyRef, 0, _
                         KeyValType, tmpVal, KeyValSize)    ' ｷｰ値の取得/作成
                        
    If (rc <> ERROR_SUCCESS) Then GoTo GetKeyError          ' ﾊﾝﾄﾞﾙ ｴﾗｰ
    
    If (Asc(Mid(tmpVal, KeyValSize, 1)) = 0) Then           ' Win95 は Null で終わる文字列を追加します...
        tmpVal = Left(tmpVal, KeyValSize - 1)               ' Null が見つかったら、文字列から抽出します。
    Else                                                    ' WinNT は Null で終わる文字列を使用しません...
        tmpVal = Left(tmpVal, KeyValSize)                   ' Null が見つからなかったら、文字列のみを抽出します。
    End If
    '------------------------------------------------------------
    ' 変換のために、ｷｰ値の型を調べます...
    '------------------------------------------------------------
    Select Case KeyValType                                  ' ﾃﾞｰﾀ型検索...
    Case REG_SZ                                             ' String ﾚｼﾞｽﾄﾘ ｷｰ ﾃﾞｰﾀ型
        KeyVal = tmpVal                                     ' String 値をｺﾋﾟｰ
    Case REG_DWORD                                          ' Double Word ﾚｼﾞｽﾄﾘ ｷｰ ﾃﾞｰﾀ型
        For i = Len(tmpVal) To 1 Step -1                    ' 各ﾋﾞｯﾄの変換
            KeyVal = KeyVal + Hex(Asc(Mid(tmpVal, i, 1)))   ' Char ごとに値を作成
        Next
        KeyVal = Format$("&h" + KeyVal)                     ' Double Word を String に変換
    End Select
    
    GetKeyValue = True                                      ' 正常終了
    rc = RegCloseKey(hKey)                                  ' ﾚｼﾞｽﾄﾘ ｷｰをｸﾛｰｽﾞ
    Exit Function                                           ' 終了
    
GetKeyError:      ' ｴﾗｰ発生後の後始末...
    KeyVal = ""                                             ' 戻り値の値を空文字列に設定
    GetKeyValue = False                                     ' 異常終了
    rc = RegCloseKey(hKey)                                  ' ﾚｼﾞｽﾄﾘ ｷｰをｸﾛｰｽﾞ
End Function
