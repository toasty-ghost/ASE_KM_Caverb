#include "deZipper.h"

deZipper::deZipper()
{
	// Initialise the private variables
	memoryBlock = 0.0;
	// Ensure we start with a zero value to avoid sudden ramp up
	feedbackCoefficient = 0.999;
	// Higher values of feedback coefficient produce more rolloff
	feedforwardCoefficient = 1 - feedbackCoefficient;
	//FF Coef for 0dB passband gain
}

deZipper::~deZipper()
{
}

/**
 * \brief Parameter smoothing via a one-pole low pass filter
 * \param input Value to be smoothed
 * \return Smoothed output
 */
double deZipper::smooth(double input)
{
	//feedbackCoefficient = steepness; // let the user decide how much the input is filtered
	// take the FF gain, multiply by the sample, add the FB gain multiplied by the last sample in memory
	double temp = feedforwardCoefficient * input + feedbackCoefficient * memoryBlock;
	// store the new value in the memory
	memoryBlock = temp;
	// pass the value back to the user
	return temp;
}