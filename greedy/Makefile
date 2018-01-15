# MAKEFILE

T= greedy

#SRCROOT= $(LOCAL)

BINDIR= $(SRCROOT)/bin
INCDIR= $(SRCROOT)/include
LIBDIR= $(SRCROOT)/lib/$(OFD)


CC= gcc
DEFINES= -D$(OS)=1
INCDIRS= -I$(INCDIR)
CFLAGS= -O4 $(DEFINES) $(INCDIRS)

LD= $(CC)
LDFLAGS=
LIBDIRS= -L$(LIBDIR)
LIBS= -ldam -lb


#CRT0= /lib/crt0.o
CRT0=



OBJ= main.o procfile.o version.o strings.o greedy.o


ALL= $(T)


all:		$(ALL)

.c.o:
	$(CC) -c $(CFLAGS) $<


$(T):		$(OBJ) Makefile
	$(LD) -o $@ $(LDFLAGS) $(CRT0) $(OBJ) $(LIBDIRS) $(LIBS)

strip:		$(T)
	strip $(T)

install:	string
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



