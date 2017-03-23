#!/bin/bash

# This script adds the VETO to R3Broot. It uses the other scripts that we have to accomplish this.

echo ''
echo ''
echo 'This script adds the segmented VETO to R3Broot. It does this by duplicating all source code'
echo 'corresponding to NeuLAND. It then renames all those files and rewrites all class names and'
echo 'function names inside those files so that R3Broot will regard this duplicate as a seperate'
echo 'detector. Finally, the new files are linked to the MasterFiles that control the compiler.'
echo 'The new detector is also added to the steering macros and new geometry files are generated.'
echo 'Then R3Broot is re-compiled to take all changes into effect. NOTE: R3Broot should already'
echo 'be installed succesfully before running this script!'
echo ''
echo 'This script is tested on the R3Broot dev-branch of 21 March 2016.'
echo ''
echo '#################################################################################################'
echo ''
echo ''

###################################################################################################
# Define the paths we need:

export SCRIPTPATH=`pwd`
# VMCWORKDIR=R3Broot path
export COMPPATH=${FAIRLIBDIR}
# and cd ../ afterwards!

####################################################################################################
# Deleting previous versions of the VETO in R3Broot:
echo '############################################### Deleting any previous versions of the VETO...'
cd ${SCRIPTPATH}
./DeleteVETO.sh

# Generate the safe directory:
cd ${VMCWORKDIR}
mkdir Safe

####################################################################################################
# Make a few modifications to R3BRoot that are useful for us:
echo '############################################### Perform initial modifications...'
cd ${SCRIPTPATH}
./Modify.sh
cd ${SCRIPTPATH}

####################################################################################################
# Duplicate the files:

