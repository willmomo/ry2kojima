# Microsoft Developer Studio Generated NMAKE File, Format Version 4.20
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Application" 0x0101

!IF "$(CFG)" == ""
CFG=SHBrowseForFolder - Win32 Debug
!MESSAGE 構成が指定されていません。ﾃﾞﾌｫﾙﾄの SHBrowseForFolder - Win32 Debug を設定します。
!ENDIF 

!IF "$(CFG)" != "SHBrowseForFolder - Win32 Release" && "$(CFG)" !=\
 "SHBrowseForFolder - Win32 Debug"
!MESSAGE 指定された ﾋﾞﾙﾄﾞ ﾓｰﾄﾞ "$(CFG)" は正しくありません。
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛ 'CFG' を定義することによって
!MESSAGE NMAKE 実行時にﾋﾞﾙﾄﾞ ﾓｰﾄﾞを指定できます。例えば:
!MESSAGE 
!MESSAGE NMAKE /f "SHBrowseForFolder.mak" CFG="SHBrowseForFolder - Win32 Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "SHBrowseForFolder - Win32 Release" ("Win32 (x86) Application" 用)
!MESSAGE "SHBrowseForFolder - Win32 Debug" ("Win32 (x86) Application" 用)
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
MTL=mktyplib.exe
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SHBrowseForFolder - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
OUTDIR=.\Release
INTDIR=.\Release

ALL : "$(OUTDIR)\SHBrowseForFolder.exe"

CLEAN : 
	-@erase "$(INTDIR)\SHBrowseForFolder.obj"
	-@erase "$(INTDIR)\SHBrowseForFolder.pch"
	-@erase "$(INTDIR)\SHBrowseForFolder.res"
	-@erase "$(INTDIR)\SHBrowseForFolderDlg.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(OUTDIR)\SHBrowseForFolder.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/SHBrowseForFolder.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x411 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x411 /d "NDEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x411 /fo"$(INTDIR)/SHBrowseForFolder.res" /d "NDEBUG" /d "_AFXDLL"\
 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/SHBrowseForFolder.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386
LINK32_FLAGS=/nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)/SHBrowseForFolder.pdb" /machine:I386\
 /out:"$(OUTDIR)/SHBrowseForFolder.exe" 
LINK32_OBJS= \
	"$(INTDIR)\SHBrowseForFolder.obj" \
	"$(INTDIR)\SHBrowseForFolder.res" \
	"$(INTDIR)\SHBrowseForFolderDlg.obj" \
	"$(INTDIR)\StdAfx.obj"

"$(OUTDIR)\SHBrowseForFolder.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "SHBrowseForFolder - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "$(OUTDIR)\SHBrowseForFolder.exe"

CLEAN : 
	-@erase "$(INTDIR)\SHBrowseForFolder.obj"
	-@erase "$(INTDIR)\SHBrowseForFolder.pch"
	-@erase "$(INTDIR)\SHBrowseForFolder.res"
	-@erase "$(INTDIR)\SHBrowseForFolderDlg.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\SHBrowseForFolder.exe"
	-@erase "$(OUTDIR)\SHBrowseForFolder.ilk"
	-@erase "$(OUTDIR)\SHBrowseForFolder.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/SHBrowseForFolder.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x411 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x411 /d "_DEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x411 /fo"$(INTDIR)/SHBrowseForFolder.res" /d "_DEBUG" /d "_AFXDLL"\
 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/SHBrowseForFolder.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386
LINK32_FLAGS=/nologo /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)/SHBrowseForFolder.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/SHBrowseForFolder.exe" 
LINK32_OBJS= \
	"$(INTDIR)\SHBrowseForFolder.obj" \
	"$(INTDIR)\SHBrowseForFolder.res" \
	"$(INTDIR)\SHBrowseForFolderDlg.obj" \
	"$(INTDIR)\StdAfx.obj"

"$(OUTDIR)\SHBrowseForFolder.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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

# Name "SHBrowseForFolder - Win32 Release"
# Name "SHBrowseForFolder - Win32 Debug"

!IF  "$(CFG)" == "SHBrowseForFolder - Win32 Release"

!ELSEIF  "$(CFG)" == "SHBrowseForFolder - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\ReadMe.txt

!IF  "$(CFG)" == "SHBrowseForFolder - Win32 Release"

!ELSEIF  "$(CFG)" == "SHBrowseForFolder - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SHBrowseForFolder.cpp
DEP_CPP_SHBRO=\
	".\SHBrowseForFolder.h"\
	".\SHBrowseForFolderDlg.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\SHBrowseForFolder.obj" : $(SOURCE) $(DEP_CPP_SHBRO) "$(INTDIR)"\
 "$(INTDIR)\SHBrowseForFolder.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\SHBrowseForFolderDlg.cpp
DEP_CPP_SHBROW=\
	".\SHBrowseForFolder.h"\
	".\SHBrowseForFolderDlg.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\SHBrowseForFolderDlg.obj" : $(SOURCE) $(DEP_CPP_SHBROW) "$(INTDIR)"\
 "$(INTDIR)\SHBrowseForFolder.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\StdAfx.cpp
DEP_CPP_STDAF=\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "SHBrowseForFolder - Win32 Release"

# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/SHBrowseForFolder.pch" /Yc"stdafx.h"\
 /Fo"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\SHBrowseForFolder.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "SHBrowseForFolder - Win32 Debug"

# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/SHBrowseForFolder.pch" /Yc"stdafx.h"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\SHBrowseForFolder.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SHBrowseForFolder.rc
DEP_RSC_SHBROWS=\
	".\res\SHBrowseForFolder.ico"\
	".\res\SHBrowseForFolder.rc2"\
	

"$(INTDIR)\SHBrowseForFolder.res" : $(SOURCE) $(DEP_RSC_SHBROWS) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


# End Source File
# End Target
# End Project
################################################################################
