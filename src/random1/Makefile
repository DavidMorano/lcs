# MAKEFILE

T= random1

ALL= $(T)

#SRCROOT= $(LOCAL)

BINDIR= $(SRCROOT)/bin
INCDIR= $(SRCROOT)/include
LIBDIR= $(SRCROOT)/lib


CC= gcc

DEFINES= -D$(OS)=1
INCDIRS= -I$(INCDIR)
#CFLAGS= -O3 $(DEFINES) $(INCDIRS)

LD= $(CC)
LDFLAGS=
LIBDIRS= -L$(LIBDIR)
LIBS= -ldam -lb


#CRT0= /lib/crt0.o
CRT0=



OBJ1= main.o procfile.o version.o strings.o random1.o 
OBJ2= brand.o count.o
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



