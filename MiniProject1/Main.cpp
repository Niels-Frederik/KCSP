#include "TupleGenerator.h"
#include "Concurrent_Partitioning_Algorithm.h"
#include "Independent_Partitioning_Algorithm.h"
#include<iostream>

using namespace std;

int main()
{
    TupleGenerator tg;
    vector<tuple<int,int>> tuples = tg.GenerateTuples(100000000);
    cout << "Done generating tuples" << endl;
    Concurrent_Partitioning_Algorithm cpa;
    Independent_Partitioning_Algorithm ipa;
    auto start = std::chrono::high_resolution_clock::now();
    cpa.ConcurrentPartition(tuples, 4);
    //ipa.IndependentPartition(tuples, 4);
    auto finish = std::chrono::high_resolution_clock::now();
    auto microseconds = std::chrono::duration_cast<std::chrono::milliseconds>(finish-start);
    cout << microseconds.count() << "ms\n";
}