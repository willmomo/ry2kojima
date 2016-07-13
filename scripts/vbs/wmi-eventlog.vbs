''
''	WMI-EVENTLOG.VBS
''	(c) 2003 Software Poetry, Inc.
''	http://www.softwarepoetry.com/webob
''
''	Reads out event log entries and spews them as XML. Keeps track of
''	the logs it has seen by timestamp, so it works indpendent of event
''	log clearing, etc. Keeps that state in a small text file (the user
''	must give a path for holding status).
''
''	See Sub Usage for syntax. Note use of string types ... it would be
''	better to use the numeric EventType field, but that doesn't work on
''	NT4 and I felt like being inclusive. So sue me.
''
''	NOTE that the first time this is run (with an empty or nonexistent
''	state file) it will ALWAYS RETURN NOTHING! The script is meant to be
''	used with an eventing system like Webob (www.softwarepoetry.com/webob),
''	so is intended to pick up only NEW events ... the first time through
''	just "primes" things so we know what timestamp to start from.
''
''	License: This software is provided as-is with NO WARRANTY WHATSOEVER.
''	Software Poetry makes no claims as to its fitness for any purpose.
''	This file and any derivatives or translations of it may be freely
''	copied and redistributed so long as the original license and copyright
''	notices are included and not altered.
''

Option Explicit

''	+-----------------------------------------------------------------------+
''	| Constants																|
''	+-----------------------------------------------------------------------+

Const DEFAULT_TYPES = "error,warning"
Const DEFAULT_EXITERR = 1

''	+-----------------------------------------------------------------------+
''	| Usage																	|
''	+-----------------------------------------------------------------------+

Sub Usage()
	WScript.Echo "Usage: "
	WScript.Echo "cscript wmi-events.vbs svr state_file [types] [exiterr]"
	WScript.Echo "   svr = name of computer to connect to"
	WScript.Echo "   state_file = path to file we can use to save state"	
	WScript.Echo "   types = (default ""error,warning"") event log types "
	WScript.Echo "           to monitor, comma-separted. If any of your "
	WScript.Echo "           choices contain a space, be sure to surround "
	WScript.Echo "           the whole list with quotes. Values are:"
	WScript.Echo "            - error"
	WScript.Echo "            - warning"
	WScript.Echo "            - information"
	WScript.Echo "            - security audit success"
	WScript.Echo "            - security audit failure"
	WScript.Echo "   exiterr = (default 1) normally script will return a "
	WScript.Echo "             non-zero exit code if any logs match. If you "
	WScript.Echo "             want to incorporate log data without "
	WScript.Echo "             triggering Webob events, pass 0 to force the "
	WSCript.Echo "             script to always return a zero exit code"
	WScript.Echo ""
	WScript.Echo "Examples:"
	WScript.Echo "cscript wmi-events.vbs myserver c:\temp\wmi.txt "
	WScript.Echo "cscript wmi-events.vbs 10.0.0.1 c:\temp\wmi.txt error 0"
End Sub

''	+-----------------------------------------------------------------------+
''	| State Helpers															|
''	+-----------------------------------------------------------------------+

Function GetMaxDate(fso, szStateFile, szTypes, wmiServices)
	Dim fileState, szDateMax

	szDateMax = ""

	On Error Resume Next
	Set fileState = fso.OpenTextFile(szStateFile)
	szDateMax = Trim(fileState.ReadLine)
	fileState.Close
	On Error Goto 0

	If (szDateMax = "") Then
		szDateMax = InitializeDateMax(szTypes, wmiServices)
	End If

	GetMaxDate = szDateMax
End Function

Function InitializeDateMax(szTypes, wmiServices)
	Dim szDateMax, evt, szQuery 

	szQuery = MakeQueryString(szTypes, "")

	szDateMax = "00000000000000.000000-000"
	For Each evt In wmiServices.ExecQuery(szQuery,,48)
		If (CStr(evt.TimeGenerated) > szDateMax) Then 
			szDateMax = CStr(evt.TimeGenerated)
		End If
	Next

	InitializeDateMax = szDateMax
End Function

Sub SaveMaxDate(fso, szStateFile, szDateMax)
	Dim fileState

	Set fileState = fso.CreateTextFile(szStateFile, True)
	fileState.Write szDateMax
	fileState.Close
End Sub

''	+-----------------------------------------------------------------------+
''	| WMI Helpers															|
''	+-----------------------------------------------------------------------+

Function GetWmiServices(szComputer)
	Dim wmiLocator

	Set wmiLocator = WScript.CreateObject("WbemScripting.SWbemLocator")
	wmiLocator.Security_.ImpersonationLevel = 3
	wmiLocator.Security_.Privileges.AddAsString("SeBackupPrivilege")
	wmiLocator.Security_.Privileges.AddAsString("SeSecurityPrivilege")

	Set GetWmiServices = wmiLocator.ConnectServer(szComputer)
