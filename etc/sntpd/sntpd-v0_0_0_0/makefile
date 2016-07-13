PROG=sntpd
OBJS=sntpd.r __os__.r kjmlog.r
CC=xcc.exe

INSTDIR=.

LIBS=socket.l \
     unix.l \
     sys_clib.l

CFLAGS=-mode=ucc -cw -eas -d_OPT_PROTOS -tp=SH3,ld,lcd,lc
LFLAGS=-mode=ucc -ol=m -ol=s -tp=SH3,ld,lcd,lc >xcc.map

all: $(INSTDIR)\$(PROG)
	@echo ok

$(INSTDIR)\$(PROG): $(OBJS)
	@names $(OBJS) >rlist
	@for %1 in ($(LIBS)) do echo -l=%1 >>rlist
	$(CC) $(LFLAGS) -z=rlist -fd=$*

$(OBJS): $*.c
	$(CC) $(CFLAGS) $*.c

depend:
	if exist depend.bak attrib -r depend.bak
	if exist depend.xcc attrib -r depend.xcc
	mkdep -o .r -f depend.xcc *.c

clean:
	-del rlist
	-del depend.bak
	-del xcc.map
	-del *.r
	-del $(INSTDIR)\$(PROG)

!INCLUDE depend.xcc

