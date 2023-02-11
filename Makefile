KULINA=/usr
PREFIX=/usr
CC	=g++ -pthread 
#CC	=cc -pthread
kgcalc		: kgcalc.o kgcalcCallbacks.o main.o evalexpr.o 
	 $(CC) -o kgcalc kgcalc.o kgcalcCallbacks.o evalexpr.o main.o \
	-I$(KULINA)/include $(KULINA)/lib/libkulina.a $(KULINA)/lib/libgm.a \
	-L/usr/X11R76/lib -lX11 -lXext -lm -lpthread  
	 install -m 755 -s kgcalc TARBALL/
install	: kgcalc
	 install -m 755 -s kgcalc $(PREFIX)/bin/
		mkdir -p /usr/share/kgcalc
		install -m 644  TARBALL/kgcalc.png /usr/share/kgcalc/kgcalc.png
		install -m 644  TARBALL/kgcalc.desktop /usr/share/applications
kgcalc.o	: kgcalc.c 
	 $(CC) -c kgcalc.c
evalexpr.o	: evalexpr.c 
	 $(CC) -c evalexpr.c
kgcalcCallbacks.o	: kgcalcCallbacks.c 
	 $(CC) -c kgcalcCallbacks.c
main.o	: main.c 
	 $(CC) -c main.c
clean	:  
	   rm -f *.o kgcalc
	   rm -f TARBALL/kgcalc
