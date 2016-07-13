'********************************************************************
'*
'* File:           Restart.vbs
'* Created:        March 1999
'* Version:        1.0
'*
'*  Main Function:  Shutsdown, PowerOff, LogOff, Restarts a machine.
'*
'*  Restart.vbs    /S <server> [/U <username>] [/W <password>] 
'*                 [/O <outputfile>] [/L} [/P] [/R] [/Q] [/F] [/T <time in seconds>]
'*
'* Copyright (C) 1999 Microsoft Corporation
'*
'********************************************************************

OPTION EXPLICIT

    'Define constants
    CONST CONST_ERROR                   = 0
    CONST CONST_WSCRIPT                 = 1
    CONST CONST_CSCRIPT                 = 2
    CONST CONST_SHOW_USAGE              = 3
    CONST CONST_PROCEED                 = 4

    'Shutdown Method Constants
    CONST CONST_SHUTDOWN                = 1
    CONST CONST_LOGOFF                  = 0
    CONST CONST_POWEROFF                = 8
    CONST CONST_REBOOT                  = 2
    CONST CONST_FORCE_REBOOT            = 6
    CONST CONST_FORCE_POWEROFF          = 12
    CONST CONST_FORCE_LOGOFF            = 4
    CONST CONST_FORCE_SHUTDOWN          = 5
     
    'Declare variables
    Dim intOpMode, i
    Dim strServer, strUserName, strPassword, strOutputFile
    Dim blnLogoff, blnPowerOff, blnReBoot, blnShutDown
    Dim blnForce
    Dim intTimer
    Dim UserArray(3)
    Dim MyCount

    'Make sure the host is csript, if not then abort
    VerifyHostIsCscript()

    'Parse the command line
    intOpMode = intParseCmdLine(strServer     ,  _
                                strUserName   ,  _
                                strPassword   ,  _
                                strOutputFile ,  _
                                blnLogoff     ,  _
                                blnPowerOff   ,  _
                                blnReBoot     ,  _
                                blnShutdown   ,  _
                                blnForce      ,  _
                                intTimer         )

    Select Case intOpMode

        Case CONST_SHOW_USAGE
            Call ShowUsage()
		
        Case CONST_PROCEED                 
            Call Reboot(strServer     , _
                          strOutputFile , _
                          strUserName   , _
                          strPassword   , _
                          blnReboot     , _
                          blnForce      , _
                          intTimer        )

            Call LogOff(strServer     , _
                          strOutputFile , _
                          strUserName   , _
                          strPassword   , _
                          blnLogoff     , _
                          blnForce      , _
                          intTimer        )

            Call PowerOff(strServer     , _
                          strOutputFile , _
                          strUserName   , _
                          strPassword   , _
                          blnPowerOff   , _
                          blnForce      , _
                          intTimer        )

            Call ShutDown(strServer     , _
                          strOutputFile , _
                          strUserName   , _
                          strPassword   , _
                          blnShutDown   , _
                          blnForce      , _
                          intTimer        )

        Case CONST_ERROR
            'Do Nothing

        Case Else                    'Default -- should never happen
            Call Wscript.Echo("Error occurred in passing parameters.")

    End Select


