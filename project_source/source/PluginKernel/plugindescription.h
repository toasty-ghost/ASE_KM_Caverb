// --- CMAKE generated variables for your plugin

#include "pluginstructures.h"

#ifndef _plugindescription_h
#define _plugindescription_h

#define QUOTE(name) #name
#define STR(macro) QUOTE(macro)
#define AU_COCOA_VIEWFACTORY_STRING STR(AU_COCOA_VIEWFACTORY_NAME)
#define AU_COCOA_VIEW_STRING STR(AU_COCOA_VIEW_NAME)

// --- AU Plugin Cocoa View Names (flat namespace) 
#define AU_COCOA_VIEWFACTORY_NAME AUCocoaViewFactory_2C17974BB1BB3AD8895362FA2160DCF2
#define AU_COCOA_VIEW_NAME AUCocoaView_2C17974BB1BB3AD8895362FA2160DCF2

// --- BUNDLE IDs (MacOS Only) 
const char* kAAXBundleID = "developer.aax.asekmkaboosefreshstart.bundleID";
const char* kAUBundleID = "developer.au.asekmkaboosefreshstart.bundleID";
const char* kVST3BundleID = "developer.vst3.asekmkaboosefreshstart.bundleID";

// --- Plugin Names 
const char* kPluginName = "Caverb";
const char* kShortPluginName = "Caverb";
const char* kAUBundleName = "ase_km_kaboose_freshstart";

// --- Plugin Type 
const pluginType kPluginType = pluginType::kFXPlugin;

// --- VST3 UUID 
const char* kVSTFUID = "{2c17974b-b1bb-3ad8-8953-62fa2160dcf2}";

// --- 4-char codes 
const int32_t kFourCharCode = 'TGAp';
const int32_t kAAXProductID = 'TGAp';
const int32_t kManufacturerID = 'TGAp';

// --- Vendor information 
const char* kVendorName = "TG Audio Processing";
const char* kVendorURL = "www.toastyghost.co.uk";
const char* kVendorEmail = "toastyghost@gmail.com";

// --- Plugin Options 
const bool kWantSidechain = false;
const uint32_t kLatencyInSamples = 0;
const double kTailTimeMsec = 0.000000;
const bool kVSTInfiniteTail = false;
const bool kVSTSAA = false;
const uint32_t kVST3SAAGranularity = 1;
const uint32_t kAAXCategory = 8;

#endif
