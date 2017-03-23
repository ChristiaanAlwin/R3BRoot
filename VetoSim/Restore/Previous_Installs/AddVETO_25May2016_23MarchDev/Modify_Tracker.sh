#!/bin/bash

# This script makes some changes to the R3BNeutronTracker that we think that are useful.
# We do this after we generate the VETO so that these changes are not taken along.
# This is because we use the VETO to modify the tracker for NeuLAND.
# We are NOT in R3BRoot-VMCWORKDIR !!!

#############################################################################################
# Then we need to modify the Neutron Tracker.

# Make sure the originals are safe:
cd ${VMCWORKDIR}/land/
cp ./R3BNeutronTracker2D.cxx ../Safe/R3BNeutronTracker2D.cxx
cp ./R3BNeutronTracker2D.h ../Safe/R3BNeutronTracker2D.h
cp ./CMakeLists.txt ../Safe/CMakeLists_land.txt

# Begin with modifying the .h-file:
sed -i '46 a void ReadCalibrFiles();' ./R3BNeutronTracker2D.h
sed -i '48 d' ./R3BNeutronTracker2D.h
sed -i '54 a TClonesArray* fveto_segDigi;' ./R3BNeutronTracker2D.h
sed -i '116 a Int_t fMaxProtons;' ./R3BNeutronTracker2D.h
sed -i '138 a Double_t* fKappa;  // 1D array: size is max. number of protons.' ./R3BNeutronTracker2D.h
sed -i '140 d' ./R3BNeutronTracker2D.h
sed -i '139 a Double_t** fCuts; // 2D-array. First index is #protons, second index is #energy cuts.' ./R3BNeutronTracker2D.h
sed -i '141 d' ./R3BNeutronTracker2D.h
sed -i '140 a Int_t* fNumberOfCuts; // 1D array: size is max. number of protons.' ./R3BNeutronTracker2D.h
sed -i '141 a Double_t Ylow;' ./R3BNeutronTracker2D.h
sed -i '142 a Double_t Yhigh;' ./R3BNeutronTracker2D.h
sed -i '143 a Double_t fdClusters;' ./R3BNeutronTracker2D.h
sed -i '144 a Double_t fNeuLAND_TOF;' ./R3BNeutronTracker2D.h
sed -i '145 a Double_t fTOF;' ./R3BNeutronTracker2D.h
sed -i '146 a Int_t fvetoHits;' ./R3BNeutronTracker2D.h
sed -i '147 a Int_t fvetoTOFhits;' ./R3BNeutronTracker2D.h
sed -i '148 a Int_t fCurrentProtons;' ./R3BNeutronTracker2D.h

# Now modify the CMakeLists.txt-file:
sed -i '14 a ${R3BROOT_SOURCE_DIR}/veto_seg' ./CMakeLists.txt
sed -i '26 a ${R3BROOT_SOURCE_DIR}/r3bdata/veto_segData' ./CMakeLists.txt

# Then modify the .cxx-file. We use some auxillary files for that. To keep
# a clear picture about our lines, we go from bottom to top.

# That means adding the new cuts first. We will read from a file and then 
# add the content into the tracker:
PLACE=299 # That is where we must begin to add:
while read Line
do
  sed -i "${PLACE} a ${Line}" ./R3BNeutronTracker2D.cxx
  PLACE=`expr ${PLACE} + 1`
done < ${SCRIPTPATH}/UseCuts.cxx

# Now comment out the old part:
sed -i '298 a */' ./R3BNeutronTracker2D.cxx
sed -i '245 a /*' ./R3BNeutronTracker2D.cxx

# Now we also must add the new function to read the calibrator files:
PLACE=155 # That is where we must begin to add:
while read Line
do
  sed -i "${PLACE} a ${Line}" ./R3BNeutronTracker2D.cxx
  PLACE=`expr ${PLACE} + 1`
done < ${SCRIPTPATH}/ReadCalibrFiles.cxx

# Then we comment out the old function:
sed -i '155 a */' ./R3BNeutronTracker2D.cxx
sed -i '122 a /*' ./R3BNeutronTracker2D.cxx

# Do the final issues:
sed -i '91 a fveto_segDigi = (TClonesArray*) ioman->GetObject("veto_segDigi");' ./R3BNeutronTracker2D.cxx
sed -i '23 a #include "R3Bveto_segDigiPar.h"' ./R3BNeutronTracker2D.cxx
sed -i '24 a #include "R3Bveto_segDigi.h"' ./R3BNeutronTracker2D.cxx
sed -i '16 a #include "TH1D.h"' ./R3BNeutronTracker2D.cxx
sed -i '17 a #include "TH2D.h"' ./R3BNeutronTracker2D.cxx
sed -i '18 a #include "TFile.h"' ./R3BNeutronTracker2D.cxx







