'********************************************************************
'*
'* Sub Reboot()
'*
'* Purpose: Reboots a machine.
'*
'* Input:   strServer           a machine name
'*          strOutputFile       an output file name
'*          strUserName         the current user's name
'*          strPassword         the current user's password
'*          blnForce            specifies whether to force the logoff
'*          intTimer            specifies the amount of time to perform the function
'*
'* Output:  Results are either printed on screen or saved in strOutputFile.
'*
'********************************************************************
Private Sub Reboot(strServer, strOutputFile, strUserName, strPassword, blnReboot, blnForce, intTimer)


    ON ERROR RESUME NEXT

    Dim objFileSystem, objOutputFile, objService, objEnumerator, objInstance
    Dim strQuery, strMessage
    Dim intStatus
    ReDim strID(0), strName(0)

    if blnreboot = false then
         Exit Sub
    End if

    if intTimer > 0 then
        wscript.echo "Rebooting machine " & strServer & " in " & intTimer & " seconds..."
        wscript.sleep (intTimer * 1000)
    End if

    'Open a text file for output if the file is requested
    If Not IsEmpty(strOutputFile) Then
        If (NOT blnOpenFile(strOutputFile, objOutputFile)) Then
            Call Wscript.Echo ("Could not open an output file.")
            Exit Sub
        End If
    End If

    'Establish a connection with the server.
    If blnConnect("root\cimv2" , _
                   strUserName , _
                   strPassword , _
                   strServer   , _
                   objService  ) Then
        Call Wscript.Echo("")
        Call Wscript.Echo("Please check the server name, " _
                        & "credentials and WBEM Core.")
        Exit Sub
    End If

    strID(0) = ""
    strName(0) = ""
    strMessage = ""
    strQuery = "Select * From Win32_OperatingSystem"

    Set objEnumerator = objService.ExecQuery(strQuery,,0)
    If Err.Number Then
        Print "Error 0x" & CStr(Hex(Err.Number)) & " occurred during the query."
        If Err.Description <> "" Then
            Print "Error description: " & Err.Description & "."
        End If
        Err.Clear
        Exit Sub
    End If

    i = 0
    For Each objInstance in objEnumerator
        If blnForce Then
            intStatus = objInstance.Win32ShutDown(CONST_FORCE_REBOOT)
        Else
            intStatus = objInstance.Win32ShutDown(CONST_REBOOT)
        End If

        IF intStatus = 0 Then
            strMessage = "Reboot a machine " & strServer & "."
        Else
            strMessage = "Failed to reboot a machine " & strServer & "."
        End If
        Call WriteLine(strMessage,objOutputFile)
    Next

    If IsObject(objOutputFile) Then
        objOutputFile.Close
        Call Wscript.Echo ("Results are saved in file " & strOutputFile & ".")
    End If
End Sub


'********************************************************************
'*
'* Sub LogOff()
'*
'* Purpose: Logs off the user currently logged onto a machine.
'*
'* Input:   strServer           a machine name
'*          strOutputFile       an output file name
'*          strUserName         the current user's name
'*          strPassword         the current user's password
'*          blnForce            specifies whether to force the logoff
'*          intTimer            specifies the amount of time to preform the function
'*
'* Output:  Results are either printed on screen or saved in strOutputFile.
'*
'********************************************************************
Private Sub LogOff(strServer, strOutputFile, strUserName, strPassword, blnLogoff, blnForce, intTimer)


    ON ERROR RESUME NEXT

    Dim objFileSystem, objOutputFile, objService, objEnumerator, objInstance
    Dim strQuery, strMessage
    Dim intStatus
    ReDim strID(0), strName(0)

     If blnlogoff = false then
          Exit Sub
     End if

	if intTimer > 1 then 
	 wscript.echo "Logging off machine " & strServer & " in " & intTimer & " seconds..."
		wscript.sleep (intTimer * 1000)
	End if

    'Open a text file for output if the file is requested
    If Not IsEmpty(strOutputFile) Then
        If (NOT blnOpenFile(strOutputFile, objOutputFile)) Then
            Call Wscript.Echo ("Could not open an output file.")
            Exit Sub
        End If
    End If

    'Establish a connection with the server.
    If blnConnect("root\cimv2" , _
                   strUserName , _
                   strPassword , _
                   strServer   , _
                   objService  ) Then
        Call Wscript.Echo("")
        Call Wscript.Echo("Please check the server name, " _
                        & "credentials and WBEM Core.")
        Exit Sub
    End If

    strID(0) = ""
    strName(0) = ""
    strMessage = ""
    strQuery = "Select * From Win32_OperatingSystem"

    Set objEnumerator = objService.ExecQuery(strQuery,,0)
    If Err.Number Then
        Print "Error 0x" & CStr(Hex(Err.Number)) & " occurred during the query."
        If Err.Description <> "" Then
            Print "Error description: " & Err.Description & "."
        End If
        Err.Clear
        Exit Sub
    End If

    i = 0
    For Each objInstance in objEnumerator
        If blnForce Then
            intStatus = objInstance.Win32ShutDown(CONST_FORCE_LOGOFF)
        Else
            intStatus = objInstance.Win32ShutDown(CONST_LOGOFF)
        End If

        IF intStatus = 0 Then
            strMessage = "Logging off the current user on machine " & _
                         strServer & "..."
        Else
            strMessage = "Failed to log off the current user from machine " _
                & strServer & "."
        End If
        Call WriteLine(strMessage,objOutputFile)
    Next

    If IsObject(objOutputFile) Then
        objOutputFile.Close
        Call Wscript.Echo ("Results are saved in file " & strOutputFile & ".")
    End If
