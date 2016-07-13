VERSION 5.00
Begin VB.Form frmMain 
   BorderStyle     =   1  '固定(実線)
   Caption         =   "POSキーボードテスト"
   ClientHeight    =   7080
   ClientLeft      =   150
   ClientTop       =   840
   ClientWidth     =   15225
   KeyPreview      =   -1  'True
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   7080
   ScaleWidth      =   15225
   StartUpPosition =   3  'Windows の既定値
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "S+C+A+8"
      Enabled         =   0   'False
      Height          =   735
      Index           =   47
      Left            =   5640
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   47
      Top             =   6120
      Width           =   1095
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "S+C+A+7"
      Enabled         =   0   'False
      Height          =   735
      Index           =   46
      Left            =   5640
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   46
      Top             =   5400
      Width           =   1095
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "S+C+A+6"
      Enabled         =   0   'False
      Height          =   735
      Index           =   45
      Left            =   5640
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   45
      Top             =   4680
      Width           =   1095
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "S+C+A+5"
      Enabled         =   0   'False
      Height          =   735
      Index           =   44
      Left            =   5640
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   44
      Top             =   3960
      Width           =   1095
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "S+C+A+4"
      Enabled         =   0   'False
      Height          =   735
      Index           =   43
      Left            =   5640
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   43
      Top             =   3240
      Width           =   1095
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "S+C+A+3"
      Enabled         =   0   'False
      Height          =   735
      Index           =   42
      Left            =   5640
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   42
      Top             =   2520
      Width           =   1095
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "S+C+A+2"
      Enabled         =   0   'False
      Height          =   735
      Index           =   41
      Left            =   5640
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   41
      Top             =   1800
      Width           =   1095
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "S+C+A+1"
      Enabled         =   0   'False
      Height          =   735
      Index           =   40
      Left            =   5640
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   40
      Top             =   1080
      Width           =   1095
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "C+A+8"
      Enabled         =   0   'False
      Height          =   735
      Index           =   39
      Left            =   4560
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   39
      Top             =   6120
      Width           =   1095
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "C+A+7"
      Enabled         =   0   'False
      Height          =   735
      Index           =   38
      Left            =   4560
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   38
      Top             =   5400
      Width           =   1095
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "C+A+6"
      Enabled         =   0   'False
      Height          =   735
      Index           =   37
      Left            =   4560
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   37
      Top             =   4680
      Width           =   1095
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "C+A+5"
      Enabled         =   0   'False
      Height          =   735
      Index           =   36
      Left            =   4560
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   36
      Top             =   3960
      Width           =   1095
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "C+A+4"
      Enabled         =   0   'False
      Height          =   735
      Index           =   35
      Left            =   4560
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   35
      Top             =   3240
      Width           =   1095
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "C+A+3"
      Enabled         =   0   'False
      Height          =   735
      Index           =   34
      Left            =   4560
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   34
      Top             =   2520
      Width           =   1095
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "C+A+2"
      Enabled         =   0   'False
      Height          =   735
      Index           =   33
      Left            =   4560
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   33
      Top             =   1800
      Width           =   1095
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "C+A+1"
      Enabled         =   0   'False
      Height          =   735
      Index           =   32
      Left            =   4560
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   32
      Top             =   1080
      Width           =   1095
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "A+8"
      Enabled         =   0   'False
      Height          =   735
      Index           =   31
      Left            =   3480
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   31
      Top             =   6120
      Width           =   1095
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "A+7"
      Enabled         =   0   'False
      Height          =   735
      Index           =   30
      Left            =   3480
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   30
      Top             =   5400
      Width           =   1095
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "A+6"
      Enabled         =   0   'False
      Height          =   735
      Index           =   29
      Left            =   3480
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   29
      Top             =   4680
      Width           =   1095
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "A+5"
      Enabled         =   0   'False
      Height          =   735
      Index           =   28
      Left            =   3480
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   28
      Top             =   3960
      Width           =   1095
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "A+4"
      Enabled         =   0   'False
      Height          =   735
      Index           =   27
      Left            =   3480
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   27
      Top             =   3240
      Width           =   1095
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "A+3"
      Enabled         =   0   'False
      Height          =   735
      Index           =   26
      Left            =   3480
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   26
      Top             =   2520
      Width           =   1095
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "A+2"
      Enabled         =   0   'False
      Height          =   735
      Index           =   25
      Left            =   3480
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   25
      Top             =   1800
      Width           =   1095
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "A+1"
      Enabled         =   0   'False
      Height          =   735
      Index           =   24
      Left            =   3480
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   24
      Top             =   1080
      Width           =   1095
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "C+8"
      Enabled         =   0   'False
      Height          =   735
      Index           =   23
      Left            =   2400
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   23
      Top             =   6120
      Width           =   1095
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "C+7"
      Enabled         =   0   'False
      Height          =   735
      Index           =   22
      Left            =   2400
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   22
      Top             =   5400
      Width           =   1095
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "C+6"
      Enabled         =   0   'False
      Height          =   735
      Index           =   21
      Left            =   2400
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   21
      Top             =   4680
      Width           =   1095
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "C+5"
      Enabled         =   0   'False
      Height          =   735
      Index           =   20
      Left            =   2400
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   20
      Top             =   3960
      Width           =   1095
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "C+4"
      Enabled         =   0   'False
      Height          =   735
      Index           =   19
      Left            =   2400
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   19
      Top             =   3240
      Width           =   1095
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "C+3"
      Enabled         =   0   'False
      Height          =   735
      Index           =   18
      Left            =   2400
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   18
      Top             =   2520
      Width           =   1095
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "C+2"
      Enabled         =   0   'False
      Height          =   735
      Index           =   17
      Left            =   2400
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   17
      Top             =   1800
      Width           =   1095
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "C+1"
      Enabled         =   0   'False
      Height          =   735
      Index           =   16
      Left            =   2400
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   16
      Top             =   1080
      Width           =   1095
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "S+F8"
      Enabled         =   0   'False
      Height          =   735
      Index           =   15
      Left            =   1320
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   15
      Top             =   6120
      Width           =   1095
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "S+F7"
      Enabled         =   0   'False
      Height          =   735
      Index           =   14
      Left            =   1320
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   14
      Top             =   5400
      Width           =   1095
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "S+F6"
      Enabled         =   0   'False
      Height          =   735
      Index           =   13
      Left            =   1320
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   13
      Top             =   4680
      Width           =   1095
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "S+F5"
      Enabled         =   0   'False
      Height          =   735
      Index           =   12
      Left            =   1320
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   12
      Top             =   3960
      Width           =   1095
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "S+F4"
      Enabled         =   0   'False
      Height          =   735
      Index           =   11
      Left            =   1320
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   11
      Top             =   3240
      Width           =   1095
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "S+F3"
      Enabled         =   0   'False
      Height          =   735
      Index           =   10
      Left            =   1320
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   10
      Top             =   2520
      Width           =   1095
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "S+F2"
      Enabled         =   0   'False
      Height          =   735
      Index           =   9
      Left            =   1320
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   9
      Top             =   1800
      Width           =   1095
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "S+F1"
      Enabled         =   0   'False
      Height          =   735
      Index           =   8
      Left            =   1320
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   8
      Top             =   1080
      Width           =   1095
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "F8"
      Enabled         =   0   'False
      Height          =   735
      Index           =   7
      Left            =   240
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   7
      Top             =   6120
      Width           =   1095
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "F7"
      Enabled         =   0   'False
      Height          =   735
      Index           =   6
      Left            =   240
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   6
      Top             =   5400
      Width           =   1095
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "F6"
      Enabled         =   0   'False
      Height          =   735
      Index           =   5
      Left            =   240
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   5
      Top             =   4680
      Width           =   1095
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "F5"
      Enabled         =   0   'False
      Height          =   735
      Index           =   4
      Left            =   240
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   4
      Top             =   3960
      Width           =   1095
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "F4"
      Enabled         =   0   'False
      Height          =   735
      Index           =   3
      Left            =   240
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   3
      Top             =   3240
      Width           =   1095
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "F3"
      Enabled         =   0   'False
      Height          =   735
      Index           =   2
      Left            =   240
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   2
      Top             =   2520
      Width           =   1095
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "F2"
      Enabled         =   0   'False
      Height          =   735
      Index           =   1
      Left            =   240
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   1
      Top             =   1800
      Width           =   1095
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "F1"
      Enabled         =   0   'False
      Height          =   735
      Index           =   0
      Left            =   240
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   0
      Top             =   1080
      Width           =   1095
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "貯玉読出"
      Enabled         =   0   'False
      Height          =   735
      Index           =   92
      Left            =   13920
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   49
      Top             =   6120
      Width           =   1095
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "返品"
      Enabled         =   0   'False
      Height          =   735
      Index           =   91
      Left            =   13920
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   50
      Top             =   5400
      Width           =   1095
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "全部"
      Enabled         =   0   'False
      Height          =   735
      Index           =   90
      Left            =   13920
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   51
      Top             =   4680
      Width           =   1095
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "終了"
      Enabled         =   0   'False
      Height          =   735
      Index           =   89
      Left            =   13920
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   52
      Top             =   3960
      Width           =   1095
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "貯玉"
      Enabled         =   0   'False
      Height          =   735
      Index           =   88
      Left            =   13920
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   53
      Top             =   2520
      Width           =   1095
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "保留"
      Enabled         =   0   'False
      Height          =   735
      Index           =   87
      Left            =   13920
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   54
      Top             =   1800
      Width           =   1095
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "連続"
      Enabled         =   0   'False
      Height          =   735
      Index           =   86
      Left            =   13920
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   55
      Top             =   1080
      Width           =   1095
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "訂正"
      Enabled         =   0   'False
      Height          =   735
      Index           =   59
      Left            =   10200
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   67
      Top             =   2520
      Width           =   1095
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "クリア"
      Enabled         =   0   'False
      Height          =   735
      Index           =   58
      Left            =   9120
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   68
      Top             =   2520
      Width           =   1095
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "文字選択"
      Enabled         =   0   'False
      Height          =   735
      Index           =   57
      Left            =   8040
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   69
      Top             =   2520
      Width           =   1095
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "記号"
      Enabled         =   0   'False
      Height          =   735
      Index           =   56
      Left            =   6960
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   70
      Top             =   2520
      Width           =   1095
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "戻し"
      Enabled         =   0   'False
      Height          =   735
      Index           =   55
      Left            =   10200
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   71
      Top             =   1800
      Width           =   1095
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "印字停止"
      Enabled         =   0   'False
      Height          =   735
      Index           =   54
      Left            =   9120
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   72
      Top             =   1800
      Width           =   1095
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "紙切"
      Enabled         =   0   'False
      Height          =   735
      Index           =   53
      Left            =   8040
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   73
      Top             =   1800
      Width           =   1095
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "紙送"
      Enabled         =   0   'False
      Height          =   735
      Index           =   52
      Left            =   6960
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   74
      Top             =   1800
      Width           =   1095
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "PAS"
      Enabled         =   0   'False
      Height          =   735
      Index           =   51
      Left            =   10200
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   75
      Top             =   1080
      Width           =   1095
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "手入力"
      Enabled         =   0   'False
      Height          =   735
      Index           =   50
      Left            =   9120
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   76
      Top             =   1080
      Width           =   1095
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "コード"
      Enabled         =   0   'False
      Height          =   735
      Index           =   49
      Left            =   8040
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   77
      Top             =   1080
      Width           =   1095
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "レシート"
      Enabled         =   0   'False
      Height          =   735
      Index           =   48
      Left            =   6960
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   78
      Top             =   1080
      Width           =   1095
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "後頁"
      Enabled         =   0   'False
      Height          =   735
      Index           =   80
      Left            =   10200
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   79
      Top             =   6120
      Width           =   1095
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "*"
      Enabled         =   0   'False
      Height          =   735
      Index           =   77
      Left            =   10200
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   82
      Top             =   5400
      Width           =   1095
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "-"
      Enabled         =   0   'False
      Height          =   735
      Index           =   76
      Left            =   10200
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   86
      Top             =   4680
      Width           =   1095
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "前頁"
      Enabled         =   0   'False
      Height          =   735
      Index           =   79
      Left            =   10200
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   90
      Top             =   3960
      Width           =   1095
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "確定"
      Enabled         =   0   'False
      Height          =   735
      Index           =   78
      Left            =   8040
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   80
      Top             =   6120
      Width           =   2175
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "3"
      Enabled         =   0   'False
      Height          =   735
      Index           =   69
      Left            =   9120
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   83
      Top             =   5400
      Width           =   1095
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "6"
      Enabled         =   0   'False
      Height          =   735
      Index           =   72
      Left            =   9120
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   87
      Top             =   4680
      Width           =   1095
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "9"
      Enabled         =   0   'False
      Height          =   735
      Index           =   75
      Left            =   9120
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   91
      Top             =   3960
      Width           =   1095
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "2"
      Enabled         =   0   'False
      Height          =   735
      Index           =   68
      Left            =   8040
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   84
      Top             =   5400
      Width           =   1095
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "5"
      Enabled         =   0   'False
      Height          =   735
      Index           =   71
      Left            =   8040
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   88
      Top             =   4680
      Width           =   1095
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "8"
      Enabled         =   0   'False
      Height          =   735
      Index           =   74
      Left            =   8040
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   92
      Top             =   3960
      Width           =   1095
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "0"
      Enabled         =   0   'False
      Height          =   735
      Index           =   66
      Left            =   6960
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   81
      Top             =   6120
      Width           =   1095
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "1"
      Enabled         =   0   'False
      Height          =   735
      Index           =   67
      Left            =   6960
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   85
      Top             =   5400
      Width           =   1095
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "4"
      Enabled         =   0   'False
      Height          =   735
      Index           =   70
      Left            =   6960
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   89
      Top             =   4680
      Width           =   1095
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "7"
      Enabled         =   0   'False
      Height          =   735
      Index           =   73
      Left            =   6960
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   93
      Top             =   3960
      Width           =   1095
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "システム"
      Enabled         =   0   'False
      Height          =   735
      Index           =   65
      Left            =   12600
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   61
      Top             =   2520
      Width           =   1095
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "日計履歴"
      Enabled         =   0   'False
      Height          =   735
      Index           =   64
      Left            =   11520
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   62
      Top             =   2520
      Width           =   1095
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "設定"
      Enabled         =   0   'False
      Height          =   735
      Index           =   63
      Left            =   12600
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   63
      Top             =   1800
      Width           =   1095
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "交換"
      Enabled         =   0   'False
      Height          =   735
      Index           =   62
      Left            =   11520
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   64
      Top             =   1800
      Width           =   1095
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "景品在庫"
      Enabled         =   0   'False
      Height          =   735
      Index           =   61
      Left            =   12600
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   65
      Top             =   1080
      Width           =   1095
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "運用"
      Enabled         =   0   'False
      Height          =   735
      Index           =   60
      Left            =   11520
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   66
      Top             =   1080
      Width           =   1095
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "特殊指定"
      Enabled         =   0   'False
      Height          =   735
      Index           =   85
      Left            =   11520
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   56
      Top             =   6120
      Width           =   2175
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "↓"
      Enabled         =   0   'False
      Height          =   735
      Index           =   84
      Left            =   11520
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   57
      Top             =   5400
      Width           =   2175
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "→"
      Enabled         =   0   'False
      Height          =   735
      Index           =   83
      Left            =   12600
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   58
      Top             =   4680
      Width           =   1095
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "←"
      Enabled         =   0   'False
      Height          =   735
      Index           =   82
      Left            =   11520
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   59
      Top             =   4680
      Width           =   1095
   End
   Begin VB.CommandButton cmdKeyTop 
      Caption         =   "↑"
      Enabled         =   0   'False
      Height          =   735
      Index           =   81
      Left            =   11520
      Style           =   1  'ｸﾞﾗﾌｨｯｸｽ
      TabIndex        =   60
      Top             =   3960
      Width           =   2175
   End
   Begin VB.Label lblMsg 
      Alignment       =   2  '中央揃え
      BorderStyle     =   1  '実線
      BeginProperty Font 
         Name            =   "ＭＳ Ｐゴシック"
         Size            =   20.25
         Charset         =   128
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H000000FF&
      Height          =   855
      Left            =   240
      TabIndex        =   48
      Top             =   120
      Width           =   10695
   End
   Begin VB.Menu mnuExit 
      Caption         =   "終了"
   End
   Begin VB.Menu mnuReset 
      Caption         =   "リセット"
   End
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Const COLOR_OK_KEY = &H60E060

