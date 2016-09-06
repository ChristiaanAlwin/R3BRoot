#!/bin/bash

# This script makes some changes to the VETO digitizer that we need.

##########################################################################################

# At first we need to make sure that the VETO digitizer obtains the correct globals:
cd ${VMCWORKDIR}/Nveto_seg/
sed -i '92 a TFile* inputs = new TFile("../../Data/Inputs/Inputs.root","read");' ./R3BNveto_segDigitizer.cxx
sed -i '93 a TH1I* Integers = (TH1I*) inputs->Get("Integers");' ./R3BNveto_segDigitizer.cxx
sed -i '94 a TH1D* Doubles = (TH1D*) inputs->Get("Doubles");' ./R3BNveto_segDigitizer.cxx

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
sed -i '67 a static Double_t fTimeRes;' ./digitizing/Digitizin_segEngine.h
sed -i '68 a static Double_t fThresh;' ./digitizing/Digitizin_segEngine.h
sed -i '69 a static Double_t fAttenuation;' ./digitizing/Digitizin_segEngine.h
sed -i '70 a static Double_t fcMedium;' ./digitizing/Digitizin_segEngine.h
sed -i '63 d' ./digitizing/Digitizin_segEngine.h
sed -i '62 d' ./digitizing/Digitizin_segEngine.h
sed -i '59 d' ./digitizing/Digitizin_segEngine.h
sed -i '58 d' ./digitizing/Digitizin_segEngine.h

# Modify the Init-function to take the values from our own Inputs.root-file:
sed -i '110 a // Replace the detector characteristics with the choices from our Inputs.root-file:' ./R3BNveto_segDigitizer.cxx
sed -i '100 a Nveto_seg::Digitizin_segEngine::fTimeRes = 0.001*Doubles->GetBinContent(310); // NOTE: conversion from [ps] to [ns]!!!' ./R3BNveto_segDigitizer.cxx
sed -i '101 a Nveto_seg::Digitizin_segEngine::fThresh = 0.001*Doubles->GetBinContent(302); // NOTE: conversion from [keV] to [MeV]!!!' ./R3BNveto_segDigitizer.cxx
sed -i '102 a Nveto_seg::Digitizin_segEngine::fAttenuation = Doubles->GetBinContent(335);' ./R3BNveto_segDigitizer.cxx
sed -i '103 a Nveto_seg::Digitizin_segEngine::fcMedium = Doubles->GetBinContent(334);' ./R3BNveto_segDigitizer.cxx
sed -i '104 a // fixed!' ./R3BNveto_segDigitizer.cxx

##########################################################################################
# We also like that the ordinary NeuLAND digitizer responds to our time resolution and energy threshold:
cd ${VMCWORKDIR}/neuland/

sed -i '92 a TFile* inputs = new TFile("../../Data/Inputs/Inputs.root","read");' ./R3BNeulandDigitizer.cxx
sed -i '93 a TH1I* Integers = (TH1I*) inputs->Get("Integers");' ./R3BNeulandDigitizer.cxx
sed -i '94 a TH1D* Doubles = (TH1D*) inputs->Get("Doubles");' ./R3BNeulandDigitizer.cxx

sed -i '32 a Double_t DigitizingEngine::fTimeRes = 0.15; // ns' ./digitizing/DigitizingEngine.cxx
sed -i '34 d' ./digitizing/DigitizingEngine.cxx
sed -i '19 a Double_t DigitizingEngine::fThresh = 0.16;' ./digitizing/DigitizingEngine.cxx
sed -i '21 d' ./digitizing/DigitizingEngine.cxx

sed -i '67 a static Double_t fTimeRes;' ./digitizing/DigitizingEngine.h
sed -i '68 a static Double_t fThresh;' ./digitizing/DigitizingEngine.h
sed -i '63 d' ./digitizing/DigitizingEngine.h
sed -i '62 d' ./digitizing/DigitizingEngine.h

sed -i '100 a // Replace the detector characteristics with the choices from our Inputs.root-file:' ./R3BNeulandDigitizer.cxx
sed -i '101 a Neuland::DigitizingEngine::fTimeRes = 0.001*Doubles->GetBinContent(309); // NOTE: conversion from [ps] to [ns]!!!' ./R3BNeulandDigitizer.cxx
sed -i '102 a Neuland::DigitizingEngine::fThresh = 0.001*Doubles->GetBinContent(301); // NOTE: conversion from [keV] to [MeV]!!!' ./R3BNeulandDigitizer.cxx
sed -i '103 a // fixed!' ./R3BNeulandDigitizer.cxx

##########################################################################################
# Add the older digitizers for simulations with the old land & veto_seg classes:

cd ${VMCWORKDIR}/neuland/
cp ${SCRIPTPATH}/R3BNeulandDigitizrOld.h ./R3BNeulandDigitizrOld.h
cp ${SCRIPTPATH}/R3BNeulandDigitizrOld.cxx ./R3BNeulandDigitizrOld.cxx

sed -i '9 a R3BNeulandDigitizrOld.cxx' ./CMakeLists.txt
sed -i '48 a ${R3BROOT_SOURCE_DIR}/r3bdata/landData' ./CMakeLists.txt
sed -i '48 a ${R3BROOT_SOURCE_DIR}/land' ./CMakeLists.txt
sed -i '15 a #pragma link C++ class R3BNeulandDigitizrOld+;' ./NeulandLinkDef.h

cd ${VMCWORKDIR}/Nveto_seg/
cp ${SCRIPTPATH}/R3BNveto_segDigitizrOld.h ./R3BNveto_segDigitizrOld.h
cp ${SCRIPTPATH}/R3BNveto_segDigitizrOld.cxx ./R3BNveto_segDigitizrOld.cxx

sed -i '9 a R3BNveto_segDigitizrOld.cxx' ./CMakeLists.txt
sed -i '48 a ${R3BROOT_SOURCE_DIR}/r3bdata/veto_segData' ./CMakeLists.txt
sed -i '48 a ${R3BROOT_SOURCE_DIR}/veto_seg' ./CMakeLists.txt
sed -i '15 a #pragma link C++ class R3BNveto_segDigitizrOld+;' ./Nveto_segLinkDef.h
