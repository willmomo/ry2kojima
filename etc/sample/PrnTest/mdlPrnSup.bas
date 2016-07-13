Attribute VB_Name = "mdlPrnSup"
Option Explicit

Private Declare Function GetDeviceCaps Lib "gdi32" (ByVal hdc As Long, ByVal nIndex As Long) As Long

Private Const LOGPIXELSY = 90        '  Logical pixels/inch in Y

Private Function CurXY(prn As Printer) As String
    CurXY = "(" & prn.CurrentX & "," & prn.CurrentY & ") (pg:" & prn.Page & ")"
End Function

Public Sub DumPrint1()
    
    Printer.Print CurXY(Printer) & " �� �y�[�W���𖳎����āA�o�͂��܂��� ��"
    Printer.Print CurXY(Printer) & " DeviceName: " & Printer.DeviceName
    Printer.Print CurXY(Printer) & " PaperSize: " & Printer.PaperSize
    Printer.Print CurXY(Printer) & " ���E��: " & Printer.Width & " , " & Printer.Height
    Printer.Print CurXY(Printer) & " ����͈�(����): (" & Printer.ScaleLeft & " , " & Printer.ScaleTop & ")"
    Printer.Print CurXY(Printer) & " ����͈�(����): (" & Printer.ScaleWidth & " , " & Printer.ScaleHeight & ")"
    Printer.Print CurXY(Printer) & " �����T�C�Y(point): " & Printer.FontSize
    Printer.Print CurXY(Printer) & " �����T�C�Y(dot): " & CStr(Printer.FontSize * GetDeviceCaps(Printer.hdc, LOGPIXELSY) / 72)
    Printer.Print CurXY(Printer) & " �����T�C�Y(twips): " & CStr((Printer.FontSize * GetDeviceCaps(Printer.hdc, LOGPIXELSY) / 72) * Printer.TwipsPerPixelY)
    
    Dim msg As String
    Dim l As Long
    For l = 0 To 100
        msg = CurXY(Printer) & " line: " & CStr(l + 1)
        Printer.Print msg & " NextY = " & CStr(Printer.CurrentY + Printer.TextHeight(msg))
    Next
    Printer.EndDoc
End Sub

Public Sub DumPrint2()
    
    Printer.Print CurXY(Printer) & " �� �y�[�W�����I�[�o�[����s�̑O�� NewPage �����s���� ��"
    Printer.Print CurXY(Printer) & " DeviceName: " & Printer.DeviceName
    Printer.Print CurXY(Printer) & " PaperSize: " & Printer.PaperSize
    Printer.Print CurXY(Printer) & " ���E��: " & Printer.Width & " , " & Printer.Height
    Printer.Print CurXY(Printer) & " ����͈�(����): (" & Printer.ScaleLeft & " , " & Printer.ScaleTop & ")"
    Printer.Print CurXY(Printer) & " ����͈�(����): (" & Printer.ScaleWidth & " , " & Printer.ScaleHeight & ")"
    Printer.Print CurXY(Printer) & " �����T�C�Y(point): " & Printer.FontSize
    Printer.Print CurXY(Printer) & " �����T�C�Y(dot): " & CStr(Printer.FontSize * GetDeviceCaps(Printer.hdc, LOGPIXELSY) / 72)
    Printer.Print CurXY(Printer) & " �����T�C�Y(twips): " & CStr((Printer.FontSize * GetDeviceCaps(Printer.hdc, LOGPIXELSY) / 72) * Printer.TwipsPerPixelY)
    
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
    
    work = " �� �e�L�X�g�o�b�t�@��n���āA���y�[�W���l�����Ĉ�� ��" & vbNewLine
    work = work & " DeviceName: " & Printer.DeviceName & vbNewLine
    work = work & " PaperSize: " & Printer.PaperSize & vbNewLine
    work = work & " ���E��: " & Printer.Width & " , " & Printer.Height & vbNewLine
    work = work & " ����͈�(����): (" & Printer.ScaleLeft & " , " & Printer.ScaleTop & ")" & vbNewLine
    work = work & " ����͈�(����): (" & Printer.ScaleWidth & " , " & Printer.ScaleHeight & ")" & vbNewLine
    work = work & " �����T�C�Y(point): " & Printer.FontSize & vbNewLine
    work = work & " �����T�C�Y(dot): " & CStr(Printer.FontSize * GetDeviceCaps(Printer.hdc, LOGPIXELSY) / 72) & vbNewLine
    work = work & " �����T�C�Y(twips): " & CStr((Printer.FontSize * GetDeviceCaps(Printer.hdc, LOGPIXELSY) / 72) * Printer.TwipsPerPixelY) & vbNewLine
    
    Dim msg As String
    Dim l As Long
    For l = 0 To 100
        msg = " line: " & CStr(l + 1)
        work = work & msg & vbNewLine
    Next
    
    DumPrint3 = work
End Function

'----------------------------------------------------------------------
' vbNewLine�ŋ�؂�ꂽ��������v�����^�ɏo�͂���B
'
' @param strText ������镶����(�s��vbNewLine�ŋ�؂�)
'
' @return True  �������������
' @return False ��������f���ꂽ
'----------------------------------------------------------------------
Public Function PrintString(ByVal strText As String, Optional owner As Form = Nothing) As Boolean
    On Error GoTo ErrHandler
    
    Dim abortDialog As dlgAbort
    Set abortDialog = New dlgAbort
    
    '����_�C�A���O�\��
    If Not (owner Is Nothing) Then
        Set abortDialog = New dlgAbort
        owner.Enabled = False
        abortDialog.Show vbModeless, owner
        DoEvents
    End If
    
    '�������vbNewLine�ŕ�������
    Dim lines() As String
    lines = Split(strText, vbNewLine)
    
    '�������ꂽ�s�������A������J��Ԃ��B
    Dim l As Long
    For l = LBound(lines) To UBound(lines)
    
        '�o�͍s���y�[�W�Ɏ��܂�Ȃ��Ƃ��́A���y�[�W���s���B
        If Printer.CurrentY + Printer.TextHeight(lines(l)) > Printer.ScaleHeight Then
            Printer.NewPage
        End If
        
        '��������b�Z�[�W�X�V
        If Not (owner Is Nothing) Then
            abortDialog.lblMsg.Caption = CStr(Printer.Page) & " �y�[�W�A������E�E�E"
        End If
        
        '���
        Printer.Print lines(l)
        
        '[������~]�{�^�����Ď�
        '������~�{�^���������ꂽ�Ƃ��́A�v�����^�G���[�𔭐�������B
        '(�v�����^�X�v�[���Œ��f�����Ƃ��̓�������ɂȂ�)
        DoEvents
        If abortDialog.m_abort Then
            Err.Raise 482
        End If
    Next
    
    '�������
    Printer.EndDoc
    
    '������_�C�A���O�������B
    If Not (owner Is Nothing) Then
        owner.Enabled = True
    End If
    
    PrintString = True
    Exit Function
    
ErrHandler:
    '�G���[�������́A����𒆒f�B
    '�v�����^�X�v�[���ň���𒆒f�����Ƃ����A����������B
    Printer.KillDoc
    If Not (owner Is Nothing) Then
        Set abortDialog = Nothing
        owner.Enabled = True
    End If
    
    PrintString = False
End Function

