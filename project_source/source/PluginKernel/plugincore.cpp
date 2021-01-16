// -----------------------------------------------------------------------------
//    ASPiK Plugin Kernel File:  plugincore.cpp
//
/**
	\file   plugincore.cpp
	\author Will Pirkle
	\date   17-September-2018
	\brief  Implementation file for PluginCore object
			- http://www.aspikplugins.com
			- http://www.willpirkle.com
*/
// -----------------------------------------------------------------------------
#include "plugincore.h"
#include "plugindescription.h"
#include <cstdio>

#include "vstgui/lib/controls/ccontrol.h"

/**
\brief PluginCore constructor is launching pad for object initialization

Operations:
- initialize the plugin description (strings, codes, numbers, see initPluginDescriptors())
- setup the plugin's audio I/O channel support
- create the PluginParameter objects that represent the plugin parameters (see FX book if needed)
- create the presets
*/
PluginCore::PluginCore()
{
	// --- describe the plugin; call the helper to init the static parts you setup in plugindescription.h
	initPluginDescriptors();

	// --- default I/O combinations
	// --- for FX plugins
	if (getPluginType() == kFXPlugin)
	{
		addSupportedIOCombination({ kCFMono, kCFMono });
		addSupportedIOCombination({ kCFMono, kCFStereo });
		addSupportedIOCombination({ kCFStereo, kCFStereo });
	}
	else // --- synth plugins have no input, only output
	{
		addSupportedIOCombination({ kCFNone, kCFMono });
		addSupportedIOCombination({ kCFNone, kCFStereo });
	}

	// --- for sidechaining, we support mono and stereo inputs; auxOutputs reserved for future use
	addSupportedAuxIOCombination({ kCFMono, kCFNone });
	addSupportedAuxIOCombination({ kCFStereo, kCFNone });

	// --- create the parameters
	initPluginParameters();

	// --- create the presets
	initPluginPresets();
}

/**
\brief create all of your plugin parameters here

\return true if parameters were created, false if they already existed
*/
bool PluginCore::initPluginParameters()
{
	if (pluginParameterMap.size() > 0)
		return false;

	// --- Add your plugin parameter instantiation code bewtween these hex codes
	// **--0xDEA7--**

	PluginParameter* piParam = nullptr;

	piParam = new PluginParameter(0, "Room_level", "mB", controlVariableType::kDouble, -10000.000000, 0.000000, -1000.000000, taper::kLinearTaper);
	piParam->setBoundVariable(&controlRoomLevel, boundVariableType::kDouble);
	piParam->setParameterSmoothing(true);
	piParam->setSmoothingMethod(smoothingMethod::kLPFSmoother);
	piParam->setSmoothingTimeMsec(20.0);
	addPluginParameter(piParam);

	piParam = new PluginParameter(1, "Room_HF_level", "mB", controlVariableType::kDouble, -10000.000000, 0.000000, -1000.000000, taper::kLinearTaper);
	piParam->setBoundVariable(&controlRoomHFLevel, boundVariableType::kDouble);
	piParam->setParameterSmoothing(true);
	piParam->setSmoothingMethod(smoothingMethod::kLPFSmoother);
	piParam->setSmoothingTimeMsec(20.0);
	addPluginParameter(piParam);

	piParam = new PluginParameter(2, "Room_rolloff_factor", "", controlVariableType::kFloat, 0.000000, 10.000000, 0.000000, taper::kLinearTaper);
	piParam->setBoundVariable(&controlRoomRolloffFactor, boundVariableType::kDouble);
	piParam->setParameterSmoothing(true);
	piParam->setSmoothingMethod(smoothingMethod::kLinearSmoother);
	piParam->setSmoothingTimeMsec(20.0);
	addPluginParameter(piParam);

	piParam = new PluginParameter(3, "Decay_time", "s", controlVariableType::kDouble, 0.100000, 20.000000, 10.500000, taper::kLinearTaper);
	piParam->setBoundVariable(&controlDecayTime, boundVariableType::kDouble);
	piParam->setParameterSmoothing(true);
	piParam->setSmoothingMethod(smoothingMethod::kLinearSmoother);
	piParam->setSmoothingTimeMsec(20.0);
	addPluginParameter(piParam);

	piParam = new PluginParameter(4, "Decay_HF_ratio", "", controlVariableType::kDouble, 0.100000, 2.000000, 0.2300000, taper::kLinearTaper);
	piParam->setBoundVariable(&controlDecayHFRatio, boundVariableType::kDouble);
	piParam->setParameterSmoothing(true);
	piParam->setSmoothingMethod(smoothingMethod::kLinearSmoother);
	piParam->setSmoothingTimeMsec(20.0);
	addPluginParameter(piParam);

	piParam = new PluginParameter(5, "Reflections_level", "mB", controlVariableType::kFloat, -10000.000000, 1000.000000, -602.000000, taper::kLinearTaper);
	piParam->setBoundVariable(&controlReflectionsLevel, boundVariableType::kDouble);
	piParam->setParameterSmoothing(true);
	piParam->setSmoothingMethod(smoothingMethod::kLPFSmoother);
	piParam->setSmoothingTimeMsec(20.0);
	addPluginParameter(piParam);

	piParam = new PluginParameter(6, "Reflections_delay", "s", controlVariableType::kDouble, 0.000000, 0.300000, 0.020000, taper::kLinearTaper);
	piParam->setBoundVariable(&controlReflectionsDelay, boundVariableType::kDouble);
	piParam->setParameterSmoothing(true);
	piParam->setSmoothingMethod(smoothingMethod::kLinearSmoother);
	piParam->setSmoothingTimeMsec(100.0);
	addPluginParameter(piParam);

	piParam = new PluginParameter(7, "Reverb_level", "mB", controlVariableType::kDouble, -10000.000000, 2000.000000, 198.000000, taper::kLinearTaper);
	piParam->setBoundVariable(&controlReverbLevel, boundVariableType::kDouble);
	piParam->setParameterSmoothing(true);
	piParam->setSmoothingMethod(smoothingMethod::kLPFSmoother);
	piParam->setSmoothingTimeMsec(20.0);
	addPluginParameter(piParam);

	piParam = new PluginParameter(8, "Reverb_delay", "s", controlVariableType::kDouble, 0.000000, 0.300000, 0.030000, taper::kLinearTaper);
	piParam->setBoundVariable(&controlReverbDelay, boundVariableType::kDouble);
	piParam->setParameterSmoothing(true);
	piParam->setSmoothingMethod(smoothingMethod::kLinearSmoother);
	piParam->setSmoothingTimeMsec(100.0);
	addPluginParameter(piParam);

	piParam = new PluginParameter(9, "Diffusion", "%", controlVariableType::kFloat, 0.000000, 100.000000, 100.000000, taper::kLinearTaper);
	piParam->setBoundVariable(&controlDiffusion, boundVariableType::kDouble);
	piParam->setParameterSmoothing(true);
	piParam->setSmoothingMethod(smoothingMethod::kLinearSmoother);
	piParam->setSmoothingTimeMsec(20.0);
	addPluginParameter(piParam);

	piParam = new PluginParameter(10, "Density", "%", controlVariableType::kFloat, 0.000000, 100.000000, 100.000000, taper::kLinearTaper);
	piParam->setBoundVariable(&controlDensity, boundVariableType::kDouble);
	piParam->setParameterSmoothing(true);
	piParam->setSmoothingMethod(smoothingMethod::kLinearSmoother);
	piParam->setSmoothingTimeMsec(100.0);
	addPluginParameter(piParam);

	piParam = new PluginParameter(11, "HF_reference", "Hz", controlVariableType::kDouble, 20.000000, 20000.000000, 5000.000000, taper::kLinearTaper);
	piParam->setBoundVariable(&controlHFReference, boundVariableType::kDouble);
	piParam->setParameterSmoothing(true);
	piParam->setSmoothingMethod(smoothingMethod::kLPFSmoother);
	piParam->setSmoothingTimeMsec(20.0);
	addPluginParameter(piParam);

	piParam = new PluginParameter(12, "Stereo_width", "%", controlVariableType::kFloat, 0.000000, 100.000000, 50.000000, taper::kLinearTaper);
	piParam->setBoundVariable(&controlStereoWidth, boundVariableType::kDouble);
	piParam->setParameterSmoothing(true);
	piParam->setSmoothingMethod(smoothingMethod::kLPFSmoother);
	piParam->setSmoothingTimeMsec(20.0);
	addPluginParameter(piParam);

	piParam = new PluginParameter(13, "Direct_sound", "OFF, ON", "ON");
	piParam->setBoundVariable(&directSoundStatus, boundVariableType::kInt);
	addPluginParameter(piParam);

	// **--0xEDA5--**

	// --- BONUS Parameter
	// --- SCALE_GUI_SIZE
	PluginParameter* piParamBonus = new PluginParameter(SCALE_GUI_SIZE, "Scale GUI", "tiny,small,medium,normal,large,giant", "normal");
	addPluginParameter(piParamBonus);

	// --- create the super fast access array
	initPluginParameterArray();

	return true;
}

/**
\brief initialize object for a new run of audio; called just before audio streams

Operation:
- store sample rate and bit depth on audioProcDescriptor - this information is globally available to all core functions
- reset your member objects here

\param resetInfo structure of information about current audio format

\return true if operation succeeds, false otherwise
*/
bool PluginCore::reset(ResetInfo& resetInfo)
{
	// --- save for audio processing
	audioProcDescriptor.sampleRate = resetInfo.sampleRate;
	audioProcDescriptor.bitDepth = resetInfo.bitDepth;

	// --- other reset inits
	for (int t = 0; t < 6; t++)
	{
		aapf_L[t].reset(fs);
		aapf_R[t].reset(fs);
	}

	workingLPF.reset(fs);

	inL_earlyDelay.reset(fs);
	inR_earlyDelay.reset(fs);
	chainL_delay.reset(fs);
	chainR_delay.reset(fs);

	leftInputLPF_tg.reset(fs);
	rightInputLPF_tg.reset(fs);
	chainL_LPF_tg.reset(fs);
	chainR_LPF_tg.reset(fs);

	return PluginBase::reset(resetInfo);
}