End Sub


'********************************************************************
'*
'* Sub PowerOff()
'*
'* Purpose: Powers off a machine.
'*
'* Input:   strServer           a machine name
'*          strOutputFile       an output file name
'*          strUserName         the current user's name
'*          strPassword         the current user's password
'*          blnForce            specifies whether to force the logoff
'*          intTimer            specifies the amount of time to perform the function
'*
'* Output:  Results are either printed on screen or saved in strOutputFile.
'*
'********************************************************************
Private Sub PowerOff(strServer, strOutputFile, strUserName, strPassword, blnPowerOff, blnForce, intTimer)


    ON ERROR RESUME NEXT

    Dim objFileSystem, objOutputFile, objService, objEnumerator, objInstance
    Dim strQuery, strMessage
    Dim intStatus
    ReDim strID(0), strName(0)

      if blnPoweroff = false then
             Exit sub
      End if

    If intTimer > 0 then	 
        wscript.echo "Powering off machine " & strServer & " in " & intTimer & " seconds..."
	wscript.sleep (intTimer * 1000)
    End if

    'Open a text file for output if the file is requested
    If Not IsEmpty(strOutputFile) Then
        If (NOT blnOpenFile(strOutputFile, objOutputFile)) Then
            Call Wscript.Echo ("Could not open an output file.")
            Exit Sub
        End If
    End If

    'Establish a connection with the server.
    If blnConnect("root\cimv2" , _
                   strUserName , _
                   strPassword , _
                   strServer   , _
                   objService  ) Then
        Call Wscript.Echo("")
        Call Wscript.Echo("Please check the server name, " _
                        & "credentials and WBEM Core.")
        Exit Sub
    End If

    strID(0) = ""
    strName(0) = ""
    strMessage = ""
    strQuery = "Select * From Win32_OperatingSystem"

    Set objEnumerator = objService.ExecQuery(strQuery,,0)
    If Err.Number Then
        Print "Error 0x" & CStr(Hex(Err.Number)) & " occurred during the query."
        If Err.Description <> "" Then
            Print "Error description: " & Err.Description & "."
        End If
        Err.Clear
        Exit Sub
    End If

    i = 0
    For Each objInstance in objEnumerator
        If blnForce Then
            intStatus = objInstance.Win32ShutDown(CONST_FORCE_POWEROFF)
        Else
            intStatus = objInstance.Win32ShutDown(CONST_POWEROFF)
        End If

        IF intStatus = 0 Then
            strMessage = "Power off machine " & strServer & "."
        Else
            strMessage = "Failed to power off machine " & strServer & "."
        End If
        Call WriteLine(strMessage,objOutputFile)
    Next

    If IsObject(objOutputFile) Then
        objOutputFile.Close
        Call Wscript.Echo ("Results are saved in file " & strOutputFile & ".")
    End If
End Sub


