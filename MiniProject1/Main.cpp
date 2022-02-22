#include "TupleGenerator.h"
#include "Concurrent_Partitioning_Algorithm.h"
#include "Independent_Partitioning_Algorithm.h"
#include<iostream>

using namespace std;

int main()
{
    const int TUPLES = 10000000;
    const int THREADS = 4;
    const int HASHBITS = 3;

    TupleGenerator tg;
    Concurrent_Partitioning_Algorithm cpa;
    Independent_Partitioning_Algorithm ipa;

    vector<tuple<int,int>> tuples = tg.GenerateTuples(TUPLES);
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