/**
\brief one-time initialize function called after object creation and before the first reset( ) call

Operation:
- saves structure for the plugin to use; you can also load WAV files or state information here
*/
bool PluginCore::initialize(PluginInfo& pluginInfo)
{
	// --- add one-time init stuff here
	// Starter gain values for the normalising elements of the late reverb chains
	NormL = 0.7;
	NormR = 0.7;
	decayTime_mSec = i3_DecayTime * 1000;

	// Now we set the starting LPF values
	Fc = 5000; // start value for HF cutoff used in LPFs

	// Create the tapped delay line for the early echo section - use a large value to ensure we accommodate the combined maximum values for the Reflections Delay and Reverb Delay sliders

	inL_earlyDelay.createDelayBuffer(fs, 1000);
	inR_earlyDelay.createDelayBuffer(fs, 1000);

	// Setup the early all-pass filters that will feed the reverberator output - these need to have no working LPF or absorbent gain
	APF_earlyL.delayLength_samples = 83 * (fs / 1000);
	APF_earlyL.lpfCoefficient = 0.7071;
	APF_earlyL.absorbentGain = 0.707;
	APF_earlyR.feedbackGain = 1;
	APF_earlyR.delayLength_samples = 97 * (fs / 1000);
	APF_earlyR.lpfCoefficient = 0.707;
	APF_earlyR.absorbentGain = 0.707;
	APF_earlyR.feedbackGain = 1;
	// Set some simple gains for the taps
	for (int t = 0; t < 6; t++)
	{
		tapGainL[t] = 1;
		tapGainR[t] = 1;
	}
	// Configure the maximum length of the delay line, which will be tapped later via percentages. This value feeds the late mixing matrix.
	SimpleDelayParameters earlyDelayParameters = inL_earlyDelay.getParameters();
	earlyDelayParameters.delayTime_mSec = 700; // Combined values for reflections delay and reverb delay don't get bigger than this
	earlyDelayParameters.interpolate = true;
	inL_earlyDelay.setParameters(earlyDelayParameters);
	inR_earlyDelay.setParameters(earlyDelayParameters);
	tg_updateVars_earlyEcho_delayTimes();
	tg_updateVars_earlyEcho_hfRef_decayTime();

	chainL_delayLength_mSec = 269;
	chainR_delayLength_mSec = 293;
	// Setup the in-line delay elements and LPF elements for each chain
	chainL_delay.createDelayBuffer(fs, 1000);
	chainR_delay.createDelayBuffer(fs, 1000);

	//tg_updateVars_lateReverb();
	tg_updateVars_lateReverb_chainDelays();
	tg_updateVars_lateReverb_controlDensity();
	tg_updateVars_lateReverb_controlDiffusion();
	tg_updateVars_lateReverb_controlHFRatio_controlHFref();
	tg_updateVars_lateReverb_controlDecayTime();
	tg_updateVars_EnergyGain();

	return true;
}

void PluginCore::tg_updateVars_earlyEcho_hfRef_decayTime()
{
	// Calculate the coefficients for the input LPFs and shove them into the filters
	// We use the Room HF Level control for these LPFs instead of Decay HF Ratio
	// There is only a one-sample delay used in these blocks
	// however a value of 1000 mSec approximates a -10dB attenuation at 1 Norm Freq
	// With a user control value of -1000 mB it produces a LPF coefficient value of 0.79
	leftInputLPF_b = leftInputLPF_tg.calculateCoefficient(roomHFLevel_lin, i3_HfReference, 1000, decayTime_mSec);
	if (leftInputLPF_b > 1.0)
	{
		leftInputLPF_b = 1.0;
	}
	leftInputLPF_tg.lpfCoefficient_b = leftInputLPF_b;

	rightInputLPF_b = rightInputLPF_tg.calculateCoefficient(roomHFLevel_lin, i3_HfReference, 1000, decayTime_mSec);
	if (rightInputLPF_b > 1.0)
	{
		rightInputLPF_b = 1.0;
	}
	rightInputLPF_tg.lpfCoefficient_b = rightInputLPF_b;
	return;
}

void PluginCore::tg_updateVars_earlyEcho_delayTimes()
{
	// Calculate the values for use in the early delay line - these need to be adjusted when the parameters are changed by the user
	reflectionsDelay_mSec = i3_ReflectionsDelay * 1000; // converted to milliseconds
	reverbDelay_mSec = i3_ReverbDelay * 1000; // converted to milliseconds
	totalEarlyDelay_mSec = reflectionsDelay_mSec + reverbDelay_mSec;
	earlyDelay_remainingTime = totalEarlyDelay_mSec - reflectionsDelay_mSec;
	return;
}

void PluginCore::tg_updateVars_lateReverb_controlDensity()
{
	// Absorbent all-pass chain values - feedback gains will be overwritten by user parameters
	// These delay values were calculated to be mutually prime, and sum to no more than 2 seconds
	for (int t = 0; t < 6; t++)
	{
		aapf_L_delayLength_mSec[t] = (i3_Density / 100) * aapf_L_delayPrimes_mSec[t];
		aapf_L[t].delayLength_samples = aapf_L_delayLength_mSec[t] * (fs / 1000);
		aapf_R_delayLength_mSec[t] = (i3_Density / 100) * aapf_R_delayPrimes_mSec[t];
		aapf_R[t].delayLength_samples = aapf_R_delayLength_mSec[t] * (fs / 1000);
	}
	return;
}

void PluginCore::tg_updateVars_lateReverb_chainDelays()
{
	SimpleDelayParameters leftChainDelayParameters = chainL_delay.getParameters();
	leftChainDelayParameters.delayTime_mSec = chainL_delayLength_mSec * (i3_Density / 100); // Scale by the Density control
	leftChainDelayParameters.interpolate = true;
	SimpleDelayParameters rightChainDelayParameters = chainR_delay.getParameters();
	rightChainDelayParameters.delayTime_mSec = chainR_delayLength_mSec * (i3_Density / 100); // Scale by the Density control
	rightChainDelayParameters.interpolate = true;
	chainL_delay.setParameters(leftChainDelayParameters);
	chainR_delay.setParameters(rightChainDelayParameters);
}

void PluginCore::tg_updateVars_lateReverb_controlDiffusion()
{
	allPassG = maxAPg * (i3_Diffusion / 100);
	gDL = allPassG;
	gDR = allPassG;

	// Absorbent all-pass chain values - feedback gains will be overwritten by user parameters

	// Set the 'g' feedback coefficient
	for (int t = 0; t < 6; t++)
	{
		aapf_L[t].feedbackGain = allPassG;
		aapf_R[t].feedbackGain = allPassG;
	}
	return;
}
void PluginCore::tg_updateVars_lateReverb_controlHFRatio_controlHFref()
{
	// Now we calculate all of the low pass filter 'b' coefficients and pass them into the instances of the AAPF class
	for (int t = 0; t < 6; t++)
	{
		lpf_bL[t] = workingLPF.calculateCoefficient(i3_DecayHfRatio, i3_HfReference, aapf_L_delayLength_mSec[t], i3_DecayTime * 1000);
		aapf_L[t].lpfCoefficient = lpf_bL[t];
		lfp_bR[t] = workingLPF.calculateCoefficient(i3_DecayHfRatio, i3_HfReference, aapf_R_delayLength_mSec[t],
			i3_DecayTime * 1000);
		aapf_R[t].lpfCoefficient = lfp_bR[t];
	}
	// Let's not forget the simple LPF blocks in the chain
	lpf_bLSolo = workingLPF.calculateCoefficient(i3_DecayHfRatio, i3_HfReference, chainL_delayLength_mSec, decayTime_mSec);
	lpf_bRSolo = workingLPF.calculateCoefficient(i3_DecayHfRatio, i3_HfReference, chainR_delayLength_mSec, decayTime_mSec);

	chainL_LPF_tg.lpfCoefficient_b = lpf_bLSolo;
	chainR_LPF_tg.lpfCoefficient_b = lpf_bRSolo;
	return;
}
void PluginCore::tg_updateVars_lateReverb_controlDecayTime()
{
	// Calculate the absorbent gain 'a' for each AAPF - these need to be updated when the user updates the Decay Time!
	for (int t = 0; t < 6; t++)
	{
		double length = aapf_L_delayLength_mSec[t];
		double adB = -60 * (length / decayTime_mSec);
		aapf_La[t] = pow(10, adB / 20.0);
		//aapf_La[t] = dB_lin.convert(adB);
		aapf_L[t].absorbentGain = aapf_La[t];
	}
	for (int t = 0; t < 6; t++)
	{
		double length = aapf_R_delayLength_mSec[t];
		double adB = -60 * (length / decayTime_mSec);
		aapf_Ra[t] = pow(10, adB / 20.0);
		//aapf_Ra[t] = dB_lin.convert(adB);
		aapf_R[t].absorbentGain = aapf_Ra[t];
	}

	// And for the in-line absorbent gain, based on the value for the preceding simple delay block
	double adB_gDL = -60 * (chainL_delayLength_mSec / decayTime_mSec);
	gDL = pow(10, adB_gDL / 20.0);
	//gDL = dB_lin.convert(adB_gDL);

	double adB_gDR = -60 * (chainR_delayLength_mSec / decayTime_mSec);
	gDR = pow(10, adB_gDR / 20.0);
	//gDR = dB_lin.convert(adB_gDL);

	// Let's not forget the simple LPF blocks in the chain
	lpf_bLSolo = workingLPF.calculateCoefficient(i3_DecayHfRatio, i3_HfReference, chainL_delayLength_mSec, decayTime_mSec);
	lpf_bRSolo = workingLPF.calculateCoefficient(i3_DecayHfRatio, i3_HfReference, chainR_delayLength_mSec, decayTime_mSec);

	chainL_LPF_tg.lpfCoefficient_b = lpf_bLSolo;
	chainR_LPF_tg.lpfCoefficient_b = lpf_bRSolo;
	return;
}