'********************************************************************
'*
'* Sub Shutdown()
'*
'* Purpose: Shutsdown a machine.
'*
'* Input:   strServer           a machine name
'*          strOutputFile       an output file name
'*          strUserName         the current user's name
'*          strPassword         the current user's password
'*          blnForce            specifies whether to force the logoff
'*          intTimer            specifies the amount of time to perform the function
'*
'* Output:  Results are either printed on screen or saved in strOutputFile.
'*
'********************************************************************
Private Sub Shutdown(strServer, strOutputFile, strUserName, strPassword, blnShutDown, blnForce, intTimer)


    ON ERROR RESUME NEXT

    Dim objFileSystem, objOutputFile, objService, objEnumerator, objInstance
    Dim strQuery, strMessage
    Dim intStatus
    ReDim strID(0), strName(0)

    If blnShutdown = False then
          Exit Sub
    End if   

    if intTimer > 0 then 
              wscript.echo "Shutting down computer " & strServer & " in " & intTimer & " seconds..."
         wscript.sleep (intTimer * 1000)
    End if


    'Open a text file for output if the file is requested
    If Not IsEmpty(strOutputFile) Then
        If (NOT blnOpenFile(strOutputFile, objOutputFile)) Then
            Call Wscript.Echo ("Could not open an output file.")
            Exit Sub
        End If
    End If

    'Establish a connection with the server.
    If blnConnect("root\cimv2" , _
                   strUserName , _
                   strPassword , _
                   strServer   , _
                   objService  ) Then
        Call Wscript.Echo("")
        Call Wscript.Echo("Please check the server name, " _
                        & "credentials and WBEM Core.")
        Exit Sub
    End If

    strID(0) = ""
    strName(0) = ""
    strMessage = ""
    strQuery = "Select * From Win32_OperatingSystem"

    Set objEnumerator = objService.ExecQuery(strQuery,,0)
    If Err.Number Then
        Print "Error 0x" & CStr(Hex(Err.Number)) & " occurred during the query."
        If Err.Description <> "" Then
            Print "Error description: " & Err.Description & "."
        End If
        Err.Clear
        Exit Sub
    End If

    i = 0
    For Each objInstance in objEnumerator
        If blnForce Then
            intStatus = objInstance.Win32ShutDown(CONST_FORCE_SHUTDOWN)
        Else
            intStatus = objInstance.Win32ShutDown(CONST_SHUTDOWN)
        End If

        IF intStatus = 0 Then
            strMessage = "Shuts down machine " & strServer & "."
        Else
            strMessage = "Failed to shutdown machine " & strServer & "."
        End If
        Call WriteLine(strMessage,objOutputFile)
    Next

    If IsObject(objOutputFile) Then
        objOutputFile.Close
        Call Wscript.Echo ("Results are saved in file " & strOutputFile & ".")
    End If
End Sub



