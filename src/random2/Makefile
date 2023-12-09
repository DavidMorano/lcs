# MAKEFILE

T= random2

#SRCROOT= $(LOCAL)

BINDIR= $(SRCROOT)/bin
INCDIR= $(SRCROOT)/include
LIBDIR= $(SRCROOT)/lib/$(OFD)


CC= gcc
CCOPTS= -O3
#CCOPTS=
#CCOPTS= -xO4 -xtarget=ultra -xsafe=mem -dalign -xdepend
DEFINES= -D$(OS)=1
INCDIRS= -I$(INCDIR)
CFLAGS= $(CCOPTS) $(DEFINES) $(INCDIRS)

LD= $(CC)
LDFLAGS=
LIBDIRS= -L$(LIBDIR)
LIBS= -ldam -lb -lu


#CRT0= /lib/crt0.o
CRT0=



OBJ1= main.o procfile.o version.o strings.o random2.o 
OBJ2= brand.o count.o ba.o
OBJ= $(OBJ1) $(OBJ2)


ALL= $(T)


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



