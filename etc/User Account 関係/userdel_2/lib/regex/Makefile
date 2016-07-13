# Makefile for regexp
#

# defaults
PROGRAM = regexp
CC = gcc
CC_OPTS = -O2 -Wall
RM = rm -f

SOURCE = main.c make_nfa.c regexp.c
OBJ = $(SOURCE:.c=.o)

all: make_nfa.o regexp.o
	$(CC) $(CC_OPTS) -c -o main.o main.c
	$(CC) $(CC_OPTS) -o $(PROGRAM) $(OBJ)
debug: make_nfa.o regexp.o
	$(CC) $(CC_OPTS) -Ddebug -c -o main.o main.c
	$(CC) $(CC_OPTS) -o $(PROGRAM) $(OBJ)
%.o: %.c
	$(CC) $(CC_OPTS) -c -o $@ $<
clean:
	$(RM) $(OBJ) $(PROGRAM)