'********************************************************************
'*
'* Function intParseCmdLine()
'*
'* Purpose: Parses the command line.
'* Input:   
'*
'* Output:  strServer         a remote server ("" = local server")
'*          strUserName       the current user's name
'*          strPassword       the current user's password
'*          strOutputFile     an output file name
'*          intTimer          amount of time in seconds
'*
'********************************************************************
Private Function intParseCmdLine( ByRef strServer,        _
                                  ByRef strUserName,      _
                                  ByRef strPassword,      _
                                  ByRef strOutputFile,    _
                                  ByRef blnLogoff,        _
                                  ByRef blnShutdown,      _
                                  ByRef blnReboot,        _
                                  ByRef blnPowerOff,      _
                                  ByRef blnForce,         _
                                  ByRef intTimer          )


    ON ERROR RESUME NEXT

    Dim strFlag
    Dim intState, intArgIter
    Dim objFileSystem

    If Wscript.Arguments.Count > 0 Then
        strFlag = Wscript.arguments.Item(0)
    End If

    If IsEmpty(strFlag) Then                'No arguments have been received
        Wscript.Echo("Arguments are Required.")
        intParseCmdLine = CONST_ERROR
        Exit Function
    End If

    'Check if the user is asking for help or is just confused
    If (strFlag="help") OR (strFlag="/h") OR (strFlag="\h") OR (strFlag="-h") _
        OR (strFlag = "\?") OR (strFlag = "/?") OR (strFlag = "?") _ 
        OR (strFlag="h") Then
        intParseCmdLine = CONST_SHOW_USAGE
        Exit Function
    End If

    'Retrieve the command line and set appropriate variables
     intArgIter = 0
    Do While intArgIter <= Wscript.arguments.Count - 1
        Select Case Left(LCase(Wscript.arguments.Item(intArgIter)),2)
  
            Case "/s"
                intParseCmdLine = CONST_PROCEED
                If Not blnGetArg("Server", strServer, intArgIter) Then
                    intParseCmdLine = CONST_ERROR
                    Exit Function
                End If
                intArgIter = intArgIter + 1

            Case "/o"
                If Not blnGetArg("Output File", strOutputFile, intArgIter) Then
                    intParseCmdLine = CONST_ERROR
                    Exit Function
                End If
                intArgIter = intArgIter + 1

            Case "/u"
                If Not blnGetArg("User Name", strUserName, intArgIter) Then
                    intParseCmdLine = CONST_ERROR
                    Exit Function
                End If
                intArgIter = intArgIter + 1

            Case "/w"
                If Not blnGetArg("User Password", strPassword, intArgIter) Then
                    intParseCmdLine = CONST_ERROR
                    Exit Function
                End If
                intArgIter = intArgIter + 1
            
            Case "/f"
                blnForce = True
                intArgIter = intArgIter + 1

            Case "/r"
                blnReBoot = True
                userarray(0) = blnReBoot
                intArgIter = intArgIter + 1

            Case "/q"
                blnPowerOff = True
                userarray(1) = blnPowerOff
                intArgIter = intArgIter + 1

            Case "/l"
                blnLogOff = True
                userarray(2) = blnLogoff
                intArgIter = intArgIter + 1

            Case "/p"
                blnShutDown = True
                userarray(3) = blnShutDown
                intArgIter = intArgIter + 1

            Case "/t"
                If Not blnGetArg("Timer", intTimer, intArgIter) Then
                    intParseCmdLine = CONST_ERROR
                    Exit Function
                End If
                intArgIter = intArgIter + 1

            Case Else 'We shouldn't get here
                Call Wscript.Echo("Invalid or misplaced parameter: " _
                   & Wscript.arguments.Item(intArgIter) & vbCRLF _
                   & "Please check the input and try again," & vbCRLF _
                   & "or invoke with '/?' for help with the syntax.")
                Wscript.Quit

        End Select

    Loop '** intArgIter <= Wscript.arguments.Count - 1

    MyCount = 0

    for i = 0 to 3
        if userarray(i) = True then
            MyCount = Mycount + 1
        End if
    Next

   if Mycount > 1 then 
        intParseCmdLine = CONST_SHOW_USAGE
   End if

    If IsEmpty(intParseCmdLine) Then 
        intParseCmdLine = CONST_ERROR
        Wscript.Echo("Arguments are Required.")
    End If

 End Function

'********************************************************************
'*
'* Sub ShowUsage()
'*
'* Purpose: Shows the correct usage to the user.
'*
'* Input:   None
'*
'* Output:  Help messages are displayed on screen.
'*
'********************************************************************
Private Sub ShowUsage()

    Wscript.Echo ""
    Wscript.Echo "Logoffs, Reboots, Powers Off, or Shuts Down a machine."
    Wscript.Echo ""
    Wscript.Echo "SYNTAX:"
    Wscript.Echo "  Restart.vbs [/S <server>] [/U <username>] [/W <password>]"
    Wscript.Echo "              [/O <outputfile>] </L> </R> </P> </Q> </F> [/T <time in seconds>]"
    Wscript.Echo ""
    Wscript.Echo "PARAMETER SPECIFIERS:"
    wscript.echo "   /T            Amount of time to perform the function."
    Wscript.Echo "   /Q            Perform Shutdown."
    Wscript.Echo "   /P            Perform Poweroff."
    Wscript.Echo "   /R            Perform Reboot."
    Wscript.Echo "   /L            Perform Logoff."
    Wscript.Echo "   /F            Force Function."
    Wscript.Echo "   server        A machine name."
    Wscript.Echo "   username      The current user's name."
    Wscript.Echo "   password      Password of the current user."
    Wscript.Echo "   outputfile    The output file name."
    Wscript.Echo ""
    Wscript.Echo "EXAMPLE:"
    Wscript.Echo "1. cscript Restart.vbs /S MyMachine2 /R"
    Wscript.Echo "   Reboots the current machine MyMachine2."
    Wscript.Echo "2. cscript Restart.vbs /S MyMachine2 /R /F"
    Wscript.Echo "   Forces MyMachine2 to reboot."
    Wscript.Echo "3. cscript Restart.vbs /S MyMachine2 /R /T 30"
    Wscript.Echo "   Reboots the current machine MyMachine2 in 30 seconds."
    Wscript.Echo "NOTE:"
    Wscript.Echo "   The force option will make the machine perform the function even " _
               & "if there are"
    Wscript.Echo "   open and unsaved docuements on the screen."

