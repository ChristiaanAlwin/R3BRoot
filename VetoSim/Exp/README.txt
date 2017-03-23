Before you can run any simulation here, you have to go to the /UI/ directory
and run the command ./Install.sh to add the VETO detector to R3BRoot. For people
who do not like this scirpt; there is also an ./UNinstall.sh script available.

Then, in order to run one of the simulations in this folder, take the following steps:

1) Copy-Paste the relevant .cpp-file to the UI-directory and re-name this file
   as SetParameters.cpp (discard the previous version, this can be done safely!)

2) Replace the content of the /UI/Calibration/ directory with the content of
   one of the directories inside /Exp/Calibrations. Use the following list:
./Pos_n.cpp                       1000 MeV
./Pos_p.cpp                       1000 MeV
./Pos_e.cpp                       1000 MeV
./Pos_Ca48.cpp                    1000 MeV
./Thick_p_160.cpp                 1000 MeV
./Thick_p_300.cpp                 1000 MeV
./Thick_p_500.cpp                 1000 MeV
./Thick_p_1000.cpp                1000 MeV
./Thick_pi_160.cpp                1000 MeV
./Thick_e_160.cpp                 1000 MeV
./Thick_mu_160.cpp                1000 MeV
./1000MeV_Bars.cpp                1000 MeV
./1000MeV_Cluster.cpp             1000 MeV
./1000MeV_Tracker.cpp             1000 MeV
./600MeV_Tracker.cpp               600 MeV
./200MeV_Tracker.cpp               200 MeV
./4n.cpp                           600 MeV
./4n1p.cpp                         600 MeV
./1n1p.cpp                         600 MeV
./Ca48.cpp                         600 MeV Real
./Ca48_Wall.cpp                    600 MeV Real
./Ca48_BG.cpp                      600 MeV
./Ca48_Wall_BG.cpp                 600 MeV Real
./Ca48_DoubleCut.cpp               600 MeV Real with TimeCuts 52 and 72 ns
./RealCalibration600.cpp           600 MeV Real
./Calibration600.cpp               600 MeV
./RealCalibration600_WithCuts.cpp  700 MeV Real
./RealCalibration700.cpp           700 MeV Real
./RealCalibration700_WithCuts.cpp  700 MeV Real
./LeadToLead.cpp                  1000 MeV Real With TimeCuts 51 and 68 ns
./Trigger.cpp                      600 MeV Real with TimeCuts 52 and 72 ns
./TriggerCa48Pb.cpp                600 MeV Real with TimeCuts 52 and 72 ns
./TriggerNi58.cpp                  700 MeV Real with TimeCuts 52 and 72 ns
./Miki_Ca48C.cpp                   500 MeV 
./Miki_Ca48Pb.cpp                  500 MeV 
./Miki_Ni58.cpp                    700 MeV

3) Open a terminal in the /UI/ directory. Run the command ./input.sh to
   process the input. Then run the command ./geo.sh to generate the
   detector geometries.

4) Run the simulation. Each simulation is run with a different command.
   Use the following list (it can take up to 24 hours!):
./Pos_n.cpp                       ./pos.sh
./Pos_p.cpp                       ./pos.sh
./Pos_e.cpp                       ./pos.sh
./Pos_Ca48.cpp                    ./pos.sh
./Thick_p_160.cpp                 ./thick.sh
./Thick_p_300.cpp                 ./thick.sh
./Thick_p_500.cpp                 ./thick.sh
./Thick_p_1000.cpp                ./thick.sh
./Thick_pi_160.cpp                ./thick.sh
./Thick_e_160.cpp                 ./thick.sh
./Thick_mu_160.cpp                ./thick.sh
./1000MeV_Bars.cpp                ./run.sh
./1000MeV_Cluster.cpp             ./run.sh
./1000MeV_Tracker.cpp             ./height.sh
./600MeV_Tracker.cpp              ./height.sh
./200MeV_Tracker.cpp              ./height.sh
./4n.cpp                          ./run.sh
./4n1p.cpp                        ./run.sh
./1n1p.cpp                        ./run.sh
./Ca48.cpp                        ./run.sh
./Ca48_Wall.cpp                   ./run.sh
./Ca48_BG.cpp                     ./run.sh
./Ca48_Wall_BG.cpp                ./run.sh
./Ca48_DoubleCut.cpp              ./run.sh
./RealCalibration600.cpp          ./calibrate.sh
./Calibration600.cpp              ./calibrate.sh
./RealCalibration600_WithCuts.cpp ./calibrate.sh
./RealCalibration700.cpp          ./calibrate.sh
./RealCalibration700_WithCuts.cpp ./calibrate.sh
./LeadToLead.cpp                  ./run.sh
./Trigger.cpp                     ./run.sh
./TriggerCa48Pb.cpp               ./run.sh
./TriggerNi58.cpp                 ./run.sh
./Miki_Ca48C.cpp                  ./run.sh
./Miki_Ca48Pb.cpp                 ./run.sh
./Miki_Ni58.cpp                   ./run.sh

5) Analyze the data by running the command ./analyse.sh
   NOTE: This script might need an input depending on the simulation
   that you run. Use the following list (it can take up to 1 h!)
