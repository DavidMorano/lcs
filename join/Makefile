# MAKEFILE

T= ex1

ALL= $(T)

#SRCROOT= $(LOCAL)

BINDIR= $(SRCROOT)/bin
INCDIR= $(SRCROOT)/include
LIBDIR= $(SRCROOT)/lib


#CC= gcc
#CC= cc
CCOPTS= -xO4 -xtarget=ultra -xsafe=mem -dalign

DEFINES1= -DPOSIX=1 -D_POSIX_PTHREAD_SEMANTICS=1 -D_REENTRANT=1
DEFINES2= -DSOLARIS=1 
DEFINES3= -D$(OSTYPE)=1
DEFINES= $(DEFINES1) $(DEFINES2) $(DEFINES3)
INCDIRS= -I$(INCDIR)
CFLAGS= $(CCOPTS) $(DEFINES) $(INCDIRS)

LD= cc
#LD= $(CC)
LDFLAGS=
LIBDIRS= -L$(LIBDIR)/$(OFD)
LIBS= -Bstatic -ldam -lb -luc -Bdynamic -lu -lsocket -lnsl


#CRT0= /lib/crt0.o
CRT0=



OBJ1= main.o procfile.o version.o strings.o ex1.o 
OBJ2= brand.o ba.o counter.o bitnum.o
OBJ= $(OBJ1) $(OBJ2)



all:		$(ALL)

.c.o:
	$(CC) -c $(CFLAGS) $<


$(T):		$(OBJ) Makefile
	$(LD) -o $@ $(LDFLAGS) $(CRT0) $(OBJ) $(LIBDIRS) $(LIBS)

strip:		$(T)
	strip $(T)

install:	strip
	bsdinstall $(T) $(BINDIR)

again:
	rm -f $(ALL)

clean:
	rm -f *.o

lint:		main.c
	lint -uxn $(DEFINES) $(INCDIRS) main.c

control:
	(uname -n ; date) > Control


main.o:		main.c config.h defs.h

version.o:	version.c config.h

local1.o:	defs.h



