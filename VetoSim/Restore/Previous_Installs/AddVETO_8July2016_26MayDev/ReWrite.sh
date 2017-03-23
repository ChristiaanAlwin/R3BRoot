#!/bin/bash

# This program rewrites the content of all files in a directory
# descriped by the variable RE_PATH. It performs the
# rewriting according to land==>veto_seg

###########################################################################
# We will simply use sed to rewrite all file content. We will however need
# to use a for-loop since the specific string might occur more
# then once on the same line.

# cd to the right directory:
cd ${RE_PATH}

# Neulands:
echo '-------------- NeuLAND strings...'
for ((n=0; n<6; ++n))
do
find ./ -type f -exec sed -i -e "s:NEULAND:NVETO_SEG:" {} \;
done
for ((n=0; n<6; ++n))
do
find ./ -type f -exec sed -i -e "s:NeuLAND:Nveto_seg:" {} \;
done
for ((n=0; n<6; ++n))
do
find ./ -type f -exec sed -i -e "s:NeuLand:Nveto_seg:" {} \;
done
for ((n=0; n<6; ++n))
do
find ./ -type f -exec sed -i -e "s:Neuland:Nveto_seg:" {} \;
done
for ((n=0; n<6; ++n))
do
find ./ -type f -exec sed -i -e "s:neuland:Nveto_seg:" {} \;
done

# Lands:
echo '-------------- LAND strings...'
for ((n=0; n<6; ++n))
do
find ./ -type f -exec sed -i -e "s:LAND:VETO_SEG:" {} \;
done
for ((n=0; n<6; ++n))
do
find ./ -type f -exec sed -i -e "s:Land:veto_seg:" {} \;
done
for ((n=0; n<6; ++n))
do
find ./ -type f -exec sed -i -e "s:land:veto_seg:" {} \;
done

# Rest of the stuff:
echo '-------------- Other strings...'
for ((n=0; n<6; ++n))
do
find ./ -type f -exec sed -i -e "s:NeutHit:NeutHt_seg:" {} \;
done
for ((n=0; n<6; ++n))
do
find ./ -type f -exec sed -i -e "s:NeutronTracker2D:NeutronTrackr2D_seg:" {} \;
done
for ((n=0; n<6; ++n))
do
find ./ -type f -exec sed -i -e "s:NeutronTracker:NeutronTrackr_seg:" {} \;
done
for ((n=0; n<6; ++n))
do
find ./ -type f -exec sed -i -e "s:NeutronCalibr2D:NeutronCalib2D_seg:" {} \;
done
for ((n=0; n<6; ++n))
do
find ./ -type f -exec sed -i -e "s:ConstantFraction:ConstantFracton_seg:" {} \;
done
for ((n=0; n<6; ++n))
do
find ./ -type f -exec sed -i -e "s:parameters:parameter_seg:" {} \;
done
for ((n=0; n<6; ++n))
do
find ./ -type f -exec sed -i -e "s:PrimPart:PrimPat_seg:" {} \;
done
for ((n=0; n<6; ++n))
do
find ./ -type f -exec sed -i -e "s:NeutronTrack:NeutronTrak_seg:" {} \;
done
for ((n=0; n<6; ++n))
do
find ./ -type f -exec sed -i -e "s:PaddleCrystalHit:PaddleCrystalHt_seg:" {} \;
done
for ((n=0; n<6; ++n))
do
find ./ -type f -exec sed -i -e "s:Digitizing:Digitizin_seg:" {} \;
done
for ((n=0; n<6; ++n))
do
find ./ -type f -exec sed -i -e "s:Monitoring:Monitorin_seg:" {} \;
done
for ((n=0; n<6; ++n))
do
find ./ -type f -exec sed -i -e "s:void compare(:void compar_seg(:" {} \;
done
for ((n=0; n<6; ++n))
do
find ./ -type f -exec sed -i -e "s:Validated:Validatd_seg:" {} \;
done
for ((n=0; n<6; ++n))
do
find ./ -type f -exec sed -i -e "s:PaddleTamex:Paddle_segTamex:" {} \;
done

