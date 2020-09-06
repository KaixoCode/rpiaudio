
#include <cmath>
#include <vector>
#include <iostream>
#include <RtMidi.h>
#include <thread>

class Midi {
private:
    RtMidiIn *midiin;
    std::thread connectThread;
    bool active;

public:
    Midi();
    ~Midi();

    void (*MidiPress)(int note, int velocity);
    void (*MidiRelease)(int note, int velocity);
    void (*MidiControl)(int cc, int value);

    static float NoteToFreq(int note) {
        return std::pow(2, (note - 69) / 12.0) * 440;
    }
};