#!/bin/bash

# This script provides a user interface to painting
# the graphics. The script has a single input 
# that denotes which graph you want. If a non-valid
# option is given, there will simply be printed
# a list with all the available options.

if [[ $1 == 'perf' ]] # First option:
then
./Performance.sh $2
elif [[ $1 == 'sec' ]]
then
./Secondaries.sh $2
elif [[ $1 == 'tab' ]]
then
./Table.sh $2
elif [[ $1 == 'secf' ]]
then
./SecGraph.sh
elif [[ $1 == 'Eres' ]]
then
./Energy_Reconstruction.sh $2
elif [[ $1 == 'veto' ]]
then
./ShowVETOedBars.sh $2 $3
elif [[ $1 == 'vetf' ]]
then
./VETO_Table.sh $2
elif [[ $1 == 'vetg' ]]
then
./VETO_graph.sh $2
elif [[ $1 == 'miki' ]]
then
./Ken_Miki.sh $2
elif [[ $1 == 'cont' ]]
then
./Miki_Back.sh $2
elif [[ $1 == 'TvsE' ]]
then
./TOFvsEdep.sh $2
elif [[ $1 == 'fire' ]]
then
./ShowFiredBars.sh $2 $3
elif [[ $1 == 'firV' ]]
then
./ShowVETOedFiredBars.sh $2
elif [[ $1 == 'list' ]]
then
./EventList.sh $2 $3
elif [[ $1 == 'time' ]]
then
./DrawTimeHists.sh $2
elif [[ $1 == 'back' ]]
then
./Background.sh $2
elif [[ $1 == 'kons' ]]
then
./Konstanze.sh $2 $3 $4 $5
elif [[ $1 == 'prob' ]]
then
./Probabilities.sh $2
elif [[ $1 == 'cuts' ]]
then
./CheckCalibration.sh $2 $3 $4
else                # No suitable option:
echo 'The script ./foto.sh should be executed with one of the following inputs:'
echo ''
echo 'perf       // This produces a graph of Veto Performance VS the input of ./analyse.sh'
echo 'sec        // This produces .png-files of all produced secondary particles in a run.'
echo 'tab        // This produces a table on the screen of all secondary particles in a run.'
echo 'secf       // This produces graphs of secondary particles VS the input of ./analyse.sh'
echo 'Eres       // This produces a graph of the reconstructed energy of some primary particles.'
echo 'veto       // This produces an OpenGL picture of VETOed NeuLAND bars for a specific Event.'
echo 'fire       // This produces an OpenGL picture of Fired NeuLAND bars for a specific Event and PDG.'
echo 'firV       // This produces an OpenGL picture of Fired & NOT-VETOed NeuLAND bars for a specific Event.'
echo 'vetf       // This produces a table of primary particles that are yes/no VETOed.'
echo 'vetg       // This produces a graph of % VETOed primary particles VS input of ./analyse.sh'
echo 'miki       // produces a plot on the G4 Track starting points according to Ken Miki his slides.'
echo 'cont       // produces a histogram of #incoming particles VS X-coordinate for a NeuLAND plane.'
echo 'TvsE       // produces a histogram of Time of Flight VS Energy Deposition per bar for a NeuLAND plane.'
echo 'time       // produces a few histograms of the Time of Flight for reconstructed vertices.'
echo 'list       // Produces a list of reconstructed events for a specific simulation run.'
echo 'back       // produces plots like the miki option and energy histograms but only for Primary Particles.'
echo 'kons       // produces background plots for Konstanze her time cuts.'
echo 'prob       // produces the probabilities for neutrons to interact in the window or the air.'
echo 'cuts       // Checks how well the calibration cuts you used apply to the real data.'
echo ''
echo 'Selecting one of these options will produce the desired picture.'
echo 'The picture will be saved in the Pictures-folder inside the UI-folder.'
echo 'The user is responsible for managing the content of this folder,'
echo 'since this script will override any picture in this folder that'
echo 'carries the same name as the new picture.'
echo ''
echo 'The picture will also appear on the screen in interactive mode.'
echo 'After you did with this picture what you want, you can close the'
echo 'application by pressing CTRL+C'
echo ''
echo 'Calling this script with any other input then one of the options listed'
echo 'above will give this list but will not produce any output.'
fi
