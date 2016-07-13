SUB_LIB=ftp.l
OBJS=__os__.r \
     ftp_cmd.r \
     ftpclient.r

CC=xcc.exe
LIB=libgen.exe
LIBDIR=..\lib

CFLAGS=-mode=ucc -cw -eas -v=..\include -d_OPT_PROTOS -dNDEBUG -tp=SH3,ld,lcd,lc

all: $(LIBDIR)\$(SUB_LIB)
	@echo SUB Library Normal Created

$(LIBDIR)\$(SUB_LIB): $(OBJS)
	$(LIB) -c -o=$@ $(OBJS)

$(OBJS): $*.c
        $(CC) $(CFLAGS) $*.c

depend:
	if exist depend.bak attrib -r depend.bak
	mkdep -I..\include -o .r -f depend.xcc *.c

clean:
	-del depend.bak
	-del *.r
	-del $(LIBDIR)\$(SUB_LIB)

!INCLUDE depend.xcc

