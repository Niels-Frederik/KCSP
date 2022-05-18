#!/bin/bash
tuples=18000000

for i in {0..1}
do
    if [ $i == 0 ]
    then
        echo Concurrent Algorithm
    else
        echo Independent Algorithm
    fi

    for j in 1 2 4 8 16 32
    do
        for h in {16..18}
        do
            echo T$j, H$h
            for g in {0..10}
            do
                ./Program 1 $tuples $j $h
            done
        done
    done
done
