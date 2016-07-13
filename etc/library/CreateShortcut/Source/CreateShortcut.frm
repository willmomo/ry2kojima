VERSION 5.00
Begin VB.Form frmCrate 
   BorderStyle     =   3  '固定ﾀﾞｲｱﾛｸﾞ
   Caption         =   "ショートカット作成法(その1)"
   ClientHeight    =   5205
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   6540
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   5205
   ScaleWidth      =   6540
   ShowInTaskbar   =   0   'False
   StartUpPosition =   3  'Windows の既定値
   Begin VB.ComboBox cmbWindowState 
      Height          =   300
      ItemData        =   "CreateShortcut.frx":0000
      Left            =   1560
      List            =   "CreateShortcut.frx":000D
      Style           =   2  'ﾄﾞﾛｯﾌﾟﾀﾞｳﾝ ﾘｽﾄ
      TabIndex        =   15
      Top             =   3120
      Width           =   4875
   End
   Begin VB.TextBox txtIconIndex 
      Alignment       =   1  '右揃え
      Height          =   330
      Left            =   6060
      TabIndex        =   13
      Text            =   "0"
      ToolTipText     =   "0以上の整数を指定します。"
      Top             =   2580
      Width           =   375
   End
   Begin VB.TextBox txtIconPath 
      Height          =   315
      Left            =   1560
      TabIndex        =   12
      Text            =   "NOTEPAD.EXE"
      Top             =   2580
      Width           =   4455
   End
   Begin VB.TextBox txtWorkDir 
      Height          =   315
      Left            =   1560
      TabIndex        =   10
      Text            =   "C:\"
      Top             =   2040
      Width           =   4875
   End
   Begin VB.TextBox txtFile 
      Height          =   315
      Left            =   1560
      TabIndex        =   2
      Text            =   "C:\SAMPLE.LNK"
      Top             =   120
      Width           =   4875
   End
   Begin VB.Frame fraHotKey 
      Caption         =   "ショートカットキー(&K):"
      Height          =   1335
      Left            =   120
      TabIndex        =   16
      Top             =   3660
      Width           =   4215
      Begin VB.ComboBox cmbHotKey 
         Height          =   300
         ItemData        =   "CreateShortcut.frx":002F
         Left            =   360
         List            =   "CreateShortcut.frx":01A0
         Style           =   2  'ﾄﾞﾛｯﾌﾟﾀﾞｳﾝ ﾘｽﾄ
         TabIndex        =   20
         Top             =   780
         Width           =   3495
      End
      Begin VB.CheckBox chkAlt 
         Caption         =   "&Alt"
         Height          =   315
         Left            =   2880
         TabIndex        =   19
         Top             =   300
         Width           =   975
      End
      Begin VB.CheckBox chkCtrl 
         Caption         =   "&Ctrl"
         Height          =   315
         Left            =   360
         TabIndex        =   17
         Top             =   300
         Width           =   1035
      End
      Begin VB.CheckBox chkShift 
         Caption         =   "&Shift"
         Height          =   315
         Left            =   1620
         TabIndex        =   18
         Top             =   300
         Width           =   975
      End
   End
   Begin VB.TextBox txtDescription 
      Height          =   315
      Left            =   1560
      TabIndex        =   8
      Text            =   "メモ帳でTipsを開きます。"
      Top             =   1560
      Width           =   4875
   End
   Begin VB.TextBox txtArguments 
      Height          =   315
      Left            =   1560
      TabIndex        =   6
      Text            =   "TIPS.TXT"
      Top             =   1080
      Width           =   4875
   End
   Begin VB.TextBox txtTarget 
      Height          =   315
      Left            =   1560
      TabIndex        =   4
      Text            =   "NOTEPAD.EXE"
      Top             =   600
      Width           =   4875
   End
   Begin VB.CommandButton cmdMake 
      Caption         =   "作成(&M)"
      Height          =   615
      Left            =   4620
      TabIndex        =   0
      Top             =   4020
      Width           =   1635
   End
   Begin VB.Label lblTitle 
      BackStyle       =   0  '透明
      Caption         =   "実行サイズ(&W):"
      Height          =   315
      Index           =   6
      Left            =   180
      TabIndex        =   14
      Top             =   3120
      Width           =   1335
   End
   Begin VB.Label lblTitle 
      BackStyle       =   0  '透明
      Caption         =   "アイコン(&I):"
      Height          =   315
      Index           =   5
      Left            =   180
      TabIndex        =   11
      Top             =   2580
      Width           =   1155
   End
   Begin VB.Label lblTitle 
      BackStyle       =   0  '透明
      Caption         =   "作業位置(&D):"
      Height          =   315
      Index           =   4
      Left            =   180
      TabIndex        =   9
      Top             =   2040
      Width           =   1155
   End
   Begin VB.Label lblTitle 
      BackStyle       =   0  '透明
      Caption         =   "作成先(&F):"
      Height          =   315
      Index           =   3
      Left            =   180
      TabIndex        =   1
      Top             =   120
      Width           =   1155
   End
   Begin VB.Label lblTitle 
      BackStyle       =   0  '透明
      Caption         =   "コメント(&M):"
      Height          =   315
      Index           =   2
      Left            =   180
      TabIndex        =   7
      Top             =   1560
      Width           =   1155
   End
   Begin VB.Label lblTitle 
      BackStyle       =   0  '透明
      Caption         =   "引数(&P):"
      Height          =   315
      Index           =   1
      Left            =   180
      TabIndex        =   5
      Top             =   1080
      Width           =   1155
   End
   Begin VB.Label lblTitle 
      BackStyle       =   0  '透明
      Caption         =   "リンク先(&T):"
      Height          =   315
      Index           =   0
      Left            =   180
      TabIndex        =   3
      Top             =   600
      Width           =   1155
   End
