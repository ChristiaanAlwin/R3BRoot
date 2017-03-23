#!/bin/bash

# This script makes some changes to the VETO digitizer that we need.

##########################################################################################

# At first we need to make sure that the VETO digitizer obtains the correct globals:
cd ${VMCWORKDIR}/Nveto_seg/
sed -i '92 a TFile* inputs = new TFile("../../Data/Inputs/Inputs.root","read");' ./R3BNveto_segDigitizer.cxx
sed -i '93 a TH1I* Integers = (TH1I*) inputs->Get("Integers");' ./R3BNveto_segDigitizer.cxx
sed -i '94 a TH1D* Doubles = (TH1D*) inputs->Get("Doubles");' ./R3BNveto_segDigitizer.cxx
sed -i '95 a fNveto_seg_PaddlesPerSinglePlane = Integers->GetBinContent(4);' ./R3BNveto_segDigitizer.cxx
sed -i '96 a fNveto_seg_NsinglePlanes = Integers->GetBinContent(3);' ./R3BNveto_segDigitizer.cxx
sed -i '97 a fNveto_seg_TotalPaddleLength = Doubles->GetBinContent(127);' ./R3BNveto_segDigitizer.cxx
sed -i '98 a fNveto_seg_TotalPaddleWidth = Doubles->GetBinContent(128);' ./R3BNveto_segDigitizer.cxx
sed -i '99 a fNveto_seg_TotalPaddleThickness = Doubles->GetBinContent(129);' ./R3BNveto_segDigitizer.cxx
sed -i '100 a fNveto_seg_Global_Xpos = Doubles->GetBinContent(121);' ./R3BNveto_segDigitizer.cxx
sed -i '101 a fNveto_seg_Global_Ypos = Doubles->GetBinContent(122);' ./R3BNveto_segDigitizer.cxx
sed -i '102 a fNveto_seg_Global_Zpos = Doubles->GetBinContent(123);' ./R3BNveto_segDigitizer.cxx
sed -i '103 a fNveto_seg_Global_XRotation = (TMath::Pi()/180.0)*Doubles->GetBinContent(124);' ./R3BNveto_segDigitizer.cxx
sed -i '104 a fNveto_seg_Global_YRotation = (TMath::Pi()/180.0)*Doubles->GetBinContent(125);' ./R3BNveto_segDigitizer.cxx
sed -i '105 a fNveto_seg_Global_ZRotation = (TMath::Pi()/180.0)*Doubles->GetBinContent(126);' ./R3BNveto_segDigitizer.cxx
sed -i '106 a fNveto_seg_UseUnformDist = kFALSE;' ./R3BNveto_segDigitizer.cxx
sed -i '107 a npaddles = fNveto_seg_PaddlesPerSinglePlane*fNveto_seg_NsinglePlanes + 1;' ./R3BNveto_segDigitizer.cxx
sed -i '108 a nplanes = fNveto_seg_NsinglePlanes;' ./R3BNveto_segDigitizer.cxx
sed -i '109 a if ((Integers->GetBinContent(312))==2) {fNveto_seg_UseUnformDist = kTRUE;}' ./R3BNveto_segDigitizer.cxx

# Remove the 'const' from the namescpace with global variables in Digitizer Engine.
# NOTE: We use the VETO-version! Hence use Digitizin_segEngine.cxx (namespace is also called like that!)
sed -i '32 a Double_t Digitizin_segEngine::fTimeRes = 0.15; // ns' ./digitizing/Digitizin_segEngine.cxx
sed -i '34 d' ./digitizing/Digitizin_segEngine.cxx
sed -i '19 a Double_t Digitizin_segEngine::fThresh = 0.16;' ./digitizing/Digitizin_segEngine.cxx
sed -i '21 d' ./digitizing/Digitizin_segEngine.cxx
sed -i '14 a Double_t Digitizin_segEngine::fAttenuation = 0.008; // [1/cm]' ./digitizing/Digitizin_segEngine.cxx
sed -i '16 d' ./digitizing/Digitizin_segEngine.cxx
sed -i '10 a Double_t Digitizin_segEngine::fcMedium = 14.; // speed of light in material in [cm/ns]' ./digitizing/Digitizin_segEngine.cxx
sed -i '12 d' ./digitizing/Digitizin_segEngine.cxx
# Now that the const-values are removed, we can alter them!

# Also remove the const-values from the header-file (and move variables to public area):
sed -i '63 d' ./digitizing/Digitizin_segEngine.h
sed -i '62 d' ./digitizing/Digitizin_segEngine.h
sed -i '59 d' ./digitizing/Digitizin_segEngine.h
sed -i '58 d' ./digitizing/Digitizin_segEngine.h
sed -i '63 a static Double_t fTimeRes;' ./digitizing/Digitizin_segEngine.h
sed -i '63 a static Double_t fThresh;' ./digitizing/Digitizin_segEngine.h
sed -i '63 a static Double_t fAttenuation;' ./digitizing/Digitizin_segEngine.h
sed -i '63 a static Double_t fcMedium;' ./digitizing/Digitizin_segEngine.h

