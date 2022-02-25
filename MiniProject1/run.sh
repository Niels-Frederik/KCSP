#!/bin/bash
hashBits=3
tuples=100000000

for i in {0..1}
do
    if [ $i == 0 ]
    then
        echo Concurrent Algorithm
    else
        echo Independent Algorithm
    fi

    ./main $i $tuples $hashBits
done
