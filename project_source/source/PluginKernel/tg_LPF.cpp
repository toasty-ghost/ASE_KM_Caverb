#include "tg_LPF.h"

#include "vstgui/lib/controls/ccontrol.h"

tg_LPF::tg_LPF()
{
	// Initialise with some sensible defaults
	lpf_readPointer = lpf_writePointer = 0;
	lpf_maxDelay_samples = 48000;
	lpf_cornerFrequency = 5000.0;
	lpf_sampleRate = 48000;
	lpf_dBGainAtFc = 0.6;
	lpf_hfRatio = 0.5;
	lpf_decayTime_Tr = 1000; // in milliseconds
	lpf_delayTime = 1; // in milliseconds
	lpfCoefficient_b = 0.6; // default coefficient
}

tg_LPF::~tg_LPF()
{
	/*delete [] lpf_delayLine;*/
}

bool tg_LPF::reset(double sampleRate)
{
	lpf_sampleRate = sampleRate;
	lpf_memoryBlock = 0.0; // Clear the single sample buffer
	/*lpf_delayLine = new double[lpf_maxDelay_samples];
	memset(lpf_delayLine,0,lpf_maxDelay_samples*sizeof(double)); // fill it with zeroes*/
	return true;
}

/**
 * \brief Calculate the correct coefficient value for a one pole low pass filter based on a given ratio factor, corner frequency, delay length and decay time. Used in I3DL2 compatible reverberators.
 * \param hfRatio Ratio of high frequency signals to low frequency signals
 * \param cornerFrequency Frequency at which low pass effect is implemented
 * \param delayLength_mSec Length of the delay line associated with the low pass filter
 * \param decayTime_mSec Desired time taken for signal to decay by -60dB
 * \return
 */
double tg_LPF::calculateCoefficient(double hfRatio, double cornerFrequency, double delayLength_mSec, double decayTime_mSec)
{
	// Store the values 'cos we're gonna do some MATHS on 'em
	lpf_cornerFrequency = cornerFrequency;
	lpf_decayTime_Tr = decayTime_mSec;
	lpf_hfRatio = hfRatio;
	lpf_delayTime = delayLength_mSec;

	// Here comes the science bit...
	//	printf("---> tg_LPF: Creating LPF coefficient\n");
	lpf_dBGainAtFc = -60 * lpf_delayTime / (lpf_hfRatio * lpf_decayTime_Tr);
	lpf_G = pow(10, lpf_dBGainAtFc / 20.0);
	if (lpf_G == 1.0)
	{
		lpfCoefficient_b = 0.0;
	}
	else
	{
		lpf_omega = cos(2 * VSTGUI::Constants::pi * lpf_cornerFrequency / lpf_sampleRate);
		lpf_A = 8.0 * lpf_G - 4.0 * lpf_G * lpf_G - 8.0 * lpf_G * lpf_omega + 4.0 * lpf_G * lpf_G * lpf_omega * lpf_omega;
		lpfCoefficient_b = (2 * lpf_G * lpf_omega - 2.0 + sqrt(lpf_A)) / (2.0 * lpf_G - 2.0);

		// Is it gonna break stuff? If so, fix it!
		if (lpfCoefficient_b > 1.0)
			lpfCoefficient_b = 1.0;
		if (lpfCoefficient_b < 0.0)
			lpfCoefficient_b = 0.0;
	}
	return lpfCoefficient_b;
}

/**
 * \brief Process samples via the one pole low pass filter. Coefficients should be set first, otherwise a default value of 0.6 will be used.
 * \param input Input sample
 * \return Processed sample
 */
double tg_LPF::processAudio(double input)
{
	//calculate read pointer position
	/*lpf_readPointer = lpf_writePointer - lpf_delayTime;
	if (lpf_readPointer < 0)
	{
		lpf_readPointer+=lpf_maxDelay_samples;
	}*/

	lpf_memoryBlock = 0.0;
	double output = (1 - lpfCoefficient_b) * input + lpf_memoryBlock;
	lpf_memoryBlock = output * (1 - lpfCoefficient_b);

	return output;
}