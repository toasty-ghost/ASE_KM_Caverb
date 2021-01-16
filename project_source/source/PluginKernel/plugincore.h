// -----------------------------------------------------------------------------
//    ASPiK Plugin Kernel File:  plugincore.h
//
/**
	\file   plugincore.h
	\author Will Pirkle
	\date   17-September-2018
	\brief  base class interface file for ASPiK plugincore object
			- http://www.aspikplugins.com
			- http://www.willpirkle.com
*/
// -----------------------------------------------------------------------------
#ifndef __pluginCore_h__
#define __pluginCore_h__

#include "pluginbase.h"
#include "tg_AAPFlite.h"
#include "tg_LPF.h"
#include "fxobjects.h"

// Some useful little function snippets
//-----------------------------------------------
// 	conversions between db and linear values
//-----------------------------------------------
#include "db2lin.h"
#include "deZipper.h"
#include "lin2db.h"

// **--0x7F1F--**

enum controlID
{
	Room_level,
	Room_HF_level,
	Room_rolloff_factor,
	Decay_time,
	Decay_HF_ratio,
	Reflections_level,
	Reflections_delay,
	Reverb_level,
	Reverb_delay,
	Diffusion,
	Density,
	HF_reference,
	Stereo_width,
};

// **--0x0F1F--**

/**
\class PluginCore
\ingroup ASPiK-Core
\brief
The PluginCore object is the default PluginBase derived object for ASPiK projects.
Note that you are fre to change the name of this object (as long as you change it in the compiler settings, etc...)

PluginCore Operations:
- overrides the main processing functions from the base class
- performs reset operation on sub-modules
- processes audio
- processes messages for custom views
- performs pre and post processing functions on parameters and audio (if needed)

\author Will Pirkle http://www.willpirkle.com
\remark This object is included in Designing Audio Effects Plugins in C++ 2nd Ed. by Will Pirkle
\version Revision : 1.0
\date Date : 2018 / 09 / 7
*/
class PluginCore : public PluginBase
{
public:
	PluginCore();

	/** Destructor: empty in default version */
	virtual ~PluginCore() {}

	// --- PluginBase Overrides ---
	//
	/** this is the creation function for all plugin parameters */
	bool initPluginParameters();

	/** called when plugin is loaded, a new audio file is playing or sample rate changes */
	virtual bool reset(ResetInfo& resetInfo);

	/** one-time post creation init function; pluginInfo contains path to this plugin */
	virtual bool initialize(PluginInfo& _pluginInfo);
	void tg_updateVars_earlyEcho_hfRef_decayTime();
	void tg_updateVars_earlyEcho_delayTimes();

	// --- Custom functions to update parameters for elements of the reverberator
	void tg_updateVars_earlyEcho();
	void tg_updateVars_lateReverb_controlDensity();
	void tg_updateVars_lateReverb_chainDelays();
	void tg_updateVars_lateReverb_controlDiffusion();
	void tg_updateVars_lateReverb_controlHFRatio_controlHFref();
	void tg_updateVars_lateReverb_controlDecayTime();
	void tg_updateVars_lateReverb();
	void tg_updateVars_EnergyGain();

	// --- preProcess: sync GUI parameters here; override if you don't want to use automatic variable-binding
	virtual bool preProcessAudioBuffers(ProcessBufferInfo& processInfo);

	/** process frames of data */
	virtual bool processAudioFrame(ProcessFrameInfo& processFrameInfo);

	// --- uncomment and override this for buffer processing; see base class implementation for
	//     help on breaking up buffers and getting info from processBufferInfo
	//virtual bool processAudioBuffers(ProcessBufferInfo& processBufferInfo);

	/** preProcess: do any post-buffer processing required; default operation is to send metering data to GUI  */
	virtual bool postProcessAudioBuffers(ProcessBufferInfo& processInfo);

