Dim IE
Set IE = WScript.CreateObject("InternetExplorer.Application")

IE.Navigate "about:blank"
While IE.busy: Wend
While IE.Document.readyState <> "complete": DoEvents : Wend
IE.Document.body.innerHTML = "<b id=""msg"">処理中です　お待ち下さい</b>"
IE.AddressBar = False
IE.ToolBar = False
IE.StatusBar = False
IE.Height = 100
IE.Width = 300
IE.Visible = True
WScript.Sleep(3000) '実際は時間の掛かる処理
IE.Document.getElementById("msg").innerHTML="完了しました"
'IE.Quit
