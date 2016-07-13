
Set dic = WScript.CreateObject("Scripting.Dictionary")

Set fso = WScript.CreateObject("Scripting.FileSystemObject")

Set re = New RegExp

re.Pattern = """(.+)@(.+)"""
re.IgnoreCase = True

Set ts = fso.OpenTextFile(WScript.Arguments.Item(0))

Do While ts.AtEndOfStream <> True
	l = ts.ReadLine

	Set ms = re.Execute(l)

	dic(ms(0).SubMatches(1)) = dic(ms(0).SubMatches(1)) + 1
Loop

ts.Close

For Each key In dic.Keys
	WScript.Echo dic(key) & "," & key
Next
