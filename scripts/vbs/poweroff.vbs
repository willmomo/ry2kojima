Set objSystemSet = GetObject("winmgmts:{impersonationLevel=impersonate,(Shutdown)}").InstancesOf("Win32_OperatingSystem")

For Each objSystem In objSystemSet
        objSystem.Win32Shutdown 8
Next