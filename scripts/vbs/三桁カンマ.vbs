Dim objRE
Set objRE = new RegExp
objRE.Pattern = "(\d)(?=(\d{3})+$)"
objRE.Global = True
WScript.Echo objRE.Replace("1234567890", "$1,")
