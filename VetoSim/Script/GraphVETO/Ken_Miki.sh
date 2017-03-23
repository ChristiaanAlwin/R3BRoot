#!/bin/bash

# This script generates pictures of produced secondary particles. Since this code
# is rougly written in the same way as the analysis code, this script, that
# controls this code, is also written according to the principle of the
# Full_Analysis.sh script.

############################################################################
# first check for the input:
if [[ -z "$1" ]]
then
echo '### NOTE: The commando miki requires one more inputs: a number to specify'
echo '### NOTE: which run you want to make a picture of. Hence call the script'
echo '### NOTE: like ./foto miki 3 or something, but not empty!'
echo ''
else

# re-generate the Inputs.root file:
cd ../../UI/
root -l -q SetParameters.cpp

# Write the counter:
cd ../Source/Graphics/
sed -i "34 a Counter = $1;" ./Ken_Miki.cpp
sed -i '36 d' ./Ken_Miki.cpp

# Then write the series:
cd ../../Data/Inputs/
if [ -f "./AnalysisTag.txt" ]
then
read Series < ./AnalysisTag.txt
cd ../../Source/Graphics/
sed -i "31 a Series = ${Series};" ./Ken_Miki.cpp
sed -i '33 d' ./Ken_Miki.cpp

# Now that we did this, we can compile and run:
find ./ -type f ! -name "*.*" -exec rm -rf {} \;
rm -rf ./*.o
cp ../../Compile/Graphics/Ken_Miki ./Makefile
make clean
make -j 4
./Ken_Miki

# Done!

else

# In case the analysis was not performed:
echo 'You did not perform an analysis! Hence we cannot run this code!'
echo ''
fi

# finish all other if-statements:
fi
