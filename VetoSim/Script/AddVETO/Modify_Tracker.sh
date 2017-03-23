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
sed -i "/h_ntracks->Fill(nNeut);/r ${SCRIPTPATH}/UseCuts.cxx" ./R3BNeutronTracker2D.cxx

# Now comment out the old part:
sed -i '300 a */' ./R3BNeutronTracker2D.cxx
sed -i '245 a /*' ./R3BNeutronTracker2D.cxx

# Now we also must add the new function to read the calibrator files:
sed -i '157 a */' ./R3BNeutronTracker2D.cxx
sed -i '121 a PatternForNewCalibrRead' ./R3BNeutronTracker2D.cxx
sed -i "/PatternForNewCalibrRead/r ${SCRIPTPATH}/ReadCalibrFiles.cxx" ./R3BNeutronTracker2D.cxx
sed -i '122 d' ./R3BNeutronTracker2D.cxx

# Do the final issues:
sed -i '91 a fveto_segDigi = (TClonesArray*) ioman->GetObject("veto_segDigi");' ./R3BNeutronTracker2D.cxx
sed -i '23 a #include "R3Bveto_segDigiPar.h"' ./R3BNeutronTracker2D.cxx
sed -i '24 a #include "R3Bveto_segDigi.h"' ./R3BNeutronTracker2D.cxx
sed -i '16 a #include "TH1D.h"' ./R3BNeutronTracker2D.cxx
sed -i '17 a #include "TH2D.h"' ./R3BNeutronTracker2D.cxx
sed -i '18 a #include "TFile.h"' ./R3BNeutronTracker2D.cxx

# Prevent index seg-faults:
sed -i '1022 a if (mothID<0) {mothID=0;}' ./R3BNeutronTracker2D.cxx
sed -i '1023 a if (mothID>=fLandMCTrack->GetEntries()) {mothID = fLandMCTrack->GetEntries()-1;}' ./R3BNeutronTracker2D.cxx
sed -i '1018 a if (trackID<0) {trackID = 0;}' ./R3BNeutronTracker2D.cxx
sed -i '1019 a if (trackID>=fLandMCTrack->GetEntries()) {trackID = fLandMCTrack->GetEntries()-1;}' ./R3BNeutronTracker2D.cxx
sed -i '1016 a if (index<0) {index = 0;}' ./R3BNeutronTracker2D.cxx
sed -i '1017 a if (index>=(fLandPoints->GetEntries())) {index = fLandPoints->GetEntries()-1;}' ./R3BNeutronTracker2D.cxx

# =======================================================================================================================
# Now we want 3 versions of the neutron tracker inside the land-directory. The original version,
# The version only with multiple cuts and a third version containing our beta-modifications as well:

mv ./R3BNeutronTracker2D.cxx ./R3BNeutronTracker2D_MultCuts.cxx
mv ./R3BNeutronTracker2D.h ./R3BNeutronTracker2D_MultCuts.h
cp ../Safe/R3BNeutronTracker2D.cxx ./R3BNeutronTracker2D_Original.cxx
cp ../Safe/R3BNeutronTracker2D.h ./R3BNeutronTracker2D_Original.h
cp ${SCRIPTPATH}/R3BNeutronTracker2D_Transf.cxx ./R3BNeutronTracker2D_Transf.cxx
cp ${SCRIPTPATH}/R3BNeutronTracker2D_Transf.h ./R3BNeutronTracker2D_Transf.h
cp ${SCRIPTPATH}/R3BNeutronTracker2D_FirstPlane.cxx ./R3BNeutronTracker2D_FirstPlane.cxx
cp ${SCRIPTPATH}/R3BNeutronTracker2D_FirstPlane.h ./R3BNeutronTracker2D_FirstPlane.h

# But it will be hard to try to also compile 3 different tasks, hence duplicate the
# one that we want and rename it as the original file:

cp ./R3BNeutronTracker2D_Transf.cxx ./R3BNeutronTracker2D.cxx
cp ./R3BNeutronTracker2D_Transf.h ./R3BNeutronTracker2D.h

# =======================================================================================================================
# Add our new reconstruction algorithms:
cd ${VMCWORKDIR}/neuland/

cp -R ${SCRIPTPATH}/Reconstruction ./Reconstruction

sed -i '9 a Reconstruction/R3BNeuLANDClustr.cxx' ./CMakeLists.txt
sed -i '10 a Reconstruction/R3BNeuLANDVertex.cxx' ./CMakeLists.txt
sed -i '11 a Reconstruction/R3BNeuLANDClusterFindr.cxx' ./CMakeLists.txt
sed -i '12 a Reconstruction/R3BNeuLANDTracker.cxx' ./CMakeLists.txt
sed -i '53 a ${R3BROOT_SOURCE_DIR}/neuland/Reconstruction' ./CMakeLists.txt
sed -i '54 a ${R3BROOT_SOURCE_DIR}/r3bdata/Nveto_segData' ./CMakeLists.txt

sed -i '15 a #pragma link C++ class R3BNeuLANDClustr+;' ./NeulandLinkDef.h
sed -i '16 a #pragma link C++ class R3BNeuLANDVertex+;' ./NeulandLinkDef.h
sed -i '17 a #pragma link C++ class R3BNeuLANDClusterFindr+;' ./NeulandLinkDef.h
sed -i '18 a #pragma link C++ class R3BNeuLANDTracker+;' ./NeulandLinkDef.h




