	/** called by host plugin at top of buffer proccess; this alters parameters prior to variable binding operation  */
	virtual bool updatePluginParameter(int32_t controlID, double controlValue, ParameterUpdateInfo& paramInfo);

	/** called by host plugin at top of buffer proccess; this alters parameters prior to variable binding operation  */
	virtual bool updatePluginParameterNormalized(int32_t controlID, double normalizedValue, ParameterUpdateInfo& paramInfo);

	/** this can be called: 1) after bound variable has been updated or 2) after smoothing occurs  */
	virtual bool postUpdatePluginParameter(int32_t controlID, double controlValue, ParameterUpdateInfo& paramInfo);

	/** this is ony called when the user makes a GUI control change */
	virtual bool guiParameterChanged(int32_t controlID, double actualValue);

	/** processMessage: messaging system; currently used for custom/special GUI operations */
	virtual bool processMessage(MessageInfo& messageInfo);

	/** processMIDIEvent: MIDI event processing */
	virtual bool processMIDIEvent(midiEvent& event);

	/** specialized joystick servicing (currently not used) */
	virtual bool setVectorJoystickParameters(const VectorJoystickData& vectorJoysickData);

	/** create the presets */
	bool initPluginPresets();

	// --- BEGIN USER VARIABLES AND FUNCTIONS -------------------------------------- //
	//	   Add your variables and methods here

	// Default I3DL2 Listener Properties
	double i3_RoomLevel = -1000.00000000;		// Attenuation of room effect, in millibels (mB). Affects the overall output level of the reverberator.
	double i3_RoomHfLevel = -1000.00000000;			// Attenuation of room effect at high frequencies, in millibels (mB). Adjusts the LPF filters at the input stage of the reverberator.
	double i3_roomRolloffFactor = 0.00000000;		//Rolloff factor for reflected signals. Not sure where or how this is used yet.
	double i3_DecayTime = 10.05000000;				// Decay time, in seconds. Adjusts the attenuation ('a') in the absorbent all - pass filters and delays in late section of reverberator based on the equation:	20 log10 | Gi(ejw) | = -60 ti / Tr(w)
	double i3_DecayHfRatio = 0.23000000;			// Ratio of the decay time at high frequencies, to the decay time at low frequencies. Adjusts the coefficients ('b') of the LPFs in the absorbent and delay elements of the late reverb.
	double i3_ReflectionsLevel = -602.00000000;	// Attenuation of the early reflections, relative to the Room Level, in millibels. Controls the intensity of the early echoes.
	double i3_ReflectionsDelay = 0.020000000;		//Delay time of the first reflection, relative to the direct path, in seconds.	Sets the delay time of the first tap in the tapped delay line, which creates the early echoes.
	double i3_ReverbLevel = 198.00000000;		// Attenuation of the late reverberations, relative to the Room Level, in millibels. Controls the intensity of the late reverb.
	double i3_ReverbDelay = 0.03000000;			// Time between the first early echo and the start of the late reverb, in seconds. Moves the tap that feeds the late reverb section, adjusting the tap lengths to ensure they span the complete time interval specified.
	double i3_Diffusion = 100.00000000;				// Echo density in the late reverberation decay, in percent. Scales the feedback coefficients('g') in each absorbent all-pass filter. Lowest value = 0, highest value = 0.6
	double i3_Density = 100.00000000;				// Modal density in the late reverberation decay, in percent. Scales the length of the delay lines in the absorbent all - pass filters.
	double i3_HfReference = 5000.00000000;			// Reference corner frequency for the derivation of the absorptive filters, in Hz. Affects the Room_HF and Decay_HF_Ratio parameters.
	double stereoWidth = 0.00000000;				// Does what it says on the tin

