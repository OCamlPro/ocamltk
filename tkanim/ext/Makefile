# Generated automatically from Makefile.in by configure.
include ../../Makefile.config

CC = gcc -g

X_CFLAGS= -I/opt/local/include
AC_FLAGS= -DHAVE_UNISTD_H=1 -DHAVE_LIMITS_H=1 -DHAVE_X11_XLIB_H=1 
# Sorry, I'm not sure about Configure, 
# so define AC_FLAGS as same as in Makefile for Tk library

INCLUDES=-I/usr/local/include $(X_CFLAGS)

# For debugging
# CFLAGS=$(INCLUDES) $(AC_FLAGS) -DTKANIM_DEBUG
# For shared library
# CFLAGS=-fPIC $(INCLUDES) $(AC_FLAGS)
CFLAGS= $(INCLUDES) $(AC_FLAGS)

TKANIM_OBJ = tkAnimGIF.o

TKANIM_SHLIB = libtkanim.so.1.0
TKANIM_STLIB = libtkanim.a

TKANIM_LIB = $(TKANIM_STLIB)
#TKANIM_LIB = $(TKANIM_SHLIB) $(TKANIM_STLIB)

LIBS = $(TKANIM_LIB)  -ltk8.5 -ltcl8.5 -ldl   -L/opt/local/lib  -lSM -lICE -lX11  -lm

all: animwish

animwish:	$(TKANIM_LIB) tkAppInit.o
	$(CC) -o $@ $(LFLAGS) tkAppInit.o \
		-L. -ltkanim $(LIBS)

$(TKANIM_SHLIB): $(TKANIM_OBJ)
# Shareable Library for SunOS
#	ld -assert pure-text -o $@ $(TKANIM_OBJ)
# Shareable Library for FreeBSD
#	ld -Bshareable -o $@ $(TKANIM_OBJ)
# Shareable Library for Linux
	gcc -shared -Wl,-soname,libtkanim.so.1 -o $@ $(TKANIM_OBJ)
 
# Static Library
$(TKANIM_STLIB): $(TKANIM_OBJ)
	ar rc $@ $(TKANIM_OBJ)
	ranlib $@

install: $(TKANIM_STLIB)
	cp $(TKANIM_STLIB) $(INSTALLDIR)
	cd $(INSTALLDIR); ranlib $(TKANIM_LIB)

clean:
	rm -rf *.o $(TKANIM_LIB) animwish
