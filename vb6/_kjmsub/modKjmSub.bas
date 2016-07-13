Attribute VB_Name = "modKjmSub"
'*********************************************************************
' 2009/01/22 r.kojima  CsvSplit関数追加
' 2009/01/22 r.kojima  CsvEncode関数追加
' 2009/01/22 r.kojima  SQLEncode関数追加
'*********************************************************************
Option Explicit

'*********************************************************************
' CSV文字列に耐えられるSplit関数
'
' " で囲まれたフィールドを正しく抜き取ります。
' エスケープされた " を正しく元に戻します。
'
' [バグ]
' 改行文字を処理していません。
' " で囲まれたフィールドが最後にくると、正しく動作しません。
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
        '文字列の先頭が " の時は、特殊文字を含むセル
        If Left(work, 1) = """" Then
            '対になる " までを切りだす
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
                '残り全部が、一つのセル
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
' 文字列をCSV入出力に耐えられる形にエンコードする
'
' 文字列に , 又は " が含まれる時は、文字列全体を " で囲います。
' 文字列中の " は "" に変換します。
'
' [バグ]
' 改行文字を処理していません。
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
' 文字列をSQL入出力に耐えられる形にエンコードする
'
' 文字列に ' が含まれる時は、'' に変換する。
'*********************************************************************
Public Function SQLEncode(strValue As String) As String
    Dim strResult As String
    Dim bFlag As Boolean
    
    bFlag = False
    strResult = strValue
    
    strResult = Replace(strResult, "'", "''")
    
    SQLEncode = strResult
End Function


