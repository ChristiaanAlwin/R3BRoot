#!/bin/bash

# This script duplicates the entire VETO simulation,
# except the simulation data. So only the source code:

cd ../../../

# remove previous backups:
if [ -d "./VetoSim_Backup" ]
then
  rm -rf ./VetoSim_Backup
fi

# Now generate the new backup:
mkdir VetoSim_Backup
cd ./VetoSim/
cp -R ./Source ../VetoSim_Backup/Source
cp -R ./Script ../VetoSim_Backup/Script
cp -R ./UI ../VetoSim_Backup/UI
cp -R ./Restore ../VetoSim_Backup/Restore
cp -R ./Compile ../VetoSim_Backup/Compile
cp -R ./Exp ../VetoSim_Backup/Exp

# Now we have the source code. Create the 
# rest of the directories blank:
cd ../VetoSim_Backup/
mkdir Data

cd ./Data
mkdir Analysis
mkdir MonteCarlo
mkdir Digitizer
mkdir Geometry
mkdir Inputs
mkdir Parameters

cd ../../VetoSim/
cp ./Manual.txt ../VetoSim_Backup/Manual.txt

# And now we are done!

