// THIS ONE COMPILES!

#include "tg_AAPFlite.h"

#include <cstdio>
#include <cstring>

tg_AAPFlite::tg_AAPFlite()
{
	// initialise with some sensible default values in case they aren't passed in
	readPointer = writePointer = 0;
	localFs = 48000;
	maxDelay_samples = 96000; // 2 seconds at 48kHz sample rate - HARDCODED VALUE!
	delayLine = new double[maxDelay_samples]; // dynamically allocate 96000 samples
	delayLength_samples = 12000;
	absorbentGain = 0.7;
	lpfCoefficient = 0.6;			//LPF FB Gain
	feedbackGain = 0.61803; 	// The maximum all-pass gain coefficient that sounds good - see Dahl & Jot paper from 2000
	reset(localFs); // call the reset function below to fill the memory with zeroes so it's ready to use
}

tg_AAPFlite::~tg_AAPFlite()
{
	printf("---> tg_AAPFlite: Deleting delayLine\n");
	delete[] delayLine;
}

/**
 * \brief Resets the delay line memory and grabs the current sample rate
 * \param sampleRate Current sample rate
 * \return True
 */
bool tg_AAPFlite::reset(double sampleRate)
{
	printf("---> tg_AAPFlite: Resetting AAPF\n");
	readPointer = writePointer = 0;
	localFs = sampleRate;
	maxDelay_samples = 2 * sampleRate; // keep it at 1 second per AAPF based on sample rate.
	delayLine = new double[maxDelay_samples];
	printf("---> tg_AAPFlite: Allocating memory\n");
	memset(delayLine, 0, maxDelay_samples * sizeof(double)); // fill the memory with zeroes so it's clear
	// otherwise you'd hear the echoes from the previous time you ran the function!
	return true;
}

void tg_AAPFlite::changeDelayLength(int newMax)
{
	delete[] delayLine; // delete the old delay line
	maxDelay_samples = newMax; // set the max delay to the new parame ter provided by the user
	delayLine = new double[maxDelay_samples];
	reset(localFs); // reset the pointers and clear the delay line
}

/**
 * \brief Processes samples through an an absorbent all pass filter, where a delay block is followed by a one pole low pass filter and a gain scaling factor
 * \param input Input sample
 * \return Processed sample
 */
double tg_AAPFlite::processAudio(double input)
{
	//processing]
	//	printf("---> tg_AAPFlite: processing audio\n");
	//calculate read pointer position
	readPointer = writePointer - delayLength_samples;
	if (readPointer < 0)
	{
		readPointer += maxDelay_samples;
	}
	// OnePole Low Pass Filter:

	lpfFeedforwardGain = 1 - lpfCoefficient;   //LPF FF coef to keep gain at 0db
	lpfMemoryBlock = 0;			//LPF Filter Memory Block

	//algorithm for echo
	delayLineOut = delayLine[readPointer];
	delayLineOut = lpfFeedforwardGain * delayLineOut + lpfMemoryBlock;
	lpfMemoryBlock = delayLineOut * lpfCoefficient;
	delayLineOut *= absorbentGain;
	Vn = input - delayLineOut * feedbackGain;
	double output = Vn * feedbackGain + delayLineOut;
	delayLine[writePointer] = Vn;

	//increment write pointer
	writePointer++;
	if (writePointer >= maxDelay_samples)
	{
		writePointer = 0;
	}
	//	printf("---> tg_AAPFlite: Passing output\n");
	return output;
}