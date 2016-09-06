#!/bin/bash

# This script processes the new Geometries and displays them in the eventdisplay.

# We assume that the required geometries have been build. It is hard to
# check for this, since if a detector is switched off, you do not need that
# geometry. But we can check for the media-file.

echo ''
echo '###########################################################################################'
echo ''
echo 'Procesing current geometries for visualization...'
if [ -f "../../Data/Geometry/media_r3b.geo" ]
then
  echo 'OK! Files exist. We can now process...'
  echo ''
  echo '###########################################################################################'
  echo 'Processing a short simulation...'
  echo ''
  cd ../../Source/Simulation/
  root -l -q R3B_ProcessGeometry.cpp
  echo ''
  echo 'OK! Geometries have been processed!'
  # clear the directory:
  rm -rf ./calor.out
  rm -rf ./flukaerr.dat
  rm -rf ./gphysi.dat
  echo '###########################################################################################'
  echo 'Finally: Firing the visualization...'
  echo ''
  root -l GeometryDisplay.cpp
  echo ''
  echo 'Done!'
  echo ''
  echo '###########################################################################################'
  echo ''
else
  echo '### ERROR: The media file does not exist. propably you should generate'
  echo '### ERROR: your geometries first and then process them! Operation is now terminated.'
  echo ''
fi
  

