#pragma once

// Neutral retail-layout description. No original class or API name is claimed.
struct FixedPointBufferState
{
    unsigned int parameters[14];                       // 000
    unsigned int* primaryBuffers[4];                    // 038
    unsigned int* secondaryBuffers[4];                  // 048
    unsigned int* feedbackBuffers[4][2];                // 058
    unsigned int* terminalBuffers[4];                   // 078
    unsigned int unknown088[4];                         // 088
    unsigned int primaryLength, primaryIndex;           // 098
    unsigned int secondaryLength, secondaryIndex;       // 0A0
    unsigned int firstFeedbackLength, secondFeedbackLength; // 0A8
    unsigned int firstFeedbackIndex, secondFeedbackIndex;   // 0B0
    unsigned int firstFeedbackCoefficient, secondFeedbackCoefficient; // 0B8
    unsigned int terminalLength, terminalIndex;         // 0C0
    unsigned int terminalCoefficient;                   // 0C8
    unsigned int accumulators[4];                       // 0CC
    unsigned int primaryCoefficient;                    // 0DC
    unsigned int outputCoefficient;                     // 0E0
    unsigned int unknown0E4;                            // 0E4
    unsigned int accumulatorCoefficient;                // 0E8
    unsigned int retainedParameters[5];                 // 0EC
    unsigned char enabled;                             // 100
};

#ifdef NON_MATCHING
extern "C" void fn_001F9F48(FixedPointBufferState*, unsigned int* const*);
#endif
