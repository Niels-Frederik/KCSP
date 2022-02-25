#include "TupleGenerator.h"
#include "Concurrent_Partitioning_Algorithm.h"
#include "Independent_Partitioning_Algorithm.h"
#include<iostream>

using namespace std;

int main(int argc, char** argv)
{
    const int TUPLES = atoi(argv[1]);
    //const int THREADS = *argv[0];
    //const int HASHBITS = *argv[1];
    const int THREADS = atoi(argv[2]);
    const int HASHBITS = atoi(argv[3]);

    TupleGenerator tg;
    Concurrent_Partitioning_Algorithm cpa;
    Independent_Partitioning_Algorithm ipa;

    auto tuples = tg.GenerateTuples(TUPLES);
    auto start = std::chrono::high_resolution_clock::now();
    cpa.ConcurrentPartition(tuples, THREADS, HASHBITS);
    auto finish = std::chrono::high_resolution_clock::now();
    auto microseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start);
    cout << "Concurrent: " << microseconds.count() << "ns\n";
    start = std::chrono::high_resolution_clock::now();
    ipa.IndependentPartition(tuples, THREADS, HASHBITS);
    finish = std::chrono::high_resolution_clock::now();
    microseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start);
    cout << "Independent: " << microseconds.count() << "ns\n";
}
