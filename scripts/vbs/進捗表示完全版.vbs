'IEキー入力.vbs
'Copyright (c) 2014 nezuq
'This software is released under the MIT License.
'http://opensource.org/licenses/mit-license.php

'シェルを起動する
Dim wsh
Set wsh = WScript.CreateObject("WScript.Shell")

'IEを起動する
Dim ie 
Set ie = CreateObject("InternetExplorer.Application")
'ie.Visible = True
'ie.FullScreen = True

'IEをアクティブにする
Dim wLoc, wSvc, wEnu, wIns
Set wLoc = CreateObject("WbemScripting.SWbemLocator")
Set wSvc = wLoc.ConnectServer
Set wEnu = wSvc.InstancesOf("Win32_Process")
Dim pId
For Each wIns in wEnu
    If Not IsEmpty(wIns.ProcessId) And wIns.Description = "iexplore.exe" Then
        pId = wIns.ProcessId
    End If
Next
Set wLoc = Nothing
Set wEnu = Nothing
Set wSvc = Nothing
While not wsh.AppActivate(pId) 
    Wscript.Sleep 100 
Wend 

'IEで指定URLを開く
IE.Navigate "about:blank"
While IE.busy: Wend
While IE.Document.readyState <> "complete": DoEvents : Wend
IE.Document.Title = WScript.ScriptFullName & " 実行中"
IE.AddressBar = False
IE.ToolBar = False
IE.StatusBar = False
IE.Width = 800
IE.Height = 200
IE.Visible = True

IE.Document.body.innerHTML = Now & " program start"

For i = 0 To 20
	WScript.Sleep(1000) '実際は時間の掛かる処理
	'IE.Document.body.innerHTML = Now & " 進行中 " & i & " <br> " & IE.Document.body.innerHTML
	IE.Document.body.innerHTML = IE.Document.body.innerHTML & "<br>" & Now & " 進行中 " & i
	IE.Document.Script.setTimeout "javascript:scrollTo(0," & IE.document.body.scrollheight & ");",100
Next

IE.Quit
