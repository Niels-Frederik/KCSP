#include "Concurrent_Partitioning_Algorithm.h"
#include<vector>
#include<tuple>
#include<thread>
#include<iostream>
#include<atomic>
#include<iterator>
#include <random>

using namespace std;

vector<tuple<int,int>>** bufferArr;
vector<std::thread> threads;
atomic_int** writeLockArr;

uint32_t hashing(int x, int hashBitNumber) {
    const std::uint32_t knuth = 2654435769;
    const std::uint32_t y = x;
    return (y * knuth) >> (32 - hashBitNumber);
}

void Concurrent_Partitioning_Algorithm::Process(vector<tuple<int,int>> tuples, int from, int to, int hashBits)  {
    for (int i = from; i < to; i++)
    {
        //tuple<int,int> tempValueForVisualization = make_tuple(i,i);
        uint32_t hash = hashing(get<0>(tuples[i]), hashBits);

        //bufferArr[0]->emplace_back(tuples[i]);
        auto writeIndex = writeLockArr[hash]->fetch_add(1);
        //bufferArr[hash]->emplace_back(tuples[i]);
        //bufferArr[hash]->emplace_back(tuples[i]);
        (*bufferArr[hash])[writeIndex] = tuples[i];

        //*bufferArr[hash] = tuples[i]);

        //auto x = *bufferArr[hash];
        //auto y = &x;
        //x[0] = tuples[i];
        //
        //vector<int> ve;
        //ve[1] = 2;
        //bufferArr[hash][0][writeLockArr[hash]++] = tuples[i];
        //buffer[writeIndex++] = tempValueForVisualization;
    }
}

void Concurrent_Partitioning_Algorithm::ConcurrentPartition(vector<tuple<int, int>> tuples, int threadCount, int hashBits) {
    int partitions = pow(2, hashBits);

    bufferArr = new vector<tuple<int,int>>*[partitions];
    writeLockArr = new atomic_int*[partitions];

    for(int i = 0; i < partitions; i++)
    {
        writeLockArr[i] = new atomic_int (0);
        //vector<tuple<int,int>>* vect = new vector<tuple<int,int>>;
        bufferArr[i] = new vector<tuple<int,int>>;
        bufferArr[i]->reserve(tuples.size()/partitions+(tuples.size() * 0.1));
    }

    int amountInEach = tuples.size()/threadCount;
    int previousLast = 0;
    while(threadCount--)
    {
        int to = previousLast+amountInEach;
        threads.emplace_back(thread(Process, tuples, previousLast, to, hashBits));
        previousLast = to;
    }

    for (auto& th : threads)
    {
        th.join();
    }
}

