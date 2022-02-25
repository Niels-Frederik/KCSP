#!/bin/bash
for i in {0..1}
do
    if [ $i == 0 ]
    then
        echo Concurrent Algorithm
    else
        echo Independent Algorithm
    fi

    for j in {1..5}
    do
       echo "Settings: $j threads, 3 hashbits"
        ./main $i 100000000 $j 3
    done
done
