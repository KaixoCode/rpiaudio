
#include "Audio.h"

#include <portaudio.h>
#include <thread>
#include <complex>
#include <valarray>


namespace Audio {

    //short-hand for complex<double> representing a+bi
    typedef std::complex<float> ComplexVal;

    //array of complex-valued samples
    typedef std::valarray<ComplexVal> SampleArray;

    // Phasedata for outgoing 2 channel audio stream
    typedef struct {
        float left_phase;
        float right_phase;
    } PaPhaseData;
    
    static int playCallback(const void*, void*, unsigned long, const PaStreamCallbackTimeInfo*, PaStreamCallbackFlags, void*);
    
    // Playback info
    const unsigned int SAMPLE_RATE = 44100;
    const unsigned int BUFFER_SIZE = 128;
    const unsigned int CHANNELS = 1;

    // Buffers
    static float buffer[BUFFER_SIZE];

    // Callback method
    void (*Callback)(float *);
    
    SampleArray samplesLeft, samplesRight;
    PaError err;
    PaStream* stream;

    // Init portaudio
    int Start() {    
        err = Pa_Initialize();
        if (err != paNoError) return -1;

        // Set input parameters
        PaPhaseData phaseData;
        err = Pa_OpenDefaultStream(&stream, 0, CHANNELS, paFloat32, SAMPLE_RATE, BUFFER_SIZE, playCallback, &phaseData);
        if (err != paNoError) return -1;

        // Start
        err = Pa_StartStream(stream);
        if (err != paNoError) return -1;
        return 0;
    }
    
    // Cleanup
    void Clean() {
        err = Pa_StopStream(stream);
    }

    // Callback Portaudio outgoing audio
    static int playCallback(const void* inputBuffer, void* outputBuffer,
        unsigned long framesPerBuffer,
        const PaStreamCallbackTimeInfo* timeInfo,
        PaStreamCallbackFlags statusFlags,
        void* userData) {

        // Put all the data in the output buffer
        float* out = (float*)outputBuffer;
        unsigned int i;

        // Call the callback method to request data
        if (Callback) Callback(buffer);

        for (i = 0; i < framesPerBuffer; i++) {
            *out++ = buffer[i];
        }
        return 0;
    }

    // Sets the callback
    void SetCallback(void func (float*)) {
        Callback = func;
    }
}