# Modify the Init-function to take the values from our own Inputs.root-file:
sed -i '110 a // Replace the detector characteristics with the choices from our Inputs.root-file:' ./R3BNveto_segDigitizer.cxx
sed -i '111 a Nveto_seg::Digitizin_segEngine::fTimeRes = 0.001*Doubles->GetBinContent(310); // NOTE: conversion from [ps] to [ns]!!!' ./R3BNveto_segDigitizer.cxx
sed -i '112 a Nveto_seg::Digitizin_segEngine::fThresh = 0.001*Doubles->GetBinContent(302); // NOTE: conversion from [keV] to [MeV]!!!' ./R3BNveto_segDigitizer.cxx
sed -i '113 a Nveto_seg::Digitizin_segEngine::fAttenuation = Doubles->GetBinContent(335);' ./R3BNveto_segDigitizer.cxx
sed -i '114 a Nveto_seg::Digitizin_segEngine::fcMedium = Doubles->GetBinContent(334);' ./R3BNveto_segDigitizer.cxx
sed -i '115 a // fixed!' ./R3BNveto_segDigitizer.cxx

##########################################################################################
# We also like that the ordinary NeuLAND digitizer responds to our time resolution and energy threshold:
cd ${VMCWORKDIR}/neuland/

sed -i '32 a Double_t DigitizingEngine::fTimeRes = 0.15; // ns' ./digitizing/DigitizingEngine.cxx
sed -i '34 d' ./digitizing/DigitizingEngine.cxx
sed -i '19 a Double_t DigitizingEngine::fThresh = 0.16;' ./digitizing/DigitizingEngine.cxx
sed -i '21 d' ./digitizing/DigitizingEngine.cxx

sed -i '63 d' ./digitizing/DigitizingEngine.h
sed -i '62 d' ./digitizing/DigitizingEngine.h
sed -i '65 a static Double_t fTimeRes;' ./digitizing/DigitizingEngine.h
sed -i '66 a static Double_t fThresh;' ./digitizing/DigitizingEngine.h

sed -i '92 a TFile* inputs = new TFile("../../Data/Inputs/Inputs.root","read");' ./R3BNeulandDigitizer.cxx
sed -i '93 a TH1I* Integers = (TH1I*) inputs->Get("Integers");' ./R3BNeulandDigitizer.cxx
sed -i '94 a TH1D* Doubles = (TH1D*) inputs->Get("Doubles");' ./R3BNeulandDigitizer.cxx
sed -i '95 a fneuland_PaddlesPerSinglePlane = 50;' ./R3BNeulandDigitizer.cxx
sed -i '96 a fneuland_NsinglePlanes = Integers->GetBinContent(2);' ./R3BNeulandDigitizer.cxx
sed -i '97 a fneuland_TotalPaddleLength = 250.0;' ./R3BNeulandDigitizer.cxx
sed -i '98 a fneuland_TotalPaddleWidth = 5.0;' ./R3BNeulandDigitizer.cxx
sed -i '99 a fneuland_TotalPaddleThickness = 5.0;' ./R3BNeulandDigitizer.cxx
sed -i '100 a fneuland_Global_Xpos = Doubles->GetBinContent(111);' ./R3BNeulandDigitizer.cxx
sed -i '101 a fneuland_Global_Ypos = Doubles->GetBinContent(112);' ./R3BNeulandDigitizer.cxx
sed -i '102 a fneuland_Global_Zpos = Doubles->GetBinContent(113);' ./R3BNeulandDigitizer.cxx
sed -i '103 a fneuland_Global_XRotation = (TMath::Pi()/180.0)*Doubles->GetBinContent(114);' ./R3BNeulandDigitizer.cxx
sed -i '104 a fneuland_Global_YRotation = (TMath::Pi()/180.0)*Doubles->GetBinContent(115);' ./R3BNeulandDigitizer.cxx
sed -i '105 a fneuland_Global_ZRotation = (TMath::Pi()/180.0)*Doubles->GetBinContent(116);' ./R3BNeulandDigitizer.cxx
sed -i '106 a fneuland_UseUnformDist = kFALSE;' ./R3BNeulandDigitizer.cxx
sed -i '107 a npaddles = fneuland_PaddlesPerSinglePlane*fneuland_NsinglePlanes + 1;' ./R3BNeulandDigitizer.cxx
sed -i '108 a nplanes = fneuland_NsinglePlanes;' ./R3BNeulandDigitizer.cxx
sed -i '109 a if ((Integers->GetBinContent(312))==2) {fneuland_UseUnformDist = kTRUE;}' ./R3BNeulandDigitizer.cxx

sed -i '110 a // Replace the detector characteristics with the choices from our Inputs.root-file:' ./R3BNeulandDigitizer.cxx
sed -i '111 a Neuland::DigitizingEngine::fTimeRes = 0.001*Doubles->GetBinContent(309); // NOTE: conversion from [ps] to [ns]!!!' ./R3BNeulandDigitizer.cxx
sed -i '112 a Neuland::DigitizingEngine::fThresh = 0.001*Doubles->GetBinContent(301); // NOTE: conversion from [keV] to [MeV]!!!' ./R3BNeulandDigitizer.cxx
sed -i '113 a // fixed!' ./R3BNeulandDigitizer.cxx
