#!/bin/bash

# This script generates a pciture of the VETOed Primary particles. Since this code
# is rougly written in the same way as the analysis code, this script, that
# controls this code, is also written according to the principle of the
# Full_Analysis.sh script.

############################################################################
# first check for the input:
if [[ -z "$1" ]]
then
echo '### NOTE: The commando list requires a second input: a number to specify'
echo '### NOTE: which run you want to make pictures of. Hence call the script'
echo '### NOTE: like ./foto list 3 or something, but not empty!'
echo ''
else

# re-generate the Inputs.root file:
cd ../../UI/
root -l -q SetParameters.cpp

# Write the run rumber to the source code:
cd ../Source/Graphics/
sed -i "28 a Nruns = $1;" ./GetEventList.cpp
sed -i '30 d' ./GetEventList.cpp

# But now we also need to put in the type of series.
# fortunately the analysis has produced a file AnalysisTag.txt
# in the Inputs.root-file which consists of exactly the right number:
cd ../../Data/Inputs/
if [ -f "./AnalysisTag.txt" ]
then
read Series < ./AnalysisTag.txt
cd ../../Source/Graphics/
sed -i "25 a Series = ${Series};" ./GetEventList.cpp
sed -i '27 d' ./GetEventList.cpp

# And now we alos need to put the selection option in:
sed -i '32 a Select = 1;' ./GetEventList.cpp
sed -i '34 d' ./GetEventList.cpp

if [[ $2 == 2 ]]
then
sed -i '32 a Select = 2;' ./GetEventList.cpp
sed -i '34 d' ./GetEventList.cpp
elif [[ $2 == 3 ]]
then
sed -i '32 a Select = 3;' ./GetEventList.cpp
sed -i '34 d' ./GetEventList.cpp
elif [[ $2 == 4 ]]
then
sed -i '32 a Select = 4;' ./GetEventList.cpp
sed -i '34 d' ./GetEventList.cpp
fi 

# And now we can actually run the code:
find ./ -type f ! -name "*.*" -exec rm -rf {} \;
rm -rf ./*.o
cp ../../Compile/Graphics/GetEventList ./Makefile
make clean
make -j 4
./GetEventList

else
echo ''
echo '### ERROR: You did not yet perform an analysis! Without that'
echo '### ERROR: this picture cannot be created! Run an analysis first!'
echo ''
fi

# Done!
fi
