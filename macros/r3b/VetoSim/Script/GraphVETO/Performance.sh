#!/bin/bash

# This script generates a pciture of the VETO performance. Since this code
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
sed -i "37 a Nfiles = ${COUNTER};" ./SimplePerformanceSeries.cpp
sed -i '39 d' ./SimplePerformanceSeries.cpp

# But now we also need to put in the type of series.
# fortunately the analysis has produced a file AnalysisTag.txt
# in the Inputs.root-file which consists of exactly the right number:
cd ../../Data/Inputs/
if [ -f "./AnalysisTag.txt" ]
then
read Series < ./AnalysisTag.txt
cd ../../Source/Graphics/
sed -i "32 a Series = ${Series};" ./SimplePerformanceSeries.cpp
sed -i '34 d' ./SimplePerformanceSeries.cpp

# And now we alos need to print how many graphs we actually
# want in our plot.
if [[ -z "$1" ]]
then
sed -i '42 a Nplots = 10;' ./SimplePerformanceSeries.cpp
sed -i '44 d' ./SimplePerformanceSeries.cpp
else
sed -i "42 a Nplots = $1;" ./SimplePerformanceSeries.cpp
sed -i '44 d' ./SimplePerformanceSeries.cpp
fi

# And now we can actually run the code:
find ./ -type f ! -name "*.*" -exec rm -rf {} \;
rm -rf ./*.o
cp ../../Compile/Graphics/SimplePerformanceSeries ./Makefile
make clean
make -j 4
./SimplePerformanceSeries

else
echo ''
echo '### ERROR: You did not yet perform an analysis! Without that'
echo '### ERROR: this picture cannot be created! Run an analysis first!'
echo ''
fi

# Done!
