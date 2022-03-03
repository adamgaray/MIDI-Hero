### Example Makefile for MUMT-306 Homework assignments on OS-X systems

PROGRAMS = playmidi
RM = /bin/rm
CC       = g++
DEFS     = -D__MACOSX_CORE__
DEFS     += -D__LITTLE_ENDIAN__
CFLAGS   = -O3 -Wall
LIBRARY = -lm -framework CoreMIDI -framework CoreFoundation -framework CoreAudio

all : $(PROGRAMS)

clean : 
	-rm -f *~ *.o
	-rm $(PROGRAMS)

strip : 
	strip $(PROGRAMS)

## Include all source files necessary for a program in the dependencies and compiler statements below
playmidi: playmidi.cpp RtMidi.o MidiFileIn.o Stk.o
	$(CC) $(CFLAGS) $(DEFS) -o playmidi playmidi.cpp RtMidi.o MidiFileIn.o Stk.o $(LIBRARY)

RtMidi.o: RtMidi.cpp RtMidi.h
	$(CC) $(CFLAGS) $(DEFS) -c RtMidi.cpp -o RtMidi.o

Stk.o: Stk.cpp
	$(CC) $(CFLAGS) $(DEFS) -c Stk.cpp -o Stk.o

MidiFileIn.o: MidiFileIn.cpp Stk.cpp
	$(CC) $(CFLAGS) $(DEFS) -c MidiFileIn.cpp -o MidiFileIn.o