void PluginCore::tg_updateVars_EnergyGain()
{
	// Determine the energy gain of each absorbent all pass filter
	// Left Side
	for (int f = 0; f < 6; f++)
	{
		fBgainSquaredL[f] = pow(aapf_L[f].feedbackGain, 2);
		absorbentGainSquaredL[f] = pow(aapf_L[f].absorbentGain, 2);
		cL[f] = (fBgainSquaredL[f] + (1 - fBgainSquaredL[f])) * (absorbentGainSquaredL[f] / (1 - absorbentGainSquaredL[f] * fBgainSquaredL[f]));
	}
	// Right side
	for (int t = 0; t < 6; t++)
	{
		fBgainSquaredR[t] = pow(aapf_R[t].feedbackGain, 2);
		absorbentGainSquaredR[t] = pow(aapf_R[t].absorbentGain, 2);
		cR[t] = (fBgainSquaredR[t] + (1 - fBgainSquaredR[t])) * (absorbentGainSquaredR[t] / (1 - absorbentGainSquaredR[t] * fBgainSquaredR[t]));
	}

	leftLoopEnergyGainA = pow(gDL, 2);
	leftLoopEnergyGainA = leftLoopEnergyGainA * cL[0] * cL[1] * cL[2] * cL[3] * cL[4] * cL[5]; // there's probably a more efficient way to do this...
	rightLoopEnergyGainA = pow(gDR, 2);
	rightLoopEnergyGainA = rightLoopEnergyGainA * cR[0] * cR[1] * cR[2] * cR[3] * cR[4] * cR[5]; // there's probably a more efficient way to do this...
	totalEnergyGainA = leftLoopEnergyGainA + rightLoopEnergyGainA;

	leftOutputGainB = pow(tapGainL[0], 2) * cL[0] + pow(tapGainL[1], 2) * cL[0] * cL[1] + pow(tapGainL[2], 2) * cL[0] * cL[1] * cL[2] + pow(tapGainL[3], 2) * cL[0] * cL[1] * cL[2] * cL[3] + pow(tapGainL[4], 2) * cL[0] * cL[1] * cL[2] * cL[3] * cL[4] + pow(tapGainL[5], 2) * pow(gDL, 2) * cL[0] * cL[1] * cL[2] * cL[3] * cL[4] * cL[5]; // likewise here, there's gotta be a better way...
	rightOutputGainB = pow(tapGainR[0], 2) * cR[0] + pow(tapGainR[1], 2) * cR[0] * cR[1] + pow(tapGainR[2], 2) * cR[0] * cR[1] * cR[2] + pow(tapGainR[3], 2) * cR[0] * cR[1] * cR[2] * cR[3] + pow(tapGainR[4], 2) * cR[0] * cR[1] * cR[2] * cR[3] * cR[4] + pow(tapGainR[5], 2) * pow(gDR, 2) * cR[0] * cR[1] * cR[2] * cR[3] * cR[4] * cR[5]; // likewise here, there's gotta be a better way...

	// Now we recalculate the Energy Normalisation values
	NormL = 1 - totalEnergyGainA / leftOutputGainB;
	NormR = 1 - totalEnergyGainA / rightOutputGainB;

	// Calculate the energy normalisation values
	outGain_val = (roomLevel_dB + reverbLevel_dB);
	outGain_val = pow(10, outGain_val / 20.0);
	if (outGain_val > 1.0)
	{
		outGain_val = 1.0;
	}
	if (NUM_CHANNELS == 1)
	{
		outGain_diff_adj = pow(10, 3.5 * i3_Diffusion / (100 * 20)); // adjust the normalisation by +3.5dB
		// for max diffusion scale linearly in dB
	}
	else
	{
		outGain_diff_adj = 1.0;
		leftReverbOutputLevel = outGain_val * NormL * outGain_diff_adj;
		rightReverbOutputLevel = outGain_val * NormR * outGain_diff_adj;
	}
	return;
}

/**
\brief do anything needed prior to arrival of audio buffers

Operation:
- syncInBoundVariables when preProcessAudioBuffers is called, it is *guaranteed* that all GUI control change information
  has been applied to plugin parameters; this binds parameter changes to your underlying variables
- NOTE: postUpdatePluginParameter( ) will be called for all bound variables that are acutally updated; if you need to process
  them individually, do so in that function
- use this function to bulk-transfer the bound variable data into your plugin's member object variables

\param processInfo structure of information about *buffer* processing

\return true if operation succeeds, false otherwise
*/
bool PluginCore::preProcessAudioBuffers(ProcessBufferInfo& processInfo)
{
	// --- sync internal variables to GUI parameters; you can also do this manually if you don't
	//     want to use the auto-variable-binding
	syncInBoundVariables();

	return true;
}

