#!/bin/bash

ni=31 # number of images
l=`wc potall.txt | awk '{print $1}'` # number of data points (you can use only a portion of the data)

# using generalized wham algorithm (exclude the block number)
awk '{for(i=1;i<=NF;i++) if(i!=2) printf "%s ",$i; printf"\n"}' potall.txt | \
./npwham -w $ni -l $l -i 10000 -t 300 > densityall.txt 2> densityall.err
# use npwham -h for the description of arguments (-w: # of windows, -l: # of data points, -t: temperature)
