#!/bin/bash

# This script performs a single simulation and then updates the run number:

# We assume that the required geometries have been build. It is hard to
# check for this, since if a detector is switched off, you do not need that
# geometry. But we can check for the media-file.

echo ''
echo '###########################################################################################'
echo ''
echo 'Running a single big simulation...'
if [ -f "../../Data/Geometry/media_r3b.geo" ]
then
  echo 'OK! Files exist. We can now proceed...'
  echo ''
  echo '###########################################################################################'
  echo 'Processing the simulation...'
  echo ''
  
  # First recreate the ASCII-files in case they are needed:
  cd ../../Data/Inputs/
  read ASCIIchoice < ./ASCII_Choice.txt  
  cd ../../Source/Simulation/
  make clean
  make -j 4
  if [[ ${ASCIIchoice} == '1' ]]
  then
  ./Create_ASCII_File_Uniform
  elif [[ ${ASCIIchoice} == '2' ]]
  then
  ./Boosting
  else
  ./Create_ASCII_File
  fi
  
  # Now execute the Monte Carlo Transport:
  ./CheckG4Engine
  cd ../../Data/Inputs/
  read MCchoice < ./MCengine.txt  
  cd ../../Source/Simulation/
  if [[ ${MCchoice} == '3' ]]
  then
     # we do first G4 and then G3:
     ./SetG4
     root -l -q R3B_Simulation.cpp
     ./G4Secondaries
     ./SetG3
     root -l -q R3B_Simulation.cpp
     ./SetG34
  else
     # We do a standard simulation: 
     root -l -q R3B_Simulation.cpp
  fi
  
  # Continue with the rest:
  
  echo ''
  echo 'OK! Geometries have been processed!'
  # clear the directory:
  rm -rf ./calor.out
  rm -rf ./flukaerr.dat
  rm -rf ./gphysi.dat
  echo '###########################################################################################'
  echo 'We do not fire visualization automatically...'
  echo '###########################################################################################'
  echo ''
  echo 'Firing NeuLAND digitizer...'
  echo ''
  root -l -q NeuLAND_Digitizer.cpp
  echo ''
  echo '###########################################################################################'
  echo ''
  echo 'Firing VETO digitizer...'
  echo ''
  root -l -q VETO_Digitizer.cpp
  echo ''
  echo '###########################################################################################'
  echo ''
  echo 'Firing Other digitizers...'
  echo ''
  root -l -q Other_Digitizers.cpp
  ./STaRTrackReconstructor
  echo ''
  echo '###########################################################################################'
  
  # NOTE: if our simulation is set on auto-calibration, we must calibrate here:
  read Line < ../../Data/Inputs/Tracker_Choice.txt
  if [[ "${Line}" == "2" ]]
  then
  ../../Script/RunVETO/Calibration.sh
  fi
  
  echo ''
  echo 'Firing NeuLAND Neutron Tracker...'
  echo ''
  root -l -q NeuLAND_Tracker.cpp
  echo ''
  echo '###########################################################################################'
  echo 'Simulation succesfully completed!'
  echo '###########################################################################################'
  echo ''
  # update the run:
  root -l -q Update_Run.cpp
else
  echo '### ERROR: The media file does not exist. propably you should generate'
  echo '### ERROR: your geometries first and then process them! Operation is now terminated.'
  echo ''
fi
  