/**
\brief frame-processing method

Operation:
- decode the plugin type - for synth plugins, fill in the rendering code; for FX plugins, delete the if(synth) portion and add your processing code
- note that MIDI events are fired for each sample interval so that MIDI is tightly sunk with audio
- doSampleAccurateParameterUpdates will perform per-sample interval smoothing

\param processFrameInfo structure of information about *frame* processing

\return true if operation succeeds, false otherwise
*/
bool PluginCore::processAudioFrame(ProcessFrameInfo& processFrameInfo)
{
	// --- fire any MIDI events for this sample interval
	processFrameInfo.midiEventQueue->fireMidiEvents(processFrameInfo.currentFrame);

	// --- do per-frame updates; VST automation and parameter smoothing
	doSampleAccurateParameterUpdates();

	// --- decode the channelIOConfiguration and process accordingly
	//
	double inL = processFrameInfo.audioInputFrame[0];
	double inR = processFrameInfo.audioInputFrame[1];
	// Some variables used for working out - these aren't strictly necessary, but make the code more readable
	double workL = inL;
	double workR = inR;
	double tempL = 0.0;
	double tempR = 0.0;
	double reverbOutL = 0.0;
	double reverbOutR = 0.0;
	double outL = 0.0;
	double outR = 0.0;
	double loopbackL = 0.0; // initialise the accumulator for the feedback loop so it doesn't crash
	double loopbackR = 0.0; // initialise the accumulator for the feedback loop so it doesn't crash

	// --- FX Plugin:
	if (processFrameInfo.channelIOConfig.inputChannelFormat == kCFMono &&
		processFrameInfo.channelIOConfig.outputChannelFormat == kCFMono)
	{
		// --- pass through code: change this with your signal processing
		workL = leftInputLPF_tg.processAudio(inL);
		tempL = inL_earlyDelay.processAudioSample(workL);

		workR = rightInputLPF_tg.processAudio(inL); // Take the left (mono) input for the right channel processing path - we'll sum this to mono later
		tempR = inR_earlyDelay.processAudioSample(workR);

		// Now we take the taps from the delay line, noting that the first value is the user controllable parameter. Some gain adjustments are applied to make each successive tap a little quieter. This is currently a linear relationship, and may need some experimentation to ensure it sounds good.
		// Define the tap delay periods, in percentage, based on the values set
		leftDelayTapLength1 = reflectionsDelay_mSec;
		leftDelayTapLength2 = 0.14 * earlyDelay_remainingTime;
		leftDelayTapLength3 = 0.20 * earlyDelay_remainingTime;
		leftDelayTapLength4 = 0.29 * earlyDelay_remainingTime;
		leftDelayTapLength5 = 0.37 * earlyDelay_remainingTime;

		rightDelayTapLength1 = reflectionsDelay_mSec;
		rightDelayTapLength2 = 0.11 * earlyDelay_remainingTime;
		rightDelayTapLength3 = 0.23 * earlyDelay_remainingTime;
		rightDelayTapLength4 = 0.27 * earlyDelay_remainingTime;
		rightDelayTapLength5 = 0.39 * earlyDelay_remainingTime;

		leftEarlyDelayTap1 = inL_earlyDelay.readDelayAtTime_mSec(leftDelayTapLength1) * 1.00;
		leftEarlyDelayTap2 = inL_earlyDelay.readDelayAtTime_mSec(leftDelayTapLength2) * 0.97;
		leftEarlyDelayTap3 = inL_earlyDelay.readDelayAtTime_mSec(leftDelayTapLength3) * 0.94;
		leftEarlyDelayTap4 = inL_earlyDelay.readDelayAtTime_mSec(leftDelayTapLength4) * 0.91;
		leftEarlyDelayTap5 = inL_earlyDelay.readDelayAtTime_mSec(leftDelayTapLength5) * 0.88;
		leftDelayOut = inL_earlyDelay.readDelayAtTime_mSec(totalEarlyDelay_mSec);

		rightEarlyDelayTap1 = inR_earlyDelay.readDelayAtTime_mSec(rightDelayTapLength1) * 1.00;
		rightEarlyDelayTap2 = inR_earlyDelay.readDelayAtTime_mSec(rightDelayTapLength2) * 0.97;
		rightEarlyDelayTap3 = inR_earlyDelay.readDelayAtTime_mSec(rightDelayTapLength3) * 0.94;
		rightEarlyDelayTap4 = inR_earlyDelay.readDelayAtTime_mSec(rightDelayTapLength4) * 0.91;
		rightEarlyDelayTap5 = inR_earlyDelay.readDelayAtTime_mSec(rightDelayTapLength5) * 0.88;
		rightDelayOut = inR_earlyDelay.readDelayAtTime_mSec(totalEarlyDelay_mSec);

		// Sum those delay taps together, and shove it into a 'normal' all pass filter - we'll use this later
		leftEarlyAPFinput = leftEarlyDelayTap1 + leftEarlyDelayTap2 + leftEarlyDelayTap3 + leftEarlyDelayTap4 + leftEarlyDelayTap5;
		
		leftEarlyAPFoutput = APF_earlyL.processAudio(inL);

		rightEarlyAPFinput = rightEarlyDelayTap1 + rightEarlyDelayTap2 + rightEarlyDelayTap3 + rightEarlyDelayTap4 + rightEarlyDelayTap5;
	
		rightEarlyAPFoutput = APF_earlyL.processAudio(inL);

		// Time for the late reverberator

		leftMatrixInput = leftDelayOut + loopbackL;
		rightMatrixInput = rightDelayOut + loopbackR;

		leftMatrixOutput = (1 / sqrt(2)) * leftMatrixInput + (1 / sqrt(2) * rightMatrixInput);
		rightMatrixOutput = (1 / sqrt(2)) * rightMatrixInput + (1 / sqrt(2) * rightMatrixInput);

		chainL = leftMatrixOutput;
		chainL = aapf_L[0].processAudio(chainL);
		leftChainTap1 = chainL * tapGainL[0];
		chainL = aapf_L[1].processAudio(chainL);
		leftChainTap2 = chainL * tapGainL[1] + leftChainTap1;
		chainL = aapf_L[2].processAudio(chainL);
		leftChainTap3 = chainL * tapGainL[2] + leftChainTap2;
		chainL = aapf_L[3].processAudio(chainL);
		leftChainTap4 = chainL * tapGainL[3] + leftChainTap3;

		chainR = rightMatrixOutput;
		chainR = aapf_R[0].processAudio(chainR);
		rightChainTap1 = chainR * tapGainR[0];
		chainR = aapf_R[1].processAudio(chainR);
		rightChainTap2 = chainR * tapGainR[1] + rightChainTap1;
		chainR = aapf_R[2].processAudio(chainR);
		rightChainTap3 = chainR * tapGainR[2] + rightChainTap2;
		chainR = aapf_R[3].processAudio(chainR);
		rightChainTap4 = chainR * tapGainR[3] + rightChainTap3;

		chainL = chainL_delay.processAudioSample(chainL); // basic delay block after the 4th absorbent all-pass filter
		chainL = chainL_LPF_tg.processAudio(chainL) * gDL; // basic low pass filter in the chain, multiplied by the gDL gain factor
		chainR = chainR_delay.processAudioSample(chainR); // basic delay block after the 4th absorbent all-pass filter
		chainR = chainR_LPF_tg.processAudio(chainR) * gDL; // basic low pass filter in the chain, multiplied by the gDR gain factor

		chainL = aapf_L[4].processAudio(chainL);
		leftChainTap5 = chainL * tapGainL[4] + leftChainTap4;
		leftChainTap6 = aapf_L[5].processAudio(chainL) * tapGainL[5] + leftChainTap5;

		chainR = aapf_R[4].processAudio(chainR);
		rightChainTap5 = chainR * tapGainR[4] + rightChainTap4;
		rightChainTap6 = aapf_R[5].processAudio(chainR) * tapGainR[5] + rightChainTap5;

		// Feed back the output of the chain of absorbent all-pass back to the matrix
		leftChainOutput = leftChainTap6;
		rightChainOutput = rightChainTap6;

		loopbackL += leftChainTap6;
		loopbackR += rightChainTap6;

		// Now combine the early echo section with the chain of the absorbent all-passes, and pass it out
		reverbOutL = leftEarlyAPFoutput * reflectionsLevel_lin + (leftChainOutput * leftReverbOutputLevel);
		reverbOutR = rightEarlyAPFoutput * reflectionsLevel_lin + (rightChainOutput * rightReverbOutputLevel);
		outL = reverbOutL * roomLevel_lin;
		outR = reverbOutR * roomLevel_lin;

		// Simple widening algorithm
		float widthTemp = 1 / fmax(1 + stereoWidth, 2);
		float widthCoefficientMid = 1 * widthTemp;
		float widthCoefficientSides = stereoWidth * widthTemp;

		float widthMid = (outL + outR) * widthCoefficientMid;
		float widthSides = (outR - outL) * widthCoefficientSides;

		double wideOutL = widthMid - widthSides;
		double wideOutR = widthMid + widthSides;

		if (directSoundStatus == 1)
		{
			processFrameInfo.audioOutputFrame[0] = 0.5*((0.35 * inL) + wideOutL) + 0.5*((0.35 * inR) + wideOutR); // -6dB scaled for summing
		}
		else
		{
			processFrameInfo.audioOutputFrame[0] =0.5*wideOutL + 0.5*wideOutR; // -6dB scaled for summing
		}

		return true; /// processed
	}

	// --- Mono-In/Stereo-Out
	else if (processFrameInfo.channelIOConfig.inputChannelFormat == kCFMono &&
		processFrameInfo.channelIOConfig.outputChannelFormat == kCFStereo)
	{
		// --- pass through code: change this with your signal processing
		workL = leftInputLPF_tg.processAudio(inL);
		tempL = inL_earlyDelay.processAudioSample(workL);

		workR = rightInputLPF_tg.processAudio(inL); // Take the left input for the right processing path
		tempR = inR_earlyDelay.processAudioSample(workR);

		// Now we take the taps from the delay line, noting that the first value is the user controllable parameter. Some gain adjustments are applied to make each successive tap a little quieter. This is currently a linear relationship, and may need some experimentation to ensure it sounds good.
		// Define the tap delay periods, in percentage, based on the values set
		leftDelayTapLength1 = reflectionsDelay_mSec;
		leftDelayTapLength2 = 0.14 * earlyDelay_remainingTime;
		leftDelayTapLength3 = 0.20 * earlyDelay_remainingTime;
		leftDelayTapLength4 = 0.29 * earlyDelay_remainingTime;
		leftDelayTapLength5 = 0.37 * earlyDelay_remainingTime;

		rightDelayTapLength1 = reflectionsDelay_mSec;
		rightDelayTapLength2 = 0.11 * earlyDelay_remainingTime;
		rightDelayTapLength3 = 0.23 * earlyDelay_remainingTime;
		rightDelayTapLength4 = 0.27 * earlyDelay_remainingTime;
		rightDelayTapLength5 = 0.39 * earlyDelay_remainingTime;

		leftEarlyDelayTap1 = inL_earlyDelay.readDelayAtTime_mSec(leftDelayTapLength1) * 1.00;
		leftEarlyDelayTap2 = inL_earlyDelay.readDelayAtTime_mSec(leftDelayTapLength2) * 0.97;
		leftEarlyDelayTap3 = inL_earlyDelay.readDelayAtTime_mSec(leftDelayTapLength3) * 0.94;
		leftEarlyDelayTap4 = inL_earlyDelay.readDelayAtTime_mSec(leftDelayTapLength4) * 0.91;
		leftEarlyDelayTap5 = inL_earlyDelay.readDelayAtTime_mSec(leftDelayTapLength5) * 0.88;
		leftDelayOut = inL_earlyDelay.readDelayAtTime_mSec(totalEarlyDelay_mSec);

		rightEarlyDelayTap1 = inR_earlyDelay.readDelayAtTime_mSec(rightDelayTapLength1) * 1.00;
		rightEarlyDelayTap2 = inR_earlyDelay.readDelayAtTime_mSec(rightDelayTapLength2) * 0.97;
		rightEarlyDelayTap3 = inR_earlyDelay.readDelayAtTime_mSec(rightDelayTapLength3) * 0.94;
		rightEarlyDelayTap4 = inR_earlyDelay.readDelayAtTime_mSec(rightDelayTapLength4) * 0.91;
		rightEarlyDelayTap5 = inR_earlyDelay.readDelayAtTime_mSec(rightDelayTapLength5) * 0.88;
		rightDelayOut = inR_earlyDelay.readDelayAtTime_mSec(totalEarlyDelay_mSec);

		// Sum those delay taps together, and shove it into a 'normal' all pass filter - we'll use this later
		leftEarlyAPFinput = leftEarlyDelayTap1 + leftEarlyDelayTap2 + leftEarlyDelayTap3 + leftEarlyDelayTap4 + leftEarlyDelayTap5;
		
		leftEarlyAPFoutput = APF_earlyL.processAudio(inL);

		rightEarlyAPFinput = rightEarlyDelayTap1 + rightEarlyDelayTap2 + rightEarlyDelayTap3 + rightEarlyDelayTap4 + rightEarlyDelayTap5;
	
		rightEarlyAPFoutput = APF_earlyL.processAudio(inL);

		// Time for the late reverberator

		leftMatrixInput = leftDelayOut + loopbackL;
		rightMatrixInput = rightDelayOut + loopbackR;

		leftMatrixOutput = (1 / sqrt(2)) * leftMatrixInput + (1 / sqrt(2) * rightMatrixInput);
		rightMatrixOutput = (1 / sqrt(2)) * rightMatrixInput + (1 / sqrt(2) * rightMatrixInput);

		chainL = leftMatrixOutput;
		chainL = aapf_L[0].processAudio(chainL);
		leftChainTap1 = chainL * tapGainL[0];
		chainL = aapf_L[1].processAudio(chainL);
		leftChainTap2 = chainL * tapGainL[1] + leftChainTap1;
		chainL = aapf_L[2].processAudio(chainL);
		leftChainTap3 = chainL * tapGainL[2] + leftChainTap2;
		chainL = aapf_L[3].processAudio(chainL);
		leftChainTap4 = chainL * tapGainL[3] + leftChainTap3;

		chainR = rightMatrixOutput;
		chainR = aapf_R[0].processAudio(chainR);
		rightChainTap1 = chainR * tapGainR[0];
		chainR = aapf_R[1].processAudio(chainR);
		rightChainTap2 = chainR * tapGainR[1] + rightChainTap1;
		chainR = aapf_R[2].processAudio(chainR);
		rightChainTap3 = chainR * tapGainR[2] + rightChainTap2;
		chainR = aapf_R[3].processAudio(chainR);
		rightChainTap4 = chainR * tapGainR[3] + rightChainTap3;

		chainL = chainL_delay.processAudioSample(chainL); // basic delay block after the 4th absorbent all-pass filter
		chainL = chainL_LPF_tg.processAudio(chainL) * gDL; // basic low pass filter in the chain, multiplied by the gDL gain factor
		chainR = chainR_delay.processAudioSample(chainR); // basic delay block after the 4th absorbent all-pass filter
		chainR = chainR_LPF_tg.processAudio(chainR) * gDL; // basic low pass filter in the chain, multiplied by the gDR gain factor

		chainL = aapf_L[4].processAudio(chainL);
		leftChainTap5 = chainL * tapGainL[4] + leftChainTap4;
		leftChainTap6 = aapf_L[5].processAudio(chainL) * tapGainL[5] + leftChainTap5;

		chainR = aapf_R[4].processAudio(chainR);
		rightChainTap5 = chainR * tapGainR[4] + rightChainTap4;
		rightChainTap6 = aapf_R[5].processAudio(chainR) * tapGainR[5] + rightChainTap5;

		// Feed back the output of the chain of absorbent all-pass back to the matrix
		leftChainOutput = leftChainTap6;
		rightChainOutput = rightChainTap6;

		loopbackL += leftChainTap6;
		loopbackR += rightChainTap6;

		// Now combine the early echo section with the chain of the absorbent all-passes, and pass it out
		reverbOutL = leftEarlyAPFoutput * reflectionsLevel_lin + (leftChainOutput * leftReverbOutputLevel);
		reverbOutR = rightEarlyAPFoutput * reflectionsLevel_lin + (rightChainOutput * rightReverbOutputLevel);
		outL = reverbOutL * roomLevel_lin;
		outR = reverbOutR * roomLevel_lin;

		// Simple widening algorithm
		float widthTemp = 1 / fmax(1 + stereoWidth, 2);
		float widthCoefficientMid = 1 * widthTemp;
		float widthCoefficientSides = stereoWidth * widthTemp;

		float widthMid = (outL + outR) * widthCoefficientMid;
		float widthSides = (outR - outL) * widthCoefficientSides;

		double wideOutL = widthMid - widthSides;
		double wideOutR = widthMid + widthSides;

		if (directSoundStatus == 1)
		{
			processFrameInfo.audioOutputFrame[0] = (0.35 * inL) + wideOutL;
			processFrameInfo.audioOutputFrame[1] = (0.35 * inR) + wideOutR;
		}
		else
		{
			processFrameInfo.audioOutputFrame[0] = wideOutL;
			processFrameInfo.audioOutputFrame[1] = wideOutR;
		}
		processFrameInfo.audioOutputFrame[0] = processFrameInfo.audioInputFrame[0];
		processFrameInfo.audioOutputFrame[1] = processFrameInfo.audioInputFrame[0];

		return true; /// processed
	}

	// --- Stereo-In/Stereo-Out
	else if (processFrameInfo.channelIOConfig.inputChannelFormat == kCFStereo &&
		processFrameInfo.channelIOConfig.outputChannelFormat == kCFStereo)
	{
		// --- pass through code: change this with your signal processing
		workL = leftInputLPF_tg.processAudio(inL);
		tempL = inL_earlyDelay.processAudioSample(workL);

		workR = rightInputLPF_tg.processAudio(inR);
		tempR = inR_earlyDelay.processAudioSample(workR);

		// Now we take the taps from the delay line, noting that the first value is the user controllable parameter. Some gain adjustments are applied to make each successive tap a little quieter. This is currently a linear relationship, and may need some experimentation to ensure it sounds good.
		// Define the tap delay periods, in percentage, based on the values set
		leftDelayTapLength1 = reflectionsDelay_mSec;
		leftDelayTapLength2 = 0.14 * earlyDelay_remainingTime;
		leftDelayTapLength3 = 0.20 * earlyDelay_remainingTime;
		leftDelayTapLength4 = 0.29 * earlyDelay_remainingTime;
		leftDelayTapLength5 = 0.37 * earlyDelay_remainingTime;

		rightDelayTapLength1 = reflectionsDelay_mSec;
		rightDelayTapLength2 = 0.11 * earlyDelay_remainingTime;
		rightDelayTapLength3 = 0.23 * earlyDelay_remainingTime;
		rightDelayTapLength4 = 0.27 * earlyDelay_remainingTime;
		rightDelayTapLength5 = 0.39 * earlyDelay_remainingTime;

		leftEarlyDelayTap1 = inL_earlyDelay.readDelayAtTime_mSec(leftDelayTapLength1) * 1.00;
		leftEarlyDelayTap2 = inL_earlyDelay.readDelayAtTime_mSec(leftDelayTapLength2) * 0.97;
		leftEarlyDelayTap3 = inL_earlyDelay.readDelayAtTime_mSec(leftDelayTapLength3) * 0.94;
		leftEarlyDelayTap4 = inL_earlyDelay.readDelayAtTime_mSec(leftDelayTapLength4) * 0.91;
		leftEarlyDelayTap5 = inL_earlyDelay.readDelayAtTime_mSec(leftDelayTapLength5) * 0.88;
		leftDelayOut = inL_earlyDelay.readDelayAtTime_mSec(totalEarlyDelay_mSec);

		rightEarlyDelayTap1 = inR_earlyDelay.readDelayAtTime_mSec(rightDelayTapLength1) * 1.00;
		rightEarlyDelayTap2 = inR_earlyDelay.readDelayAtTime_mSec(rightDelayTapLength2) * 0.97;
		rightEarlyDelayTap3 = inR_earlyDelay.readDelayAtTime_mSec(rightDelayTapLength3) * 0.94;
		rightEarlyDelayTap4 = inR_earlyDelay.readDelayAtTime_mSec(rightDelayTapLength4) * 0.91;
		rightEarlyDelayTap5 = inR_earlyDelay.readDelayAtTime_mSec(rightDelayTapLength5) * 0.88;
		rightDelayOut = inR_earlyDelay.readDelayAtTime_mSec(totalEarlyDelay_mSec);

		// Sum those delay taps together, and shove it into a 'normal' all pass filter - we'll use this later
		leftEarlyAPFinput = leftEarlyDelayTap1 + leftEarlyDelayTap2 + leftEarlyDelayTap3 + leftEarlyDelayTap4 + leftEarlyDelayTap5;
		
		leftEarlyAPFoutput = APF_earlyL.processAudio(inL);

		rightEarlyAPFinput = rightEarlyDelayTap1 + rightEarlyDelayTap2 + rightEarlyDelayTap3 + rightEarlyDelayTap4 + rightEarlyDelayTap5;
	
		rightEarlyAPFoutput = APF_earlyL.processAudio(inL);

		// Time for the late reverberator

		leftMatrixInput = leftDelayOut + loopbackL;
		rightMatrixInput = rightDelayOut + loopbackR;

		leftMatrixOutput = (1 / sqrt(2)) * leftMatrixInput + (1 / sqrt(2) * rightMatrixInput);
		rightMatrixOutput = (1 / sqrt(2)) * rightMatrixInput + (1 / sqrt(2) * rightMatrixInput);

		chainL = leftMatrixOutput;
		chainL = aapf_L[0].processAudio(chainL);
		leftChainTap1 = chainL * tapGainL[0];
		chainL = aapf_L[1].processAudio(chainL);
		leftChainTap2 = chainL * tapGainL[1] + leftChainTap1;
		chainL = aapf_L[2].processAudio(chainL);
		leftChainTap3 = chainL * tapGainL[2] + leftChainTap2;
		chainL = aapf_L[3].processAudio(chainL);
		leftChainTap4 = chainL * tapGainL[3] + leftChainTap3;

		chainR = rightMatrixOutput;
		chainR = aapf_R[0].processAudio(chainR);
		rightChainTap1 = chainR * tapGainR[0];
		chainR = aapf_R[1].processAudio(chainR);
		rightChainTap2 = chainR * tapGainR[1] + rightChainTap1;
		chainR = aapf_R[2].processAudio(chainR);
		rightChainTap3 = chainR * tapGainR[2] + rightChainTap2;
		chainR = aapf_R[3].processAudio(chainR);
		rightChainTap4 = chainR * tapGainR[3] + rightChainTap3;

		chainL = chainL_delay.processAudioSample(chainL); // basic delay block after the 4th absorbent all-pass filter
		chainL = chainL_LPF_tg.processAudio(chainL) * gDL; // basic low pass filter in the chain, multiplied by the gDL gain factor
		chainR = chainR_delay.processAudioSample(chainR); // basic delay block after the 4th absorbent all-pass filter
		chainR = chainR_LPF_tg.processAudio(chainR) * gDL; // basic low pass filter in the chain, multiplied by the gDR gain factor

		chainL = aapf_L[4].processAudio(chainL);
		leftChainTap5 = chainL * tapGainL[4] + leftChainTap4;
		leftChainTap6 = aapf_L[5].processAudio(chainL) * tapGainL[5] + leftChainTap5;

		chainR = aapf_R[4].processAudio(chainR);
		rightChainTap5 = chainR * tapGainR[4] + rightChainTap4;
		rightChainTap6 = aapf_R[5].processAudio(chainR) * tapGainR[5] + rightChainTap5;

		// Feed back the output of the chain of absorbent all-pass back to the matrix
		leftChainOutput = leftChainTap6;
		rightChainOutput = rightChainTap6;

		loopbackL += leftChainTap6;
		loopbackR += rightChainTap6;

		// Now combine the early echo section with the chain of the absorbent all-passes, and pass it out
		reverbOutL = leftEarlyAPFoutput * reflectionsLevel_lin + (leftChainOutput * leftReverbOutputLevel);
		reverbOutR = rightEarlyAPFoutput * reflectionsLevel_lin + (rightChainOutput * rightReverbOutputLevel);
		outL = reverbOutL * roomLevel_lin;
		outR = reverbOutR * roomLevel_lin;

		// Simple widening algorithm
		float widthTemp = 1 / fmax(1 + stereoWidth, 2);
		float widthCoefficientMid = 1 * widthTemp;
		float widthCoefficientSides = stereoWidth * widthTemp;

		float widthMid = (outL + outR) * widthCoefficientMid;
		float widthSides = (outR - outL) * widthCoefficientSides;

		double wideOutL = widthMid - widthSides;
		double wideOutR = widthMid + widthSides;

		if (directSoundStatus == 1)
		{
			processFrameInfo.audioOutputFrame[0] = (0.35 * inL) + wideOutL;
			processFrameInfo.audioOutputFrame[1] = (0.35 * inR) + wideOutR;
		}
		else
		{
			processFrameInfo.audioOutputFrame[0] = wideOutL;
			processFrameInfo.audioOutputFrame[1] = wideOutR;
		}

		return true; /// processed
	}

	return false; /// NOT processed
}

