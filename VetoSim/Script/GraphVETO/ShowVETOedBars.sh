#!/bin/bash

# This script generates pictures of produced secondary particles. Since this code
# is rougly written in the same way as the analysis code, this script, that
# controls this code, is also written according to the principle of the
# Full_Analysis.sh script.

############################################################################
# first check for the input:
if [[ -z "$1" ]]
then
echo '### NOTE: The commando veto requires two more inputs: a number to specify'
echo '### NOTE: which run you want to make pictures of and a number to specify'
echo '### NOTE: the Event you want to make a picture of. Hence call the script'
echo '### NOTE: like ./foto veto 3 214 or something, but not empty!'
echo ''
else

if [[ -z "$2" ]]
then
echo '### NOTE: The commando veto requires two more inputs: a number to specify'
echo '### NOTE: which run you want to make pictures of and a number to specify'
echo '### NOTE: the Event you want to make a picture of. Hence call the script'
echo '### NOTE: like ./foto veto 3 214 or something, but not empty!'
echo ''
else

# re-generate the Inputs.root file:
cd ../../UI/
root -l -q SetParameters.cpp

# Now write our information into the geometry source code:

# Write the counter:
cd ../Source/Graphics/
sed -i "66 a Counter = $1;" ./VETOedBars_Geometry.cpp
sed -i '68 d' ./VETOedBars_Geometry.cpp

# Write the event:
sed -i "63 a Event = $2;" ./VETOedBars_Geometry.cpp
sed -i '65 d' ./VETOedBars_Geometry.cpp

# And now we can actually run the code:
rm -rf ./*core_dump*
root -l -q VETOedBars_Geometry.cpp
root -l -q VETOedBars_ProcessGeometry.cpp
rm -rf ./calor.out
rm -rf ./gphysi.dat
root -l VETOedBars_Display.cpp

fi
fi
