#!/bin/bash

# This script makes some changes to R3BRoot that we think that are useful.
# We do this before we generate the VETO so that these changes are taken along.
# We are NOT in R3BRoot-VMCWORKDIR !!!

###########################################################################################
# We begin with modifying the histogram bounds for the NeutronCalibr2d-class.

# Make sure the originals are safe:
cd ${VMCWORKDIR}/land/
cp ./R3BNeutronCalibr2D.cxx ../Safe/R3BNeutronCalibr2D.cxx
cp ./R3BNeutronCalibr2D.h ../Safe/R3BNeutronCalibr2D.h

# Now add the histogram boundaries to the header-file:
sed -i '37 a Double_t fEhistBoundary_Clusters = 1500.0;' ./R3BNeutronCalibr2D.h
sed -i '38 a Double_t fEhistBoundary_Energy = 150.0;' ./R3BNeutronCalibr2D.h
# NOTE that we add a default value identical to the usual values.

# Then add SetFunctions and Getfunctions to the header-file:
sed -i '34 a inline void SetEnergyBoundary(Double_t Ebound) { fEhistBoundary_Energy = Ebound; }' ./R3BNeutronCalibr2D.h
sed -i '35 a // blank' ./R3BNeutronCalibr2D.h
sed -i '36 a inline void SetClusterBoundary(Double_t Cbound) { fEhistBoundary_Clusters = Cbound; }' ./R3BNeutronCalibr2D.h
sed -i '37 a // blank' ./R3BNeutronCalibr2D.h

# Now we must replace the CreateHistograms-function in the .cxx-file with a file of our choice:
sed -i '81 a /*' ./R3BNeutronCalibr2D.cxx
sed -i '93 a */' ./R3BNeutronCalibr2D.cxx

# Add the new function:
sed -i '96 a void R3BNeutronCalibr2D::CreateHistograms()' ./R3BNeutronCalibr2D.cxx
sed -i '97 a {' ./R3BNeutronCalibr2D.cxx
sed -i '98 a Int_t NbinsE = (Int_t) fEhistBoundary_Energy;' ./R3BNeutronCalibr2D.cxx
sed -i '99 a Int_t NbinsC = (Int_t) fEhistBoundary_Clusters;' ./R3BNeutronCalibr2D.cxx
sed -i '100 a if (NbinsE>10000) {NbinsE = 10000;}' ./R3BNeutronCalibr2D.cxx
sed -i '101 a if (NbinsC>10000) {NbinsC = 10000;}' ./R3BNeutronCalibr2D.cxx
sed -i '102 a fh_etot = new TH1F("h_etot","Total light",NbinsE,0.0,fEhistBoundary_Energy);' ./R3BNeutronCalibr2D.cxx
sed -i '103 a fh_ncl_etot = new TH2F("h_ncl_etot","Number of clusters vs. total light",NbinsE,0.0,fEhistBoundary_Energy,NbinsC,-0.5,fEhistBoundary_Clusters-0.5);' ./R3BNeutronCalibr2D.cxx
sed -i '104 a }' ./R3BNeutronCalibr2D.cxx

# That's it!

#############################################################################################
# Then we need to modify the Neutron Tracker. We will do this in a separate script, since
# we need this to happen after the VETO classes are created, not before.s

#############################################################################################
# We will also modify the NeuLAND digitizer to correctly process the global orientation of the detector:

# we begin by adding the required code to the Exec()-function inside the R3BneulandDigitizer.cxx-file:
cd ${VMCWORKDIR}/neuland/
cp ./R3BNeulandDigitizer.cxx ../Safe/R3BNeulandDigitizer.cxx
cp ./digitizing/DigitizingEngine.cxx ../Safe/DigitizingEngine.cxx
cp ./R3BNeulandDigitizer.h ../Safe/R3BNeulandDigitizer.h
cp ./digitizing/DigitizingEngine.h ../Safe/DigitizingEngine.h

sed -i "/hMultTwo->Fill(multTwo);/r ${SCRIPTPATH}/DigitizerGlobals.cxx" ./R3BNeulandDigitizer.cxx
sed -i "/paddle.leftPMT.GetTDC();/r ${SCRIPTPATH}/DigitizerTransform.cxx" ./R3BNeulandDigitizer.cxx

