
#include "Midi.h"
#include <chrono>
#include <thread>

void Callback(double deltatime, std::vector<unsigned char> *message, void *userData) {
    
    // Get amount of bytes in message
    unsigned int nBytes = message->size();

    // Distribute event
    if (nBytes >= 3) {
        Midi *data = (Midi*)userData;

        if (message->at(0) == 144 && data->MidiPress) {
            data->MidiPress(message->at(1), message->at(2));
        } else if (message->at(0) == 128 && data->MidiRelease) {
            data->MidiRelease(message->at(1), message->at(2));
        } else if (message->at(0) == 176 && data->MidiControl) {
            data->MidiControl(message->at(1), message->at(2));
        }
    }

    // Midi Debug
    if (true) {
        for ( unsigned int i=0; i<nBytes; i++ )
            std::cout << "Byte " << i << " = " << (int)message->at(i) << ", ";
        if ( nBytes > 0 )
            std::cout << "stamp = " << deltatime << std::endl;
    }
}

// Init Midi
Midi::Midi() {

    // new midiin
    midiin = new RtMidiIn();
    
    // Set thread to active
    active = true;
    connectThread = std::thread([this] {
        
        // Set callback
        bool connected = false;
        midiin->setCallback(&Callback, this);
        midiin->ignoreTypes(false, false, false);
        
        // This loop ensures that if theres a disconnect it can reconnect later.
        while (active)
        {
            
            // Get nmr of ports
            unsigned int nPorts = midiin->getPortCount();

            // Check if connected 
            if (nPorts > 1 && !connected)
            {
                midiin->openPort(1);
                connected = true;
            }
            else if (nPorts <= 1 && connected)
            {
                midiin->closePort();
                connected = false;
            }

            // Only check every 500ms
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    });
}

Midi::~Midi() {
    active = false;
    connectThread.join();
    delete midiin;
}
