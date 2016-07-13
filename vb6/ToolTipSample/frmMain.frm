VERSION 5.00
Object = "{065E6FD1-1BF9-11D2-BAE8-00104B9E0792}#3.0#0"; "ssa3d30.ocx"
Begin VB.Form frmMain 
   BorderStyle     =   1  '固定(実線)
   Caption         =   "Form1"
   ClientHeight    =   3225
   ClientLeft      =   45
   ClientTop       =   435
   ClientWidth     =   7800
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   3225
   ScaleWidth      =   7800
   StartUpPosition =   3  'Windows の既定値
   Begin Threed.SSRibbon SSRibbon1 
      Height          =   855
      Index           =   0
      Left            =   3840
      TabIndex        =   7
      Top             =   240
      Width           =   1575
      _ExtentX        =   2778
      _ExtentY        =   1508
      _Version        =   196610
      Caption         =   "SSRibbon1"
   End
   Begin VB.CommandButton Command3 
      Caption         =   "サブウィンドウを開く"
      Height          =   495
      Left            =   1560
      TabIndex        =   6
      Top             =   1440
      Width           =   2055
   End
   Begin VB.CommandButton Command2 
      Caption         =   "サブウィンドウを開く"
      Height          =   495
      Left            =   1560
      TabIndex        =   4
      Top             =   600
      Width           =   2055
   End
   Begin VB.CommandButton Command1 
      Caption         =   "終わり"
      Height          =   495
      Left            =   120
      TabIndex        =   5
      Top             =   2520
      Width           =   3495
   End
   Begin Threed.SSRibbon SSRibbon1 
      Height          =   855
      Index           =   1
      Left            =   5520
      TabIndex        =   8
      Top             =   240
      Width           =   1575
      _ExtentX        =   2778
      _ExtentY        =   1508
      _Version        =   196610
      Caption         =   "SSRibbon1"
   End
   Begin VB.Label Label4 
      BackColor       =   &H00C0FFC0&
      Caption         =   "個々台誤差玉"
      Height          =   495
      Left            =   120
      TabIndex        =   3
      Top             =   1920
      Width           =   1215
   End
   Begin VB.Label Label3 
      BackColor       =   &H00C0FFC0&
      Caption         =   "台誤差玉"
      Height          =   495
      Left            =   120
      TabIndex        =   2
      Top             =   1320
      Width           =   1215
   End
   Begin VB.Label Label2 
      BackColor       =   &H00C0FFC0&
      Caption         =   "打止持玉"
      Height          =   495
      Left            =   120
      TabIndex        =   1
      Top             =   720
      Width           =   1215
   End
   Begin VB.Label Label1 
      BackColor       =   &H008080FF&
      Caption         =   "個々持玉"
      Height          =   495
      Left            =   120
      TabIndex        =   0
      Top             =   120
      Width           =   1215
   End
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private m_toolTip As New clsToolTip

Private Sub Command1_Click()
    Unload frmSub
    Unload Me
End Sub

Private Sub Command2_Click()
    frmSub.Show vbModal, Me
End Sub

Private Sub Command3_Click()
    frmSub.Show vbModeless, Me
End Sub

Private Sub Form_Load()
    ' ツールチップを作成する
    If m_toolTip.createToolTip(Me.hwnd, False) = False Then
        MsgBox "ツールチップの作成に失敗!!"
        End
    End If

    ' -1 以外を指定するとマルチラインになる。
    m_toolTip.MaxTipWidth = 1000

    ' 表示するツールチップを登録する。
    m_toolTip.AddTool Command1, "アプリケーションを終了します。"
    m_toolTip.AddTool Command2, "サブウィンドウを表示して、いろいろなコントロールにツールチップを追加。" & vbNewLine & "モーダルで開きます。"
    m_toolTip.AddTool Command3, "サブウィンドウを表示して、いろいろなコントロールにツールチップを追加。" & vbNewLine & "モードレスで開きます。"
    m_toolTip.AddToolForLabel Label1, 1, "開放操作からの持玉数(売上玉を含む）"
    m_toolTip.AddToolForLabel Label2, 2, "自動打止を行う持玉数" & vbNewLine & "※開放操作からの持玉数(売上玉を含まない）"
    m_toolTip.AddToolForLabel Label3, 3, "累計の誤差玉数。" & vbNewLine & "※補正売上玉＋持玉払出数－差玉－各台計数値"
    m_toolTip.AddToolForLabel Label4, 4, "開放操作からの誤差玉数。"
    
    m_toolTip.AddTool SSRibbon1(0), "コントロール(0)の状態は" & vbNewLine & SSRibbon1(0).Value & vbNewLine & "です。"
    m_toolTip.AddTool SSRibbon1(1), "コントロール(1)の状態は" & vbNewLine & SSRibbon1(1).Value & vbNewLine & "です。"
End Sub

Private Sub Form_Unload(Cancel As Integer)
    ' ツールチップを破棄する
    m_toolTip.destroyToolTip
End Sub

Private Sub SSRibbon1_Click(Index As Integer, Value As Integer)
    m_toolTip.UpdateTipText SSRibbon1(Index), Now & vbNewLine & "コントロール(" & Index & ")の状態は" & vbNewLine & SSRibbon1(Index).Value & vbNewLine & "です。"
End Sub
