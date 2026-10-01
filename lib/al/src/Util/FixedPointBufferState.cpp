#include <Util/FixedPointBufferState.h>

#ifdef NON_MATCHING
// The low-word adds, subtracts, negations and products use unsigned arithmetic.
// ARMCC and the supported native targets interpret an unsigned word converted to
// int as its two's-complement signed value, and right-shift it arithmetically.
// These implementation-defined choices are explicit requirements, not signed UB.
typedef char FixedPointWordWidth[(sizeof(unsigned int) == 4 && sizeof(int) == 4) ? 1 : -1];
typedef char FixedPointSignedView[(static_cast<int>(0x80000000u) == (-2147483647 - 1)) ? 1 : -1];
typedef char FixedPointSignedShift[((-1 >> 7) == -1) ? 1 : -1];

extern "C" void fn_001F9F48(FixedPointBufferState* state, unsigned int* const* buffers)
{
    if (!state->enabled)
        return;

    int channel = 0;
    unsigned int* sampleBuffers[4] = {buffers[0], buffers[1], buffers[2], buffers[3]};
    unsigned int primaryIndex;
    unsigned int secondaryIndex;
    unsigned int firstFeedbackIndex;
    unsigned int secondFeedbackIndex;
    unsigned int terminalIndex;
    for (; channel < 2; ++channel)
    {
        primaryIndex = state->primaryIndex;
        secondaryIndex = state->secondaryIndex;
        firstFeedbackIndex = state->firstFeedbackIndex;
        secondFeedbackIndex = state->secondFeedbackIndex;
        terminalIndex = state->terminalIndex;
        unsigned int* primaryBuffer = state->primaryBuffers[channel];
        unsigned int* secondaryBuffer = state->secondaryBuffers[channel];
        for (int sample = 0; sample < 160; ++sample)
        {
            unsigned int primaryValue = primaryBuffer[primaryIndex];
            unsigned int sampleValue = sampleBuffers[channel][sample];
            primaryBuffer[primaryIndex] = sampleValue;
            unsigned int secondaryValue = secondaryBuffer[secondaryIndex];
            secondaryBuffer[secondaryIndex] = sampleValue;
            unsigned int primaryContribution = primaryValue * state->primaryCoefficient;
            unsigned int* firstFeedbackBuffer = state->feedbackBuffers[channel][0];
            unsigned int firstFeedbackValue = firstFeedbackBuffer[firstFeedbackIndex];
            unsigned int firstFeedbackContribution;
            if (static_cast<int>(firstFeedbackValue) < 0)
                firstFeedbackContribution = -(static_cast<int>((0u - firstFeedbackValue) * state->firstFeedbackCoefficient) >> 7);
            else
                firstFeedbackContribution = static_cast<int>(firstFeedbackValue * state->firstFeedbackCoefficient) >> 7;
            firstFeedbackBuffer[firstFeedbackIndex] = secondaryValue + firstFeedbackContribution;
            unsigned int* secondFeedbackBuffer = state->feedbackBuffers[channel][1];
            unsigned int secondFeedbackValue = secondFeedbackBuffer[secondFeedbackIndex];
            unsigned int secondFeedbackContribution;
            if (static_cast<int>(secondFeedbackValue) < 0)
                secondFeedbackContribution = -(static_cast<int>((0u - secondFeedbackValue) * state->secondFeedbackCoefficient) >> 7);
            else
                secondFeedbackContribution = static_cast<int>(secondFeedbackValue * state->secondFeedbackCoefficient) >> 7;
            secondFeedbackBuffer[secondFeedbackIndex] = secondaryValue + secondFeedbackContribution;
            unsigned int difference = firstFeedbackValue - secondFeedbackValue;
            unsigned int* terminalBuffer = state->terminalBuffers[channel];
            unsigned int terminalValue = terminalBuffer[terminalIndex];
            unsigned int terminalCoefficient = state->terminalCoefficient;
            unsigned int terminalContribution = terminalValue;
            if (static_cast<int>(terminalContribution) < 0)
            {
                terminalContribution = -terminalContribution;
                terminalContribution *= terminalCoefficient;
                terminalContribution = static_cast<int>(terminalContribution) >> 7;
                terminalContribution = -terminalContribution;
            }
            else
            {
                terminalContribution *= terminalCoefficient;
                terminalContribution = static_cast<int>(terminalContribution) >> 7;
            }
            difference += terminalContribution;
            terminalBuffer[terminalIndex] = difference;
            unsigned int scaledDifference;
            if (static_cast<int>(difference) < 0)
                scaledDifference = -(static_cast<int>((0u - difference) * terminalCoefficient) >> 7);
            else
                scaledDifference = static_cast<int>(difference * terminalCoefficient) >> 7;
            unsigned int result = terminalValue - scaledDifference;
            unsigned int accumulated = result + state->accumulators[channel];
            result -= static_cast<int>(accumulated * state->accumulatorCoefficient) >> 7;
            state->accumulators[channel] = result;
            sampleBuffers[channel][sample] =
                static_cast<int>(result * state->outputCoefficient + primaryContribution) >> 7;
            ++primaryIndex;
            ++secondaryIndex;
            ++firstFeedbackIndex;
            ++secondFeedbackIndex;
            ++terminalIndex;
        }
    }
    if (state->primaryLength > primaryIndex)
        state->primaryIndex = primaryIndex;
    else
        state->primaryIndex = 0;
    if (state->secondaryLength > secondaryIndex)
        state->secondaryIndex = secondaryIndex;
    else
        state->secondaryIndex = 0;
    if (state->firstFeedbackLength > firstFeedbackIndex)
        state->firstFeedbackIndex = firstFeedbackIndex;
    else
        state->firstFeedbackIndex = 0;
    if (state->secondFeedbackLength > secondFeedbackIndex)
        state->secondFeedbackIndex = secondFeedbackIndex;
    else
        state->secondFeedbackIndex = 0;
    if (state->terminalLength > terminalIndex)
        state->terminalIndex = terminalIndex;
    else
        state->terminalIndex = 0;
}
#endif
