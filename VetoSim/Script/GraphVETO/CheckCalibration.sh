#!/bin/bash

# This script generates pictures of produced secondary particles. Since this code
# is rougly written in the same way as the analysis code, this script, that
# controls this code, is also written according to the principle of the
# Full_Analysis.sh script.

############################################################################
# first check for the input:
if [[ -z "$3" ]]
then
echo '### NOTE: The commando kons requires more inputs. Call the script like'
echo '### NOTE: ./foto cuts 1 nn np to show the picture for run 1, nn neutrons'
echo '### NOTE: and np protons (= VETO hits).'
echo ''
else

# re-generate the Inputs.root file:
cd ../../UI/
root -l -q SetParameters.cpp

# Write the counter:
cd ../Source/Graphics/
sed -i "34 a Counter = $1;" ./CheckCalibration.cpp
sed -i '36 d' ./CheckCalibration.cpp

# Then write the series:
cd ../../Data/Inputs/
if [ -f "./AnalysisTag.txt" ]
then
read Series < ./AnalysisTag.txt
cd ../../Source/Graphics/
sed -i "37 a Series = ${Series};" ./CheckCalibration.cpp
sed -i '39 d' ./CheckCalibration.cpp

# Write the outher stuff.
sed -i "40 a Nneutrons = $2;" ./CheckCalibration.cpp
sed -i '42 d' ./CheckCalibration.cpp

sed -i "43 a Nprotons = $3;" ./CheckCalibration.cpp
sed -i '45 d' ./CheckCalibration.cpp

# Now that we did this, we can compile and run:
find ./ -type f ! -name "*.*" -exec rm -rf {} \;
rm -rf ./*.o
cp ../../Compile/Graphics/CheckCalibration ./Makefile
make clean
make -j 4
./CheckCalibration

# Done!

else

# In case the analysis was not performed:
echo 'You did not perform an analysis! Hence we cannot run this code!'
echo ''
fi

# finish all other if-statements:
fi
