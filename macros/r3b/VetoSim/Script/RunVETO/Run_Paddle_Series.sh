#!/bin/bash

# This script performs an entire series of simulations (and digitizers).
# after a single simulation (and digitizers), the run number is updated
# and the number of paddles of the VETO is updated as well. This continues
# for as long as the the for-loop is specified in SetParameters.cpp

# Written by C. A. Douma.

##########################################################################

# The first thing we have to do is to obtain the size of the for-loop:
cd ../../Source/Simulation/
root -l -q GetForNumber.cpp
cd ../../Data/Inputs/
read SIZE < ./Nsim.txt
read ASCIIchoice < ./ASCII_Choice.txt

# Then we test if we can run the simulations:
echo ''
echo '###########################################################################################'
echo ''
echo 'Running a series of big simulations for different number of paddles in the VETO...'
echo "We run ${SIZE} big simulations"
echo ''
if [ -f "../../Data/Geometry/media_r3b.geo" ]
then
  echo 'OK: Media file exists! Now we can proceed...'
  echo ''
  
  # Now we know that we can run our series! Hence we cd to the right directory:
  cd ../../Source/Simulation/
  
  # now we run the for-loop:
  for (( k=0; k<${SIZE}; ++k ))
  do
    # Run the simulation:
    echo '###########################################################################################'
    echo ''
    echo '################################################################ Running the simulation...'
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
    
    # clear the directory:
    rm -rf ./calor.out
    rm -rf ./flukaerr.dat
    rm -rf ./gphysi.dat
    
    # Fire NeuLAND digitizer:
    echo ''
    echo '################################################################ Computing NeuLAND digitizer...'
    echo ''
    root -l -q NeuLAND_Digitizer.cpp
    
    # Fire VETO digitizer:
    echo ''
    echo '################################################################ Computing VETO digitizer...'
    echo ''
    root -l -q VETO_Digitizer.cpp
    
    # Fire the other digitizers:
    echo ''
    echo '################################################################ Computing other digitizers...'
    echo ''
    root -l -q Other_Digitizers.cpp
    ./STaRTrackReconstructor
    
    # NOTE: if our simulation is set on auto-calibration, we must calibrate here:
    read Line < ../../Data/Inputs/Tracker_Choice.txt
    if [[ "${Line}" == "2" ]]
    then
    ./Calibration.sh
    fi
    
    # Fire NeuLAND Neutron Tracker:
    echo ''
    echo '################################################################ Computing NeuLAND Neutron Tracker...'
    echo ''
    root -l -q NeuLAND_Tracker.cpp
    
    # Update the counter:
    echo ''
    echo '################################################################ Updating the simulation counter...'
    echo ''
    root -l -q Update_Run.cpp
    
    # Update the geometry:
    echo ''
    echo '################################################################ Updating VETO geometry...'
    echo ''
    root -l -q Update_VETO_Paddles.cpp
    
    # rebuild the geometry:
    cd ../Geometry/
    root -l -q VETO_Geometry.cpp
    cd ../Simulation/
    
    # And now we are done!
    echo ''
    echo '################################################################ One big simulation is complete!'
    echo ''
  
  done

else
  echo '### ERROR: The media file does not exist. propably you should generate'
  echo '### ERROR: your geometries first and then process them! Operation is now terminated.'
  echo ''
  cd ../../Source/Simulation/
fi

# Finally, delete the Nsim.txt:
cd ../../Data/Inputs/
rm -rf ./Nsim.txt

# Done!

  

