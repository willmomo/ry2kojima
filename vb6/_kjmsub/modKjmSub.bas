Attribute VB_Name = "modKjmSub"
'*********************************************************************
' 2009/01/22 r.kojima  CsvSplit�֐��ǉ�
' 2009/01/22 r.kojima  CsvEncode�֐��ǉ�
' 2009/01/22 r.kojima  SQLEncode�֐��ǉ�
'*********************************************************************
Option Explicit

'*********************************************************************
' CSV������ɑς�����Split�֐�
'
' " �ň͂܂ꂽ�t�B�[���h�𐳂����������܂��B
' �G�X�P�[�v���ꂽ " �𐳂������ɖ߂��܂��B
'
' [�o�O]
' ���s�������������Ă��܂���B
' " �ň͂܂ꂽ�t�B�[���h���Ō�ɂ���ƁA���������삵�܂���B
'*********************************************************************
Public Function CsvSplit(expression, delimiter) As String()
    Dim result() As String
    Dim nSize As Long
    Dim pos As Long
    Dim work As String
    Dim strValue As String
    Dim c1 As String
    Dim c2 As String
    
    nSize = 0
    work = expression
    
    Do While work <> ""
        '������̐擪�� " �̎��́A���ꕶ�����܂ރZ��
        If Left(work, 1) = """" Then
            '�΂ɂȂ� " �܂ł�؂肾��
            strValue = ""
            work = Mid(work, 2)
            Do While work <> ""
                c1 = Left(work, 1)
                c2 = Mid(work, 2, 1)
                
                If c1 = """" And c2 = """" Then
                    strValue = strValue & """"
                    work = Mid(work, 3)
                ElseIf c1 = """" And c2 <> """" Then
                    work = Mid(work, 3)
                    Exit Do
                Else
                    strValue = strValue & c1
                    work = Mid(work, 2)
                End If
            Loop
        Else
            pos = InStr(work, delimiter)
            If pos = 0 Then
                '�c��S�����A��̃Z��
                strValue = work
                work = ""
            Else
                strValue = Left(work, pos - 1)
                work = Mid(work, pos + 1)
            End If
        End If
        
        ReDim Preserve result(nSize)
        result(nSize) = strValue
        nSize = nSize + 1
    Loop
    
    CsvSplit = result
End Function

'*********************************************************************
' �������CSV���o�͂ɑς�����`�ɃG���R�[�h����
'
' ������� , ���� " ���܂܂�鎞�́A������S�̂� " �ň͂��܂��B
' �����񒆂� " �� "" �ɕϊ����܂��B
'
' [�o�O]
' ���s�������������Ă��܂���B
'*********************************************************************
Public Function CsvEncode(strValue As String) As String
    Dim strResult As String
    Dim bFlag As Boolean
    
    bFlag = False
    strResult = strValue
    
    If InStr(strResult, """") > 0 Then
        bFlag = True
        strResult = Replace(strResult, """", """""")
    End If
    
    If InStr(strResult, ",") > 0 Then
        bFlag = True
    End If
    
    If bFlag Then
        strResult = """" & strResult & """"
    End If
    
    CsvEncode = strResult
End Function

'*********************************************************************
' �������SQL���o�͂ɑς�����`�ɃG���R�[�h����
'
' ������� ' ���܂܂�鎞�́A'' �ɕϊ�����B
'*********************************************************************
Public Function SQLEncode(strValue As String) As String
    Dim strResult As String
    Dim bFlag As Boolean
    
    bFlag = False
    strResult = strValue
    
    strResult = Replace(strResult, "'", "''")
    
    SQLEncode = strResult
End Function


