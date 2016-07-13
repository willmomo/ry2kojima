Set objSystemSet = GetObject("winmgmts:{impersonationLevel=impersonate,(Shutdown)}").InstancesOf("Win32_OperatingSystem")

For Each objSystem In objSystemSet
        objSystem.Win32Shutdown 2
Next