#!/bin/bash

ni=31
nc=1
force=2
# generates image_id (block_id) time U_0 U_1 U-2 ...
cat centers_old.tcl cvsall.txt | \
awk -v n=$ni -v k=$force '
{
    if (NR<=n) {
	c[NR-1]=$11
    } else {
#        printf "%d %d %d",$3,$2,$1
         printf "%d %d %d",$3,int(($1-1)/1000000),$1
#        printf "%d %d",$3,$1
	for (i=0;i<=n-1;i++) {
	    printf " %f",0.5*k*($4-c[i])*($4-c[i])
	}
	printf "\n"
    }
}' > potall.txt
