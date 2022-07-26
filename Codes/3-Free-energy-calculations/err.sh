#!/bin/bash

ni=31 # number of images
nc=5 # number of copies
l=`wc potall.txt | awk '{print $1}'` # number of data points (you can use only a portion of the data)


# using generalized wham algorithm with bootstrapping
./npwham -w $ni -l $l -t 300 -b $nc -B 100 < potall.txt > bootstrap1.txt 2> bootstrap1.err
# use npwham -h for the description of arguments (-b: # of blocks, -B: number of bootstraps)
