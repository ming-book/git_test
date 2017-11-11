# Makefile 
.SUFFIXES:
.SUFFIXES:  .c .o

all: testmsgreal

CC	= gcc

WORKDIR=.
LIBDIR=$(WORKDIR)/lib
LIBS = -L$(LIBDIR) -lmymessagereal
OUTDIR=$(HOME)/bin

INCLDIR=$(WORKDIR)/inc
INCLFLG= -I$(WORKDIR)/inc
CFLAG= $(INCLFLG) -g -c -Wall

VPATH = $(WORKDIR)/src

OBJ1 = testkeymngmsg.o

testmsgreal:$(OBJ1)
	$(CC) $^ -o $@ $(LIBS)

#testmsgreal:testkeymngmsg.o testkeymngmsg2.o  testkeymngmsg3.o
#	gcc -L/home/it01/lib -lmymessagereal testkeymngmsg.o testkeymngmsg2.o  testkeymngmsg3.o -o testmsgreal

.c.o:
	$(CC) $(CFLAG) $<
	

#*.o:*.c      1.c 2.c 3.c 
#	gcc -I./incl -g -c -Wall $<
	
#	gcc -I./incl -g  1.c   -c 1.o
#	gcc -I./incl -g  2.c   -c 2.o
#	gcc -I./incl -g  3.c   -c 3.o
	
	
.PHONY:clean
clean:
	rm *.o
	rm testmsgreal
	rm *.ber
	rm *.txt
	


