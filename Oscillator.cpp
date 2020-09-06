#include "Oscillator.h"
#include <vector>
#include <iostream>
#include <math.h>
#include <algorithm>
#include <mutex>

namespace Wavetables {

    double Saw(double phase) {
        return (phase/TWO_PI) * 2 - 1;
    }

    double Sine(double phase) {
        return sin(phase);
    }

    double Square(double phase) {
        return phase > PI ? -1 : 1;
    }
}

float * Oscillator::NextBuffer() {
    std::vector<float> buffer(Audio::BUFFER_SIZE);

    for (int i = 0; i < Audio::BUFFER_SIZE; i++) 
        buffer[i] = NextSample();
    
    return buffer.data();
}

float Oscillator::GetSample() {
    return WaveTable(phase);
}

float Oscillator::NextSample() {
    psample = GetSample();
    phase = std::fmod(phase + (fm + frequency) * TWO_PI / Audio::SAMPLE_RATE, TWO_PI);
    fm = 0;
    return WaveTable(phase);
}

void Oscillator::ResetPhase() {
    phase = 0;
}

float Oscillator::GetDelta() {
    return (GetSample() - psample) / (1.0 / Audio::SAMPLE_RATE);
}

void Oscillator::FM(Oscillator fm, float amt) {
    this->fm = fm.GetDelta() * amt;
}

void Oscillator::AM(float am) {
    this->am = am;
}




