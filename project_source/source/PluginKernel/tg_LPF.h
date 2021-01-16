#pragma once

#ifndef _tg_LPF_h
#define _tg_LPF_h

class tg_LPF
{
public:
	double lpf_cornerFrequency{};
	double lpf_sampleRate{};
	double lpf_dBGainAtFc{};
	double lpf_hfRatio{};
	double lpf_delayTime;
	double lpf_decayTime_Tr{}; // Decay time in milliseconds
	double lpfCoefficient_b{};

	tg_LPF();  // Constructor
	~tg_LPF(); // Destructor

	double calculateCoefficient(double hfRatio, double cornerFrequency, double delayLength_mSec, double decayTime_mSec);// Determine the correct filter coefficient based on user control parameters
	double processAudio(double input); // Pass a single sample through the low pass filter, and spit it back out
	bool reset(double sampleRate); // The big red reset button

private:
	double lpf_G{};
	double lpf_A{};
	double lpf_omega{};
	double lpf_memoryBlock{}; // Z^-1 single sample delay block
	/*double *lpf_delayLine;*/
	double lpf_maxDelay_samples;
	int lpf_readPointer{}, lpf_writePointer{};
};

#endif
