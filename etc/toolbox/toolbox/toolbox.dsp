# Microsoft Developer Studio Project File - Name="toolbox" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=toolbox - Win32 Debug
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "toolbox.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "toolbox.mak" CFG="toolbox - Win32 Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "toolbox - Win32 Release" ("Win32 (x86) Application" 用)
!MESSAGE "toolbox - Win32 Debug" ("Win32 (x86) Application" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/tools/toolbox", BNJLAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "toolbox - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ../lib/wsslog.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "toolbox - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ../libd/wsslog.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "toolbox - Win32 Release"
# Name "toolbox - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "kjm_cpp"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\_kjm\kjm_eventlog.cpp
# End Source File
# Begin Source File

SOURCE=..\_kjm\kjm_library.cpp
# End Source File
# Begin Source File

SOURCE=..\_kjm\kjm_lockfile.cpp
# End Source File
# Begin Source File

SOURCE=..\_kjm\kjm_memfile.cpp
# End Source File
# Begin Source File

SOURCE=..\_kjm\kjm_mutex.cpp
# End Source File
# Begin Source File

SOURCE=..\_kjm\kjm_path_string.cpp
# End Source File
# Begin Source File

SOURCE=..\_kjm\kjm_physical_drive.cpp
# End Source File
# Begin Source File

SOURCE=..\_kjm\kjm_registry.cpp
# End Source File
# Begin Source File

SOURCE=..\_kjm\kjm_time.cpp
# End Source File
# Begin Source File

SOURCE=..\_kjm\kjm_util.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\evlogthrd.cpp
# End Source File
# Begin Source File

SOURCE=.\smartthrd.cpp
# End Source File
# Begin Source File

SOURCE=.\tbcommon.cpp
# End Source File
# Begin Source File

SOURCE=.\toolbox.cpp
# End Source File
# Begin Source File

SOURCE=.\toolbox.rc
# End Source File
# Begin Source File

SOURCE=.\toolboxini.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "kjm_h"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\_kjm\kjm.h
# End Source File
# Begin Source File

SOURCE=..\_kjm\kjm_eventlog.h
# End Source File
# Begin Source File

SOURCE=..\_kjm\kjm_library.h
# End Source File
# Begin Source File

SOURCE=..\_kjm\kjm_lockfile.h
# End Source File
# Begin Source File

SOURCE=..\_kjm\kjm_memfile.h
# End Source File
# Begin Source File

SOURCE=..\_kjm\kjm_mutex.h
# End Source File
# Begin Source File

SOURCE=..\_kjm\kjm_path_string.h
# End Source File
# Begin Source File

SOURCE=..\_kjm\kjm_physical_drive.h
# End Source File
# Begin Source File

SOURCE=..\_kjm\kjm_registry.h
# End Source File
# Begin Source File

SOURCE=..\_kjm\kjm_Smart.h
# End Source File
# Begin Source File

SOURCE=..\_kjm\kjm_sub.h
# End Source File
# Begin Source File

SOURCE=..\_kjm\kjm_time.h
# End Source File
# Begin Source File

SOURCE=..\_kjm\kjm_util.h
# End Source File
# Begin Source File

SOURCE=..\_kjm\kjm_window.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\toolbox.h
# End Source File
# Begin Source File

SOURCE=..\WssLog\WSSLOG.H
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\appl.ico
# End Source File
# End Group
# Begin Group "Document Files"

# PROP Default_Filter "sample"
# Begin Source File

SOURCE=.\toolbox.ini.sample
# End Source File
# End Group
# End Target
# End Project
