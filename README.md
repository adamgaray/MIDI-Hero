# MIDI Hero

"Guitar Hero" type game that involves timing button presses to flashing lights corresponding to MIDI events. Created for MUMT 306 Final Project.

## Introduction

As someone relatively new to electronics and other concepts covered over the semester, when designing a project for this course, I wanted something relatively simple that involved, at some level, all of the main ideas of MUMT 306. I wanted something that incorporated the Arduino Uno such that the user could do something on the breadboard, and I could control it with a .ino program. I wanted to use Max to incorporate serial communication, and I wanted to use RtMidi to control playback of MIDI files. As well, at some level I wanted to involve sequencing, whether it was with the Arduino, Max, or RtMidi. Most of all, I wanted something that I could have fun with, and enjoy using as well as making.

MIDI Hero incorporates all of this into a miniature, MIDI version of a "Guitar Hero" type game (hence the name). It can play a MIDI file, and will flash different LED lights corresponding to each note. Afterwards, it will repeat the file, so that the user can try and match the timing of the flashing LEDs by pressing a button. An RGB light indicates how accurate the timing of the button press was: green means very good, red means very bad, and yellowish colours mean somewhere in between. As well, the user can generate a random sequence of notes, and play a memorization game.

## Usage

Compile `playmidi/playmidi.cpp`, referring to [RtMidi](https://www.music.mcgill.ca/~gary/rtmidi/index.html#compiling) compiling instructions if necessary. 

Place any MIDI files to be played in `playmidi/midifiles` directory. The lights will flash according to whatever messages are sent on Channel 1. 

To play a repeated MIDI file, use `playmidi midifiles/file.mid`. To play a repeated random sequence of notes, use `playmidi num time`, where `num` is an integer defining the number of notes in the sequence, and `time` is an integer defining the time between notes in milliseconds.

Make sure `midisend.maxpat` is open and properly connected to the serial port. When prompted by `playmidi`, choose the output port to be the same as the input port of `midiin` in `midisend.maxpat`.

## Project Overview

### Electronics

There are 5 LED lights and 5 corresponding buttons, using 10 of the 12 digital pins. The buttons use the internal pull-up resistor, instead of external ones, to reduce the amount of external circuitry so that the buttons are more easily pressed.

The RGB light uses the remaining 2 digital (PWM) pins; the blue light was not used.

![image](https://user-images.githubusercontent.com/20567327/144700265-ad521ac5-d7d7-4acf-bf4a-77b0b2b9e01e.jpg)

### `lights.ino`

`lights.ino` controls the lights and the button timing mechanism. Due to the limited number of pins, it is only possible to represent 5 possible note values with LEDs, so the C major pentatonic scale is used. As a note is received from the Serial port (NoteOn), if the note is in the pentatonic scale (in any octave), the corresponding LED is lit, and the time is stored by `millis()`. When the same note is received again (NoteOff), the LED turns off.

When a button is pressed, the difference in time `diff` between the lighting of the respective LED and the button press is calculated. If the time is more than 1 second, the RGB will light red. If the time is less than 100 ms, the RGB will light green. For values in between, the RGB will light as some colour between red and green: `(255, 255-diff/4, 0)` such that the lower the `diff`, the more green, and the higher the more red.

[Here](https://www.youtube.com/watch?v=D1KF2Vptnd4&t=4s&ab_channel=AdamGaray) is a brief demonstration of the lights and button timing.

### Max

![image](https://user-images.githubusercontent.com/20567327/144700841-ea4da62d-3893-4f6d-bf8d-02c730956b5f.png)

The `midiparse` object is used to receive MIDI input, check for NoteOn messages on Channel 1, and send the note numbers through the serial port.

### RtMidi

`playmidi.cpp` uses [RtMidi](https://www.music.mcgill.ca/~gary/rtmidi/) and [Stk](https://ccrma.stanford.edu/software/stk/classstk_1_1MidiFileIn.html) to either read input MIDI files or output a random sequence. When a MIDI file is played, it is repeated to allow the user to hear it once, and then time the button presses with the repeated playback. The user can also choose to generate a random sequence of 5 notes from the pentatonic scale (only within one octave), which will also be played twice. The user can choose the number of notes and the time in between them.

## Demonstration

[Here](https://www.youtube.com/watch?v=lSF5s1EZKHo&feature=emb_title&ab_channel=AdamGaray) is a demonstration of the random sequence generation.

[Here](https://www.youtube.com/watch?v=dC_wzNfy00Y&ab_channel=AdamGaray) is a demonstration of a full MIDI file being played.

## Development Process, Results, and Project Expansion

Overall, the project development process was relatively smooth. Most of the effort was in developing the Arduino program. The Max patch is simple and I already had a template for `playmidi.cpp` so all that needed to be written was the random sequence generator. The most time was used in figuring out how the LEDs would light and how the button timing mechanism would work. As well, it took some trial and error figuring out exactly how I wanted the RGB to light, to make sure I had a reasonable balance of green or red or something in between. Personally, I have very little prior experience in electronics, so perhaps more time than expected was used to figure out and put together the circuits.

MIDI Hero is clearly just a prototype. Since the Arduino Uno only has 12 digital pins when using serial communication, only 5 note values are possible. In a full implementation, perhaps all 12 notes could be used, although this would make for a harder game. As well, using the LCD screen to act as a menu would be a nice feature, and would greater add to the feeling of an actual game, as opposed to using the command line. However, given the scope and amount of time dedicated, I think a reasonable prototype was created that achieved all the goals of this project.

## Conclusion

I'm happy with what I've created. This was an excellent way to dive a bit deeper into some concepts seen in lectures, and actually creating something myself was a great experience. While there are certainly many ways to improve this project, overall I achieved the goals I set for myself, and also had fun doing it.
