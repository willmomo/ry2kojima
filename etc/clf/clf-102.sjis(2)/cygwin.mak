.SUFFIXES: .cpp

PROG		= clf
OBJS		= clf.o getopt.o getopt1.o
LDLIBS		= 
ifeq ($(CFG),release)
	CPPFLAGS	= -O -Wall
else
	CPPFLAGS	= -g -Wall 
endif

$(PROG): $(OBJS)
	$(LINK.o) $^ $(LOADLIBES) $(LDLIBS) -o $@

.cpp.o:
	$(COMPILE.cc) $< $(OUTPUT_OPTION)

clean:
	$(RM) *.o *~

rebuild: clean
	make -kf cygwin.mak

