Attribute VB_Name = "mdlSafeConvert"
'----------------------------------------------------------------------
'   [概要]
'   安全に型変換する関数群
'
'   [更新日]
'   2003/11/01 : 新規作成

Option Explicit

'----------------------------------------------------------------------
'   [概要]
'   安全なCInt
'
'   [引数]
'   expression  : 変換元式
'   intValue    : 変換後値
'
'   [戻り値]
'   0   : 変換に成功
'   !0  : 変換に失敗(エラーコード)
'
'   [説明]
'   expressionが正しく変換できたときは、0を戻し、intValueに変換後の
'   値が入ります。
'
'   正しく変換できないときは、0以外を戻し、intValueは変化しません。
'   エラーの詳細は、戻り値をError関数に渡すことでわかります。

Public Function SafeCInt(expression, intValue As Integer) As Long
    On Error GoTo Err_SafeCInt
    
    intValue = CInt(expression)
    SafeCInt = 0
    Exit Function
    
Err_SafeCInt:
    SafeCInt = Err.Number
End Function

'----------------------------------------------------------------------
'   [概要]
'   安全なCLng
'
'   [引数]
'   expression  : 変換元式
'   lngValue    : 変換後値
'
'   [戻り値]
'   0   : 変換に成功
'   !0  : 変換に失敗(エラーコード)
'
'   [説明]
'   expressionが正しく変換できたときは、0を戻し、lngValueに変換後の値が
'   入ります。
'
'   正しく変換できないときは、0以外を戻し、lngValueは変化しません。
'   エラーの詳細は、戻り値をError関数に渡すことでわかります。

Public Function SafeCLng(expression, lngValue As Long) As Long
    On Error GoTo Err_SafeCLng
    
    lngValue = CLng(expression)
    SafeCLng = 0
    Exit Function
    
Err_SafeCLng:
    SafeCLng = Err.Number
End Function
