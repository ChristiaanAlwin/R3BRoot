#!/bin/bash

# This script rebuilds all the geometries of the R3B setup at FAIR:

echo ''
echo 'All geometries of the R3B setup at FAIR will now be re-build from scratch'
echo 'using the current version of Inputs.root located in ./Data/Inputs/'
echo ''
echo 'Checking of Inputs.root exists...'
echo ''
if [ -f "../../Data/Inputs/Inputs.root" ]
then
cd ../../Data/Geometry/
rm -rf ./*
cd ../../Source/Geometry/
echo 'Inputs.root exists! We can now proceed!'
echo ''
cp ${VMCWORKDIR}/geometry/media_r3b.geo ../../Data/Geometry/media_r3b.geo

echo '####################################################################### Re-building CAVE geometry:'
cp ${VMCWORKDIR}/geometry/r3b_cave.geo ../../Data/Geometry/r3b_cave.geo
echo ''
echo ''
read NUMBER < ../../Data/Inputs/Cave_Medium.txt
sed -i '3 d' ../../Data/Geometry/r3b_cave.geo
if [[ ${NUMBER} == '0' ]]
then
sed -i '2 a Air' ../../Data/Geometry/r3b_cave.geo
elif [[ ${NUMBER} == '1' ]]
then
sed -i '2 a Vacuum' ../../Data/Geometry/r3b_cave.geo
else
sed -i '2 a Air' ../../Data/Geometry/r3b_cave.geo
fi

echo '####################################################################### Re-building Target geometry:'
root -l -q Target_Geometry.cpp
echo ''
echo ''
echo '####################################################################### Re-building STaRTracker geometry:'
root -l -q StaRTrack_Geometry.cpp
echo ''
echo ''
echo '####################################################################### Re-building geometry for Vacuum Vessel:'
root -l -q Vacuum_Vessel_Geometry.cpp
echo ''
echo ''
echo '####################################################################### Re-building geometry for PSP in-beam detectors:'
root -l -q PSP_Geometry.cpp
echo ''
echo ''

echo '####################################################################### Re-building CALIFA geometry:'
root -l -q CALIFA_Geometry.cpp
echo ''
echo ''
echo '####################################################################### Re-building MFI geometry:'
root -l -q MFI_Geometry.cpp
echo ''
echo ''
echo '####################################################################### Re-building mTOF geometry:'
root -l -q mTOF_Geometry.cpp
echo ''
echo ''
echo '####################################################################### Re-building TOF geometry:'
root -l -q TOF_Geometry.cpp
echo ''
echo ''
echo '####################################################################### Re-building DCH geometry:'
root -l -q DCH_Geometry.cpp
echo ''
echo ''
echo '####################################################################### Re-building GLAD geometry:'
root -l -q GLAD_Geometry.cpp
echo ''
echo ''
echo '####################################################################### Re-building Chamber Dummy geometry:'
root -l -q Chamber_Dummy_Geometry.cpp
echo ''
echo ''
echo '####################################################################### Re-building NeuLAND geometry:'
root -l -q NeuLAND_Geometry.cpp
echo ''
echo ''
echo '####################################################################### Re-building Real NeuLAND geometry:'
root -l -q NeuLAND_RealGeometry.cpp
echo ''
echo ''
echo '####################################################################### Re-building NEBULA geometry:'
root -l -q NEBULA_Geometry.cpp
echo ''
echo ''
echo '####################################################################### Re-building VETO geometry:'
root -l -q VETO_Geometry.cpp
echo ''
echo ''
echo '####################################################################### Re-building Real VETO geometry:'
root -l -q VETO_RealGeometry.cpp
echo ''
echo ''
echo '####################################################################### Re-building NEBULA VETO geometry:'
root -l -q NEBULA_VETO_Geometry.cpp
echo ''
echo ''

echo '######################################################################################################'
echo ''
echo 'All geometries have successfully been rebuild!'
echo ''

else
echo 'ERROR: The file Inputs.root does not exist. Generate it first before running this script!!!'
echo ''
fi