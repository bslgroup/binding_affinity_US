#!/bin/bash

#for i in `seq 0 30`
#do
#cat > tmp.tcl << EOF
#mol new step3_pbcsetup.xplor.ext.psf
#mol addfile step3_pbcsetup.pdb
#mol addfile dcd/distbeus_rmsd.dcd first 0 last -1 step 1 filebonds 1 autobonds 1 waitfor all

#set hbr [atomselect top "protein and resid 112 to 128 and name CA"]
#set hep [atomselect top "segname CARA"]

#set num_steps [molinfo top get numframes]

#for {set frame 0} {$frame < $num_steps} {incr frame} {
   # $hbr frame $frame
   # $hep frame $frame

#set d1 [vecsub [measure center $hbr] [measure center $hep]]]

#set d1 [vecsub [lindex [$hbr get {x y z}] 0] [lindex [$hep get {x y z}] 0] ]

#puts stderr "$frame $d1"

#}

#quit


#EOF
vmd -dispdev text -e rvector.tcl 2> rv.txt 
#done

