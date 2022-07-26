#!/bin/bash

# comment out if you have the output dcd files
#vmd -dispdev text -e analyze.tcl 2> analyze.txt

#awk '$2%2500==0{print}' densityd.txt > densitydist1.txt

paste density_test.txt rv.txt | \
awk -v dx=1 -v dy=1 -v dz=1 -v T=300 '
function acos(o) {
    if(o==1||o==-1) {
	return 2*(1-o)*atan2(0.5,0.5)
    } else {
	return atan2(-o,sqrt(1-o*o))+2*atan2(0.5,0.5)
    }
}function round(x) {
    ival = int(x)
    if (ival == x) {
	return x
    } else {
	if (x < 0) {
	    aval = -x
	    ival = int(aval)
	    fraction = aval - ival
	    if (fraction >= 0.5)
		return int(x)-1
	    else
		return ((int(x)==-0)?0:int(x))
	} else {
	    fraction = x - ival
	    if (fraction >= .5)
		return ival + 1
	    else
		return ival
	}
    }
}BEGIN{
    x0=1e8
    x1=-1e8
    y0=1e8
    y1=-1e8
    z0=1e8
    z1=-1e8
}
{
    Pr=$3
     x=round($5/dx)
     y=round($6/dx)
     z=round($7/dx)
     p0[x,y,z]++
     p[x,y,z]+=Pr
     if(x<x0)x0=x
     if(y<y0)y0=y
     if(z<z0)z0=z
     if(x>x1)x1=x
     if(y>y1)y1=y
     if(z>z1)z1=z
}END{
    KBT=0.0019872041*T
    for(x=x0;x<=x1;x++)
        for(y=y0;y<=y1;y++)
         for(z=z0;z<=z1;z++)
            print x*dx,y*dy,z*dz,-KBT*log(p[x,y,z]),-KBT*log(p0[x,y,z])
}' > pmf_3d_distorient.txt



