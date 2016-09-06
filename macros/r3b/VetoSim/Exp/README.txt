Before you can run any simulation here, you have to go to the /UI/ directory
and run the command ./Install.sh to add the VETO detector to R3BRoot. For people
who do not like this scirpt; there is also an ./UNinstall.sh script available.

Then, in order to run one of the simulations in this folder, take the following steps:

1) Copy-Paste the relevant .cpp-file to the UI-directory and re-name this file
   as SetParameters.cpp (discard the previous version, this can be done safely!)

2) Replace the content of the /UI/Calibration/ directory with the content of
   one of the directories inside /Exp/Calibrations. Use the following list:
./Pos_n.cpp                     1000 MeV
./Pos_p.cpp                     1000 MeV
./Pos_e.cpp                     1000 MeV
./Pos_Ca48.cpp                  1000 MeV
./Thick_p_160.cpp               1000 MeV
./Thick_p_300.cpp               1000 MeV
./Thick_p_500.cpp               1000 MeV
./Thick_p_1000.cpp              1000 MeV
./Thick_pi_160.cpp              1000 MeV
./Thick_e_160.cpp               1000 MeV
./Thick_mu_160.cpp              1000 MeV
./1000MeV_Bars.cpp              1000 MeV
./1000MeV_Cluster.cpp           1000 MeV
./1000MeV_Tracker.cpp           1000 MeV
./600MeV_Tracker.cpp             600 MeV
./200MeV_Tracker.cpp             200 MeV
./4n.cpp                         600 MeV
./4n1p.cpp                       600 MeV
./Ca48.cpp                       500 MeV
./EOS.cpp                        300 MeV
./F29.cpp                       1000 MeV

3) Open a terminal in the /UI/ directory. Run the command ./input.sh to
   process the input. Then run the command ./geo.sh to generate the
   detector geometries.

4) Run the simulation. Each simulation is run with a different command.
   Use the following list (it can take up to 24 hours!):
./Pos_n.cpp                     ./pos.sh
./Pos_p.cpp                     ./pos.sh
./Pos_e.cpp                     ./pos.sh
./Pos_Ca48.cpp                  ./pos.sh
./Thick_p_160.cpp               ./thick.sh
./Thick_p_300.cpp               ./thick.sh
./Thick_p_500.cpp               ./thick.sh
./Thick_p_1000.cpp              ./thick.sh
./Thick_pi_160.cpp              ./thick.sh
./Thick_e_160.cpp               ./thick.sh
./Thick_mu_160.cpp              ./thick.sh
./1000MeV_Bars.cpp              ./run.sh
./1000MeV_Cluster.cpp           ./run.sh
./1000MeV_Tracker.cpp           ./height.sh
./600MeV_Tracker.cpp            ./height.sh
./200MeV_Tracker.cpp            ./height.sh
./4n.cpp                        ./run.sh
./4n1p.cpp                      ./run.sh
./Ca48.cpp                      ./run.sh
./EOS.cpp                       ./run.sh
./F29.cpp                       ./run.sh

5) Analyze the data by running the command ./analyse.sh
   NOTE: This script might need an input depending on the simulation
   that you run. Use the following list (it can take up to 1 h!)
./Pos_n.cpp                     ./analyse.sh pos
./Pos_p.cpp                     ./analyse.sh pos
./Pos_e.cpp                     ./analyse.sh pos
./Pos_Ca48.cpp                  ./analyse.sh pos
./Thick_p_160.cpp               ./analyse.sh thick
./Thick_p_300.cpp               ./analyse.sh thick
./Thick_p_500.cpp               ./analyse.sh thick
./Thick_p_1000.cpp              ./analyse.sh thick
./Thick_pi_160.cpp              ./analyse.sh thick
./Thick_e_160.cpp               ./analyse.sh thick
./Thick_mu_160.cpp              ./analyse.sh thick
./1000MeV_Bars.cpp              ./analyse.sh
./1000MeV_Cluster.cpp           ./analyse.sh
./1000MeV_Tracker.cpp           ./analyse.sh paddle
./600MeV_Tracker.cpp            ./analyse.sh paddle
./200MeV_Tracker.cpp            ./analyse.sh paddle
./4n.cpp                        ./analyse.sh
./4n1p.cpp                      ./analyse.sh
./Ca48.cpp                      ./analyse.sh
./EOS.cpp                       ./analyse.sh
./F29.cpp                       ./analyse.sh

NOTE: In order to explore different VETO conditions, one should adapt the 
bincontent at line 378 to the desired value. Carefully read the comments
to the left of this line to see all the options. When one whishes to
explore a new VETO condition, one should change this number and then re-analyze
the data. There is no need to re-run the simulation, but the output of the
previous data analysis is overwritten.

6) Produce your results. This can be done fast. In order to get
   the appropriate results, use the commands specified in the following list:
./Pos_n.cpp                     ./foto perf
./Pos_p.cpp                     ./foto perf
./Pos_e.cpp                     ./foto perf
./Pos_Ca48.cpp                  ./foto perf
./Thick_p_160.cpp               ./foto perf 1
./Thick_p_300.cpp               ./foto perf 1
./Thick_p_500.cpp               ./foto perf 1
./Thick_p_1000.cpp              ./foto perf 1
./Thick_pi_160.cpp              ./foto perf 1
./Thick_e_160.cpp               ./foto perf 1
./Thick_mu_160.cpp              ./foto perf 1
./1000MeV_Bars.cpp              ./foto vetf 1
./1000MeV_Cluster.cpp           ./foto vetf 1
./1000MeV_Tracker.cpp           ./foto vetg
./600MeV_Tracker.cpp            ./foto vetg
./200MeV_Tracker.cpp            ./foto vetg
./4n.cpp                        ./foto vetf 1
./4n1p.cpp                      ./foto vetf 1
./Ca48.cpp                      ./foto vetf 1
./EOS.cpp                       ./foto vetf 1
./F29.cpp                       ./foto vetf 1

NOTE: in the simulations Ca48, EOS & F29 one has to adapt the parameter 
at line 468 (only change the bincontent!) to the number of neutrons one
is interested in. Then one should run the ./foto vetf 1 command several times
in order to obtain a complete set of results. In is not necessary to
re-analyze or re-run simulations for this.

7) In order to view the geometry of the setup, run the command ./vis.sh























