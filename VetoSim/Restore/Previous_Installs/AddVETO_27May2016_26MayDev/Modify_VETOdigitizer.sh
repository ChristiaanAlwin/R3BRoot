#!/bin/bash

# This script makes some changes to the VETO digitizer that we need.

##########################################################################################

# At first we need to make sure that the VETO digitizer obtains the correct number of paddles:
cd ${VMCWORKDIR}/Nveto_seg/
sed -i '89 a TFile* inputs = new TFile("../../Data/Inputs/Inputs.root","read");' ./R3BNveto_segDigitizer.cxx
sed -i '90 a TH1I* Integers = (TH1I*) inputs->Get("Integers");' ./R3BNveto_segDigitizer.cxx
sed -i '91 a TH1D* Doubles = (TH1D*) inputs->Get("Doubles");' ./R3BNveto_segDigitizer.cxx
sed -i '92 a npaddles = (Integers->GetBinContent(3))*(Integers->GetBinContent(4)) + 1;' ./R3BNveto_segDigitizer.cxx
sed -i '93 a nplanes = Integers->GetBinContent(3) + 1;' ./R3BNveto_segDigitizer.cxx

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
cp ./R3BNeulandDigitizer.cxx ../Safe/R3BNeulandDigitizer.cxx
cp ./digitizing/DigitizingEngine.cxx ../Safe/DigitizingEngine.cxx
cp ./R3BNeulandDigitizer.h ../Safe/R3BNeulandDigitizer.h
cp ./digitizing/DigitizingEngine.h ../Safe/DigitizingEngine.h

sed -i '32 a Double_t DigitizingEngine::fTimeRes = 0.15; // ns' ./digitizing/DigitizingEngine.cxx
sed -i '34 d' ./digitizing/DigitizingEngine.cxx
sed -i '19 a Double_t DigitizingEngine::fThresh = 0.16;' ./digitizing/DigitizingEngine.cxx
sed -i '21 d' ./digitizing/DigitizingEngine.cxx

sed -i '63 d' ./digitizing/DigitizingEngine.h
sed -i '62 d' ./digitizing/DigitizingEngine.h
sed -i '65 a static Double_t fTimeRes;' ./digitizing/DigitizingEngine.h
sed -i '66 a static Double_t fThresh;' ./digitizing/DigitizingEngine.h

sed -i '89 a TFile* inputs = new TFile("../../Data/Inputs/Inputs.root","read");' ./R3BNeulandDigitizer.cxx
sed -i '90 a TH1I* Integers = (TH1I*) inputs->Get("Integers");' ./R3BNeulandDigitizer.cxx
sed -i '91 a TH1D* Doubles = (TH1D*) inputs->Get("Doubles");' ./R3BNeulandDigitizer.cxx

sed -i '108 a // Replace the detector characteristics with the choices from our Inputs.root-file:' ./R3BNeulandDigitizer.cxx
sed -i '109 a Neuland::DigitizingEngine::fTimeRes = 0.001*Doubles->GetBinContent(309); // NOTE: conversion from [ps] to [ns]!!!' ./R3BNeulandDigitizer.cxx
sed -i '110 a Neuland::DigitizingEngine::fThresh = 0.001*Doubles->GetBinContent(301); // NOTE: conversion from [keV] to [MeV]!!!' ./R3BNeulandDigitizer.cxx
sed -i '111 a // fixed!' ./R3BNeulandDigitizer.cxx
