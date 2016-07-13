Option Explicit
On Error Resume Next

Dim lngLoop         ' ループカウンタ
Dim objIE           ' IE オブジェクト

Set objIE = CreateObject("InternetExplorer.Application")
If Err.Number = 0 Then
    objIE.Width = 180
    objIE.Height = 46
    objIE.AddressBar = False
    objIE.MenuBar = False
    objIE.ToolBar = False
    objIE.Resizable = False
    objIE.Visible = True

    For lngLoop = 0 To 10
        objIE.StatusText = "(" & lngLoop * 10 & "%)" & _
            String(lngLoop, "■") & String(10 - lngLoop, "□")

        ' 適当に待つ
        WScript.Sleep 1000
    Next

    objIE.Quit
End If

Set objIE = Nothing