/**
\brief do anything needed prior to arrival of audio buffers

Operation:
- updateOutBoundVariables sends metering data to the GUI meters

\param processInfo structure of information about *buffer* processing

\return true if operation succeeds, false otherwise
*/
bool PluginCore::postProcessAudioBuffers(ProcessBufferInfo& processInfo)
{
	// --- update outbound variables; currently this is meter data only, but could be extended
	//     in the future
	updateOutBoundVariables();

	return true;
}

/**
\brief update the PluginParameter's value based on GUI control, preset, or data smoothing (thread-safe)

Operation:
- update the parameter's value (with smoothing this initiates another smoothing process)
- call postUpdatePluginParameter to do any further processing

\param controlID the control ID value of the parameter being updated
\param controlValue the new control value
\param paramInfo structure of information about why this value is being udpated (e.g as a result of a preset being loaded vs. the top of a buffer process cycle)

\return true if operation succeeds, false otherwise
*/
bool PluginCore::updatePluginParameter(int32_t controlID, double controlValue, ParameterUpdateInfo& paramInfo)
{
	// --- use base class helper
	setPIParamValue(controlID, controlValue);

	// --- do any post-processing
	postUpdatePluginParameter(controlID, controlValue, paramInfo);

	return true; /// handled
}

/**
\brief update the PluginParameter's value based on *normalized* GUI control, preset, or data smoothing (thread-safe)

Operation:
- update the parameter's value (with smoothing this initiates another smoothing process)
- call postUpdatePluginParameter to do any further processing

\param controlID the control ID value of the parameter being updated
\param normalizedValue the new control value in normalized form
\param paramInfo structure of information about why this value is being updated (e.g as a result of a preset being loaded vs. the top of a buffer process cycle)

\return true if operation succeeds, false otherwise
*/
bool PluginCore::updatePluginParameterNormalized(int32_t controlID, double normalizedValue, ParameterUpdateInfo& paramInfo)
{
	// --- use base class helper, returns actual value
	double controlValue = setPIParamValueNormalized(controlID, normalizedValue, paramInfo.applyTaper);

	// --- do any post-processing
	postUpdatePluginParameter(controlID, controlValue, paramInfo);

	return true; /// handled
}

