Set obj = CreateObject("basp21")

data = obj.BinaryRead("D:\work\ComSampleUser\Project1.exe")

'MsgBox(TypeName(data))
'MsgBox(LBound(data) & " " & UBound(data))

Set obj = CreateObject("ComSample.SharedMem")

obj.Open "okok.bin"
data = obj.ReadByte(128)
obj.Close

MsgBox(TypeName(data) & " " & data(0) & " " & data(127))
