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
sed -i '100 a fh_etot = new TH1F("h_etot","Total light",NbinsE,0.0,fEhistBoundary_Energy);' ./R3BNeutronCalibr2D.cxx
sed -i '101 a fh_ncl_etot = new TH2F("h_ncl_etot","Number of clusters vs. total light",NbinsE,0.0,fEhistBoundary_Energy,NbinsC,-0.5,fEhistBoundary_Clusters-0.5);' ./R3BNeutronCalibr2D.cxx
sed -i '102 a }' ./R3BNeutronCalibr2D.cxx

# That's it!

#############################################################################################
# Then we need to modify the Neutron Tracker. We will do this in a separate script, since
# we need this to happen after the VETO classes are created, not before.s
