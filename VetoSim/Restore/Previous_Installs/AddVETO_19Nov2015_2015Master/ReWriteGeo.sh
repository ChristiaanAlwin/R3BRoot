#!/bin/bash

# This macro adds the VETO to the geometry files of R3Broot.
cd ${VMCWORKDIR}

###############################################################################
# We begin:
echo '-------------- Rewriting geometry macro...'

# Begin with duplicating the macro to create the geometry file:
cp ./macros/r3b/geo/create_neuland_geo.C ./macros/r3b/geo/create_Nveto_seg_geo.C

# Then search the file for the geometry strings:
for ((n=0; n<4; ++n))
do
sed -i -e 's:padle_h_box1:paddle_h_box1_seg:' ./macros/r3b/geo/create_Nveto_seg_geo.C
done
for ((n=0; n<4; ++n))
do
sed -i -e 's:padle_h_box2:paddle_h_box2_seg:' ./macros/r3b/geo/create_Nveto_seg_geo.C
done
for ((n=0; n<4; ++n))
do
sed -i -e 's:padle_h_box3:paddle_h_box3_seg:' ./macros/r3b/geo/create_Nveto_seg_geo.C
done
for ((n=0; n<4; ++n))
do
sed -i -e 's:padle_h_box4:paddle_h_box4_seg:' ./macros/r3b/geo/create_Nveto_seg_geo.C
done
for ((n=0; n<4; ++n))
do
sed -i -e 's:padle_h_box5:paddle_h_box5_seg:' ./macros/r3b/geo/create_Nveto_seg_geo.C
done
for ((n=0; n<4; ++n))
do
sed -i -e 's:"CELL":"CEL_SEG":' ./macros/r3b/geo/create_Nveto_seg_geo.C
done

# Then adapt the rest:
sed -i -e 's:Double_t fZ = 1550.;:Double_t fZ = 1000.;:' ./macros/r3b/geo/create_Nveto_seg_geo.C
sed -i -e 's:create_neuland_geo:create_Nveto_seg_geo:' ./macros/r3b/geo/create_Nveto_seg_geo.C
sed -i -e 's:Double_t neuLAND_depth_dim     = 150.;:Double_t neuLAND_depth_dim     = 5.;:' ./macros/r3b/geo/create_Nveto_seg_geo.C
sed -i -e 's:"/geometry/neuland_";:"/geometry/Nveto_seg_";:' ./macros/r3b/geo/create_Nveto_seg_geo.C
sed -i -e 's:"NEULANDgeom":"NVETO_SEGgeom":' ./macros/r3b/geo/create_Nveto_seg_geo.C
sed -i -e 's:"ALAND":"AVETO_SEG":' ./macros/r3b/geo/create_Nveto_seg_geo.C

###############################################################################
# Now we have to run this macro to add the real geometry file:

echo '-------------- Generating geometry file...'
cd ${SCRIPTPATH}
pwd
root -l -q GeoRun.C

###############################################################################
# Done!
echo '-------------- Done!'