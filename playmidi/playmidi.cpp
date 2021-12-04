// Based on playsmf.cpp: Simple program to test the MidiFileIn class by reading and playing
// all tracks from a given Standard MIDI file (by Gary Scavone, 2020)
//
// Modified to repeat file once, with added funtionality to create a random sequence
// of notes in the C major pentatonic scale.

#include "MidiFileIn.h"
#include "RtMidi.h"
#include <signal.h>
#include <unistd.h>

using namespace stk;

bool done = false;
static void finish(int ignore){ done = true; }

void usage(void) {
  // Error function in case of incorrect command-line
  // argument specifications.
  std::cout << "\nusage: playmidi file \n";
  std::cout << "   where file = a standard MIDI file, OR\n";
  std::cout << "usage: playmidi num time \n";
  std::cout << "   where num = number of notes to be played, \n";
  std::cout << "         time = time (ms) in between notes\n";
  exit(0);
}

bool chooseMidiPort( RtMidi *rtmidi );

int main( int argc, char *argv[] )
{
  RtMidiOut *midiout = 0;

  if ( argc < 2 || argc > 3 ) usage();

  // Attempt to instantiate MIDI output class.
  try {
    midiout = new RtMidiOut();
  }
  catch ( RtMidiError& error ) {
    error.printMessage();
    exit(0);
  }

  // Choose port
  try {
    if ( chooseMidiPort( midiout ) == false ) goto cleanup;
  }
  catch ( RtMidiError& error ) {
    error.printMessage();
    goto cleanup;
  }

  // Install an interrupt handler function.  Type "ctrl-c" to quit the
  // program.
  (void) signal(SIGINT, finish);
  
  // Play MIDI file
  if (argc == 2) {
    try {
      MidiFileIn midiFile( argv[1] );

      // Print a little information about the file.
      std::cout << "\nThe MIDI file (" << argv[1] << ") information:\n";
      std::cout << "  - format = " << midiFile.getFileFormat() << "\n";
      std::cout << "  - tracks = " << midiFile.getNumberOfTracks() << "\n";
      std::cout << "  - seconds / ticks = " << midiFile.getTickSeconds() << "\n";

      int nTracks = midiFile.getNumberOfTracks();
      std::vector<long> ticks( nTracks );
      std::vector< std::vector<unsigned char> > events( nTracks );

      std::cout << "\nPress <enter> to start reading/playing ... type Ctrl-C to quit.\n";
      char input;
      std::cin.get(input);

      // Initially get first event from each track
      for ( int i=0; i<nTracks; i++ ) {
        ticks[i] = (long) midiFile.getNextMidiEvent( &events[i], i );
        if ( events[i].size() == 0 ) ticks[i] = -1;
      }

      long minTicks;
      int minTrack;
      while ( !done ) {

        // Look for lowest tick value
        minTicks = 1000000000;
        minTrack = nTracks;
        for ( int i=0; i<nTracks; i++ ) {
          if ( ticks[i] >= 0 && ticks[i] < minTicks ) {
            minTicks = ticks[i];
            minTrack = i;
          }
        }
        if ( minTrack >= nTracks ) {
          done = true;
          goto repeat;
        }

        // Pause for the MIDI event delta time.
        Stk::sleep( (unsigned long) (minTicks * midiFile.getTickSeconds( minTrack ) * 1000 ) );

        // Subtrack minTicks from all tick values, send message(s) and reload
        for ( int i=0; i<nTracks; i++ ) {
          if ( ticks[i] < 0 ) continue;  // skip 'dead' tracks
          ticks[i] -= minTicks;
          if ( ticks[i] == 0 ) { // send out this event and reload
            midiout->sendMessage( &events[i] );
            ticks[i] = midiFile.getNextMidiEvent( &events[i], i );
            if ( events[i].size() == 0 ) ticks[i] = -1;
          }
        }
      }

    repeat:
      Stk::sleep(2000);

      for (int i = 0; i < nTracks; i++) {
        midiFile.rewindTrack(i);
      }

      for ( int i=0; i<nTracks; i++ ) {
        ticks[i] = (long) midiFile.getNextMidiEvent( &events[i], i );
        if ( events[i].size() == 0 ) ticks[i] = -1;
      }
      
      done = false;
      while ( !done ) {

        // Look for lowest tick value
        minTicks = 1000000000;
        minTrack = nTracks;
        for ( int i=0; i<nTracks; i++ ) {
          if ( ticks[i] >= 0 && ticks[i] < minTicks ) {
            minTicks = ticks[i];
            minTrack = i;
          }
        }
        if ( minTrack >= nTracks ) {
          done = true;
          goto clear;
        }

        // Pause for the MIDI event delta time.
        Stk::sleep( (unsigned long) (minTicks * midiFile.getTickSeconds( minTrack ) * 1000 ) );

        // Subtrack minTicks from all tick values, send message(s) and reload
        for ( int i=0; i<nTracks; i++ ) {
          if ( ticks[i] < 0 ) continue;  // skip 'dead' tracks
          ticks[i] -= minTicks;
          if ( ticks[i] == 0 ) { // send out this event and reload
            midiout->sendMessage( &events[i] );
            ticks[i] = midiFile.getNextMidiEvent( &events[i], i );
            if ( events[i].size() == 0 ) ticks[i] = -1;
          }
        }
      }

    clear:
      // Send an "all notes off" to the synthesizer on all 16 channels.
      for ( int i=0; i<16; i++ ) {
        events[0].clear();
        events[0].push_back( 0xb0+i );
        events[0].push_back( 0x7b );
        events[0].push_back( 0x0 );
        midiout->sendMessage( &events[0] );
      }
    }
    catch ( StkError & ) {
      // You might want to do something more useful here.
      std::cout << "\nAborting program!\n";
      goto cleanup;
    }
  }

  // Random sequence played twice
  else {
    std::vector<unsigned char> notes;
    notes.push_back(60);
    notes.push_back(62);
    notes.push_back(64);
    notes.push_back(67);
    notes.push_back(69);

    std::vector<unsigned char> to_play;
    srand(time(NULL));
    int size = atoi(argv[1]);
    for (int i = 0; i < size; i++) {
      to_play.push_back(notes[rand() % 5]);
    }

    std::cout << "\nPress <enter> to start reading/playing ... type Ctrl-C to quit.\n";
    char input;
    std::cin.get(input);

    std::vector<unsigned char> event(3);
    int wait = atoi(argv[2]);
    for (int t = 0; t < 2; t++) {
      for (int i = 0; i < size; i++) {
        event[0] = 144;
        event[1] = to_play[i];
        event[2] = 100;
        midiout->sendMessage( &event );
        Stk::sleep(wait);
        event[0] = 128;
        midiout->sendMessage( &event );
      }
      Stk::sleep(wait * 3);
    }
    goto cleanup;
  }

 cleanup:
  delete midiout;

  return 0;
}

bool chooseMidiPort( RtMidi *rtmidi )
{

  std::cout << "\nWould you like to open a virtual output port? [y/N] ";

  std::string keyHit;
  std::getline( std::cin, keyHit );
  if ( keyHit == "y" ) {
    rtmidi->openVirtualPort();
    return true;
  }

  std::string portName;
  unsigned int i = 0, nPorts = rtmidi->getPortCount();
  if ( nPorts == 0 ) {
    std::cout << "No output ports available!" << std::endl;
    return false;
  }

  if ( nPorts == 1 ) {
    std::cout << "\nOpening " << rtmidi->getPortName() << std::endl;
  }
  else {
    for ( i=0; i<nPorts; i++ ) {
      portName = rtmidi->getPortName(i);
      std::cout << "  Output port #" << i << ": " << portName << '\n';
    }

    do {
      std::cout << "\nChoose a port number: ";
      std::cin >> i;
    } while ( i >= nPorts );
    std::getline( std::cin, keyHit );  // used to clear out stdin
  }

  std::cout << "\n";
  rtmidi->openPort( i );

  return true;
}