End Sub

'********************************************************************
'* General Routines
'********************************************************************

'********************************************************************
'*
'* Function strPackString()
'*
'* Purpose: Attaches spaces to a string to increase the length to intWidth.
'*
'* Input:   strString   a string
'*          intWidth    the intended length of the string
'*          blnAfter    Should spaces be added after the string?
'*          blnTruncate specifies whether to truncate the string or not if
'*                      the string length is longer than intWidth
'*
'* Output:  strPackString is returned as the packed string.
'*
'********************************************************************
Private Function strPackString( ByVal strString, _
                                ByVal intWidth,  _
                                ByVal blnAfter,  _
                                ByVal blnTruncate)

    ON ERROR RESUME NEXT

    intWidth      = CInt(intWidth)
    blnAfter      = CBool(blnAfter)
    blnTruncate   = CBool(blnTruncate)

    If Err.Number Then
        Call Wscript.Echo ("Argument type is incorrect!")
        Err.Clear
        Wscript.Quit
    End If

    If IsNull(strString) Then
        strPackString = "null" & Space(intWidth-4)
        Exit Function
    End If

    strString = CStr(strString)
    If Err.Number Then
        Call Wscript.Echo ("Argument type is incorrect!")
        Err.Clear
        Wscript.Quit
    End If

    If intWidth > Len(strString) Then
        If blnAfter Then
            strPackString = strString & Space(intWidth-Len(strString))
        Else
            strPackString = Space(intWidth-Len(strString)) & strString & " "
        End If
    Else
        If blnTruncate Then
            strPackString = Left(strString, intWidth-1) & " "
        Else
            strPackString = strString & " "
        End If
    End If

End Function

'********************************************************************
'* 
'*  Function blnGetArg()
'*
'*  Purpose: Helper to intParseCmdLine()
'* 
'*  Usage:
'*
'*     Case "/s" 
'*       blnGetArg ("server name", strServer, intArgIter)
'*
'********************************************************************
Private Function blnGetArg ( ByVal StrVarName,   _
                             ByRef strVar,       _
                             ByRef intArgIter) 

    blnGetArg = False 'failure, changed to True upon successful completion

    If Len(Wscript.Arguments(intArgIter)) > 2 then
        If Mid(Wscript.Arguments(intArgIter),3,1) = ":" then
            If Len(Wscript.Arguments(intArgIter)) > 3 then
                strVar = Right(Wscript.Arguments(intArgIter), _
                         Len(Wscript.Arguments(intArgIter)) - 3)
                blnGetArg = True
                Exit Function
            Else
                intArgIter = intArgIter + 1
                If intArgIter > (Wscript.Arguments.Count - 1) Then
                    Call Wscript.Echo( "Invalid " & StrVarName & ".")
                    Call Wscript.Echo( "Please check the input and try again.")
                    Exit Function
                End If

                strVar = Wscript.Arguments.Item(intArgIter)
                If Err.Number Then
                    Call Wscript.Echo( "Invalid " & StrVarName & ".")
                    Call Wscript.Echo( "Please check the input and try again.")
                    Exit Function
                End If

                If InStr(strVar, "/") Then
                    Call Wscript.Echo( "Invalid " & StrVarName)
                    Call Wscript.Echo( "Please check the input and try again.")
                    Exit Function
                End If

                blnGetArg = True 'success
            End If
        Else
            strVar = Right(Wscript.Arguments(intArgIter), _
                     Len(Wscript.Arguments(intArgIter)) - 2)
            blnGetArg = True 'success
            Exit Function
        End If
    Else
        intArgIter = intArgIter + 1
        If intArgIter > (Wscript.Arguments.Count - 1) Then
            Call Wscript.Echo( "Invalid " & StrVarName & ".")
            Call Wscript.Echo( "Please check the input and try again.")
            Exit Function
        End If

        strVar = Wscript.Arguments.Item(intArgIter)
        If Err.Number Then
            Call Wscript.Echo( "Invalid " & StrVarName & ".")
            Call Wscript.Echo( "Please check the input and try again.")
            Exit Function
        End If

        If InStr(strVar, "/") Then
            Call Wscript.Echo( "Invalid " & StrVarName)
            Call Wscript.Echo( "Please check the input and try again.")
            Exit Function
        End If
        blnGetArg = True 'success
    End If
