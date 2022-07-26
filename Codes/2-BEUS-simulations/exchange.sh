#!/bin/bash

ni=31 # number of images
nc=1 # number of copies
log="job0.log job1.log job2.log" # list all log files

grep EXCHANGING $log | awk -v ni=$ni -v nc=$nc '{
    count[$8]++
    nr=$6+1
}END{
    for(i=0;i<ni-1;i++) print i,count[i]/(nr*nc/2),nr
}' | sort -n | \
awk 'BEGIN{
    print "#image1 image2 rate(percentage)"
}{
    sum+=$2
    n++
    print $1,$1+1,100*$2
    nr=$3
}END{
    print "#avr",100*sum/n
    print "#runs",nr
}' > exchange.txt
