#include "Audio.h"

#include <iostream>
#include <math.h>
#include <chrono>
#include <thread>
#include "Oscillator.h"
#include "Midi.h"
#include "Envelope.h"

void AudioCallback(float *buffer);

#define VOICES 1

Oscillator osc[VOICES];
ADSR env[VOICES];

Oscillator fmosc;

int notepervoice[VOICES];
bool active[VOICES];


int pvoice = 0;

float amt = 0;

void MidiPress(int note, int velocity) {
    if (active[pvoice]) {
        env[pvoice].Gate(false);
    }
    
    osc[pvoice].frequency = Midi::NoteToFreq(note);
    osc[pvoice].ResetPhase();
    env[pvoice].Trigger();
    env[pvoice].Gate(true);
    active[pvoice] = true;
    notepervoice[pvoice] = note;
    pvoice = (pvoice + 1) % VOICES;
}

void MidiRelease(int note, int velocity) {
    int index = -1;
    for (int i = 0; i < VOICES; i++)
    {
        if (notepervoice[i] == note) {
            index = i;
            break;
        }
    }
    
    if (index == -1)
        return;
    notepervoice[index] = -1;
    active[index] = false;
    env[index].Gate(false);
}

void MidiControl(int cc, int value) {
    if (cc == 1) {
        amt = 0.2*value;
    }
}

int main(void) {

    Midi midi;
    midi.MidiPress = MidiPress;
    midi.MidiRelease = MidiRelease;
    midi.MidiControl = MidiControl;
    
    for (int i = 0; i < VOICES; i++)
    {
        osc[i].WaveTable = Wavetables::Sine;
    }
    fmosc.WaveTable = Wavetables::Sine;
    fmosc.frequency = 1;

    Audio::Start();    
    Audio::SetCallback(AudioCallback);
    
  
    while (true)
    {

    }

    Audio::Clean();
    return 0;
}


void AudioCallback(float *buffer)
{
    for (int i = 0; i < Audio::BUFFER_SIZE; i++) {
        double mix = 0;

        fmosc.frequency = osc[0].frequency * 1.01;
        osc[0].NextSample();
        fmosc.FM(osc[0], amt);
        //std::cout << fmosc.GetDelta() * 100 << std::endl;
        double o1 = fmosc.NextSample();
        double e1 = env[0].GenerateNextSample();
        mix += o1 * e1;
        
        buffer[i] = mix * 0.2;
    }
}