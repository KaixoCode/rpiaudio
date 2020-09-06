#pragma once
#include "Audio.h"
#include <mutex>

#define TWO_PI 6.28318530718
#define PI TWO_PI/2

namespace Wavetables {
    double Saw(double phase);
    double Sine(double phase);
    double Square(double phase);
}

class Oscillator {

private:
    float phase = 0;
    float psample = 0;

    float fm = 0;
    float am = 1;

public:
    double frequency = 60; // Hz

    double (*WaveTable)(double);

    // Constructor with sample rate and buffer size
    Oscillator() { WaveTable = Wavetables::Sine; };

    // Destructor
    ~Oscillator() {};

    // Generate the next buffer
    float * NextBuffer();
    float NextSample();
    float GetSample();

    float GetDelta();

    void ResetPhase();

    void FM(Oscillator, float);
    void AM(float);
};