# Duplicate the land-class:
echo '############################################### Creating veto_seg class...'
cd ${VMCWORKDIR}
mkdir veto_seg
cp -R ./land/* ./veto_seg/
export RE_PATH=${VMCWORKDIR}/veto_seg/
cd ${SCRIPTPATH}
./ReName.sh
cd ${VMCWORKDIR}/veto_seg/
mv ./CMakeLists_re.txt ./CMakeLists.txt
cd ${SCRIPTPATH}
./ReWrite.sh

# Duplicate the neuland-class:
echo '############################################### Creating Nveto_seg class...'
cd ${VMCWORKDIR}
mkdir Nveto_seg
cp -R ./neuland/* ./Nveto_seg/
export RE_PATH=${VMCWORKDIR}/Nveto_seg/
cd ${SCRIPTPATH}
./ReName.sh
cd ${VMCWORKDIR}/Nveto_seg/
mv ./CMakeLists_re.txt ./CMakeLists.txt
mv ./readme_re.md ./readme.md
cd ${SCRIPTPATH}
./ReWrite.sh

# Duplicate the landData-class:
echo '############################################### Creating veto_segData class...'
cd ${VMCWORKDIR}/r3bdata/
mkdir veto_segData
cp -R ./landData/* ./veto_segData/
export RE_PATH=${VMCWORKDIR}/r3bdata/veto_segData/
cd ${SCRIPTPATH}
./ReName.sh
cd ${SCRIPTPATH}
./ReWrite.sh

# Duplicate the neulandData-class:
echo '############################################### Creating Nveto_segData class...'
cd ${VMCWORKDIR}/r3bdata/
mkdir Nveto_segData
cp -R ./neulandData/* ./Nveto_segData/
export RE_PATH=${VMCWORKDIR}/r3bdata/Nveto_segData/
cd ${SCRIPTPATH}
./ReName.sh
cd ${SCRIPTPATH}
./ReWrite.sh

# Duplicate the landdb-class:
echo '############################################### Creating veto_segdb class...'
cd ${VMCWORKDIR}/r3bdb/
mkdir veto_segdb
cp -R ./landdb/* ./veto_segdb/
export RE_PATH=${VMCWORKDIR}/r3bdb/veto_segdb/
cd ${SCRIPTPATH}
./ReName.sh
cd ${VMCWORKDIR}/r3bdb/veto_segdb/
mv ./CMakeLists_re.txt ./CMakeLists.txt
cd ${SCRIPTPATH}
./ReWrite.sh

# Duplicate the neuland digitizer macros:
echo '############################################### Creating veto_seg Macros...'
cd ${VMCWORKDIR}/macros/r3b/
mkdir Nveto_seg
cp -R ./neuland/* ./Nveto_seg/
export RE_PATH=${VMCWORKDIR}/macros/r3b/Nveto_seg/
cd ${SCRIPTPATH}
./ReName.sh
cd ${SCRIPTPATH}
./ReWrite.sh

######################################################################################################
# Now add everything to the masterfiles etc:

echo '############################################### Adding VETO to Masterfiles...'
cd ${SCRIPTPATH}
./ReWriteMasters.sh

echo '############################################### Adding VETO to Steering macros...'
cd ${SCRIPTPATH}
./ReWriteMacros.sh

echo '############################################### Creating VETO geometry...'
cd ${SCRIPTPATH}
./ReWriteGeo.sh

######################################################################################################
# Now modify the VETO digitizer to process our own non-square VETO:
cd ${VMCWORKDIR}/Nveto_seg/
sed -i '89 a TFile* inputs = new TFile("../../Data/Inputs/Inputs.root","read");' ./R3BNveto_segDigitizer.cxx
sed -i '90 a TH1I* Integers = (TH1I*) inputs->Get("Integers");' ./R3BNveto_segDigitizer.cxx
sed -i '91 a npaddles = (Integers->GetBinContent(3))*(Integers->GetBinContent(4)) + 1;' ./R3BNveto_segDigitizer.cxx
sed -i '92 a nplanes = Integers->GetBinContent(3) + 1;' ./R3BNveto_segDigitizer.cxx

####################################################################################################
# Modify the Neutron Tracker to use different calibrations:
echo '############################################### Perform R3BRoot NeutronTracker modifications...'

# NOTE: Unfortunately this Modify_Tracker-script misses some parts of the
# NOTE: required modifications to the neutron tracker. hence for now we will
# NOTE: have to do a bit more brute-force (23 March dev version:)

# Make sure the originals are safe:
cd ${VMCWORKDIR}/land/
cp ./R3BNeutronTracker2D.cxx ../Safe/R3BNeutronTracker2D.cxx
cp ./R3BNeutronTracker2D.h ../Safe/R3BNeutronTracker2D.h
cp ./CMakeLists.txt ../Safe/CMakeLists_land.txt

# Now modify the CMakeLists.txt-file:
sed -i '14 a ${R3BROOT_SOURCE_DIR}/veto_seg' ./CMakeLists.txt
sed -i '26 a ${R3BROOT_SOURCE_DIR}/r3bdata/veto_segData' ./CMakeLists.txt

# Then duplicate the new versions into the land directory:
cp ${SCRIPTPATH}/R3BNeutronTracker2D_ProtonCal.h ${VMCWORKDIR}/land/R3BNeutronTracker2D.h
cp ${SCRIPTPATH}/R3BNeutronTracker2D_ProtonCal.cxx ${VMCWORKDIR}/land/R3BNeutronTracker2D.cxx

cd ${SCRIPTPATH}

# NOTE: Code that does not work:
# cd ${SCRIPTPATH}
# ./Modify_Tracker.sh
# cd ${SCRIPTPATH}

######################################################################################################
# Finally: re-compile R3Broot:

echo '############################################### ReCompiling R3Broot...'
cd ${COMPPATH}
cd ../
make -j 4

######################################################################################################
# Display final messages:

echo ''
echo '#################################################################################################'
echo ''
echo 'If no error messages have showed up, your segmented VETO is now succesfully added to R3Broot!'
echo ''
echo ''
