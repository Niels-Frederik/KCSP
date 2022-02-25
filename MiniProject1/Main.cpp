#include "TupleGenerator.h"
#include "Concurrent_Partitioning_Algorithm.h"
#include "Independent_Partitioning_Algorithm.h"
#include<iostream>

using namespace std;

int main(int argc, char** argv)
{
    const int TUPLES = atoi(argv[2]);
    const int THREADS = atoi(argv[3]);
    const int HASHBITS = atoi(argv[4]);

    TupleGenerator tg;
    Concurrent_Partitioning_Algorithm cpa;
    Independent_Partitioning_Algorithm ipa;

    auto tuples = tg.GenerateTuples(TUPLES);
    auto start = std::chrono::high_resolution_clock::now();

    if(atoi(argv[1]) == 0)
    {
        cpa.ConcurrentPartition(tuples, THREADS, HASHBITS);
    }
    else
    {
        ipa.IndependentPartition(tuples, THREADS, HASHBITS);
    }

    auto finish = std::chrono::high_resolution_clock::now();
    auto microseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start);
    cout << microseconds.count() << "ns\n";
}
