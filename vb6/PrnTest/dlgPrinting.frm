VERSION 5.00
Begin VB.Form dlgPrinting 
   BorderStyle     =   3  '固定ﾀﾞｲｱﾛｸﾞ
   Caption         =   "印刷中"
   ClientHeight    =   1755
   ClientLeft      =   2760
   ClientTop       =   3750
   ClientWidth     =   3915
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   1755
   ScaleWidth      =   3915
   ShowInTaskbar   =   0   'False
   Begin VB.CommandButton cmdAbort 
      Caption         =   "印刷中止"
      Height          =   495
      Left            =   1350
      TabIndex        =   0
      Top             =   1080
      Width           =   1215
   End
   Begin VB.Label lblMsg 
      Alignment       =   2  '中央揃え
      Height          =   255
      Left            =   150
      TabIndex        =   1
      Top             =   480
      Width           =   3615
   End
End
Attribute VB_Name = "dlgPrinting"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'----------------------------------------------------------------------
' 印刷中断フォームつき印刷処理
'----------------------------------------------------------------------
Option Explicit

Private m_abort As Boolean

'----------------------------------------------------------------------
' 印刷可能枠を描画
'
' デバッグで描画範囲を確認するための関数
'----------------------------------------------------------------------
Private Sub DrawPrintableRect(prn As Printer)
    ''prn.Line (0, 0)-(prn.ScaleWidth - prn.TwipsPerPixelX, prn.ScaleHeight - prn.TwipsPerPixelY), RGB(0, 0, 0), B
End Sub

'----------------------------------------------------------------------
' vbNewLineで区切られた文字列をプリンタに出力する。
'
' @param strText 印刷する文字列(行をvbNewLineで区切る)
'
' @return True  印刷が完了した
' @return False 印刷が中断された
'----------------------------------------------------------------------
Public Function PrintString(ByVal strText As String, Optional owner As Form = Nothing) As Boolean
    On Error GoTo ErrHandler
    
    m_abort = False
    
    'ownerが指定されているときは、印刷ダイアログ表示
    If Not (owner Is Nothing) Then
        owner.Enabled = False
        Me.Show vbModeless, owner
        DoEvents
    End If
    
    '文字列をvbNewLineで分割する
    Dim lines() As String
    lines = Split(strText, vbNewLine)
    
    '最大ページ数を計算する
    Dim maxPage As Long
    Dim maxLineInPage As Long    '1ページに収まる行数
    maxLineInPage = Printer.ScaleHeight \ Printer.TextHeight(lines(0))
    maxPage = (UBound(lines) + 1) \ maxLineInPage + IIf((UBound(lines) + 1) Mod maxLineInPage, 1, 0)
    
    '行の数だけ、印刷を繰り返す。
    Dim l As Long
    For l = LBound(lines) To UBound(lines)
    
        '出力行がページに収まらないときは、改ページを行う。
        If Printer.CurrentY + Printer.TextHeight(lines(l)) > Printer.ScaleHeight Then
            DrawPrintableRect Printer
            Printer.NewPage
        End If
        
        'ダイアログ表示中は、印刷中メッセージ更新
        If Not (owner Is Nothing) Then
            lblMsg.Caption = CStr(Printer.Page) & " / " & CStr(maxPage) & " ページ、印刷中・・・"
        End If
        
        '印刷
        Printer.Print lines(l)
        
        '[印刷中止]ボタンを監視
        '印刷中止ボタンを押されたときは、プリンタエラーを発生させる。
        '(プリンタスプーラで中断したときの同じ動作になる)
        DoEvents
        If m_abort Then
            Err.Raise 482
        End If
    Next
    
    '印刷完了
    DrawPrintableRect Printer
    Printer.EndDoc
    
    '印刷中ダイアログを消す。
    If Not (owner Is Nothing) Then
        owner.Enabled = True
        Unload Me
    End If
    
    PrintString = True
    Exit Function
    
ErrHandler:
    'エラー発生時は、印刷を中断。
    'プリンタスプーラで印刷を中断したときも、ここが走る。
    Printer.KillDoc
    If Not (owner Is Nothing) Then
        owner.Enabled = True
        Unload Me
    End If
    
    PrintString = False
End Function

'----------------------------------------------------------------------
' 印刷中断ボタンクリック
'----------------------------------------------------------------------
Private Sub cmdAbort_Click()
    m_abort = True
End Sub
