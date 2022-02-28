#!/bin/bash
tuples=10000000

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
        for h in {1..4}
        do
            echo Threads: $j, HashBits: $h
            for g in {0..10}
            do
                ./Program $i $tuples $j $h
            done
        done
    done
done