/**
\brief perform any operations after the plugin parameter has been updated; this is one paradigm for
	   transferring control information into vital plugin variables or member objects. If you use this
	   method you can decode the control ID and then do any cooking that is needed. NOTE: do not
	   overwrite bound variables here - this is ONLY for any extra cooking that is required to convert
	   the GUI data to meaninful coefficients or other specific modifiers.

\param controlID the control ID value of the parameter being updated
\param controlValue the new control value
\param paramInfo structure of information about why this value is being udpated (e.g as a result of a preset being loaded vs. the top of a buffer process cycle)

\return true if operation succeeds, false otherwise
*/
bool PluginCore::postUpdatePluginParameter(int32_t controlID, double controlValue, ParameterUpdateInfo& paramInfo)
{
	// --- now do any post update cooking; be careful with VST Sample Accurate automation
	//     If enabled, then make sure the cooking functions are short and efficient otherwise disable it
	//     for the Parameter involved
	switch (controlID)
	{
	case controlID::Room_level:
	{
		i3_RoomLevel = controlRoomLevel;
		roomLevel_dB = i3_RoomLevel / 100;	// Convert to dB
		roomLevel_lin = pow(10, roomLevel_dB / 20); // Convert to linear scale
		tg_updateVars_EnergyGain();
		return true;    /// handled
	}
	case controlID::Room_HF_level:
	{
		i3_RoomHfLevel = controlRoomHFLevel;
		roomHFLevel_dB = i3_RoomHfLevel / 100;	// Convert to dB
		roomHFLevel_lin = pow(10, roomHFLevel_dB / 20);  // Convert to linear scale
		tg_updateVars_earlyEcho_hfRef_decayTime();
		return true;    /// handled
	}
	case controlID::Reflections_level:
	{
		i3_ReflectionsLevel = controlReflectionsLevel;
		reflectionsLeveldB = i3_ReflectionsLevel / 100;	// Convert to dB
		reflectionsLevel_lin = pow(10, reflectionsLeveldB / 20);  // Convert to linear scale
		return true;    /// handled
	}
	case controlID::Reverb_level:
	{
		i3_ReverbLevel = controlReverbLevel;
		reverbLevel_dB = i3_ReverbLevel / 100;	// Convert to dB
		reverbLevel_lin = pow(10, reverbLevel_dB / 20); // Convert to linear scale
		tg_updateVars_EnergyGain();
		return true;    /// handled
	}
	case controlID::Decay_HF_ratio:
	{
		i3_DecayHfRatio = controlDecayHFRatio;
		tg_updateVars_earlyEcho_hfRef_decayTime();
		tg_updateVars_lateReverb_controlHFRatio_controlHFref();
		tg_updateVars_EnergyGain();
		return true;
	}
	case controlID::Decay_time:
	{
		i3_DecayTime = controlDecayTime;
		tg_updateVars_earlyEcho_hfRef_decayTime();
		tg_updateVars_lateReverb_controlDecayTime();
		tg_updateVars_EnergyGain();
		return true;
	}
	case controlID::Reflections_delay:
	{
		i3_ReflectionsDelay = controlReflectionsDelay;
		double tempRoomLevel = roomLevel_lin;
		//roomLevel_lin = 0.0;
		tg_updateVars_earlyEcho_delayTimes();
		roomLevel_lin = tempRoomLevel;
		return true;
	}
	case controlID::Reverb_delay:
	{
		i3_ReverbDelay = controlReverbDelay;
		double tempRoomLevel = roomLevel_lin;
		//roomLevel_lin = 0.0;
		tg_updateVars_earlyEcho_delayTimes();
		roomLevel_lin = tempRoomLevel;
		return true;
	}
	case controlID::Diffusion:
	{
		i3_Diffusion = controlDiffusion;
		tg_updateVars_lateReverb_controlDiffusion();
		tg_updateVars_EnergyGain();
		return true;
	}
	case controlID::Density:
	{
		i3_Density = controlDensity;
		double tempRoomLevel = roomLevel_lin;
		//roomLevel_lin = 0.0;
		tg_updateVars_lateReverb_controlDensity();
		tg_updateVars_lateReverb_controlDecayTime();
		tg_updateVars_lateReverb_chainDelays();
		tg_updateVars_lateReverb_controlHFRatio_controlHFref();
		tg_updateVars_EnergyGain();
		return true;
	}
	case controlID::HF_reference:
	{
		i3_HfReference = controlHFReference;
		Fc = i3_HfReference;
		tg_updateVars_earlyEcho_hfRef_decayTime();
		tg_updateVars_lateReverb_controlHFRatio_controlHFref();
		tg_updateVars_EnergyGain();
		return true;
	}
	case controlID::Stereo_width:
	{
		stereoWidth = controlStereoWidth / 10; // Convert the width control percentage to a value that is less exaggerated
		return true;
	}

	default:
		return false;   /// not handled
	}

	return false;
}

/**
\brief has nothing to do with actual variable or updated variable (binding)

CAUTION:
- DO NOT update underlying variables here - this is only for sending GUI updates or letting you
  know that a parameter was changed; it should not change the state of your plugin.

WARNING:
- THIS IS NOT THE PREFERRED WAY TO LINK OR COMBINE CONTROLS TOGETHER. THE PROPER METHOD IS
  TO USE A CUSTOM SUB-CONTROLLER THAT IS PART OF THE GUI OBJECT AND CODE.
  SEE http://www.willpirkle.com for more information

\param controlID the control ID value of the parameter being updated
\param actualValue the new control value

\return true if operation succeeds, false otherwise
*/
bool PluginCore::guiParameterChanged(int32_t controlID, double actualValue)
{
	/*
	switch (controlID)
	{
		case controlID::<your control here>
		{
			return true; // handled
		}

		default:
			break;
	}*/

	return false; /// not handled
}

/**
\brief For Custom View and Custom Sub-Controller Operations

NOTES:
- this is for advanced users only to implement custom view and custom sub-controllers
- see the SDK for examples of use

\param messageInfo a structure containing information about the incoming message

\return true if operation succeeds, false otherwise
*/
bool PluginCore::processMessage(MessageInfo& messageInfo)
{
	// --- decode message
	switch (messageInfo.message)
	{
		// --- add customization appearance here
	case PLUGINGUI_DIDOPEN:
	{
		return false;
	}

	// --- NULL pointers so that we don't accidentally use them
	case PLUGINGUI_WILLCLOSE:
	{
		return false;
	}

	// --- update view; this will only be called if the GUI is actually open
	case PLUGINGUI_TIMERPING:
	{
		return false;
	}

	// --- register the custom view, grab the ICustomView interface
	case PLUGINGUI_REGISTER_CUSTOMVIEW:
	{
		return false;
	}

	case PLUGINGUI_REGISTER_SUBCONTROLLER:
	case PLUGINGUI_QUERY_HASUSERCUSTOM:
	case PLUGINGUI_USER_CUSTOMOPEN:
	case PLUGINGUI_USER_CUSTOMCLOSE:
	case PLUGINGUI_EXTERNAL_SET_NORMVALUE:
	case PLUGINGUI_EXTERNAL_SET_ACTUALVALUE:
	{
		return false;
	}

	default:
		break;
	}

	return false; /// not handled
}

/**
\brief process a MIDI event

NOTES:
- MIDI events are 100% sample accurate; this function will be called repeatedly for every MIDI message
- see the SDK for examples of use

\param event a structure containing the MIDI event data

\return true if operation succeeds, false otherwise
*/
bool PluginCore::processMIDIEvent(midiEvent& event)
{
	return true;
}

/**
\brief (for future use)

NOTES:
- MIDI events are 100% sample accurate; this function will be called repeatedly for every MIDI message
- see the SDK for examples of use

\param vectorJoysickData a structure containing joystick data

\return true if operation succeeds, false otherwise
*/
bool PluginCore::setVectorJoystickParameters(const VectorJoystickData& vectorJoysickData)
{
	return true;
}

