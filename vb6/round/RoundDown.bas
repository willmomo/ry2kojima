Attribute VB_Name = "Module1"
Option Explicit

'----------------------------------------------------------------------
'[概要]
'   数値を指定された桁数で切り捨てます。
'
'[引数]
'   expression          切り捨ての対象となる実数値を指定します。
'   numdecimalplaces    expressionを切り捨てた結果の桁数を指定します。
'
'[説明]
'   ExcelのROUNDDOWN関数と同等の動きをします。
'   numdecimalplacesに正の数を指定すると、数値 は小数点の右(小数点以下)
'   の指定した桁で切り捨てられます。
'   numdecimalplacesに0(ゼロ)を指定するか、または省略すると、expression
'   は最も近い整数に切り捨てられます。
'   numdecimalplacesに負の数を指定すると、expressionは小数点の左(整数部
'   分)の指定した桁で切り捨てられます。

Public Function RoundDown(expression, Optional numdecimalplaces = 0)
    Dim lngInflate As Long
    Dim i As Long
    
    '桁数での切り捨て処理のため下駄を用意
    lngInflate = 1
    For i = 1 To Abs(numdecimalplaces)
        lngInflate = lngInflate * 10
    Next i
    
    '基本はFix関数を使って切り捨て
    If numdecimalplaces > 0 Then
        RoundDown = Fix(expression * lngInflate) / lngInflate
    Else
        RoundDown = Fix(expression / lngInflate) * lngInflate
    End If
End Function