End Function

'********************************************************************
'*
'* Function blnConnect()
'*
'* Purpose: Connects to machine strServer.
'*
'* Input:   strServer       a machine name
'*          strNameSpace    a namespace
'*          strUserName     name of the current user
'*          strPassword     password of the current user
'*
'* Output:  objService is returned  as a service object.
'*          strServer is set to local host if left unspecified
'*
'********************************************************************
Private Function blnConnect(ByVal strNameSpace, _
                            ByVal strUserName,  _
                            ByVal strPassword,  _
                            ByRef strServer,    _
                            ByRef objService)

    ON ERROR RESUME NEXT

    Dim objLocator, objWshNet

    blnConnect = False     'There is no error.

    'Create Locator object to connect to remote CIM object manager
    Set objLocator = CreateObject("WbemScripting.SWbemLocator")
    If Err.Number then
        Call Wscript.Echo( "Error 0x" & CStr(Hex(Err.Number)) & _
                           " occurred in creating a locator object." )
        If Err.Description <> "" Then
            Call Wscript.Echo( "Error description: " & Err.Description & "." )
        End If
        Err.Clear
        blnConnect = True     'An error occurred
        Exit Function
    End If

    'following two lines added by Ravi
    objLocator.Security_.Privileges.Add 18, True    'wbemPrivilegeShutdown
    objLocator.Security_.Privileges.Add 23, True    'wbemPrivilegeRemoteShutdown

    'Connect to the namespace which is either local or remote
    Set objService = objLocator.ConnectServer (strServer, strNameSpace, _
       strUserName, strPassword)
    ObjService.Security_.impersonationlevel = 3
    If Err.Number then
        Call Wscript.Echo( "Error 0x" & CStr(Hex(Err.Number)) & _
                           " occurred in connecting to server " _
           & strServer & ".")
        If Err.Description <> "" Then
            Call Wscript.Echo( "Error description: " & Err.Description & "." )
        End If
        Err.Clear
        blnConnect = True     'An error occurred
    End If

    'Get the current server's name if left unspecified
    If IsEmpty(strServer) Then
        Set objWshNet = CreateObject("Wscript.Network")
    strServer     = objWshNet.ComputerName
    End If

End Function

