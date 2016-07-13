strComputer = "localhost"
Set objWMIService = GetObject("winmgmts:\\" & strComputer & "\root\cimv2")

Set objNewJob = objWMIService.Get("Win32_ScheduledJob")

errJobCreated = objNewJob.Create ("Notepad.exe",  "********000000.000000+540", True, &h7F, , , JobID)
'errJobCreated = objNewJob.Create ("Notepad.exe", "********123000.000000-420", True , 1 OR 4 OR 16, , , JobID)
Wscript.Echo errJobCreated