/**
\brief use this method to add new presets to the list

NOTES:
- see the SDK for examples of use
- for non RackAFX users that have large parameter counts, there is a secret GUI control you
  can enable to write C++ code into text files, one per preset. See the SDK or http://www.willpirkle.com for details

\return true if operation succeeds, false otherwise
*/
bool PluginCore::initPluginPresets()
{
	// **--0xFF7A--**
	int index = 0;	/*** declare this once at the top of the presets function, comment out otherwise */
	// --- Plugin Preset: Generic
	PresetInfo* Generic = new PresetInfo(index++, "Generic");
	initPresetParameters(Generic->presetParameters);
	setPresetParameter(Generic->presetParameters, 0, -100);	// --- Room_level
	setPresetParameter(Generic->presetParameters, 1, -100);	// --- Room_HF_level
	setPresetParameter(Generic->presetParameters, 2, 0);	// --- Room_rolloff_factor
	setPresetParameter(Generic->presetParameters, 3, 1.49);	// --- Decay_time
	setPresetParameter(Generic->presetParameters, 4, 0.83);	// --- Decay_HF_ratio
	setPresetParameter(Generic->presetParameters, 5, -2602);	// --- Reflections_level
	setPresetParameter(Generic->presetParameters, 6, 0.007);	// --- Reflections_delay
	setPresetParameter(Generic->presetParameters, 7, 200);	// --- Reverb_level
	setPresetParameter(Generic->presetParameters, 8, 0.011);	// --- Reverb_delay
	setPresetParameter(Generic->presetParameters, 9, 100);	// --- Diffusion
	setPresetParameter(Generic->presetParameters, 10, 100);	// --- Density
	setPresetParameter(Generic->presetParameters, 11, 5000);	// --- HF_reference
	setPresetParameter(Generic->presetParameters, 13, 1);	// --- Direct_sound
	addPreset(Generic);

	// --- Plugin Preset:  Cell
	PresetInfo* PaddedCell = new PresetInfo(index++, "Padded Cell");
	initPresetParameters(PaddedCell->presetParameters);
	setPresetParameter(PaddedCell->presetParameters, 0, -100);	// --- Room_level
	setPresetParameter(PaddedCell->presetParameters, 1, -6000);	// --- Room_HF_level
	setPresetParameter(PaddedCell->presetParameters, 2, 0);	// --- Room_rolloff_factor
	setPresetParameter(PaddedCell->presetParameters, 3, 0.17);	// --- Decay_time
	setPresetParameter(PaddedCell->presetParameters, 4, 0.10);	// --- Decay_HF_ratio
	setPresetParameter(PaddedCell->presetParameters, 5, -1204);	// --- Reflections_level
	setPresetParameter(PaddedCell->presetParameters, 6, 0.001);	// --- Reflections_delay
	setPresetParameter(PaddedCell->presetParameters, 7, 207);	// --- Reverb_level
	setPresetParameter(PaddedCell->presetParameters, 8, 0.002);	// --- Reverb_delay
	setPresetParameter(PaddedCell->presetParameters, 9, 100);	// --- Diffusion
	setPresetParameter(PaddedCell->presetParameters, 10, 100);	// --- Density
	setPresetParameter(PaddedCell->presetParameters, 11, 5000);	// --- HF_reference
	setPresetParameter(PaddedCell->presetParameters, 13, 1);	// --- Direct_sound
	addPreset(PaddedCell);

	// --- Plugin Preset: Room
	PresetInfo* Room = new PresetInfo(index++, "Room");
	initPresetParameters(Room->presetParameters);
	setPresetParameter(Room->presetParameters, 0, -100);	// --- Room_level
	setPresetParameter(Room->presetParameters, 1, -454);	// --- Room_HF_level
	setPresetParameter(Room->presetParameters, 2, 0);	// --- Room_rolloff_factor
	setPresetParameter(Room->presetParameters, 3, 0.4);	// --- Decay_time
	setPresetParameter(Room->presetParameters, 4, 0.83);	// --- Decay_HF_ratio
	setPresetParameter(Room->presetParameters, 5, -1646);	// --- Reflections_level
	setPresetParameter(Room->presetParameters, 6, 0.002);	// --- Reflections_delay
	setPresetParameter(Room->presetParameters, 7, 53);	// --- Reverb_level
	setPresetParameter(Room->presetParameters, 8, 0.003);	// --- Reverb_delay
	setPresetParameter(Room->presetParameters, 9, 100);	// --- Diffusion
	setPresetParameter(Room->presetParameters, 10, 100);	// --- Density
	setPresetParameter(Room->presetParameters, 11, 5000);	// --- HF_reference
	setPresetParameter(Room->presetParameters, 13, 1);	// --- Direct_sound
	addPreset(Room);

	// --- Plugin Preset: Auditorium
	PresetInfo* Auditorium = new PresetInfo(index++, "Auditorium");
	initPresetParameters(Auditorium->presetParameters);
	setPresetParameter(Auditorium->presetParameters, 0, -100);	// --- Room_level
	setPresetParameter(Auditorium->presetParameters, 1, -476);	// --- Room_HF_level
	setPresetParameter(Auditorium->presetParameters, 2, 0);	// --- Room_rolloff_factor
	setPresetParameter(Auditorium->presetParameters, 3, 4.32);	// --- Decay_time
	setPresetParameter(Auditorium->presetParameters, 4, 0.59);	// --- Decay_HF_ratio
	setPresetParameter(Auditorium->presetParameters, 5, -789);	// --- Reflections_level
	setPresetParameter(Auditorium->presetParameters, 6, 0.02);	// --- Reflections_delay
	setPresetParameter(Auditorium->presetParameters, 7, -289);	// --- Reverb_level
	setPresetParameter(Auditorium->presetParameters, 8, 0.03);	// --- Reverb_delay
	setPresetParameter(Auditorium->presetParameters, 9, 100);	// --- Diffusion
	setPresetParameter(Auditorium->presetParameters, 10, 100);	// --- Density
	setPresetParameter(Auditorium->presetParameters, 11, 5000);	// --- HF_reference
	setPresetParameter(Auditorium->presetParameters, 13, 1);	// --- Direct_sound
	addPreset(Auditorium);

	// --- Plugin Preset: ConcertHall
	PresetInfo* ConcertHall = new PresetInfo(index++, "Concert Hall");
	initPresetParameters(ConcertHall->presetParameters);
	setPresetParameter(ConcertHall->presetParameters, 0, -100);	// --- Room_level
	setPresetParameter(ConcertHall->presetParameters, 1, -500);	// --- Room_HF_level
	setPresetParameter(ConcertHall->presetParameters, 2, 0);	// --- Room_rolloff_factor
	setPresetParameter(ConcertHall->presetParameters, 3, 3.92);	// --- Decay_time
	setPresetParameter(ConcertHall->presetParameters, 4, 0.7);	// --- Decay_HF_ratio
	setPresetParameter(ConcertHall->presetParameters, 5, -1230);	// --- Reflections_level
	setPresetParameter(ConcertHall->presetParameters, 6, 0.02);	// --- Reflections_delay
	setPresetParameter(ConcertHall->presetParameters, 7, -2);	// --- Reverb_level
	setPresetParameter(ConcertHall->presetParameters, 8, 0.029);	// --- Reverb_delay
	setPresetParameter(ConcertHall->presetParameters, 9, 100);	// --- Diffusion
	setPresetParameter(ConcertHall->presetParameters, 10, 100);	// --- Density
	setPresetParameter(ConcertHall->presetParameters, 11, 5000);	// --- HF_reference
	setPresetParameter(ConcertHall->presetParameters, 13, 1);	// --- Direct_sound
	addPreset(ConcertHall);

	// --- Plugin Preset: Cave
	PresetInfo* Cave = new PresetInfo(index++, "Cave");
	initPresetParameters(Cave->presetParameters);
	setPresetParameter(Cave->presetParameters, 0, -100);	// --- Room_level
	setPresetParameter(Cave->presetParameters, 1, 0);	// --- Room_HF_level
	setPresetParameter(Cave->presetParameters, 2, 0);	// --- Room_rolloff_factor
	setPresetParameter(Cave->presetParameters, 3, 2.91);	// --- Decay_time
	setPresetParameter(Cave->presetParameters, 4, 1.3);	// --- Decay_HF_ratio
	setPresetParameter(Cave->presetParameters, 5, -602);	// --- Reflections_level
	setPresetParameter(Cave->presetParameters, 6, 0.015);	// --- Reflections_delay
	setPresetParameter(Cave->presetParameters, 7, -302);	// --- Reverb_level
	setPresetParameter(Cave->presetParameters, 8, 0.022);	// --- Reverb_delay
	setPresetParameter(Cave->presetParameters, 9, 100);	// --- Diffusion
	setPresetParameter(Cave->presetParameters, 10, 100);	// --- Density
	setPresetParameter(Cave->presetParameters, 11, 5000);	// --- HF_reference
	setPresetParameter(Cave->presetParameters, 13, 1);	// --- Direct_sound
	addPreset(Cave);

	// --- Plugin Preset: Forest
	PresetInfo* Forest = new PresetInfo(index++, "Forest");
	initPresetParameters(Forest->presetParameters);
	setPresetParameter(Forest->presetParameters, 0, -100);	// --- Room_level
	setPresetParameter(Forest->presetParameters, 1, -3300);	// --- Room_HF_level
	setPresetParameter(Forest->presetParameters, 2, 0);	// --- Room_rolloff_factor
	setPresetParameter(Forest->presetParameters, 3, 1.49);	// --- Decay_time
	setPresetParameter(Forest->presetParameters, 4, 0.54);	// --- Decay_HF_ratio
	setPresetParameter(Forest->presetParameters, 5, -2560);	// --- Reflections_level
	setPresetParameter(Forest->presetParameters, 6, 0.162);	// --- Reflections_delay
	setPresetParameter(Forest->presetParameters, 7, -613);	// --- Reverb_level
	setPresetParameter(Forest->presetParameters, 8, 0.88);	// --- Reverb_delay
	setPresetParameter(Forest->presetParameters, 9, 79);	// --- Diffusion
	setPresetParameter(Forest->presetParameters, 10, 100);	// --- Density
	setPresetParameter(Forest->presetParameters, 11, 5000);	// --- HF_reference
	setPresetParameter(Forest->presetParameters, 13, 1);	// --- Direct_sound
	addPreset(Forest);

	// --- Plugin Preset: Mountains
	PresetInfo* Mountains = new PresetInfo(index++, "Mountains");
	initPresetParameters(Mountains->presetParameters);
	setPresetParameter(Mountains->presetParameters, 0, -100);	// --- Room_level
	setPresetParameter(Mountains->presetParameters, 1, -2500);	// --- Room_HF_level
	setPresetParameter(Mountains->presetParameters, 2, 0);	// --- Room_rolloff_factor
	setPresetParameter(Mountains->presetParameters, 3, 1.49);	// --- Decay_time
	setPresetParameter(Mountains->presetParameters, 4, 0.21);	// --- Decay_HF_ratio
	setPresetParameter(Mountains->presetParameters, 5, -2780);	// --- Reflections_level
	setPresetParameter(Mountains->presetParameters, 6, 0.300);	// --- Reflections_delay
	setPresetParameter(Mountains->presetParameters, 7, -2014);	// --- Reverb_level
	setPresetParameter(Mountains->presetParameters, 8, 0.100);	// --- Reverb_delay
	setPresetParameter(Mountains->presetParameters, 9, 27);	// --- Diffusion
	setPresetParameter(Mountains->presetParameters, 10, 100);	// --- Density
	setPresetParameter(Mountains->presetParameters, 11, 5000);	// --- HF_reference
	setPresetParameter(Mountains->presetParameters, 13, 1);	// --- Direct_sound
	addPreset(Mountains);

	// --- Plugin Preset: City
	PresetInfo* City = new PresetInfo(index++, "City");
	initPresetParameters(City->presetParameters);
	setPresetParameter(City->presetParameters, 0, -100);	// --- Room_level
	setPresetParameter(City->presetParameters, 1, -800);	// --- Room_HF_level
	setPresetParameter(City->presetParameters, 2, 0);	// --- Room_rolloff_factor
	setPresetParameter(City->presetParameters, 3, 1.49);	// --- Decay_time
	setPresetParameter(City->presetParameters, 4, 0.67);	// --- Decay_HF_ratio
	setPresetParameter(City->presetParameters, 5, -2273);	// --- Reflections_level
	setPresetParameter(City->presetParameters, 6, 0.007);	// --- Reflections_delay
	setPresetParameter(City->presetParameters, 7, -2217);	// --- Reverb_level
	setPresetParameter(City->presetParameters, 8, 0.011);	// --- Reverb_delay
	setPresetParameter(City->presetParameters, 9, 50);	// --- Diffusion
	setPresetParameter(City->presetParameters, 10, 100);	// --- Density
	setPresetParameter(City->presetParameters, 11, 5000);	// --- HF_reference
	setPresetParameter(City->presetParameters, 13, 1);	// --- Direct_sound
	addPreset(City);

	// --- Plugin Preset: SmallRoom
	PresetInfo* SmallRoom = new PresetInfo(index++, "Small Room");
	initPresetParameters(SmallRoom->presetParameters);
	setPresetParameter(SmallRoom->presetParameters, 0, -100);	// --- Room_level
	setPresetParameter(SmallRoom->presetParameters, 1, -600);	// --- Room_HF_level
	setPresetParameter(SmallRoom->presetParameters, 2, 0);	// --- Room_rolloff_factor
	setPresetParameter(SmallRoom->presetParameters, 3, 1.10);	// --- Decay_time
	setPresetParameter(SmallRoom->presetParameters, 4, 0.83);	// --- Decay_HF_ratio
	setPresetParameter(SmallRoom->presetParameters, 5, -400);	// --- Reflections_level
	setPresetParameter(SmallRoom->presetParameters, 6, 0.005);	// --- Reflections_delay
	setPresetParameter(SmallRoom->presetParameters, 7, 500);	// --- Reverb_level
	setPresetParameter(SmallRoom->presetParameters, 8, 0.010);	// --- Reverb_delay
	setPresetParameter(SmallRoom->presetParameters, 9, 100);	// --- Diffusion
	setPresetParameter(SmallRoom->presetParameters, 10, 100);	// --- Density
	setPresetParameter(SmallRoom->presetParameters, 11, 5000);	// --- HF_reference
	setPresetParameter(SmallRoom->presetParameters, 13, 1);	// --- Direct_sound
	addPreset(SmallRoom);

	// --- Plugin Preset: MediumRoom
	PresetInfo* MediumRoom = new PresetInfo(index++, "Medium Room");
	initPresetParameters(MediumRoom->presetParameters);
	setPresetParameter(MediumRoom->presetParameters, 0, -100);	// --- Room_level
	setPresetParameter(MediumRoom->presetParameters, 1, -600);	// --- Room_HF_level
	setPresetParameter(MediumRoom->presetParameters, 2, 0);	// --- Room_rolloff_factor
	setPresetParameter(MediumRoom->presetParameters, 3, 1.30);	// --- Decay_time
	setPresetParameter(MediumRoom->presetParameters, 4, 0.83);	// --- Decay_HF_ratio
	setPresetParameter(MediumRoom->presetParameters, 5, -1000);	// --- Reflections_level
	setPresetParameter(MediumRoom->presetParameters, 6, 0.010);	// --- Reflections_delay
	setPresetParameter(MediumRoom->presetParameters, 7, -200);	// --- Reverb_level
	setPresetParameter(MediumRoom->presetParameters, 8, 0.020);	// --- Reverb_delay
	setPresetParameter(MediumRoom->presetParameters, 9, 100);	// --- Diffusion
	setPresetParameter(MediumRoom->presetParameters, 10, 100);	// --- Density
	setPresetParameter(MediumRoom->presetParameters, 11, 5000);	// --- HF_reference
	setPresetParameter(MediumRoom->presetParameters, 13, 1);	// --- Direct_sound
	addPreset(MediumRoom);

	// --- Plugin Preset: LargeRoom
	PresetInfo* LargeRoom = new PresetInfo(index++, "Large Room");
	initPresetParameters(LargeRoom->presetParameters);
	setPresetParameter(LargeRoom->presetParameters, 0, -100);	// --- Room_level
	setPresetParameter(LargeRoom->presetParameters, 1, -600);	// --- Room_HF_level
	setPresetParameter(LargeRoom->presetParameters, 2, 0);	// --- Room_rolloff_factor
	setPresetParameter(LargeRoom->presetParameters, 3, 1.50);	// --- Decay_time
	setPresetParameter(LargeRoom->presetParameters, 4, 0.83);	// --- Decay_HF_ratio
	setPresetParameter(LargeRoom->presetParameters, 5, -1600);	// --- Reflections_level
	setPresetParameter(LargeRoom->presetParameters, 6, 0.020);	// --- Reflections_delay
	setPresetParameter(LargeRoom->presetParameters, 7, -1000);	// --- Reverb_level
	setPresetParameter(LargeRoom->presetParameters, 8, 0.040);	// --- Reverb_delay
	setPresetParameter(LargeRoom->presetParameters, 9, 100);	// --- Diffusion
	setPresetParameter(LargeRoom->presetParameters, 10, 100);	// --- Density
	setPresetParameter(LargeRoom->presetParameters, 11, 5000);	// --- HF_reference
	setPresetParameter(LargeRoom->presetParameters, 13, 1);	// --- Direct_sound
	addPreset(LargeRoom);

	// --- Plugin Preset: MediumHall
	PresetInfo* MediumHall = new PresetInfo(index++, "Medium Hall");
	initPresetParameters(MediumHall->presetParameters);
	setPresetParameter(MediumHall->presetParameters, 0, -100);	// --- Room_level
	setPresetParameter(MediumHall->presetParameters, 1, -600);	// --- Room_HF_level
	setPresetParameter(MediumHall->presetParameters, 2, 0);	// --- Room_rolloff_factor
	setPresetParameter(MediumHall->presetParameters, 3, 1.80);	// --- Decay_time
	setPresetParameter(MediumHall->presetParameters, 4, 0.70);	// --- Decay_HF_ratio
	setPresetParameter(MediumHall->presetParameters, 5, -1300);	// --- Reflections_level
	setPresetParameter(MediumHall->presetParameters, 6, 0.015);	// --- Reflections_delay
	setPresetParameter(MediumHall->presetParameters, 7, -800);	// --- Reverb_level
	setPresetParameter(MediumHall->presetParameters, 8, 0.030);	// --- Reverb_delay
	setPresetParameter(MediumHall->presetParameters, 9, 100);	// --- Diffusion
	setPresetParameter(MediumHall->presetParameters, 10, 100);	// --- Density
	setPresetParameter(MediumHall->presetParameters, 11, 5000);	// --- HF_reference
	setPresetParameter(MediumHall->presetParameters, 13, 1);	// --- Direct_sound
	addPreset(MediumHall);

	// --- Plugin Preset: LargeHall
	PresetInfo* LargeHall = new PresetInfo(index++, "Large Hall");
	initPresetParameters(LargeHall->presetParameters);
	setPresetParameter(LargeHall->presetParameters, 0, -100);	// --- Room_level
	setPresetParameter(LargeHall->presetParameters, 1, -600);	// --- Room_HF_level
	setPresetParameter(LargeHall->presetParameters, 2, 0);	// --- Room_rolloff_factor
	setPresetParameter(LargeHall->presetParameters, 3, 1.80);	// --- Decay_time
	setPresetParameter(LargeHall->presetParameters, 4, 0.70);	// --- Decay_HF_ratio
	setPresetParameter(LargeHall->presetParameters, 5, -2000);	// --- Reflections_level
	setPresetParameter(LargeHall->presetParameters, 6, 0.030);	// --- Reflections_delay
	setPresetParameter(LargeHall->presetParameters, 7, -1400);	// --- Reverb_level
	setPresetParameter(LargeHall->presetParameters, 8, 0.060);	// --- Reverb_delay
	setPresetParameter(LargeHall->presetParameters, 9, 100);	// --- Diffusion
	setPresetParameter(LargeHall->presetParameters, 10, 100);	// --- Density
	setPresetParameter(LargeHall->presetParameters, 11, 5000);	// --- HF_reference
	setPresetParameter(LargeHall->presetParameters, 13, 1);	// --- Direct_sound
	addPreset(LargeHall);

	// --- Plugin Preset: Plate
	PresetInfo* Plate = new PresetInfo(index++, "Plate");
	initPresetParameters(Plate->presetParameters);
	setPresetParameter(Plate->presetParameters, 0, -100);	// --- Room_level
	setPresetParameter(Plate->presetParameters, 1, -200);	// --- Room_HF_level
	setPresetParameter(Plate->presetParameters, 2, 0);	// --- Room_rolloff_factor
	setPresetParameter(Plate->presetParameters, 3, 1.30);	// --- Decay_time
	setPresetParameter(Plate->presetParameters, 4, 0.90);	// --- Decay_HF_ratio
	setPresetParameter(Plate->presetParameters, 5, 0.00);	// --- Reflections_level
	setPresetParameter(Plate->presetParameters, 6, 0.002);	// --- Reflections_delay
	setPresetParameter(Plate->presetParameters, 7, 0.00);	// --- Reverb_level
	setPresetParameter(Plate->presetParameters, 8, 0.010);	// --- Reverb_delay
	setPresetParameter(Plate->presetParameters, 9, 100);	// --- Diffusion
	setPresetParameter(Plate->presetParameters, 10, 75);	// --- Density
	setPresetParameter(Plate->presetParameters, 11, 5000);	// --- HF_reference
	setPresetParameter(Plate->presetParameters, 13, 1);	// --- Direct_sound
	addPreset(Plate);

	// **--0xA7FF--**

	return true;
}

