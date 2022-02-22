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
atomic_int writeLock(0);
atomic_int* writeLockArr;

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

        bufferArr[0]->emplace_back(tuples[i]);
        bufferArr[hash][0][writeLockArr[hash]++] = tuples[i];
        //buffer[writeIndex++] = tempValueForVisualization;
    }
}

void Concurrent_Partitioning_Algorithm::ConcurrentPartition(vector<tuple<int, int>> tuples, int threadCount, int hashBits) {
    int size = pow(2, hashBits);
    //int size = 2;
    bufferArr = new vector<tuple<int,int>>*[size];
    writeLockArr = new atomic_int [size];

    for(int i = 0; i < size; i++)
    {
        //writeLockArr[i] new atomic_int (0);
        //vector<tuple<int,int>>* vect = new vector<tuple<int,int>>;
        bufferArr[i] = new vector<tuple<int,int>>;
    }

    //bufferArr = new vector<tuple<int,int>>*;

    //vector<tuple<int,int>>* bufferArr[size];
    //bufferArr = new tuple<vector<int,int>>*;


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

