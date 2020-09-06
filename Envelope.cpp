#include "Envelope.h"
#include <vector>
#include <iostream>
#include <algorithm>
#include <cmath>


float * ADSR::GenerateNextBuffer() {
    std::vector<float> buffer(Audio::BUFFER_SIZE);

    for (int i = 0; i < Audio::BUFFER_SIZE; i++) 
        buffer[i] = GenerateNextSample();
    
    return buffer.data();
}

float ADSR::GenerateNextSample() {
    if (phase >= 0 && (phase < A + D || !gate)) phase += 1.0/Audio::SAMPLE_RATE;
    else if (gate)
        phase = A + D;
    return phase < 0 ? 0 : phase < A ? std::pow(phase / A, AC) : phase <= A + D ? 1 - (1-S)*std::pow((phase-A) / D, DC) : phase < A + D + R ? S - S*std::pow((phase-A-D) / R, RC) : 0;
}

void ADSR::Trigger() {
    phase = 0;
}

void ADSR::Gate(bool g) {
    gate = g;
}