PROG	= im01self.exe
OBJS	= im01self.obj

CC	= wcl
CFLAGS	= /ox /ot /oh /c /wx /zq /bt=dos
LFLAGS	= /ox /ot /oh /wx /zq /bt=dos
#CFLAGS	= /c /wx /zq /bt=dos
#LFLAGS	= /wx /zq /bt=dos
RM      = del

$(PROG) : $(OBJS)
	$(CC) $(LFLAGS) $(OBJS)

.cpp.obj:
	$(CC) $(CFLAGS) $[@

clean
        $(RM) *~
	$(RM) *.obj

