#!/bin/bash

# This script removes the VETO from R3Broot:
export SCRIPTPATH=`pwd`
# VMCWORKDIR=R3Broot path
export COMPPATH=${FAIRLIBDIR}
# and cd ../ afterwards!

# We begin with deleting the VETO from the source code:
echo '############################################### Deleting any previous versions of the VETO...'
cd ${SCRIPTPATH}
./DeleteVETO.sh

# Then all that is left is to re-compile R3Broot:
echo '############################################### Recompiling R3Broot...'
cd ${COMPPATH}
rm -rf *
cmake ${VMCWORKDIR}
make -j 4

# Done!
echo ''
echo '##############################################################################################'
echo ''
echo 'All traces of the segmented VETO have been removed from R3Broot. Your release has been'
echo 'restored to how it previously was.'
echo ''