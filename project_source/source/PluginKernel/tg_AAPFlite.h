#pragma once

#ifndef _tg_AAPFlite_h__
#define _tg_AAPFlite_h__

class tg_AAPFlite
{
public:
	double delayLength_samples; // delay time
	double maxDelay_samples; // max delay time
	int readPointer, writePointer; // read and write pointers
	double absorbentGain; // Jot's a gain.
	double lpfCoefficient;
	double feedbackGain; // feedback gain (f [slider])
	double localFs;

	double* delayLine{}; // pointer to memory
	tg_AAPFlite();  // constructor
	~tg_AAPFlite(); // destructor

	void changeDelayLength(int newMax);// allow user to change our default maximum delay value
	bool reset(double sampleRate); // reset or initialise
	double processAudio(double input); // take in single sample and pass back single sample

private:
	double lpfFeedforwardGain;
	double lpfMemoryBlock;
	double delayLineOut;
	double Vn;
};

#endif