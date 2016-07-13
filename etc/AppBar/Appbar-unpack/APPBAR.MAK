# Microsoft Visual C++ Generated NMAKE File, Format Version 2.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

!IF "$(CFG)" == ""
CFG=Win32 Debug
!MESSAGE No configuration specified.  Defaulting to Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "Win32 Release" && "$(CFG)" != "Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "AppBar.mak" CFG="Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

################################################################################
# Begin Project
# PROP Target_Last_Scanned "Win32 Debug"
MTL=MkTypLib.exe
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "WinRel"
# PROP BASE Intermediate_Dir "WinRel"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "WinRel"
# PROP Intermediate_Dir "WinRel"
OUTDIR=.\WinRel
INTDIR=.\WinRel

ALL : $(OUTDIR)/AppBar.exe $(OUTDIR)/AppBar.bsc

$(OUTDIR) : 
    if not exist $(OUTDIR)/nul mkdir $(OUTDIR)

# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE CPP /nologo /W3 /GX /YX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FR /c
# ADD CPP /nologo /W3 /GX /YX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FR /c
CPP_PROJ=/nologo /W3 /GX /YX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /FR$(INTDIR)/ /Fp$(OUTDIR)/"AppBar.pch" /Fo$(INTDIR)/ /c 
CPP_OBJS=.\WinRel/
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
RSC_PROJ=/l 0x409 /fo$(INTDIR)/"AppBar.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o$(OUTDIR)/"AppBar.bsc" 
BSC32_SBRS= \
	$(INTDIR)/Main.sbr \
	$(INTDIR)/WndProc.sbr \
	$(INTDIR)/AppBar.sbr \
	$(INTDIR)/Util.sbr \
	$(INTDIR)/PropSht.sbr

$(OUTDIR)/AppBar.bsc : $(OUTDIR)  $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /NOLOGO /SUBSYSTEM:windows /MACHINE:I386
# ADD LINK32 comctl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /NOLOGO /SUBSYSTEM:windows /MACHINE:I386
LINK32_FLAGS=comctl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib\
 comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /NOLOGO\
 /SUBSYSTEM:windows /INCREMENTAL:no /PDB:$(OUTDIR)/"AppBar.pdb" /MACHINE:I386\
 /OUT:$(OUTDIR)/"AppBar.exe" 
DEF_FILE=
LINK32_OBJS= \
	$(INTDIR)/Main.obj \
	$(INTDIR)/WndProc.obj \
	$(INTDIR)/AppBar.obj \
	$(INTDIR)/AppBar.res \
	$(INTDIR)/Util.obj \
	$(INTDIR)/PropSht.obj

$(OUTDIR)/AppBar.exe : $(OUTDIR)  $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "WinDebug"
# PROP BASE Intermediate_Dir "WinDebug"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "WinDebug"
# PROP Intermediate_Dir "WinDebug"
OUTDIR=.\WinDebug
INTDIR=.\WinDebug

ALL : $(OUTDIR)/AppBar.exe $(OUTDIR)/AppBar.bsc

$(OUTDIR) : 
    if not exist $(OUTDIR)/nul mkdir $(OUTDIR)

# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE CPP /nologo /W3 /GX /Zi /YX /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /c
# ADD CPP /nologo /W3 /GX /Zi /YX /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /c
CPP_PROJ=/nologo /W3 /GX /Zi /YX /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /FR$(INTDIR)/ /Fp$(OUTDIR)/"AppBar.pch" /Fo$(INTDIR)/ /Fd$(OUTDIR)/"AppBar.pdb"\
 /c 
CPP_OBJS=.\WinDebug/
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
RSC_PROJ=/l 0x409 /fo$(INTDIR)/"AppBar.res" /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o$(OUTDIR)/"AppBar.bsc" 
BSC32_SBRS= \
	$(INTDIR)/Main.sbr \
	$(INTDIR)/WndProc.sbr \
	$(INTDIR)/AppBar.sbr \
	$(INTDIR)/Util.sbr \
	$(INTDIR)/PropSht.sbr

$(OUTDIR)/AppBar.bsc : $(OUTDIR)  $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /NOLOGO /SUBSYSTEM:windows /DEBUG /MACHINE:I386
# ADD LINK32 comctl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /NOLOGO /SUBSYSTEM:windows /DEBUG /MACHINE:I386
LINK32_FLAGS=comctl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib\
 comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /NOLOGO\
 /SUBSYSTEM:windows /INCREMENTAL:yes /PDB:$(OUTDIR)/"AppBar.pdb" /DEBUG\
 /MACHINE:I386 /OUT:$(OUTDIR)/"AppBar.exe" 
DEF_FILE=
LINK32_OBJS= \
	$(INTDIR)/Main.obj \
	$(INTDIR)/WndProc.obj \
	$(INTDIR)/AppBar.obj \
	$(INTDIR)/AppBar.res \
	$(INTDIR)/Util.obj \
	$(INTDIR)/PropSht.obj

$(OUTDIR)/AppBar.exe : $(OUTDIR)  $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

################################################################################
# Begin Group "Source Files"

################################################################################
# Begin Source File

SOURCE=.\Main.c
DEP_MAIN_=\
	.\Globals.h

$(INTDIR)/Main.obj :  $(SOURCE)  $(DEP_MAIN_) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\WndProc.c
DEP_WNDPR=\
	.\Globals.h\
	.\AppBar.h

$(INTDIR)/WndProc.obj :  $(SOURCE)  $(DEP_WNDPR) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\AppBar.c
DEP_APPBA=\
	.\Globals.h\
	.\AppBar.h

$(INTDIR)/AppBar.obj :  $(SOURCE)  $(DEP_APPBA) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\AppBar.rc
DEP_APPBAR=\
	.\Res\appbar.ICO\
	.\Res\appbar.bmp\
	.\Res\base.bmp\
	.\Res\window.bmp

$(INTDIR)/AppBar.res :  $(SOURCE)  $(DEP_APPBAR) $(INTDIR)
   $(RSC) $(RSC_PROJ)  $(SOURCE) 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Util.c
DEP_UTIL_=\
	.\Globals.h\
	.\AppBar.h

$(INTDIR)/Util.obj :  $(SOURCE)  $(DEP_UTIL_) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\PropSht.c
DEP_PROPS=\
	.\Globals.h\
	.\AppBar.h

$(INTDIR)/PropSht.obj :  $(SOURCE)  $(DEP_PROPS) $(INTDIR)

# End Source File
# End Group
# End Project
################################################################################
