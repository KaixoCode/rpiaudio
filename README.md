# rpiaudio
This is a test project for generating sound on a Raspberry Pi, it needs ALSA to work on linux. And uses Portaudio on Windows, mainly for testing purposes. It also relies on RtMidi to receive Midi signals.

**Dependencies are not included in this repository.**

The Audio namespace is one of the main parts. It takes care of forwarding the sound to the OS. This is easily used by first calling the Start() method and setting a callback method, which takes as argument a float array, which will be of size Audio::BUFFER_SIZE, which you can fill with samples between -1 and 1. The other part is the Midi class, which connects to the first Midi device. You can also set a MidiPress, MidiRelease and MidiControl callback method. When the midi device disconnects and reconnects it makes sure to bind it again.
