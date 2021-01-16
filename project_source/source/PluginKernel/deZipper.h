#pragma once

#ifndef deZipper_h
#define deZipper_h

#include <stdio.h>

class deZipper
{
public:
	deZipper(); // Constructor
	~deZipper(); // Destructor

	/**
	 * \brief Parameter smoothing via a one-pole low pass filter
	 * \param input Value to be smoothed
	 * \return Smoothed output
	 */
	double smooth(double input);
	// define the output and input variables

private:
	float memoryBlock;
	//single sample delay memory
	float feedbackCoefficient;
	//Feed Back Coefficient
	float feedforwardCoefficient;
	//Feed Forward Coefficient
};

#endif