for ((k = 1; k<6; ++k))
do
sed -i -e "s:landPoint->GetXIn():PosIn.X():" ./R3BNeulandDigitizer.cxx
sed -i -e "s:landPoint->GetYIn():PosIn.Y():" ./R3BNeulandDigitizer.cxx
sed -i -e "s:landPoint->GetZIn():PosIn.Z():" ./R3BNeulandDigitizer.cxx
sed -i -e "s:landPoint->GetXOut():PosOut.X():" ./R3BNeulandDigitizer.cxx
sed -i -e "s:landPoint->GetYOut():PosOut.Y():" ./R3BNeulandDigitizer.cxx
sed -i -e "s:landPoint->GetZOut():PosOut.Z():" ./R3BNeulandDigitizer.cxx
done

# Give this command AFTER the substitution commands, otherwise this piece of code is affected too.
sed -i "/*)fLandPoints->At(l);/r ${SCRIPTPATH}/MonteCarloTransform.cxx" ./R3BNeulandDigitizer.cxx

# then we also need to add the required headers to the digitizer-file:
sed -i '11 a #include "TVector3.h"' ./R3BNeulandDigitizer.cxx
sed -i '12 a #include "TRandom3.h"' ./R3BNeulandDigitizer.cxx

# Add the global members to the header-file:
sed -i '44 a Int_t fneuland_PaddlesPerSinglePlane;   // NOTE: Int_t' ./R3BNeulandDigitizer.h
sed -i '45 a Int_t fneuland_NsinglePlanes;           // NOTE: Int_t' ./R3BNeulandDigitizer.h
sed -i '46 a Double_t fneuland_TotalPaddleLength;    // NOTE: Double_t cm!' ./R3BNeulandDigitizer.h
sed -i '47 a Double_t fneuland_TotalPaddleWidth;     // NOTE: Double_t cm!' ./R3BNeulandDigitizer.h
sed -i '48 a Double_t fneuland_TotalPaddleThickness; // NOTE: Double_t cm!' ./R3BNeulandDigitizer.h
sed -i '49 a Double_t fneuland_Global_Xpos;          // NOTE: Double_t cm!' ./R3BNeulandDigitizer.h
sed -i '50 a Double_t fneuland_Global_Ypos;          // NOTE: Double_t cm!' ./R3BNeulandDigitizer.h
sed -i '51 a Double_t fneuland_Global_Zpos;          // NOTE: Double_t cm!' ./R3BNeulandDigitizer.h
sed -i '52 a Double_t fneuland_Global_XRotation;     // NOTE: Double_t radians!' ./R3BNeulandDigitizer.h
sed -i '53 a Double_t fneuland_Global_YRotation;     // NOTE: Double_t radians!' ./R3BNeulandDigitizer.h
sed -i '54 a Double_t fneuland_Global_ZRotation;     // NOTE: Double_t radians!' ./R3BNeulandDigitizer.h
sed -i '55 a Double_t fneuland_UseUnformDist;        // NOTE: Bool_t' ./R3BNeulandDigitizer.h
sed -i '56 a TRandom3* fneuland_RandGen;             // NOTE: TRandom3-pointer' ./R3BNeulandDigitizer.h

# Now initialize the TRandom3-pointer:
sed -i '69 a fneuland_RandGen = new TRandom3();' ./R3BNeulandDigitizer.cxx

# NOTE: In order for this system to work one needs to specify also the other fneuland-globals.
# NOTE: This is done after the generation of the veto)_seg-classes, since that code is different
# NOTE: for the VETO and NeuLAND, while this code is not.
#############################################################################################
# Modifying the clusterfinder:
cd ${VMCWORKDIR}/land/

cp ./R3BNeuLandClusterFinder.h ../Safe/R3BNeuLandClusterFinder.h
cp ./R3BNeuLandClusterFinder.cxx ../Safe/R3BNeuLandClusterFinder.cxx
cp ./R3BNeuLandCluster.h ../Safe/R3BNeuLandCluster.h
cp ./R3BNeuLandCluster.cxx ../Safe/R3BNeuLandCluster.cxx

cp ${SCRIPTPATH}/R3BNeuLandClusterFinder.h ./R3BNeuLandClusterFinder.h
cp ${SCRIPTPATH}/R3BNeuLandClusterFinder.cxx ./R3BNeuLandClusterFinder.cxx
cp ${SCRIPTPATH}/R3BNeuLandCluster.h ./R3BNeuLandCluster.h
cp ${SCRIPTPATH}/R3BNeuLandCluster.cxx ./R3BNeuLandCluster.cxx



