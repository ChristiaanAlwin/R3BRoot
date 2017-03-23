#!/bin/bash

# This script generates a pciture of the VETOed Primary particles. Since this code
# is rougly written in the same way as the analysis code, this script, that
# controls this code, is also written according to the principle of the
# Full_Analysis.sh script.

############################################################################

# re-generate the Inputs.root file:
cd ../../UI/
root -l -q SetParameters.cpp

# put all Analysis files into a .txt-file
cd ../Data/Analysis/
find ./ -type f > ../Inputs/MC_AnaNames.txt

# obtain the current NameTag from the Inputs.root-file:
cd ../../Source/Analysis/
root -l -q GetNameTag.cpp
cd ../../Data/Inputs/

# Now read that NameTag:
read NameTag < ./NameTag.txt

# Then read the filenames and count:
COUNTER=0;         # Counter number of files
while read Line    # Read the file with filenames
do
if [[ ${Line} == *${NameTag}* ]]   # Check on the NameTag:
then
COUNTER=`expr ${COUNTER} + 1`      # Count
fi
done < ./MC_AnaNames.txt
rm -rf ./MC_AnaNames.txt

# So now we know exactly how many files containing the NameTag actually exist:
# So write this into the graphics file:
cd ../../Source/Graphics/
sed -i "31 a Nruns = ${COUNTER};" ./VETOedGraph.cpp
sed -i '33 d' ./VETOedGraph.cpp

# But now we also need to put in the type of series.
# fortunately the analysis has produced a file AnalysisTag.txt
# in the Inputs.root-file which consists of exactly the right number:
cd ../../Data/Inputs/
if [ -f "./AnalysisTag.txt" ]
then
read Series < ./AnalysisTag.txt
cd ../../Source/Graphics/
sed -i "28 a Series = ${Series};" ./VETOedGraph.cpp
sed -i '30 d' ./VETOedGraph.cpp

# And now we alos need to put the selection option in:
sed -i '35 a Select = 1;' ./VETOedGraph.cpp
sed -i '37 d' ./VETOedGraph.cpp

if [[ $1 == 2 ]]
then
sed -i '35 a Select = 2;' ./VETOedGraph.cpp
sed -i '37 d' ./VETOedGraph.cpp
elif [[ $1 == 3 ]]
then
sed -i '35 a Select = 3;' ./VETOedGraph.cpp
sed -i '37 d' ./VETOedGraph.cpp
elif [[ $1 == 4 ]]
then
sed -i '35 a Select = 4;' ./VETOedGraph.cpp
sed -i '37 d' ./VETOedGraph.cpp
fi 

# And now we can actually run the code:
find ./ -type f ! -name "*.*" -exec rm -rf {} \;
rm -rf ./*.o
cp ../../Compile/Graphics/VETOedGraph ./Makefile
make clean
make -j 4
./VETOedGraph

else
echo ''
echo '### ERROR: You did not yet perform an analysis! Without that'
echo '### ERROR: this picture cannot be created! Run an analysis first!'
echo ''
fi

# Done!