	// User control bindings
	double controlRoomLevel, controlRoomHFLevel, controlRoomRolloffFactor, controlDecayTime, controlDecayHFRatio, controlReflectionsLevel, controlReflectionsDelay, controlReverbLevel, controlReverbDelay, controlDensity, controlDiffusion, controlHFReference, controlStereoWidth, controlShortLongDelays;
	int directSoundStatus = 1;
	enum class directSoundStatusEnum { OFF, ON };

	// Little helper functions
	db2lin dB_lin;
	lin2db lin_dB;
	deZipper dZ[11];

	double fs = 48000;	// Define a default starting sample rate - this will be overwritten
	// Cooking variables
	double roomLevel_dB = i3_RoomLevel / 100;	// Convert to dB
	double roomLevel_lin = pow(10, roomLevel_dB / 20); // Convert to linear scale
	double roomHFLevel_dB = i3_RoomHfLevel / 100;	// Convert to dB
	double roomHFLevel_lin = pow(10, roomHFLevel_dB / 20); // Convert to linear scale
	double reflectionsLeveldB = i3_ReflectionsLevel / 100;	// Convert to dB
	double reflectionsLevel_lin = pow(10, reflectionsLeveldB / 20);  // Convert to linear scale
	double reverbLevel_dB = i3_ReverbLevel / 100;	// Convert to dB
	double reverbLevel_lin = pow(10, reverbLevel_dB / 20); // Convert to linear scale

	double maxAPg = 0.61803;	// The maximum all-pass gain coefficient that sounds good - see Dahl & Jot paper from 2000
	double Fc = i3_HfReference;	// Defines the cutoff frequency Fc based on the user input i3_HfReference in Hz
	double decayTime_mSec;

	double RT60_DC = i3_DecayTime;	// Decay time at DC
	double RT60_HF = i3_DecayHfRatio * i3_DecayTime;	// Decay time at HF, based on user set i3_DecayHfRatio
	double reflectionsDelay_mSec;
	double reverbDelay_mSec;
	double totalEarlyDelay_mSec;
	double earlyDelay_remainingTime;

	double leftDelayTapLength1, leftDelayTapLength2, leftDelayTapLength3, leftDelayTapLength4, leftDelayTapLength5;
	double rightDelayTapLength1, rightDelayTapLength2, rightDelayTapLength3, rightDelayTapLength4, rightDelayTapLength5;
	// Gain blocks for each tap of the early echo delay line
	float early_tapGain[6];

	double leftInputLPF_b, rightInputLPF_b;
	double lpf_bLSolo, lpf_bRSolo; // low pass filter coefficients for the in-line LPFs between AAPF 4 & 5

	// Absorbent all pass delay length vectors
	//float aapf_L_delayLength_mSec[6] = {77, 119, 159, 229, 399, 441}; // short set
	float aapf_L_delayPrimes_mSec[6] = { 127, 189, 253, 303, 451, 521 }; // long set
	float aapf_L_delayLength_mSec[6];
	float chainL_delayLength_mSec, chainR_delayLength_mSec;
	//float aapf_R_delayLength_mSec[6] = {53, 101, 179, 241, 377, 459}; // short set
	float aapf_R_delayPrimes_mSec[6] = { 131, 207, 269, 337, 441, 513 }; // long set
	float aapf_R_delayLength_mSec[6];

	// Coefficients used in the absorbent all pass filters
	double allPassG;
	double lpf_bL[6];
	double lfp_bR[6];
	float aapf_La[6];
	float aapf_Ra[6];
	double fBgainSquaredL[6], fBgainSquaredR[6];
	double absorbentGainSquaredL[6], absorbentGainSquaredR[6];
	float gDL, gDR;

	// Values used in the chain as gain multipliers - these need to be calculated properly....
	float NormL, NormR;
	double leftLoopEnergyGainA, rightLoopEnergyGainA, leftOutputGainB, rightOutputGainB, totalEnergyGainA;
	// cLi - energy gain for each absorbent all pass filter
	double cL[6];
	double cR[6];
	// Gain controls for taps after each absorbent all pass filter in the late reverb section
	float tapGainL[6];
	float tapGainR[6];
	float outGain_diff_adj, outGain_val, leftReverbOutputLevel, rightReverbOutputLevel;

