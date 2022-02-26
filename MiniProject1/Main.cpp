#include "TupleGenerator.h"
#include "Concurrent_Partitioning_Algorithm.h"
#include "Independent_Partitioning_Algorithm.h"
#include<iostream>

using namespace std;

int main(int argc, char** argv)
{
    const int TUPLES = atoi(argv[2]);
    //const int THREADS = atoi(argv[3]);
    const int HASHBITS = atoi(argv[3]);

    TupleGenerator tg;
    Independent_Partitioning_Algorithm ipa;
    Concurrent_Partitioning_Algorithm cpa;

    auto tuples = tg.GenerateTuples(TUPLES);

    for(int i = 1; i <= 32; i*=2)
    {
        auto start = std::chrono::high_resolution_clock::now();

        if(atoi(argv[1]) == 0)
        {
            cpa.ConcurrentPartition(tuples, i, HASHBITS);
        }
        else
        {
            ipa.IndependentPartition(tuples, i, HASHBITS);
        }

        auto finish = std::chrono::high_resolution_clock::now();
        auto microseconds = std::chrono::duration_cast<std::chrono::milliseconds>(finish-start);
        cout << i << " threads: " << microseconds.count() << "ms\n";
    }

}
