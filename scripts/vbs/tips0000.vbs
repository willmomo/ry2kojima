Dim IE
Set IE = WScript.CreateObject("InternetExplorer.Application")

IE.Navigate "about:blank"
While IE.busy: Wend
While IE.Document.readyState <> "complete": DoEvents : Wend
IE.Document.body.innerHTML = "<b id=""msg"">�������ł��@���҂�������</b>"
IE.AddressBar = False
IE.ToolBar = False
IE.StatusBar = False
IE.Height = 100
IE.Width = 300
IE.Visible = True
WScript.Sleep(3000) '���ۂ͎��Ԃ̊|���鏈��
IE.Document.getElementById("msg").innerHTML="�������܂���"
'IE.Quit
