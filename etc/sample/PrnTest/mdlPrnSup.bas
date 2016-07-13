Attribute VB_Name = "mdlPrnSup"
Option Explicit

Private Declare Function GetDeviceCaps Lib "gdi32" (ByVal hdc As Long, ByVal nIndex As Long) As Long

Private Const LOGPIXELSY = 90        '  Logical pixels/inch in Y

Private Function CurXY(prn As Printer) As String
    CurXY = "(" & prn.CurrentX & "," & prn.CurrentY & ") (pg:" & prn.Page & ")"
End Function

Public Sub DumPrint1()
    
    Printer.Print CurXY(Printer) & " ☆ ページ長を無視して、出力しまくる ☆"
    Printer.Print CurXY(Printer) & " DeviceName: " & Printer.DeviceName
    Printer.Print CurXY(Printer) & " PaperSize: " & Printer.PaperSize
    Printer.Print CurXY(Printer) & " 幅・高: " & Printer.Width & " , " & Printer.Height
    Printer.Print CurXY(Printer) & " 印刷範囲(左上): (" & Printer.ScaleLeft & " , " & Printer.ScaleTop & ")"
    Printer.Print CurXY(Printer) & " 印刷範囲(幅高): (" & Printer.ScaleWidth & " , " & Printer.ScaleHeight & ")"
    Printer.Print CurXY(Printer) & " 文字サイズ(point): " & Printer.FontSize
    Printer.Print CurXY(Printer) & " 文字サイズ(dot): " & CStr(Printer.FontSize * GetDeviceCaps(Printer.hdc, LOGPIXELSY) / 72)
    Printer.Print CurXY(Printer) & " 文字サイズ(twips): " & CStr((Printer.FontSize * GetDeviceCaps(Printer.hdc, LOGPIXELSY) / 72) * Printer.TwipsPerPixelY)
    
    Dim msg As String
    Dim l As Long
    For l = 0 To 100
        msg = CurXY(Printer) & " line: " & CStr(l + 1)
        Printer.Print msg & " NextY = " & CStr(Printer.CurrentY + Printer.TextHeight(msg))
    Next
    Printer.EndDoc
End Sub

Public Sub DumPrint2()
    
    Printer.Print CurXY(Printer) & " ☆ ページ長をオーバーする行の前に NewPage を実行する ☆"
    Printer.Print CurXY(Printer) & " DeviceName: " & Printer.DeviceName
    Printer.Print CurXY(Printer) & " PaperSize: " & Printer.PaperSize
    Printer.Print CurXY(Printer) & " 幅・高: " & Printer.Width & " , " & Printer.Height
    Printer.Print CurXY(Printer) & " 印刷範囲(左上): (" & Printer.ScaleLeft & " , " & Printer.ScaleTop & ")"
    Printer.Print CurXY(Printer) & " 印刷範囲(幅高): (" & Printer.ScaleWidth & " , " & Printer.ScaleHeight & ")"
    Printer.Print CurXY(Printer) & " 文字サイズ(point): " & Printer.FontSize
    Printer.Print CurXY(Printer) & " 文字サイズ(dot): " & CStr(Printer.FontSize * GetDeviceCaps(Printer.hdc, LOGPIXELSY) / 72)
    Printer.Print CurXY(Printer) & " 文字サイズ(twips): " & CStr((Printer.FontSize * GetDeviceCaps(Printer.hdc, LOGPIXELSY) / 72) * Printer.TwipsPerPixelY)
    
    Dim msg As String
    Dim l As Long
    For l = 0 To 100
        msg = CurXY(Printer) & " line: " & CStr(l + 1)
        
        If Printer.CurrentY + Printer.TextHeight(msg) > Printer.ScaleHeight Then
            Printer.NewPage
            msg = CurXY(Printer) & " line: " & CStr(l + 1)
        End If
        
        Printer.Print msg & " NextY = " & CStr(Printer.CurrentY + Printer.TextHeight(msg))
    Next
    Printer.EndDoc
End Sub


Public Function DumPrint3() As String
    
    Dim work As String
    
    work = " ☆ テキストバッファを渡して、改ページを考慮して印刷 ☆" & vbNewLine
    work = work & " DeviceName: " & Printer.DeviceName & vbNewLine
    work = work & " PaperSize: " & Printer.PaperSize & vbNewLine
    work = work & " 幅・高: " & Printer.Width & " , " & Printer.Height & vbNewLine
    work = work & " 印刷範囲(左上): (" & Printer.ScaleLeft & " , " & Printer.ScaleTop & ")" & vbNewLine
    work = work & " 印刷範囲(幅高): (" & Printer.ScaleWidth & " , " & Printer.ScaleHeight & ")" & vbNewLine
    work = work & " 文字サイズ(point): " & Printer.FontSize & vbNewLine
    work = work & " 文字サイズ(dot): " & CStr(Printer.FontSize * GetDeviceCaps(Printer.hdc, LOGPIXELSY) / 72) & vbNewLine
    work = work & " 文字サイズ(twips): " & CStr((Printer.FontSize * GetDeviceCaps(Printer.hdc, LOGPIXELSY) / 72) * Printer.TwipsPerPixelY) & vbNewLine
    
    Dim msg As String
    Dim l As Long
    For l = 0 To 100
        msg = " line: " & CStr(l + 1)
        work = work & msg & vbNewLine
    Next
    
    DumPrint3 = work
End Function

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
    
    Dim abortDialog As dlgAbort
    Set abortDialog = New dlgAbort
    
    '印刷ダイアログ表示
    If Not (owner Is Nothing) Then
        Set abortDialog = New dlgAbort
        owner.Enabled = False
        abortDialog.Show vbModeless, owner
        DoEvents
    End If
    
    '文字列をvbNewLineで分割する
    Dim lines() As String
    lines = Split(strText, vbNewLine)
    
    '分割された行数だけ、印刷を繰り返す。
    Dim l As Long
    For l = LBound(lines) To UBound(lines)
    
        '出力行がページに収まらないときは、改ページを行う。
        If Printer.CurrentY + Printer.TextHeight(lines(l)) > Printer.ScaleHeight Then
            Printer.NewPage
        End If
        
        '印刷中メッセージ更新
        If Not (owner Is Nothing) Then
            abortDialog.lblMsg.Caption = CStr(Printer.Page) & " ページ、印刷中・・・"
        End If
        
        '印刷
        Printer.Print lines(l)
        
        '[印刷中止]ボタンを監視
        '印刷中止ボタンを押されたときは、プリンタエラーを発生させる。
        '(プリンタスプーラで中断したときの同じ動作になる)
        DoEvents
        If abortDialog.m_abort Then
            Err.Raise 482
        End If
    Next
    
    '印刷完了
    Printer.EndDoc
    
    '印刷中ダイアログを消す。
    If Not (owner Is Nothing) Then
        owner.Enabled = True
    End If
    
    PrintString = True
    Exit Function
    
ErrHandler:
    'エラー発生時は、印刷を中断。
    'プリンタスプーラで印刷を中断したときも、ここが走る。
    Printer.KillDoc
    If Not (owner Is Nothing) Then
        Set abortDialog = Nothing
        owner.Enabled = True
    End If
    
    PrintString = False
End Function

