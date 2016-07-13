Attribute VB_Name = "Module1"
Option Explicit


Private Type VERSION_INFO
    m_strPrefix As String
    m_nMajor As Long
    m_nMinor1 As Long
    m_nMinor2 As Long
    m_nRevision As Long
    m_strSuffix As String
End Type

Private Function ParseVersion(strVer As String, verInfo As VERSION_INFO) As Boolean
    On Error GoTo ErrHandler
    
    Dim arVer As Variant
    Dim n As Long
    
    ParseVersion = False
    
    arVer = Split(strVer, ".")
    If UBound(arVer) <> 3 Then Exit Function
    
    '数字以外が続く間は、Prefix。文字がとぎれたらMajor
    For n = 1 To Len(arVer(0))
        If IsNumeric(Mid(arVer(0), n, 1)) Then Exit For
        verInfo.m_strPrefix = verInfo.m_strPrefix & Mid(arVer(0), n)
    Next
    
    If n < Len(arVer(0)) Then
        verInfo.m_nMajor = CLng(Mid(arVer(0), n))
    End If
    
    verInfo.m_nMinor1 = CLng(arVer(1))
    verInfo.m_nMinor2 = CLng(arVer(2))
    
    '数字が続く間は、Revision。数字がとぎれたらSuffix
    For n = 1 To Len(arVer(3))
        If Not IsNumeric(Mid(arVer(3), n, 1)) Then Exit For
        verInfo.m_nRevision = verInfo.m_nRevision * 10 + CLng(Mid(arVer(3), n, 1))
    Next
    
    If n < Len(arVer(3)) Then
        verInfo.m_strSuffix = Trim(Mid(arVer(3), n))
    End If
    
    ParseVersion = True
    Exit Function
    
ErrHandler:
    Exit Function
    
End Function

Private Function CompareFunc(lhs As String, rhs As String) As Boolean
    Dim lhsTemp As String
    Dim rhsTemp As String
    Dim lhsVer As VERSION_INFO
    Dim rhsVer As VERSION_INFO
    
    lhsTemp = UCase(lhs)
    rhsTemp = UCase(rhs)
    
    If ParseVersion(lhsTemp, lhsVer) = True And ParseVersion(rhsTemp, rhsVer) = True Then
        If lhsVer.m_strPrefix < rhsVer.m_strPrefix Then
            CompareFunc = True
        ElseIf lhsVer.m_strPrefix > rhsVer.m_strPrefix Then
            CompareFunc = False
        Else
            If lhsVer.m_nMajor < rhsVer.m_nMajor Then
                CompareFunc = True
            ElseIf lhsVer.m_nMajor > rhsVer.m_nMajor Then
                CompareFunc = False
            Else
                If lhsVer.m_nMinor1 < rhsVer.m_nMinor1 Then
                    CompareFunc = True
                ElseIf lhsVer.m_nMinor1 > rhsVer.m_nMinor1 Then
                    CompareFunc = False
                Else
                    If lhsVer.m_nMinor2 < rhsVer.m_nMinor2 Then
                        CompareFunc = True
                    ElseIf lhsVer.m_nMinor2 > rhsVer.m_nMinor2 Then
                        CompareFunc = False
                    Else
                        If lhsVer.m_nRevision < rhsVer.m_nRevision Then
                            CompareFunc = True
                        ElseIf lhsVer.m_nRevision > rhsVer.m_nRevision Then
                            CompareFunc = False
                        Else
                            If lhsVer.m_strSuffix < rhsVer.m_strSuffix Then
                                CompareFunc = True
                            ElseIf lhsVer.m_strSuffix > rhsVer.m_strSuffix Then
                                CompareFunc = False
                            Else
                                CompareFunc = False
                            End If
                        End If
                    End If
                End If
            End If
        End If
    Else
        If UCase(lhs) < UCase(rhs) Then
            CompareFunc = True
        Else
            CompareFunc = False
        End If
    End If
End Function

Public Sub BubbleSort(sArray() As String)
    Dim nLoop1 As Long
    Dim nLoop2 As Long
    Dim sTemp As String
    
    For nLoop1 = UBound(sArray) To LBound(sArray) Step -1
        For nLoop2 = LBound(sArray) + 1 To nLoop1
            
            '比較処理
            If CompareFunc(sArray(nLoop2 - 1), sArray(nLoop2)) Then
                sTemp = sArray(nLoop2 - 1)
                sArray(nLoop2 - 1) = sArray(nLoop2)
                sArray(nLoop2) = sTemp
            End If
        Next
    Next
End Sub
