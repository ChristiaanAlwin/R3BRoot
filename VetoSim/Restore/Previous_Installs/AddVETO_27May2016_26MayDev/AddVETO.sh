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
echo 'This script is tested on the R3Broot dev-branch of 26 May 2016.'
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

if [ -d "${VMCWORKDIR}/r3bdb/landdb" ]
then
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
fi

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
# Now modify the VETO digitizer to process the data in the right way:
echo '############################################### Perform VETO Digitizer Modifications...'
cd ${SCRIPTPATH}
./Modify_VETOdigitizer.sh

####################################################################################################
# Modify the Neutron Tracker to use different calibrations:
echo '############################################### Perform R3BRoot NeutronTracker modifications...'
cd ${SCRIPTPATH}
./Modify_Tracker.sh

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
