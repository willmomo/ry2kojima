# Microsoft Visual C++ generated build script - Do not modify

PROJ = CNPREST
DEBUG = 0
PROGTYPE = 0
CALLER = 
ARGS = 
DLLS = 
D_RCDEFINES = -d_DEBUG
R_RCDEFINES = -dNDEBUG
ORIGIN = MSVC
ORIGIN_VER = 1.00
PROJPATH = D:\PROJECTS\WIN16\PSCOMM\
USEMFC = 0
CC = cl
CPP = cl
CXX = cl
CCREATEPCHFLAG = 
CPPCREATEPCHFLAG = 
CUSEPCHFLAG = 
CPPUSEPCHFLAG = 
FIRSTC = DIB.C       
FIRSTCPP = COMMON.CPP  
RC = rc
CFLAGS_D_WEXE = /nologo /W3 /FR /G2 /Zi /D_DEBUG /Od /AM /GA /Fd"CNPREST.PDB"
CFLAGS_R_WEXE = /nologo /W3 /FR /O1 /DNDEBUG /AM /GA
LFLAGS_D_WEXE = /NOLOGO /ONERROR:NOEXE /NOD /PACKC:61440 /CO /ALIGN:16 /STACK:10240
LFLAGS_R_WEXE = /NOLOGO /ONERROR:NOEXE /NOD /PACKC:61440 /ALIGN:16 /STACK:10240
LIBS_D_WEXE = oldnames libw commdlg shell olecli olesvr winnls mlibcew
LIBS_R_WEXE = oldnames libw commdlg shell olecli olesvr winnls mlibcew
RCFLAGS = /nologo
RESFLAGS = /nologo
RUNFLAGS = 
DEFFILE = PSCOMM.DEF
OBJS_EXT = 
LIBS_EXT = 
!if "$(DEBUG)" == "1"
CFLAGS = $(CFLAGS_D_WEXE)
LFLAGS = $(LFLAGS_D_WEXE)
LIBS = $(LIBS_D_WEXE)
MAPFILE = nul
RCDEFINES = $(D_RCDEFINES)
!else
CFLAGS = $(CFLAGS_R_WEXE)
LFLAGS = $(LFLAGS_R_WEXE)
LIBS = $(LIBS_R_WEXE)
MAPFILE = nul
RCDEFINES = $(R_RCDEFINES)
!endif
!if [if exist MSVC.BND del MSVC.BND]
!endif
SBRS = COMMON.SBR \
		MAIN.SBR \
		PRESTAR.SBR \
		STATDLG.SBR \
		TTY.SBR \
		DIB.SBR \
		ERRORS.SBR


COMMON_DEP = d:\projects\win16\pscomm\main.h \
	d:\projects\win16\pscomm\prestar.h \
	d:\projects\win16\pscomm\tty.h


MAIN_DEP = d:\projects\win16\pscomm\main.h \
	d:\projects\win16\pscomm\prestar.h \
	d:\projects\win16\pscomm\tty.h \
	d:\projects\win16\pscomm\ext.h


PRESTAR_DEP = c:\msvc\include\mbstring.h \
	d:\projects\win16\pscomm\main.h \
	d:\projects\win16\pscomm\prestar.h \
	d:\projects\win16\pscomm\tty.h \
	d:\projects\win16\pscomm\ext.h \
	d:\projects\win16\pscomm\dib.h \
	d:\projects\win16\pscomm\errors.h \
	d:\projects\win16\pscomm\palette.h


STATDLG_DEP = d:\projects\win16\pscomm\main.h \
	d:\projects\win16\pscomm\prestar.h \
	d:\projects\win16\pscomm\tty.h \
	d:\projects\win16\pscomm\ext.h


TTY_DEP = d:\projects\win16\pscomm\main.h \
	d:\projects\win16\pscomm\prestar.h \
	d:\projects\win16\pscomm\tty.h \
	d:\projects\win16\pscomm\ext.h


DEFAULT_RCDEP = d:\projects\win16\pscomm\icon1.ico \
	d:\projects\win16\pscomm\parent.bmp \
	d:\projects\win16\pscomm\child.bmp \
	d:\projects\win16\pscomm\trance1.bmp \
	d:\projects\win16\pscomm\trance2.bmp \
	d:\projects\win16\pscomm\trance3.bmp \
	d:\projects\win16\pscomm\trance4.bmp \
	d:\projects\win16\pscomm\trance5.bmp \
	d:\projects\win16\pscomm\trance6.bmp \
	d:\projects\win16\pscomm\trance7.bmp \
	d:\projects\win16\pscomm\trance8.bmp \
	d:\projects\win16\pscomm\trance9.bmp \
	d:\projects\win16\pscomm\trance10.bmp


DIB_DEP = d:\projects\win16\pscomm\dib.h \
	d:\projects\win16\pscomm\errors.h \
	d:\projects\win16\pscomm\palette.h


ERRORS_DEP = d:\projects\win16\pscomm\errors.h


all:	$(PROJ).EXE $(PROJ).BSC

COMMON.OBJ:	COMMON.CPP $(COMMON_DEP)
	$(CPP) $(CFLAGS) $(CPPCREATEPCHFLAG) /c COMMON.CPP

MAIN.OBJ:	MAIN.CPP $(MAIN_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c MAIN.CPP

PRESTAR.OBJ:	PRESTAR.CPP $(PRESTAR_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c PRESTAR.CPP

STATDLG.OBJ:	STATDLG.CPP $(STATDLG_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c STATDLG.CPP

TTY.OBJ:	TTY.CPP $(TTY_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c TTY.CPP

DEFAULT.RES:	DEFAULT.RC $(DEFAULT_RCDEP)
	$(RC) $(RCFLAGS) $(RCDEFINES) -r DEFAULT.RC

DIB.OBJ:	DIB.C $(DIB_DEP)
	$(CC) $(CFLAGS) $(CCREATEPCHFLAG) /c DIB.C

ERRORS.OBJ:	ERRORS.C $(ERRORS_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c ERRORS.C


$(PROJ).EXE::	DEFAULT.RES

$(PROJ).EXE::	COMMON.OBJ MAIN.OBJ PRESTAR.OBJ STATDLG.OBJ TTY.OBJ DIB.OBJ ERRORS.OBJ $(OBJS_EXT) $(DEFFILE)
	echo >NUL @<<$(PROJ).CRF
COMMON.OBJ +
MAIN.OBJ +
PRESTAR.OBJ +
STATDLG.OBJ +
TTY.OBJ +
DIB.OBJ +
ERRORS.OBJ +
$(OBJS_EXT)
$(PROJ).EXE
$(MAPFILE)
c:\msvc\lib\+
c:\msvc\mfc\lib\+
$(LIBS)
$(DEFFILE);
<<
	link $(LFLAGS) @$(PROJ).CRF
	$(RC) $(RESFLAGS) DEFAULT.RES $@
	@copy $(PROJ).CRF MSVC.BND

$(PROJ).EXE::	DEFAULT.RES
	if not exist MSVC.BND 	$(RC) $(RESFLAGS) DEFAULT.RES $@

run: $(PROJ).EXE
	$(PROJ) $(RUNFLAGS)


$(PROJ).BSC: $(SBRS)
	bscmake @<<
/o$@ $(SBRS)
<<