./Pos_n.cpp                       ./analyse.sh pos
./Pos_p.cpp                       ./analyse.sh pos
./Pos_e.cpp                       ./analyse.sh pos
./Pos_Ca48.cpp                    ./analyse.sh pos
./Thick_p_160.cpp                 ./analyse.sh thick
./Thick_p_300.cpp                 ./analyse.sh thick
./Thick_p_500.cpp                 ./analyse.sh thick
./Thick_p_1000.cpp                ./analyse.sh thick
./Thick_pi_160.cpp                ./analyse.sh thick
./Thick_e_160.cpp                 ./analyse.sh thick
./Thick_mu_160.cpp                ./analyse.sh thick
./1000MeV_Bars.cpp                ./analyse.sh
./1000MeV_Cluster.cpp             ./analyse.sh
./1000MeV_Tracker.cpp             ./analyse.sh paddle
./600MeV_Tracker.cpp              ./analyse.sh paddle
./200MeV_Tracker.cpp              ./analyse.sh paddle
./4n.cpp                          ./analyse.sh
./4n1p.cpp                        ./analyse.sh
./1n1p.cpp                        ./analyse.sh
./Ca48.cpp                        ./analyse.sh
./Ca48_Wall.cpp                   ./analyse.sh
./Ca48_BG.cpp                     ./analyse.sh
./Ca48_Wall_BG.cpp                ./analyse.sh
./Ca48_DoubleCut.cpp              ./analyse.sh
./RealCalibration600.cpp          <<Data is in /UI/Calibration>>
./Calibration600.cpp              <<Data is in /UI/Calibration>>
./RealCalibration600_WithCuts.cpp <<Data is in /UI/Calibration>>
./RealCalibration700.cpp          <<Data is in /UI/Calibration>>
./RealCalibration700_WithCuts.cpp <<Data is in /UI/Calibration>>
./LeadToLead.cpp                  ./analyse.sh
./Miki_Ca48C.cpp                  ./analyse.sh
./Miki_Ca48Pb.cpp                 ./analyse.sh
./Miki_Ni58.cpp                   ./analyse.sh
./Trigger.cpp                     ./analyse.sh
./TriggerCa48Pb.cpp               ./analyse.sh
./TriggerNi58.cpp                 ./analyse.sh

NOTE: In order to explore different VETO conditions, one should adapt the 
bincontent at line 378 to the desired value. Carefully read the comments
to the left of this line to see all the options. When one whishes to
explore a new VETO condition, one should change this number and then re-analyze
the data. There is no need to re-run the simulation, but the output of the
previous data analysis is overwritten.

6) Produce your results. This can be done fast. In order to get
   the appropriate results, use the commands specified in the following list:
./Pos_n.cpp                       ./foto perf
./Pos_p.cpp                       ./foto perf
./Pos_e.cpp                       ./foto perf
./Pos_Ca48.cpp                    ./foto perf
./Thick_p_160.cpp                 ./foto perf 1
./Thick_p_300.cpp                 ./foto perf 1
./Thick_p_500.cpp                 ./foto perf 1
./Thick_p_1000.cpp                ./foto perf 1
./Thick_pi_160.cpp                ./foto perf 1
./Thick_e_160.cpp                 ./foto perf 1
./Thick_mu_160.cpp                ./foto perf 1
./1000MeV_Bars.cpp                ./foto vetf 1
./1000MeV_Cluster.cpp             ./foto vetf 1
./1000MeV_Tracker.cpp             ./foto vetg
./600MeV_Tracker.cpp              ./foto vetg
./200MeV_Tracker.cpp              ./foto vetg
./4n.cpp                          ./foto vetf 1
./4n1p.cpp                        ./foto vetf 1
./1n1p.cpp                        ./foto vetf 1
./Ca48.cpp                        ./foto list 1 & ./foto.sh vetf 1
./Ca48_Wall.cpp                   ./foto list 1
./Ca48_BG.cpp                     ./foto list 1
./Ca48_Wall_BG.cpp                ./foto list 1
./Ca48_DoubleCut.cpp              ./foto list 1 & ./foto.sh vetf 1
./RealCalibration600.cpp          <<Data is in /UI/Calibration>>
./Calibration600.cpp              <<Data is in /UI/Calibration>>
./RealCalibration600_WithCuts.cpp <<Data is in /UI/Calibration>>
./RealCalibration700.cpp          <<Data is in /UI/Calibration>>
./RealCalibration700_WithCuts.cpp <<Data is in /UI/Calibration>>
./LeadtoLead.cpp                  ./foto list 1 & ./foto.sh vetf 1
./Trigger.cpp                     ./foto list 1 & ./foto.sh vetf 1
./TriggerCa48Pb.cpp               ./foto list 1 & ./foto.sh vetf 1
./TriggerNi58.cpp                 ./foto list 1 & ./foto.sh vetf 1
./Miki_Ca48Pb.cpp                 ./foto cont 1
./Miki_Ca48C.cpp                  ./foto cont 1
./Miki_Ni58.cpp                   ./foto cont 1

NOTE: in the Ca48-simulations, one can also run ./foto vetf 1 for additional information.

7) In order to view the geometry of the setup, run the command ./vis.sh























