#!/bin/bash

ni=31 # number of images
nc=1 # number of copies
nr=$((ni*nc)) # number of replicas
# pick the right directory
output="output"
#outputx="output1"
#output="../../sample-output"

rm colvarsall/*
rm histall/*
#
for r in `seq 0 $((nr-1))`
do
    c=$((r % nc))
    i=$((r / nc))
    cat $output/$i/out.job0.$r.colvars.traj $output/$i/out.job1.$r.colvars.traj  | grep -v "#" | uniq | \
    awk -v c=$c '($1>0) {printf "%s %s %s",$1,c,$2;printf"\n"}' | awk '!t||t!=$1{print;t=$1}' >> colvarsall/$i # reports time, copy id, and colvar values
    cat $output/$i/out.job0.$r.history $output/$i/out.job1.$r.history | grep -v "#" | uniq | \
    awk -v c=$c -v nc=$nc '{print $1,c,int($2/nc)}' >> histall/$i # reports time, copy id, and actual image id
done

rm cvsall.txt
for i in `seq 0 $((nr/nc-1))`
do
    paste histall/$i colvarsall/$i | awk '($1==$4)&&($2==$5){printf "%s %s %s",$1,$2,$3; for(i=6;i<=NF;i++) printf " %s",$i; printf "\n"}' >> cvsall.txt
done
