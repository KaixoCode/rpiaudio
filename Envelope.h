#include "Audio.h"


class Envelope {
public:
    virtual float * GenerateNextBuffer() = 0;
    virtual float GenerateNextSample() = 0;
    virtual void Trigger() = 0;
    virtual void Gate(bool) = 0;
};

class ADSR : Envelope {
    double phase = -1;
    bool gate = false;

public:
    double A = 0.0; // s
    double D = 0.1; // s
    double S = 1.0; // (0-1)
    double R = 0.3; // s

    double AC = 1.0; // curve
    double DC = 1.0; // curve
    double RC = 1.0; // curve

    float * GenerateNextBuffer() override;
    float GenerateNextSample() override;
    void Trigger() override;
    void Gate(bool) override;
};