End Function

Function SpewEvents(wmiServices, szTypes, ByRef szDateMax)
	Dim szQuery, evt, xmlDoc, xmlRoot, xmlEvent, xmlTemp

	SpewEvents = False

	Set xmlDoc = WScript.CreateObject("Msxml2.DOMDocument")
	Set xmlRoot = xmlDoc.createElement("events")
	xmlDoc.appendChild(xmlRoot)

	szQuery = MakeQueryString(szTypes, szDateMax)
	For Each evt In wmiServices.ExecQuery(szQuery,,48)
		SpewEvents = True

		If (CStr(evt.TimeGenerated) > szDateMax) Then 
			szDateMax = CStr(evt.TimeGenerated)
		End If

		Set xmlEvent = xmlDoc.createElement("event")
		xmlRoot.appendChild(xmlEvent)

		Set xmlTemp = xmlDoc.createElement("code")
		xmlTemp.Text = evt.EventCode
		xmlEvent.appendChild(xmlTemp)

		Set xmlTemp = xmlDoc.createElement("type")
		xmlTemp.Text = evt.Type
		xmlEvent.appendChild(xmlTemp)

		Set xmlTemp = xmlDoc.createElement("identifier")
		xmlTemp.Text = "0x" & Hex(evt.EventIdentifier)
		xmlEvent.appendChild(xmlTemp)

		Set xmlTemp = xmlDoc.createElement("logfile")
		xmlTemp.Text = evt.Logfile
		xmlEvent.appendChild(xmlTemp)

		Set xmlTemp = xmlDoc.createElement("message")
		xmlTemp.Text = evt.Message & ""
		xmlEvent.appendChild(xmlTemp)

		Set xmlTemp = xmlDoc.createElement("source")
		xmlTemp.Text = evt.SourceName & ""
		xmlEvent.appendChild(xmlTemp)

		Set xmlTemp = xmlDoc.createElement("time-generated")
		xmlTemp.Text = evt.TimeGenerated
		xmlEvent.appendChild(xmlTemp)

		Set xmlTemp = xmlDoc.createElement("time-written")
		xmlTemp.Text = evt.TimeWritten
		xmlEvent.appendChild(xmlTemp)

		Set xmlTemp = xmlDoc.createElement("user")
		xmlTemp.Text = evt.User & ""
		xmlEvent.appendChild(xmlTemp)
	Next

	WScript.Echo xmlDoc.xml
End Function

Function MakeQueryString(szTypes,szDateMax)
	Dim szQuery, szType, fFirst

	szQuery = "select * from Win32_NTLogEvent where "

	If (szTypes <> "") Then
		szQuery = szQuery & " ("
		fFirst = True

		For Each szType In Split(szTypes, ",")
			If (fFirst) Then
				fFirst = False
			Else
				szQuery = szQuery & " or "
			End If
			
			szType = LCase(Trim(szType))
			szQuery = szQuery & " Type = '" & szType & "' "
		Next
		szQuery = szQuery & ") "
	End If

	If (szDateMax <> "") Then
		If (szTypes <> "") Then
			szQuery = szQuery & " and "
		End If

		szQuery = szQuery & " TimeGenerated > '" & szDateMax & "'"
	End If

	MakeQueryString = szQuery
End Function

''	+-----------------------------------------------------------------------+
''	| Entrypoint															|
''	+-----------------------------------------------------------------------+

Dim szComputer, szStateFile, szTypes, nExitErr
Dim wmiServices, fso, szDateMax, fAnyEvents

If (WScript.Arguments.Count < 2) Then
	Usage
	WScript.Quit(1)
End If

szComputer = WScript.Arguments(0)
szStateFile = WScript.Arguments(1)

If (WScript.Arguments.Count >= 3) Then
	szTypes = WScript.Arguments(2)
Else
	szTypes = DEFAULT_TYPES
End If

If (WScript.Arguments.Count >= 4) Then
	nExitErr = CLng(WScript.Arguments(3))
Else
	nExitErr = DEFAULT_EXITERR
End If

Set fso = WScript.CreateObject("Scripting.FileSystemObject")
Set wmiServices = GetWmiServices(szComputer)

szDateMax = GetMaxDate(fso, szStateFile, szTypes, wmiServices)
fAnyEvents = SpewEvents(wmiServices, szTypes, szDateMax)
SaveMaxDate fso, szStateFile, szDateMax

If (nExitErr And fAnyEvents) Then
	WScript.Quit(2)
End If

WScript.Quit(0)