End
Attribute VB_Name = "frmCrate"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Sub cmdMake_Click()
    Dim PF As IPersistFile
    Dim objLnk As ShellLink     'As IShellLinkA
    Dim intHotKey As Integer
    Dim lngShowCmd As Long
    Dim lngIconIndex As Long
    
    If ValidationCheck = True Then
        intHotKey = HotKeyValue()
        lngIconIndex = CLng(txtIconIndex.Text)
        lngShowCmd = cmbWindowState.ItemData(cmbWindowState.ListIndex)
        
        '初期化
        Set objLnk = New ShellLink
        
        '各種設定
        objLnk.SetArguments txtArguments.Text
        objLnk.SetDescription txtDescription.Text
        objLnk.SetHotkey intHotKey
        objLnk.SetIconLocation txtIconPath.Text, lngIconIndex
        objLnk.SetShowCmd lngShowCmd
        objLnk.SetWorkingDirectory txtWorkDir.Text
        objLnk.SetPath txtTarget.Text
        
        '保存
        Set PF = objLnk
        PF.Save txtFile.Text, 0
        
        '終了処理
        Set PF = Nothing
        Set objLnk = Nothing
        
        MsgBox "作成完了", vbInformation
    End If
End Sub

Private Sub cmbHotKey_Click()
    Dim Flag As Boolean
    
    Flag = CBool(cmbHotKey.ListIndex <> 0)
    chkCtrl.Enabled = Flag
    chkAlt.Enabled = Flag
    chkShift.Enabled = Flag
End Sub

Private Sub Form_Load()
    txtTarget.Text = AddSep(Environ("WINDIR"), "NOTEPAD.EXE")
    txtArguments.Text = AddSep(Environ("WINDIR"), "TIPS.TXT")
    txtIconPath.Text = txtTarget.Text
    txtIconIndex.Text = "0"
    cmbHotKey.ListIndex = 0
    cmbWindowState.ListIndex = 0
End Sub

Private Function AddSep(ByVal Path As String, Optional AddFolder As String = "") As String
    If Right$(Path, 1) = "\" Then
        AddSep = Path & AddFolder
    Else
        AddSep = Path & "\" & AddFolder
    End If
End Function

Private Function HotKeyValue() As HOTKEY_MASK_FLAGS
    Dim Flag As HOTKEY_MASK_FLAGS
    
    If cmbHotKey.ListIndex = 0 Then
        Flag = HOTKEY_NONE
    Else
        Flag = cmbHotKey.ItemData(cmbHotKey.ListIndex)
        If chkCtrl.Value = vbChecked Then
            Flag = Flag Or HOTKEY_CONTROL_MASK
        End If
        If chkAlt.Value = vbChecked Then
            Flag = Flag Or HOTKEY_ALT_MASK
        End If
        If chkShift.Value = vbChecked Then
            Flag = Flag Or HOTKEY_SHIFT_MASK
        End If
    End If
    HotKeyValue = Flag
End Function

Private Function PutQuotationMarks(ByVal Path As String) As String
    If InStr(Path, " ") > 0 Then
        If Left$(Path, 1) <> """" Then
            Path = """" & Path
        End If
        If Right$(Path, 1) <> """" Then
            Path = Path & """"
        End If
    End If
    PutQuotationMarks = Path
End Function

Private Sub txtIconPath_Change()
    txtIconIndex.Enabled = CBool(txtIconPath.Text <> "")
End Sub

Private Function ValidationCheck() As Boolean
    If txtIconIndex.Enabled = True Then
        If IsNumeric(txtIconIndex.Text) = False Then
            MsgBox "アイコンの指定に誤りがあります。" & vbNewLine & _
                   "0以上の整数を指定してください。", vbExclamation
            txtIconIndex.SetFocus
            ValidationCheck = False
            Exit Function
        End If
        txtIconIndex.Text = CStr(Int(txtIconIndex.Text))
    Else
        txtIconIndex.Text = "0"
    End If

    txtTarget.Text = PutQuotationMarks(txtTarget.Text)
    txtWorkDir.Text = PutQuotationMarks(txtWorkDir.Text)
    ValidationCheck = True
End Function
