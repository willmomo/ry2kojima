Option Explicit
'MsgBox GetOSType()
'MsgBox Mid(GetOSVer(), 1, 3)
MsgBox GetOSVer
 
'-----------------------------------------------------------
' OSのバージョン番号を返す
'
' 5.1.2600	Windows XP
' 6.1.7601	Windows 7
'-----------------------------------------------------------
Function GetOSVer()
	Dim objWMIService
	Dim objComputer
	Dim colComputers

	Set objWMIService = GetObject("winmgmts:{impersonationLevel=impersonate}!\\.\root\cimv2")
	Set colComputers = objWMIService.ExecQuery("Select * from Win32_OperatingSystem")
	For Each objComputer In colComputers
		GetOSVer = objComputer.Version
		Exit For
	Next
End Function

'-----------------------------------------------------------
'処理内容：OS情報の取得
'戻り値：OS情報(SP含む)
'-----------------------------------------------------------
Function GetOSType()
	On Error Resume Next
	Err.Clear
	Dim objWMIService
	Dim objComputer
	Dim colComputers
	Dim OsVal
	Dim szTmp
	Dim strRet
	Dim strOSAr
	szTmp=""
	strRet=""
	strOSAr=""
 
	Set objWMIService = GetObject("winmgmts:{impersonationLevel=impersonate}!\\.\root\cimv2")
	Set colComputers = objWMIService.ExecQuery("Select * from Win32_OperatingSystem")
	For Each objComputer in colComputers
		szTmp = objComputer.Version
		OsVal = Mid(szTmp,1,3)
		If OsVal = "5.0" Then
			strRet = "Windows 2000 "
		ElseIf OsVal = "5.1" Then
			strRet = "Windows XP "
		ElseIf OsVal = "5.2" Then
			strRet = "Windows Server 2003 "
		ElseIf OsVal = "6.0" Then
			If InStr(UCase(objComputer.Caption),"SERVER") > 1 Then
				strRet = "Windows Server 2008 "
			Else
				strRet = "Windows Vista "
			End If
		ElseIf OsVal = "6.1" Then
			If InStr(UCase(objComputer.Caption),"SERVER") > 1 Then
				strRet = "Windows Server 2008 R2 "
			Else
				strRet = "Windows 7 "
			End If
		ElseIf OsVal = "6.2" Then
			If InStr(UCase(objComputer.Caption),"SERVER") > 1 Then
				strRet = "Windows Server 2012 "
			Else
				strRet = "Windows 8 "
			End IF
		Else
			strRet = "Windows " & "(" & szTmp & ") "
		End If
		strRet = strRet & "SP" & objComputer.ServicePackMajorVersion & "." & objComputer.ServicePackMinorVersion
		strOSAr = ""
		strOSAr = objComputer.OSArchitecture
		If strOSAr = "" Or IsNull(strOSAr) Then
			strOSAr = "32ビット"
		End If
		strRet = strRet & " " &strOSAr
	Next
	GetOSType = strRet
	Err.Clear
End Function
