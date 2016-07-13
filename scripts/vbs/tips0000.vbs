Dim IE
Set IE = WScript.CreateObject("InternetExplorer.Application")

IE.Navigate "about:blank"
While IE.busy: Wend
While IE.Document.readyState <> "complete": DoEvents : Wend
IE.Document.body.innerHTML = "<b id=""msg"">ˆ—’†‚Å‚·@‚¨‘Ò‚¿‰º‚³‚¢</b>"
IE.AddressBar = False
IE.ToolBar = False
IE.StatusBar = False
IE.Height = 100
IE.Width = 300
IE.Visible = True
WScript.Sleep(3000) 'ÀÛ‚ÍŠÔ‚ÌŠ|‚©‚éˆ—
IE.Document.getElementById("msg").innerHTML="Š®—¹‚µ‚Ü‚µ‚½"
'IE.Quit