'********************************************************************
'*
'* Sub      VerifyHostIsCscript()
'*
'* Purpose: Determines which program is used to run this script.
'*
'* Input:   None
'*
'* Output:  If host is not cscript, then an error message is printed 
'*          and the script is aborted.
'*
'********************************************************************
Sub VerifyHostIsCscript()

    ON ERROR RESUME NEXT

    Dim strFullName, strCommand, i, j, intStatus

    strFullName = WScript.FullName

    If Err.Number then
        Call Wscript.Echo( "Error 0x" & CStr(Hex(Err.Number)) & " occurred." )
        If Err.Description <> "" Then
            Call Wscript.Echo( "Error description: " & Err.Description & "." )
        End If
        intStatus =  CONST_ERROR
    End If

    i = InStr(1, strFullName, ".exe", 1)
    If i = 0 Then
        intStatus =  CONST_ERROR
    Else
        j = InStrRev(strFullName, "\", i, 1)
        If j = 0 Then
            intStatus =  CONST_ERROR
        Else
            strCommand = Mid(strFullName, j+1, i-j-1)
            Select Case LCase(strCommand)
                Case "cscript"
                    intStatus = CONST_CSCRIPT
                Case "wscript"
                    intStatus = CONST_WSCRIPT
                Case Else       'should never happen
                    Call Wscript.Echo( "An unexpected program was used to " _
                                       & "run this script." )
                    Call Wscript.Echo( "Only CScript.Exe or WScript.Exe can " _
                                       & "be used to run this script." )
                    intStatus = CONST_ERROR
                End Select
        End If
    End If

    If intStatus <> CONST_CSCRIPT Then
        Call WScript.Echo( "Please run this script using CScript." & vbCRLF & _
             "This can be achieved by" & vbCRLF & _
             "1. Using ""CScript Restart.vbs arguments"" for Windows 95/98 or" _
             & vbCRLF & "2. Changing the default Windows Scripting Host " _
             & "setting to CScript" & vbCRLF & "    using ""CScript " _
             & "//H:CScript //S"" and running the script using" & vbCRLF & _
             "    ""Restart.vbs arguments"" for Windows NT/2000." )
        WScript.Quit
    End If

End Sub

'********************************************************************
'*
'* Sub WriteLine()
'* Purpose: Writes a text line either to a file or on screen.
'* Input:   strMessage  the string to print
'*          objFile     an output file object
'* Output:  strMessage is either displayed on screen or written to a file.
'*
'********************************************************************
Sub WriteLine(ByVal strMessage, ByVal objFile)

    On Error Resume Next
    If IsObject(objFile) then        'objFile should be a file object
        objFile.WriteLine strMessage
    Else
        Call Wscript.Echo( strMessage )
    End If

End Sub

'********************************************************************
'* 
'* Function blnErrorOccurred()
'*
'* Purpose: Reports error with a string saying what the error occurred in.
'*
'* Input:   strIn		string saying what the error occurred in.
'*
'* Output:  displayed on screen 
'* 
'********************************************************************
Private Function blnErrorOccurred (ByVal strIn)

    If Err.Number Then
        Call Wscript.Echo( "Error 0x" & CStr(Hex(Err.Number)) & ": " & strIn)
        If Err.Description <> "" Then
            Call Wscript.Echo( "Error description: " & Err.Description)
        End If
        Err.Clear
        blnErrorOccurred = True
    Else
        blnErrorOccurred = False
    End If

End Function

'********************************************************************
'* 
'* Function blnOpenFile
'*
'* Purpose: Opens a file.
'*
'* Input:   strFileName		A string with the name of the file.
'*
'* Output:  Sets objOpenFile to a FileSystemObject and setis it to 
'*            Nothing upon Failure.
'* 
'********************************************************************
Private Function blnOpenFile(ByVal strFileName, ByRef objOpenFile)

    ON ERROR RESUME NEXT

    Dim objFileSystem

    Set objFileSystem = Nothing

    If IsEmpty(strFileName) OR strFileName = "" Then
        blnOpenFile = False
        Set objOpenFile = Nothing
        Exit Function
    End If

    'Create a file object
    Set objFileSystem = CreateObject("Scripting.FileSystemObject")
    If blnErrorOccurred("Could not create filesystem object.") Then
        blnOpenFile = False
        Set objOpenFile = Nothing
        Exit Function
    End If

    'Open the file for output
    Set objOpenFile = objFileSystem.OpenTextFile(strFileName, 8, True)
    If blnErrorOccurred("Could not open") Then
        blnOpenFile = False
        Set objOpenFile = Nothing
        Exit Function
    End If
    blnOpenFile = True

End Function

'********************************************************************
'*                                                                  *
'*                           End of File                            *
'*                                                                  *
'********************************************************************