Private Sub Form_KeyDown(KeyCode As Integer, Shift As Integer)
    Dim idx As Long
    idx = -1
    
    '48キー
    'F単独
    If KeyCode = vbKeyF1 And Shift = 0 Then
        idx = 0
    ElseIf KeyCode = vbKeyF2 And Shift = 0 Then
        idx = 1
    ElseIf KeyCode = vbKeyF3 And Shift = 0 Then
        idx = 2
    ElseIf KeyCode = vbKeyF4 And Shift = 0 Then
        idx = 3
    ElseIf KeyCode = vbKeyF5 And Shift = 0 Then
        idx = 4
    ElseIf KeyCode = vbKeyF6 And Shift = 0 Then
        idx = 5
    ElseIf KeyCode = vbKeyF7 And Shift = 0 Then
        idx = 6
    ElseIf KeyCode = vbKeyF8 And Shift = 0 Then
        idx = 7
    'Shift+Fキー
    ElseIf KeyCode = vbKeyF1 And Shift = (vbShiftMask) Then
        idx = 8
    ElseIf KeyCode = vbKeyF2 And Shift = (vbShiftMask) Then
        idx = 9
    ElseIf KeyCode = vbKeyF3 And Shift = (vbShiftMask) Then
        idx = 10
    ElseIf KeyCode = vbKeyF4 And Shift = (vbShiftMask) Then
        idx = 11
    ElseIf KeyCode = vbKeyF5 And Shift = (vbShiftMask) Then
        idx = 12
    ElseIf KeyCode = vbKeyF6 And Shift = (vbShiftMask) Then
        idx = 13
    ElseIf KeyCode = vbKeyF7 And Shift = (vbShiftMask) Then
        idx = 14
    ElseIf KeyCode = vbKeyF8 And Shift = (vbShiftMask) Then
        idx = 15
    'Ctrl+1～8キー
    ElseIf KeyCode = vbKey1 And Shift = (vbCtrlMask) Then
        idx = 16
    ElseIf KeyCode = vbKey2 And Shift = (vbCtrlMask) Then
        idx = 17
    ElseIf KeyCode = vbKey3 And Shift = (vbCtrlMask) Then
        idx = 18
    ElseIf KeyCode = vbKey4 And Shift = (vbCtrlMask) Then
        idx = 19
    ElseIf KeyCode = vbKey5 And Shift = (vbCtrlMask) Then
        idx = 20
    ElseIf KeyCode = vbKey6 And Shift = (vbCtrlMask) Then
        idx = 21
    ElseIf KeyCode = vbKey7 And Shift = (vbCtrlMask) Then
        idx = 22
    ElseIf KeyCode = vbKey8 And Shift = (vbCtrlMask) Then
        idx = 23
    'Alt+1～8キー
    ElseIf KeyCode = vbKey1 And Shift = (vbAltMask) Then
        idx = 24
    ElseIf KeyCode = vbKey2 And Shift = (vbAltMask) Then
        idx = 25
    ElseIf KeyCode = vbKey3 And Shift = (vbAltMask) Then
        idx = 26
    ElseIf KeyCode = vbKey4 And Shift = (vbAltMask) Then
        idx = 27
    ElseIf KeyCode = vbKey5 And Shift = (vbAltMask) Then
        idx = 28
    ElseIf KeyCode = vbKey6 And Shift = (vbAltMask) Then
        idx = 29
    ElseIf KeyCode = vbKey7 And Shift = (vbAltMask) Then
        idx = 30
    ElseIf KeyCode = vbKey8 And Shift = (vbAltMask) Then
        idx = 31
    'Ctrl+Alt+1～8キー
    ElseIf KeyCode = vbKey1 And Shift = (vbCtrlMask Or vbAltMask) Then
        idx = 32
    ElseIf KeyCode = vbKey2 And Shift = (vbCtrlMask Or vbAltMask) Then
        idx = 33
    ElseIf KeyCode = vbKey3 And Shift = (vbCtrlMask Or vbAltMask) Then
        idx = 34
    ElseIf KeyCode = vbKey4 And Shift = (vbCtrlMask Or vbAltMask) Then
        idx = 35
    ElseIf KeyCode = vbKey5 And Shift = (vbCtrlMask Or vbAltMask) Then
        idx = 36
    ElseIf KeyCode = vbKey6 And Shift = (vbCtrlMask Or vbAltMask) Then
        idx = 37
    ElseIf KeyCode = vbKey7 And Shift = (vbCtrlMask Or vbAltMask) Then
        idx = 38
    ElseIf KeyCode = vbKey8 And Shift = (vbCtrlMask Or vbAltMask) Then
        idx = 39
    'Shift+Ctrl+Alt+1～8キー
    ElseIf KeyCode = vbKey1 And Shift = (vbShiftMask Or vbCtrlMask Or vbAltMask) Then
        idx = 40
    ElseIf KeyCode = vbKey2 And Shift = (vbShiftMask Or vbCtrlMask Or vbAltMask) Then
        idx = 41
    ElseIf KeyCode = vbKey3 And Shift = (vbShiftMask Or vbCtrlMask Or vbAltMask) Then
        idx = 42
    ElseIf KeyCode = vbKey4 And Shift = (vbShiftMask Or vbCtrlMask Or vbAltMask) Then
        idx = 43
    ElseIf KeyCode = vbKey5 And Shift = (vbShiftMask Or vbCtrlMask Or vbAltMask) Then
        idx = 44
    ElseIf KeyCode = vbKey6 And Shift = (vbShiftMask Or vbCtrlMask Or vbAltMask) Then
        idx = 45
    ElseIf KeyCode = vbKey7 And Shift = (vbShiftMask Or vbCtrlMask Or vbAltMask) Then
        idx = 46
    ElseIf KeyCode = vbKey8 And Shift = (vbShiftMask Or vbCtrlMask Or vbAltMask) Then
        idx = 47
    'Ctrl+a～lキー
    ElseIf KeyCode = vbKeyA And Shift = (vbCtrlMask) Then
        idx = 48
    ElseIf KeyCode = vbKeyB And Shift = (vbCtrlMask) Then
        idx = 49
    ElseIf KeyCode = vbKeyC And Shift = (vbCtrlMask) Then
        idx = 50
    ElseIf KeyCode = vbKeyD And Shift = (vbCtrlMask) Then
        idx = 51
    ElseIf KeyCode = vbKeyE And Shift = (vbCtrlMask) Then
        idx = 52
    ElseIf KeyCode = vbKeyF And Shift = (vbCtrlMask) Then
        idx = 53
    ElseIf KeyCode = vbKeyG And Shift = (vbCtrlMask) Then
        idx = 54
    ElseIf KeyCode = vbKeyH And Shift = (vbCtrlMask) Then
        idx = 55
    ElseIf KeyCode = vbKeyI And Shift = (vbCtrlMask) Then
        idx = 56
    ElseIf KeyCode = vbKeyJ And Shift = (vbCtrlMask) Then
        idx = 57
    ElseIf KeyCode = vbKeyK And Shift = (vbCtrlMask) Then
        idx = 58
    ElseIf KeyCode = vbKeyL And Shift = (vbCtrlMask) Then
        idx = 59
    'Alt+a～fキー
    ElseIf KeyCode = vbKeyA And Shift = (vbAltMask) Then
        idx = 60
    ElseIf KeyCode = vbKeyB And Shift = (vbAltMask) Then
        idx = 61
    ElseIf KeyCode = vbKeyC And Shift = (vbAltMask) Then
        idx = 62
    ElseIf KeyCode = vbKeyD And Shift = (vbAltMask) Then
        idx = 63
    ElseIf KeyCode = vbKeyE And Shift = (vbAltMask) Then
        idx = 64
    ElseIf KeyCode = vbKeyF And Shift = (vbAltMask) Then
        idx = 65
    '1～8キー
    ElseIf KeyCode = vbKey0 And Shift = 0 Then
        idx = 66
    ElseIf KeyCode = vbKey1 And Shift = 0 Then
        idx = 67
    ElseIf KeyCode = vbKey2 And Shift = 0 Then
        idx = 68
    ElseIf KeyCode = vbKey3 And Shift = 0 Then
        idx = 69
    ElseIf KeyCode = vbKey4 And Shift = 0 Then
        idx = 70
    ElseIf KeyCode = vbKey5 And Shift = 0 Then
        idx = 71
    ElseIf KeyCode = vbKey6 And Shift = 0 Then
        idx = 72
    ElseIf KeyCode = vbKey7 And Shift = 0 Then
        idx = 73
    ElseIf KeyCode = vbKey8 And Shift = 0 Then
        idx = 74
    ElseIf KeyCode = vbKey9 And Shift = 0 Then
        idx = 75
    ElseIf KeyCode = 189 And Shift = 0 Then
        idx = 76
    ElseIf KeyCode = 106 And Shift = 0 Then
        idx = 77
    ElseIf KeyCode = vbKeyReturn And Shift = 0 Then
        idx = 78
    ElseIf KeyCode = vbKeyPageUp And Shift = 0 Then
        idx = 79
    ElseIf KeyCode = vbKeyPageDown And Shift = 0 Then
        idx = 80
    'カーソルキーなど
    ElseIf KeyCode = vbKeyUp And Shift = 0 Then
        idx = 81
    ElseIf KeyCode = vbKeyLeft And Shift = 0 Then
        idx = 82
    ElseIf KeyCode = vbKeyRight And Shift = 0 Then
        idx = 83
    ElseIf KeyCode = vbKeyDown And Shift = 0 Then
        idx = 84
    ElseIf KeyCode = vbKeyG And Shift = (vbAltMask) Then
        idx = 85
    'Ctrl+Alt+a～gキー
    ElseIf KeyCode = vbKeyA And Shift = (vbCtrlMask Or vbAltMask) Then
        idx = 86
    ElseIf KeyCode = vbKeyB And Shift = (vbCtrlMask Or vbAltMask) Then
        idx = 87
    ElseIf KeyCode = vbKeyC And Shift = (vbCtrlMask Or vbAltMask) Then
        idx = 88
    ElseIf KeyCode = vbKeyD And Shift = (vbCtrlMask Or vbAltMask) Then
        idx = 89
    ElseIf KeyCode = vbKeyE And Shift = (vbCtrlMask Or vbAltMask) Then
        idx = 90
    ElseIf KeyCode = vbKeyF And Shift = (vbCtrlMask Or vbAltMask) Then
        idx = 91
    ElseIf KeyCode = vbKeyG And Shift = (vbCtrlMask Or vbAltMask) Then
        idx = 92
    End If
    
    Debug.Print KeyCode
    If idx >= 0 Then cmdKeyTop(idx).BackColor = COLOR_OK_KEY
End Sub

Private Sub Form_Load()
    ''lblMsg = "重要：必ず操作が動いていない状態でチェックしてください。"
    lblMsg = "重要：必ずPOSが動いていない状態でチェックしてください。"
End Sub

Private Sub mnuExit_Click()
    Unload Me
End Sub

Private Sub mnuReset_Click()
    Dim c
    
    For Each c In cmdKeyTop
        c.BackColor = &H8000000F
    Next c
End Sub