# Captitals:
echo '-------------- Capital Other strings...'
for ((n=0; n<6; ++n))
do
find ./ -type f -exec sed -i -e "s:NEUTHIT:NEUTHT_SEG:" {} \;
done
for ((n=0; n<6; ++n))
do
find ./ -type f -exec sed -i -e "s:NEUTRONTRACKER2D:NEUTRONTRACKR2D_SEG:" {} \;
done
for ((n=0; n<6; ++n))
do
find ./ -type f -exec sed -i -e "s:NEUTRONTRACKER:NEUTRONTRACKR_SEG:" {} \;
done
for ((n=0; n<6; ++n))
do
find ./ -type f -exec sed -i -e "s:NEUTRONCALIBR2D:NEUTRONCALIB2D_SEG:" {} \;
done
for ((n=0; n<6; ++n))
do
find ./ -type f -exec sed -i -e "s:CONSTANTFRACTION:CONSTANTFRACTON_SEG:" {} \;
done
for ((n=0; n<6; ++n))
do
find ./ -type f -exec sed -i -e "s:PARAMETERS:PARAMETER_SEG:" {} \;
done
for ((n=0; n<6; ++n))
do
find ./ -type f -exec sed -i -e "s:PRIMPART:PRIMPAT_SEG:" {} \;
done
for ((n=0; n<6; ++n))
do
find ./ -type f -exec sed -i -e "s:NEUTRONTRACK:NEUTRONTRAK_SEG:" {} \;
done
for ((n=0; n<6; ++n))
do
find ./ -type f -exec sed -i -e "s:PADDLECRYSTALHIT:PADDLECRYSTALHT_SEG:" {} \;
done
for ((n=0; n<6; ++n))
do
find ./ -type f -exec sed -i -e "s:DIGITIZING:DIGITIZIN_seg:" {} \;
done
for ((n=0; n<6; ++n))
do
find ./ -type f -exec sed -i -e "s:MONITORING:MONITORIN_seg:" {} \;
done
for ((n=0; n<6; ++n))
do
find ./ -type f -exec sed -i -e "s:COMPARE:COMPAR_SEG:" {} \;
done
for ((n=0; n<6; ++n))
do
find ./ -type f -exec sed -i -e "s:VALIDATED:VALIDATD_SEG:" {} \;
done
for ((n=0; n<6; ++n))
do
find ./ -type f -exec sed -i -e "s:PADDLETAMEX:PADDLE_SEGTAMEX:" {} \;
done

# Geometry strings:
echo '-------------- Geometry strings...'
for ((n=0; n<6; ++n))
do
find ./ -type f -exec sed -i -e 's:"padle_h_box1":"paddle_h_box1_seg":' {} \;
done
for ((n=0; n<6; ++n))
do
find ./ -type f -exec sed -i -e 's:"padle_h_box2":"paddle_h_box2_seg":' {} \;
done
for ((n=0; n<6; ++n))
do
find ./ -type f -exec sed -i -e 's:"padle_h_box3":"paddle_h_box3_seg":' {} \;
done
for ((n=0; n<6; ++n))
do
find ./ -type f -exec sed -i -e 's:"padle_h_box4":"paddle_h_box4_seg":' {} \;
done
for ((n=0; n<6; ++n))
do
find ./ -type f -exec sed -i -e 's:"padle_h_box5":"paddle_h_box5_seg":' {} \;
done
for ((n=0; n<6; ++n))
do
find ./ -type f -exec sed -i -e 's:"volBC408":"vollBC408_seg":' {} \;
done
for ((n=0; n<6; ++n))
do
find ./ -type f -exec sed -i -e 's:"volAlWrapping":"vollAlWrapping_seg":' {} \;
done
for ((n=0; n<6; ++n))
do
find ./ -type f -exec sed -i -e 's:"volTapeWrapping":"vollTapeWrapping_seg":' {} \;
done
for ((n=0; n<6; ++n))
do
find ./ -type f -exec sed -i -e 's:"volPaddle":"vollPaddle_seg":' {} \;
done
















# Done!
echo '-------------- Done!'

