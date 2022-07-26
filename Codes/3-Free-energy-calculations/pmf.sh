#!/bin/bash

T=300 # Temperarure
dx=1 # bin size

paste densityall.txt cvsall.txt | \
awk -v dx=$dx -v T=$T '
function round(x) {
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
}
{
    Pr=$3
	x=round($7/dx)
	p0[x]++
	p[x]+=Pr
	if(x<x0)x0=x
	if(x>x1)x1=x
}END{
    KBT=0.0019872041*T
    for(x=x0;x<=x1;x++) {
	printf "%s ",x*dx
	printf "%s ",-KBT*log(p[x])
	printf "%s ",-KBT*log(p0[x])
        printf "\n"
    }
}' > pmfall.txt

