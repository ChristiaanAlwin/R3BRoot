#!/bin/bash

# This script clears all data given a specific nametag:
echo 'This script clears all data that contains the input'
echo 'string that you gave the script. If the input is empty,'
echo 'no data is cleared.'
echo ''

if [[ $1 == '' ]]
then
echo 'No data is deleted.'
echo ''
else
cd ../../Data/Digitizer/
rm -rf ./*$1*
cd ../Geometry/
rm -rf ./* # this does not contain the nametag!
cd ../MonteCarlo/
rm -rf ./*$1*
rm -rf ./Geometry_Visualization*
cd ../Inputs/
rm -rf ./* # this does not contain the nametag!
cd ../Parameters/
rm -rf ./*$1*
rm -rf ./Geometry_Visualization*
cd ../Analysis/
rm -rf ./*$1*
echo "All datafiles containing <<$1>> are deleted."
echo ''
fi