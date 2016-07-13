# Microsoft Developer Studio Generated NMAKE File, Format Version 4.20
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Application" 0x0101

!IF "$(CFG)" == ""
CFG=Wmkabox2 - Win32 Debug
!MESSAGE 構成が指定されていません。ﾃﾞﾌｫﾙﾄの Wmkabox2 - Win32 Debug を設定します。
!ENDIF 

!IF "$(CFG)" != "Wmkabox2 - Win32 Release" && "$(CFG)" !=\
 "Wmkabox2 - Win32 Debug"
!MESSAGE 指定された ﾋﾞﾙﾄﾞ ﾓｰﾄﾞ "$(CFG)" は正しくありません。
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛ 'CFG' を定義することによって
!MESSAGE NMAKE 実行時にﾋﾞﾙﾄﾞ ﾓｰﾄﾞを指定できます。例えば:
!MESSAGE 
!MESSAGE NMAKE /f "Wmkabox2.mak" CFG="Wmkabox2 - Win32 Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "Wmkabox2 - Win32 Release" ("Win32 (x86) Application" 用)
!MESSAGE "Wmkabox2 - Win32 Debug" ("Win32 (x86) Application" 用)
!MESSAGE 
!ERROR 無効な構成が指定されています。
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 
################################################################################
# Begin Project
# PROP Target_Last_Scanned "Wmkabox2 - Win32 Debug"
CPP=cl.exe
RSC=rc.exe
MTL=mktyplib.exe

!IF  "$(CFG)" == "Wmkabox2 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
OUTDIR=.\Release
INTDIR=.\Release

ALL : "$(OUTDIR)\Wmkabox2.exe"

CLEAN : 
	-@erase "$(INTDIR)\dcp.obj"
	-@erase "$(INTDIR)\default.res"
	-@erase "$(INTDIR)\SbDisk.obj"
	-@erase "$(INTDIR)\SbFile.obj"
	-@erase "$(INTDIR)\wmkabox2.obj"
	-@erase "$(OUTDIR)\Wmkabox2.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)/Wmkabox2.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
RSC_PROJ=/l 0x411 /fo"$(INTDIR)/default.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Wmkabox2.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\
 /subsystem:windows /incremental:no /pdb:"$(OUTDIR)/Wmkabox2.pdb" /machine:I386\
 /out:"$(OUTDIR)/Wmkabox2.exe" 
LINK32_OBJS= \
	"$(INTDIR)\dcp.obj" \
	"$(INTDIR)\default.res" \
	"$(INTDIR)\SbDisk.obj" \
	"$(INTDIR)\SbFile.obj" \
	"$(INTDIR)\wmkabox2.obj"

"$(OUTDIR)\Wmkabox2.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Wmkabox2 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "$(OUTDIR)\Wmkabox2.exe"

CLEAN : 
	-@erase "$(INTDIR)\dcp.obj"
	-@erase "$(INTDIR)\default.res"
	-@erase "$(INTDIR)\SbDisk.obj"
	-@erase "$(INTDIR)\SbFile.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(INTDIR)\wmkabox2.obj"
	-@erase "$(OUTDIR)\Wmkabox2.exe"
	-@erase "$(OUTDIR)\Wmkabox2.ilk"
	-@erase "$(OUTDIR)\Wmkabox2.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)/Wmkabox2.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
RSC_PROJ=/l 0x411 /fo"$(INTDIR)/default.res" /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Wmkabox2.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo\
 /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)/Wmkabox2.pdb" /debug\
 /machine:I386 /out:"$(OUTDIR)/Wmkabox2.exe" 
LINK32_OBJS= \
	"$(INTDIR)\dcp.obj" \
	"$(INTDIR)\default.res" \
	"$(INTDIR)\SbDisk.obj" \
	"$(INTDIR)\SbFile.obj" \
	"$(INTDIR)\wmkabox2.obj"

"$(OUTDIR)\Wmkabox2.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

################################################################################
# Begin Target

# Name "Wmkabox2 - Win32 Release"
# Name "Wmkabox2 - Win32 Debug"

!IF  "$(CFG)" == "Wmkabox2 - Win32 Release"

!ELSEIF  "$(CFG)" == "Wmkabox2 - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\wmkabox2.cpp
DEP_CPP_WMKAB=\
	".\dcp.h"\
	

"$(INTDIR)\wmkabox2.obj" : $(SOURCE) $(DEP_CPP_WMKAB) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\default.rc
DEP_RSC_DEFAU=\
	".\icon1.ico"\
	

"$(INTDIR)\default.res" : $(SOURCE) $(DEP_RSC_DEFAU) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\SbFile.cpp
DEP_CPP_SBFIL=\
	".\SbFile.h"\
	

"$(INTDIR)\SbFile.obj" : $(SOURCE) $(DEP_CPP_SBFIL) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\SbDisk.cpp
DEP_CPP_SBDIS=\
	".\SbDisk.h"\
	

"$(INTDIR)\SbDisk.obj" : $(SOURCE) $(DEP_CPP_SBDIS) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\dcp.cpp
DEP_CPP_DCP_C=\
	".\dcp.h"\
	".\SbDisk.h"\
	".\SbFile.h"\
	

"$(INTDIR)\dcp.obj" : $(SOURCE) $(DEP_CPP_DCP_C) "$(INTDIR)"


# End Source File
# End Target
# End Project
################################################################################