/**
\brief setup the plugin description strings, flags and codes; this is ordinarily done through the ASPiKreator or CMake

\return true if operation succeeds, false otherwise
*/
bool PluginCore::initPluginDescriptors()
{
	pluginDescriptor.pluginName = getPluginName();
	pluginDescriptor.shortPluginName = getShortPluginName();
	pluginDescriptor.vendorName = getVendorName();
	pluginDescriptor.pluginTypeCode = getPluginType();

	// --- describe the plugin attributes; set according to your needs
	pluginDescriptor.hasSidechain = kWantSidechain;
	pluginDescriptor.latencyInSamples = kLatencyInSamples;
	pluginDescriptor.tailTimeInMSec = kTailTimeMsec;
	pluginDescriptor.infiniteTailVST3 = kVSTInfiniteTail;

	// --- AAX
	apiSpecificInfo.aaxManufacturerID = kManufacturerID;
	apiSpecificInfo.aaxProductID = kAAXProductID;
	apiSpecificInfo.aaxBundleID = kAAXBundleID;  /* MacOS only: this MUST match the bundle identifier in your info.plist file */
	apiSpecificInfo.aaxEffectID = "aaxDeveloper.";
	apiSpecificInfo.aaxEffectID.append(getPluginName());
	apiSpecificInfo.aaxPluginCategoryCode = kAAXCategory;

	// --- AU
	apiSpecificInfo.auBundleID = kAUBundleID;   /* MacOS only: this MUST match the bundle identifier in your info.plist file */
	apiSpecificInfo.auBundleName = kAUBundleName;

	// --- VST3
	apiSpecificInfo.vst3FUID = getVSTFUID(); // OLE string format
	apiSpecificInfo.vst3BundleID = kVST3BundleID;/* MacOS only: this MUST match the bundle identifier in your info.plist file */
	apiSpecificInfo.enableVST3SampleAccurateAutomation = kVSTSAA;
	apiSpecificInfo.vst3SampleAccurateGranularity = kVST3SAAGranularity;

	// --- AU and AAX
	apiSpecificInfo.fourCharCode = getFourCharCode();

	return true;
}

// --- static functions required for VST3/AU only --------------------------------------------- //
const char* PluginCore::getPluginBundleName() { return kAUBundleName; }
const char* PluginCore::getPluginName() { return kPluginName; }
const char* PluginCore::getShortPluginName() { return kShortPluginName; }
const char* PluginCore::getVendorName() { return kVendorName; }
const char* PluginCore::getVendorURL() { return kVendorURL; }
const char* PluginCore::getVendorEmail() { return kVendorEmail; }
const char* PluginCore::getAUCocoaViewFactoryName() { return AU_COCOA_VIEWFACTORY_STRING; }
pluginType PluginCore::getPluginType() { return kPluginType; }
const char* PluginCore::getVSTFUID() { return kVSTFUID; }
int32_t PluginCore::getFourCharCode() { return kFourCharCode; }