	// Let's try again, keep it simple, stupid
	tg_AAPFlite APF_earlyL;
	tg_AAPFlite APF_earlyR;

	tg_AAPFlite aapf_L[6], aapf_R[6];

	tg_LPF workingLPF; // This is not used for any audio processing, it's just used to calculate coefficients for use in other sections.
	tg_LPF leftInputLPF_tg, rightInputLPF_tg;
	tg_LPF chainL_LPF_tg, chainR_LPF_tg;

	// deZipper to try and improve the performance of the delay lines
	deZipper dZ_reflectionsDelay, dZ_reverbDelay, dZ_Density;

	// Sod  this, I'm just going to build it using the ASPiK examples for the delay lines
	SimpleDelay inL_earlyDelay, inR_earlyDelay, chainL_delay, chainR_delay;

	double* leftMatrixLoopBuffer{};
	double* rightMatrixLoopBuffer{};
	double leftCombinedAAPF_delayTime_samples, rightCombinedAAPF_delayTime_samples, maxMatrixBuffer_samples;
	int  leftMatrixReadPtr{}, leftMatrixWritePtr{}, rightMatrixReadPtr{}, rightMatrixWritePtr{};

	// Define our inputs and outputs

	double leftEarlyDelayTap1{}, leftEarlyDelayTap2{}, leftEarlyDelayTap3{}, leftEarlyDelayTap4{}, leftEarlyDelayTap5{}, leftDelayOut{};
	double rightEarlyDelayTap1{}, rightEarlyDelayTap2{}, rightEarlyDelayTap3{}, rightEarlyDelayTap4{}, rightEarlyDelayTap5{}, rightDelayOut{};
	double leftEarlyAPFinput{}, rightEarlyAPFinput{}, leftEarlyAPFoutput{}, rightEarlyAPFoutput{};
	double leftMatrixInput{}, leftMatrixOutput{}, rightMatrixInput{}, rightMatrixOutput{};
	double chainL{}, chainR{};
	double leftChainTap1{}, leftChainTap2{}, leftChainTap3{}, leftChainTap4{}, leftChainTap5{}, leftChainTap6{};
	double rightChainTap1{}, rightChainTap2{}, rightChainTap3{}, rightChainTap4{}, rightChainTap5{}, rightChainTap6{};
	double leftChainOutput{}, rightChainOutput{};

	// --- END USER VARIABLES AND FUNCTIONS -------------------------------------- //

private:
	//  **--0x07FD--**

	// **--0x1A7F--**
	// --- end member variables

public:
	/** static description: bundle folder name

	\return bundle folder name as a const char*
	*/
	static const char* getPluginBundleName();

	/** static description: name

	\return name as a const char*
	*/
	static const char* getPluginName();

	/** static description: short name

	\return short name as a const char*
	*/
	static const char* getShortPluginName();

	/** static description: vendor name

	\return vendor name as a const char*
	*/
	static const char* getVendorName();

	/** static description: URL

	\return URL as a const char*
	*/
	static const char* getVendorURL();

	/** static description: email

	\return email address as a const char*
	*/
	static const char* getVendorEmail();

	/** static description: Cocoa View Factory Name

	\return Cocoa View Factory Name as a const char*
	*/
	static const char* getAUCocoaViewFactoryName();

	/** static description: plugin type

	\return type (FX or Synth)
	*/
	static pluginType getPluginType();

	/** static description: VST3 GUID

	\return VST3 GUID as a const char*
	*/
	static const char* getVSTFUID();

	/** static description: 4-char code

	\return 4-char code as int
	*/
	static int32_t getFourCharCode();

	/** initalizer */
	bool initPluginDescriptors();
};

#endif /* defined(__pluginCore_h__) */
