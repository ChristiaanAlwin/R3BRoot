#!/bin/bash

# This script generates a picture of the Secondary particles. Since this code
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
sed -i "31 a Nfiles = ${COUNTER};" ./GetTableGraphics.cpp
sed -i '33 d' ./GetTableGraphics.cpp

# But now we also need to put in the type of series.
# fortunately the analysis has produced a file AnalysisTag.txt
# in the Inputs.root-file which consists of exactly the right number:
cd ../../Data/Inputs/
if [ -f "./AnalysisTag.txt" ]
then
read Series < ./AnalysisTag.txt
cd ../../Source/Graphics/
sed -i "27 a Series = ${Series};" ./GetTableGraphics.cpp
sed -i '29 d' ./GetTableGraphics.cpp

# And now we can actually run the code:
find ./ -type f ! -name "*.*" -exec rm -rf {} \;
rm -rf ./*.o
cp ../../Compile/Graphics/./GetTableGraphics ./Makefile
make clean
make -j 4
./GetTableGraphics

else
echo ''
echo '### ERROR: You did not yet perform an analysis! Without that'
echo '### ERROR: this picture cannot be created! Run an analysis first!'
echo ''
fi

# Done!
